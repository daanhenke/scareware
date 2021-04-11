#include "config.hh"
#include <fstream>
#include <stdlib.h>
#include <Windows.h>
#include "console.hh"

fs::path sw::config::ConfigPath;
Yaml::Node sw::config::ConfigNode;

// Helper function to expand ~ in paths
fs::path ExpandPath(fs::path input)
{
	if (input.empty()) return input;

	std::string inpstr = input.string();
	if (inpstr[0] == '~')
	{
		const char* user_dir = getenv("USERPROFILE");
		if (user_dir == nullptr)
		{
			MessageBoxA(nullptr, "OH NO", "couldn't resolve home path, how fucked up is ur pc?", MB_OK);
			exit(1);
		}

		input = fs::path(std::string(user_dir)) / fs::path(inpstr.substr(2));
	}

	return input;
}

#include <console.hh>
void sw::config::UseConfig(fs::path file_path)
{
	file_path = ExpandPath(file_path);

	if (! fs::exists(file_path))
	{
		std::ofstream first_write_file(file_path);
		first_write_file << "\n";
		first_write_file.close();
	}

	ConfigPath = file_path;
	Yaml::Parse(ConfigNode, file_path.string());
	SetCurrentConfig();
}

std::string sw::config::GetStringNode(std::string section, std::string defaultValue)
{
	auto result = ConfigNode[section].As<std::string>("");
	if (result.length() == 0)
	{
		ConfigNode[section] = defaultValue;
		SaveConfig();
		result = defaultValue;
	}

	return result;
}

bool sw::config::GetBoolNode(std::string section, bool defaultValue)
{
	auto strval = GetStringNode(section, defaultValue ? "true" : "false");
	if (strval == "true") return true;
	if (strval == "false") return false;
	return defaultValue;
}

void sw::config::SaveConfig()
{
	Yaml::Serialize(ConfigNode, ConfigPath.string().c_str());
}


sw::config::config_t sw::config::CurrentConfig;
void sw::config::SetCurrentConfig()
{
	CurrentConfig = 
	{
		.bhop =
		{
			.enabled = GetBoolNode("bhop_enabled", true)
		},
		.chams =
		{
			.enabled = true,
			.terrorist_color = {1.f, .5f, 0, 0},
			.counterterrorist_color = {0.f, .5f, 1.f, 0}
		},
		.noflash =
		{
			.enabled = true,
			.intensity = .1f
		},
	};
}