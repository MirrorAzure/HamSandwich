#include "appdata.h"
#include "log.h"
#include "erase_if.h"
#include "jamultypes.h"
#include "nsis.h"
#include "inno.h"
#include "metadata.h"
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <set>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#ifndef _MSC_VER
#include <unistd.h>
#include <sys/wait.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#include <SDL_platform.h>
#include <SDL_rwops.h>
#include <SDL_messagebox.h>

/*
The interface in appdata.h currently distinguishes between AppdataOpen
and AssetOpen, but the implementation treats them the same, instead switching
based on whether the mode is read-only.

We always first try to use the read-write ("appdata") directory. If the mode
is read-only, we then try to use the read-only ("asset") directories in
sequence.

Emscripten
	RW: /appdata/$GAME
	Ro: /
		Provided by build process + JS environment
Android
	if external storage is writeable:
		RW: $EXTERNAL
		Ro: $INTERNAL
	else:
		RW: $INTERNAL
	Ro: SDL_RWFromFile, which reads from .apk
		Tempfiles in $INTERNAL/.bundle_tmp when needed
		Provided by build process
Default
	RW: $PWD

Psuedocode for SDL_RWFromFile(file, mode):
	if android:
		if file is absolute:
			return fopen(file, mode) if successful
		else:
			return fopen($INTERNAL/file, mode) if successful
		return bundled asset from .apk
	else if windows:
		return windows_file_open(file, mode)
	else:
		if apple and mode is readonly:
			return fopen($APP_BUNDLE/file, mode) if successful
		return fopen(file, mode)
*/

// ----------------------------------------------------------------------------
// Stdio helpers
#ifdef _WIN32
#include <direct.h>

#define platform_mkdir(path) _mkdir(path)
#else
#define platform_mkdir(path) mkdir(path, 0777)
#endif

static int mkdir_one(const char *path) {
	if (platform_mkdir(path) != 0 && errno != EEXIST)
		return -1;
	return 0;
}

static int mkdir_parents(const char *path) {
	std::string copypath = path;
	char *start = copypath.data();
	char *next;

	int status = 0;
	while (status == 0 && (next = strchr(start, '/'))) {
		if (next != start) {
			// skip the root directory and double-slashes
			*next = '\0';
			status = mkdir_one(copypath.c_str());
			*next = '/';
		}
		start = next + 1;
	}

	if (status != 0)
		LogError("mkdirs(%s): %s", copypath.c_str(), strerror(errno));

	return status;
}

static bool is_write_mode(const char* mode) {
	return mode[0] == 'w' || mode[0] == 'a' || (mode[0] == 'r' && mode[1] == '+');
}

// ----------------------------------------------------------------------------
// VFS interface
class Vfs {
	Vfs(const Vfs&) = delete;
	Vfs(Vfs&&) = default;
	Vfs& operator=(const Vfs&) = delete;
	Vfs& operator=(Vfs&&) = default;
public:
	Vfs() {}
	virtual ~Vfs() {}

	virtual FILE* open_stdio(const char* file, const char* mode, bool write) = 0;
	virtual SDL_RWops* open_sdl(const char* file, const char* mode, bool write) = 0;
	virtual bool list_dir(const char* directory, std::set<std::string>& output) = 0;
	virtual bool delete_file(const char* file);
};

bool Vfs::delete_file(const char* file) {
	(void) file;
	return false;
}

struct VfsStack {
	std::vector<std::unique_ptr<Vfs>> stack;
public:
	VfsStack() {}

	void push_back(std::unique_ptr<Vfs>&& entry) { stack.push_back(std::move(entry)); }

	// Returns true if this VfsStack is empty and therefore not useable.
	bool empty() const { return stack.empty(); }

	// Get the Vfs that should be used for writing.
	Vfs* appdata() { return stack.front().get(); }

	// Iterate over the Vfses to use for reading, in priority order.
	auto begin() { return stack.begin(); }
	auto end() { return stack.end(); }

