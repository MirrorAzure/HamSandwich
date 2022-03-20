--[[ binary-package-info.lua
Provides a "binary-package-info" premake action which creates a PowerShell
script and text files useful for exporting projects. ]]

local p = premake
local m = {}
p.modules.run_config = m

newaction {
	trigger = "binary-package-info",
	description = "Format metadata for build-binary-package.ps1",
	onWorkspace = function(wks)
		-- print("onWorkspace", wks.name)
		p.generate(wks, "binary-package-info.ps1", m.workspace_binary_package_info)
	end,
}

function m.is_application(prj)
	return prj.kind == "ConsoleApp" or prj.kind == "WindowedApp"
end

function m.workspace_binary_package_info(wks)
	local seen = {}

	p.w('# globals: $allprojects $exeprojects')

	p.push('$ExeProjects = @{')

	local comma = ""
	for prj in p.workspace.eachproject(wks) do
		if m.is_application(prj) then
			for cfg in p.project.eachconfig(prj) do
				p.push('%s"%s|%s" = @{', comma, prj.name, cfg.name:gsub("|", "_"))
				comma = "; "

				p.push('"installers" = @(')
				local comma2 = ""
				for _, inst in ipairs(cfg.installers) do
					p.w('%s@{"filename" = %s; "link" = %s; "file_id" = %s; "sha256sum" = %s}', comma2, json.encode(inst.filename), json.encode(inst.link), json.encode(inst.file_id), json.encode(inst.sha256sum))
					comma2 = ", "
				end
				p.pop(')')
				p.w('; "appdata_folder_name" = %s', json.encode(cfg.appdata_name or cfg.name))
				p.push('; "assetdirs" = @(')
				comma2 = ""
				for _, v in ipairs(cfg.assetdirs) do
					p.w('%s%s', comma2, json.encode(v))
					comma2 = ", "
				end
				p.pop(')')

				p.pop('}')
			end
		end
	end

	p.pop('}')
end
