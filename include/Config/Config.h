#pragma once
#include <string>
#include "SimpleIni\SimpleIni.h"
#include "spdlog/spdlog.h"

#define PARAM_CONFIG			"param.ini"
#define SYS_CONFIG_PATH			"SystemSettings.ini"
#define SYS_CONFIG_F_PATH		"SystemSettingsF.ini"

#define LOGGER_DIR				"./Logs"
#define LOGGER					"FaceAnalyse"

#define RESOURCE_DIR			"./Resources"
#define SKIN_DIR				"./Resources/Skin"
#define LANGUAGE_DIR			"./Resources/Language"
#define PLUGIN_DIR				"./Plugin"

class Config
{
public:
	Config();
	~Config();

	static void appAutoRun(bool flag);
	static void generateFolder();
	static void generateFolder(std::string dir);
	static void deleteFolder(std::string dir);
	static void generateLogger();

	static bool writeInit(std::string path, std::string group, std::string user_key, std::string user_value);
	static bool writeInit(std::string path, std::string group, std::string user_key, int user_value);
	static bool writeInit(std::string path, std::string group, std::string user_key, double user_value);
	static bool writeInit(std::string path, std::string group, std::string user_key, bool user_value);
	static bool readInit(std::string path, std::string group, std::string user_key, std::string &user_value, std::string def = "");
	static bool readInit(std::string path, std::string group, std::string user_key, int &user_value, int def = 0);
	static bool readInit(std::string path, std::string group, std::string user_key, double &user_value, double def = 0.0f);
	static bool readInit(std::string path, std::string group, std::string user_key, bool &user_value, bool def = false);
};