	// Forward to children
	SDL_RWops* open_sdl(const char* file, const char* mode);
};

SDL_RWops* VfsStack::open_sdl(const char* file, const char* mode) {
	bool write = is_write_mode(mode);
	if (write) {
		return appdata()->open_sdl(file, mode, write);
	}

	for (auto& vfs : *this) {
		SDL_RWops* rw = vfs->open_sdl(file, mode, write);
		if (rw) {
			return rw;
		}
	}

	return nullptr;
}

// ----------------------------------------------------------------------------
// Stdio VFS implementation
class StdioVfs : public Vfs {
	std::string prefix;
public:
	StdioVfs(std::string prefix) : prefix(prefix) {}
	FILE* open_stdio(const char* file, const char* mode, bool write);
	SDL_RWops* open_sdl(const char* file, const char* mode, bool write);
	bool list_dir(const char* directory, std::set<std::string>& output);
	bool delete_file(const char* file);
};

FILE* StdioVfs::open_stdio(const char* file, const char* mode, bool write) {
	std::string buffer = prefix;
	buffer.append("/");
	buffer.append(file);
	if (write) {
		mkdir_parents(buffer.c_str());
	}
	FILE* fp = fopen(buffer.c_str(), mode);

#ifndef _WIN32
	// On non-Windows, try to case-correct file lookups
	if (!fp) {
		size_t i = buffer.rfind("/");
		buffer[i] = '\0';

		std::set<std::string> temp;
		list_dir(buffer.c_str(), temp);

		for (const auto& name : temp)
		{
			if (!strcasecmp(&buffer[i + 1], name.c_str()))
			{
				buffer[i] = '/';
				memcpy(&buffer[i + 1], name.data(), name.length());
				fp = fopen(buffer.c_str(), mode);
				break;
			}
		}
	}
#endif

	if (!fp && write) {
		LogError("fopen(%s, %s): %s", buffer.c_str(), mode, strerror(errno));
	}
	return fp;
}

SDL_RWops* StdioVfs::open_sdl(const char* file, const char* mode, bool write) {
#if defined(_WIN32) && !defined(__GNUC__)
	// The public MSVC binaries of SDL2 are compiled without support for SDL_RWFromFP.
	std::string buffer = prefix;
	buffer.append("/");
	buffer.append(file);
	if (write) {
		mkdir_parents(buffer.c_str());
	}

	SDL_RWops* rw = SDL_RWFromFile(buffer.c_str(), mode);
	if (!rw && write) {
		LogError("SDL_RWFromFile(%s, %s): %s", buffer.c_str(), mode, SDL_GetError());
	}
	return rw;
#else
	// Delegate to open_stdio above.
	FILE* fp = open_stdio(file, mode, write);
	return fp ? SDL_RWFromFP(fp, SDL_TRUE) : nullptr;
#endif
}

#ifdef __GNUC__
#include <dirent.h>

bool StdioVfs::list_dir(const char* directory, std::set<std::string>& output) {
	std::string buffer = prefix;
	buffer.append("/");
	buffer.append(directory);
	DIR* dir = opendir(buffer.c_str());
	if (!dir) {
		return false;
	}

	while (struct dirent *dp = readdir(dir)) {
		output.insert(dp->d_name);
	}

	closedir(dir);
	return true;
}

#elif defined(_MSC_VER)
#include <io.h>

bool StdioVfs::list_dir(const char* directory, std::set<std::string>& output) {
	std::string buffer = prefix;
	buffer.append("/");
	buffer.append(directory);
	buffer.append("/*");

	struct _finddata_t finddata;
	long hFile = _findfirst(buffer.c_str(), &finddata);
	if (hFile == -1) {
		return false;
	}

	do {
		output.insert(finddata.name);
	} while (_findnext(hFile, &finddata) == 0);

	_findclose(hFile);
	return true;
}

#endif  // __GNUC__ and _MSC_VER

