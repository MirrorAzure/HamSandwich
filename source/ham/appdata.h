#ifndef APPDATA_H
#define APPDATA_H

#include <stdio.h>
#include <vector>
#include <string>

typedef struct SDL_RWops SDL_RWops;

// Methods for accessing assets/saves 'correctly' on each platform. In some
// situations, Appdata and Asset folders may overlap, so names should not be
// reused. See appdata.cpp for implementation details for each platform.

void AppdataInit();
bool AppdataIsInit();

// Use for saves, options, config, etc.
FILE* AppdataOpen(const char* file, const char* mode);
// On platforms that need it, ensure appdata is really saved.
void AppdataSync();

// Use for graphics, worlds, etc. Writes may go to a different location than reads.
FILE* AssetOpen(const char* file, const char* mode);
SDL_RWops* AssetOpen_SDL(const char* file, const char* mode);

std::vector<std::string> ListDirectory(const char* directory, const char* extension = nullptr, size_t maxlen = 0);

void AppdataDelete(const char* file);

#endif  // APPDATA_H
