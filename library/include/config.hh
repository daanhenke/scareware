#pragma once
#include <filesystem>
#include "external/Yaml.hh"
#include "ui/render.hh"

namespace fs = std::filesystem;

namespace sw::config
{
	extern fs::path ConfigPath;
	extern Yaml::Node ConfigNode;

	void UseConfig(fs::path file_path);
	void SaveConfig();
	
	std::string GetStringNode(std::string section, std::string defaultValue);
	bool GetBoolNode(std::string section, bool defaultValue);

	struct color_t
	{
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t alpha;

		color_t(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
		{
			red = r;
			green = g;
			blue = b;
			alpha = a;
		}
	};

	typedef struct
	{
		struct
		{
			bool enabled;
		} bhop;

		struct
		{
			int enabled;
			nk_colorf terrorist_color;
			nk_colorf counterterrorist_color;
		} chams;

		struct
		{
			int enabled;
			float intensity;
		} noflash;
	} config_t;

	void SetCurrentConfig();
	extern config_t CurrentConfig;
}