bool StdioVfs::delete_file(const char* file) {
	std::string buffer = prefix;
	buffer.append("/");
	buffer.append(file);

	int status =
#ifdef _MSC_VER
		_unlink
#else
		unlink
#endif
		(buffer.c_str());

	if (status == 0) {
		return true;
	} else {
		LogError("unlink(%s): %s", buffer.c_str(), strerror(errno));
		return false;
	}
}

// ----------------------------------------------------------------------------
// "Extract to temporary directory" helper
FILE* fp_from_bundle(const char* file, const char* mode, SDL_RWops* rw, const char* tempdir, bool reuse_safe) {
	// Check internal storage to see if we've already extracted the file.
	std::string fname_buf = tempdir;
	fname_buf.append("/");
	fname_buf.append(file);
	if (reuse_safe) {
		FILE* fp = fopen(fname_buf.c_str(), mode);
		if (fp) {
			return fp;
		}
	}
	// If we have, delete it and extract it again, in case it's changed.
	unlink(fname_buf.c_str());

	mkdir_parents(fname_buf.c_str());
	FILE* fp = fopen(fname_buf.c_str(), "wb");
	if (!fp) {
		LogError("fp_from_bundle(%s) bad save: %s", file, strerror(errno));
		return nullptr;
	}

	// Copy everything.
	char buffer[4096];
	int read;
	while ((read = SDL_RWread(rw, buffer, 1, sizeof(buffer))) > 0) {
		fwrite(buffer, 1, read, fp);
	}

	// Return a FILE* pointing to the extracted asset.
	fclose(fp);
	fp = fopen(fname_buf.c_str(), mode);
	if (!fp) {
		LogError("fp_from_bundle(%s) bad readback: %s", file, strerror(errno));
	}
	return fp;
}

// ----------------------------------------------------------------------------
// NSIS VFS implementation

class NsisVfs : public Vfs {
	vanilla::nsis::Archive archive;
public:
	NsisVfs(SDL_RWops* fp) : archive(fp) {}
	FILE* open_stdio(const char* file, const char* mode, bool write);
	SDL_RWops* open_sdl(const char* file, const char* mode, bool write);
	bool list_dir(const char* directory, std::set<std::string>& output);
};

FILE* NsisVfs::open_stdio(const char* file, const char* mode, bool write) {
	SDL_RWops* rw = open_sdl(file, mode, write);
	return rw ? fp_from_bundle(file, mode, rw, ".nsis_tmp", true) : nullptr;
}

SDL_RWops* NsisVfs::open_sdl(const char* file, const char* mode, bool write) {
	if (write) {
		LogError("NsisVfs(%s, %s): does not support write modes", file, mode);
		return nullptr;
	}

	return archive.open_file(file);
}

bool NsisVfs::list_dir(const char* directory, std::set<std::string>& output) {
	return archive.list_dir(directory, output);
}

// ----------------------------------------------------------------------------
// Inno VFS implementation

class InnoVfs : public Vfs {
	vanilla::inno::Archive archive;
public:
	InnoVfs(FILE* fp) : archive(fp) {}
	FILE* open_stdio(const char* file, const char* mode, bool write);
	SDL_RWops* open_sdl(const char* file, const char* mode, bool write);
	bool list_dir(const char* directory, std::set<std::string>& output);
};

FILE* InnoVfs::open_stdio(const char* file, const char* mode, bool write) {
	SDL_RWops* rw = open_sdl(file, mode, write);
	return rw ? fp_from_bundle(file, mode, rw, ".inno_tmp", true) : nullptr;
}

SDL_RWops* InnoVfs::open_sdl(const char* file, const char* mode, bool write) {
	if (write) {
		LogError("InnoVfs(%s, %s): does not support write modes", file, mode);
		return nullptr;
	}

	return archive.open_file(file);
}

bool InnoVfs::list_dir(const char* directory, std::set<std::string>& output) {
	return archive.list_dir(directory, output);
}

