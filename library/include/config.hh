#pragma once
#include <filesystem>
#include "external/Yaml.hh"

namespace fs = std::filesystem;

namespace sw::config
{
	extern fs::path ConfigPath;
	extern Yaml::Node ConfigNode;

	void UseConfig(fs::path file_path);
	void SaveConfig();
	
	template<typename T>
	T GetNode(std::string section, T defaultValue)
	{
		ConfigNode[section] = defaultValue;
		SaveConfig();
		return defaultValue;
	}
}