static std::unique_ptr<Vfs> init_vfs_spec(const char* what, const char* spec) {
	std::string spec2 = spec;
	char* mountpoint = spec2.data();
	char* kind = mountpoint + strcspn(mountpoint, "@") + 1;
	kind[-1] = 0;
	char* param = kind + strcspn(kind, "@") + 1;
	param[-1] = 0;

	if (!strcmp(kind, "stdio")) {
		return std::make_unique<StdioVfs>(param);
	} else if (!strcmp(kind, "nsis")) {
		SDL_RWops* fp = SDL_RWFromFile(param, "rb");
		if (!fp) {
			LogError("%s: failed to open '%s' in VFS spec '%s'", what, param, spec);
			return nullptr;
		}
		return std::make_unique<NsisVfs>(fp);
	} else if (!strcmp(kind, "inno")) {
		FILE* fp = fopen(param, "rb");
		if (!fp) {
			LogError("%s: failed to open '%s' in VFS spec '%s'", what, param, spec);
			return nullptr;
		}
		return std::make_unique<InnoVfs>(fp);
	} else {
		LogError("%s: unknown kind '%s' in VFS spec '%s'", what, kind, spec);
		return nullptr;
	}
}

// ----------------------------------------------------------------------------
#if 0  // #ifdef _WIN32
// Windows %APPDATA% configuration (not currently in use)

#include <io.h>
#include <shlobj.h> // for SHGetFolderPath
#ifdef _MSC_VER
#include <direct.h>
#endif

static VfsStack default_vfs_stack() {
	char get_folder_path[MAX_PATH];
	SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, get_folder_path);

	std::string buffer = get_folder_path;
	buffer.append("\\Hamumu\\");
	buffer.append(GetHamSandwichMetadata()->appdata_folder_name);

	VfsStack result;
	result.push_back(std::make_unique<StdioVfs>(buffer));
	result.push_back(std::make_unique<StdioVfs>("."));
	return result;
}

// ----------------------------------------------------------------------------
#elif defined(__EMSCRIPTEN__)
// Emscripten configuration

#include <emscripten.h>

static VfsStack default_vfs_stack() {
	std::string buffer = "/appdata/";
	buffer.append(GetHamSandwichMetadata()->appdata_folder_name);

	VfsStack result;
	result.push_back(std::make_unique<StdioVfs>(buffer));
	result.push_back(std::make_unique<StdioVfs>(""));
	return result;
}

#define HAS_APPDATA_SYNC
void AppdataSync() {
	EM_ASM(
		HamSandwich.fsSync();
	);
}

// ----------------------------------------------------------------------------
#elif defined(__ANDROID__) && __ANDROID__
// Android VFS implementation and Android configuration

#include <SDL_system.h>
#include <string.h>

class AndroidBundleVfs : public Vfs {
public:
	FILE* open_stdio(const char* file, const char* mode, bool write);
	SDL_RWops* open_sdl(const char* file, const char* mode, bool write);
	bool list_dir(const char* directory, std::set<std::string>& output);
};

FILE* AndroidBundleVfs::open_stdio(const char* file, const char* mode, bool write) {
	SDL_RWops* rw = open_sdl(file, mode, write);
	if (!rw) {
		return nullptr;
	}

	// Use a directory which definitely doesn't overlap with appdata.
	std::string tempdir = SDL_AndroidGetInternalStoragePath();
	tempdir.append("/.bundle_tmp");
	return fp_from_bundle(file, mode, rw, tempdir.c_str(), false);
}

SDL_RWops* AndroidBundleVfs::open_sdl(const char* file, const char* mode, bool write) {
	if (write) {
		LogError("AndroidBundleVfs(%s, %s): does not support write modes", file, mode);
		return nullptr;
	}

	SDL_RWops* rw = SDL_RWFromFile(file, mode);
	if (!rw && write) {
		LogError("SDL_RWFromFile(%s, %s): %s", file, mode, SDL_GetError());
	}
	return rw;
}

#include "appdata_jni.inc"

static VfsStack default_vfs_stack() {
	VfsStack result;
	int need_flags = SDL_ANDROID_EXTERNAL_STORAGE_READ | SDL_ANDROID_EXTERNAL_STORAGE_WRITE;
	if ((SDL_AndroidGetExternalStorageState() & need_flags) == need_flags) {
		result.push_back(std::make_unique<StdioVfs>(SDL_AndroidGetExternalStoragePath()));
	}
	result.push_back(std::make_unique<StdioVfs>(SDL_AndroidGetInternalStoragePath()));
	result.push_back(std::make_unique<AndroidBundleVfs>());
	return result;
}

// ----------------------------------------------------------------------------
#else
// Naive stdio configuration

static bool detect_installers(VfsStack* result, const HamSandwichMetadata* meta) {
	// `appdata/$NAME/`
	std::string appdata = "appdata/";
	appdata.append(meta->appdata_folder_name);
	result->push_back(std::make_unique<StdioVfs>(appdata));

	// Assets from specs
	for (int i = 0; meta->default_asset_specs[i]; ++i) {
		if (auto vfs = init_vfs_spec("built-in", meta->default_asset_specs[i])) {
			result->push_back(std::move(vfs));
		} else {
			return false;
		}
	}
	return true;
}

static VfsStack default_vfs_stack() {
	const HamSandwichMetadata* meta = GetHamSandwichMetadata();
	VfsStack result;
	result.push_back(std::make_unique<StdioVfs>("."));

	if (detect_installers(&result, meta)) {
		// If we found all the installers we were looking for, use 'em,
		// and also use `appdata/$NAME`
		std::string appdata = "appdata/";
		appdata.append(meta->appdata_folder_name);
		appdata.append("/");
		printf("all installers found; chdir(%s)\n", appdata.c_str());
		mkdir_parents(appdata.c_str());
		chdir(appdata.c_str());
	}

	return result;
}

// ----------------------------------------------------------------------------
#endif
// Common implementation and interface

static void missing_assets_message() {
	struct stat sb;
	// Tweak wording/ordering based on whether "installers" exists.
	if (stat("installers", &sb) == 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Missing Assets - HamSandwich",
			"The game's assets appear to be missing.\n"
			"\n"
			"Download the appropriate installer and save it in\n"
			"the \"installers\" folder.\n"
#if defined(_WIN32) && !defined(_DEBUG)
			// Only talk about "nearby .dll files" on Windows release builds.
			"\n"
			"Alternatively, copy this .exe and nearby .dll files\n"
			"into an existing installation of the game."
#endif
			, nullptr);
	} else {
		// The .exe has been divorced from the install package.
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Missing Assets - HamSandwich",
			"The game's assets appear to be missing.\n"
			"\n"
#if defined(_WIN32) && !defined(_DEBUG)
			// Only talk about "nearby .dll files" on Windows release builds.
			"Copy this .exe and nearby .dll files\n"
			"into an existing installation of the game.\n"
			"\n"
			"Alternatively, create "
#else
			"Create "
#endif
			"an \"installers\" folder and\n"
			"download the appropriate installers there.",
			nullptr);
	}
}

static VfsStack vfs_stack_from_env() {
	VfsStack result;
	if (const char *appdata_spec = getenv("HSW_APPDATA")) {
		if (auto vfs = init_vfs_spec("HSW_APPDATA", appdata_spec)) {
			result.push_back(std::move(vfs));
		}

		char buffer[32];
		for (int i = 0; i < 1024; ++i) {
			sprintf(buffer, "HSW_ASSETS_%d", i);
			if (const char* asset_spec = getenv(buffer)) {
				if (auto vfs = init_vfs_spec(buffer, asset_spec)) {
					result.push_back(std::move(vfs));
				}
			} else {
				break;
			}
		}
	} else {
		result = default_vfs_stack();
	}
	return result;
}

static bool check_assets(VfsStack& vfs) {
	// Every game has this asset, so use it to sanity check.
	SDL_RWops* check = vfs.open_sdl("graphics/verdana.jft", "rb");
	if (check) {
		SDL_RWclose(check);
		return true;
	}

	return false;
}

static bool run_download_helper() {
#if defined(_WIN32)
	if (GetFileAttributesA("installers/download-helper.ps1") != INVALID_FILE_ATTRIBUTES) {
		std::string cmdline = "powershell -ExecutionPolicy Bypass installers/download-helper.ps1 ";
		cmdline.append(GetHamSandwichMetadata()->appdata_folder_name);

		STARTUPINFOA startupInfo = {};
		PROCESS_INFORMATION processInfo = {};
		startupInfo.cb = sizeof(startupInfo);
		startupInfo.dwFlags |= STARTF_USESHOWWINDOW;
		startupInfo.wShowWindow = SW_HIDE;
		if (CreateProcessA("C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe", cmdline.data(), nullptr, nullptr, false, CREATE_NO_WINDOW, nullptr, nullptr, &startupInfo, &processInfo)) {
			WaitForSingleObject(processInfo.hProcess, INFINITE);
			CloseHandle(processInfo.hProcess);
			CloseHandle(processInfo.hThread);
			return true;
		}
	}
#elif !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
	struct stat sb;
	if (stat("installers/.download-helper", &sb) == 0) {
		int child_pid = fork();
		if (child_pid == 0) {
			char first[] = "installers/.download-helper";
			std::string second = GetHamSandwichMetadata()->appdata_folder_name;
			char* const argv[] = { first, second.data(), nullptr };
			exit(execvp(argv[0], argv));
		} else if (child_pid > 0) {
			int wstatus;
			if (waitpid(child_pid, &wstatus, 0) >= 0) {
				if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 0) {
					return true;
				}
			}
		}
	}
#endif
	return false;
}

static VfsStack init_vfs_stack() {
	VfsStack result = vfs_stack_from_env();
	if (check_assets(result)) {
		return result;
	}

	if (run_download_helper()) {
		result = vfs_stack_from_env();
		if (check_assets(result)) {
			return result;
		}
	}

	missing_assets_message();
	exit(1);
}

// Android does not play nice with static initializers.
static VfsStack vfs_stack;

void AppdataInit() {
	vfs_stack = init_vfs_stack();
}

bool AppdataIsInit() {
	return !vfs_stack.empty();
}

FILE* AppdataOpen(const char* file, const char* mode) {
	return AssetOpen(file, mode);
}

FILE* AssetOpen(const char* file, const char* mode) {
	bool write = is_write_mode(mode);
	if (write) {
		return vfs_stack.appdata()->open_stdio(file, mode, write);
	}

	for (auto& vfs : vfs_stack) {
		FILE* fp = vfs->open_stdio(file, mode, write);
		if (fp) {
			return fp;
		}
	}

	LogError("AssetOpen(%s, %s): not found in any vfs", file, mode);
	return nullptr;
}

SDL_RWops* AssetOpen_SDL(const char* file, const char* mode) {
	SDL_RWops* rw = vfs_stack.open_sdl(file, mode);
	if (!rw) {
		LogError("AssetOpen_SDL(%s, %s): not found in any vfs", file, mode);
	}
	return rw;
}

std::vector<std::string> ListDirectory(const char* directory, const char* extension, size_t maxlen) {
	std::set<std::string> output;
	for (auto& vfs : vfs_stack) {
		vfs->list_dir(directory, output);
	}

	if (extension || maxlen > 0) {
		size_t extlen = extension ? strlen(extension) : 0;
		erase_if(output, [=](const std::string& value) {
			size_t len = value.size();
			if (maxlen > 0 && len >= maxlen) {
				return true;
			}
			if (extlen > 0 && (len < extlen || strcasecmp(extension, &value.c_str()[len - extlen]))) {
				return true;
			}
			return false;
		});
	}

	return { output.begin(), output.end() };;
}

void AppdataDelete(const char* file) {
	vfs_stack.appdata()->delete_file(file);
}

#ifndef HAS_APPDATA_SYNC
void AppdataSync() {}
#endif
