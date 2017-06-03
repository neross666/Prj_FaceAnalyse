#include "Config.h"
#include <vector>
#include <iostream>


#ifdef WIN32
#include <io.h>
#include <direct.h>
#endif
#ifdef linux 
#include <unistd.h>
#include <sys/types.h>  
#include <sys/stat.h>
#include <dirent.h>
#endif

using namespace std;

Config::Config()
{
}

Config::~Config()
{
}

void Config::appAutoRun(bool bAutoRun)
{

}

void Config::generateFolder()
{
	generateFolder(RESOURCE_DIR);
	generateFolder(SKIN_DIR);
	generateFolder(LANGUAGE_DIR);
	generateFolder(PLUGIN_DIR);
}

void Config::generateFolder(std::string dir)
{
	if (access(dir.c_str(), 0) == -1)
	{
#ifdef WIN32
		int flag = mkdir(dir.c_str());
#endif
#ifdef linux 
		int flag = mkdir(dir.c_str(), 0777);
#endif
		if (flag != 0)
		{
			spdlog::get(LOGGER)->error("generate folder {0} error.", dir);
		}
	}
}

void Config::deleteFolder(std::string dir)
{
	if (access(dir.c_str(), 0) == 0)
	{
		int flag = rmdir(dir.c_str());
		if (flag != 0)
		{
			spdlog::get(LOGGER)->error("delete folder {0} error.", dir);
		}
	}
	else
	{
		spdlog::get(LOGGER)->error("access folder {0}.", dir);
	}
}

void Config::generateLogger()
{
	try
	{
		generateFolder(LOGGER_DIR);
		string log_dir = LOGGER_DIR;
		string log_file_name = log_dir + "/" + LOGGER;

		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
		sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(log_file_name, 23, 59));
		auto combined_logger = std::make_shared<spdlog::logger>(LOGGER, begin(sinks), end(sinks));
		spdlog::register_logger(combined_logger);
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%L] [thread %t] %v");
		combined_logger->set_level(spdlog::level::debug);

		spdlog::get(LOGGER)->info("Application launch.");
	}
	catch (const spdlog::spdlog_ex& ex)
	{
	}
}

bool Config::writeInit(std::string path, std::string group, std::string user_key, std::string user_value)
{
	if (path.empty() || user_key.empty())
	{
		return false;
	}

	CSimpleIni config;
	if (config.LoadFile(path.c_str()) == SI_OK)
	{
		SI_Error res = config.SetValue(group.c_str(), user_key.c_str(), user_value.c_str());
		config.SaveFile(path.c_str());
		return (res == SI_UPDATED || res == SI_INSERTED);
	}
	return false;
}

bool Config::writeInit(std::string path, std::string group, std::string user_key, int user_value)
{
	if (path.empty() || user_key.empty())
	{
		return false;
	}

	CSimpleIni config;
	if (config.LoadFile(path.c_str()) == SI_OK)
	{
		SI_Error res = config.SetLongValue(group.c_str(), user_key.c_str(), user_value);
		config.SaveFile(path.c_str());
		return (res == SI_UPDATED || res == SI_INSERTED);
	}
	return false;
}

bool Config::writeInit(std::string path, std::string group, std::string user_key, double user_value)
{
	if (path.empty() || user_key.empty())
	{
		return false;
	}

	CSimpleIni config;
	if (config.LoadFile(path.c_str()) == SI_OK)
	{
		SI_Error res = config.SetDoubleValue(group.c_str(), user_key.c_str(), user_value);
		config.SaveFile(path.c_str());
		return (res == SI_UPDATED || res == SI_INSERTED);
	}
	return false;
}

bool Config::writeInit(std::string path, std::string group, std::string user_key, bool user_value)
{
	if (path.empty() || user_key.empty())
	{
		return false;
	}

	CSimpleIni config;
	if (config.LoadFile(path.c_str()) == SI_OK)
	{
		SI_Error res = config.SetBoolValue(group.c_str(), user_key.c_str(), user_value);
		config.SaveFile(path.c_str());
		return (res == SI_UPDATED || res == SI_INSERTED);
	}
	return false;
}

bool Config::readInit(std::string path, std::string group, std::string user_key, std::string &user_value, std::string def)
{
	if (path.empty() || user_key.empty())
	{
		return false;
	}

	CSimpleIni config;
	if (config.LoadFile(path.c_str()) == SI_OK)
	{
		user_value = config.GetValue(group.c_str(), user_key.c_str(), def.c_str());
		return true;
	}
	return false;
}

bool Config::readInit(std::string path, std::string group, std::string user_key, int &user_value, int def)
{
	if (path.empty() || user_key.empty())
	{
		return false;
	}

	CSimpleIni config;
	if (config.LoadFile(path.c_str()) == SI_OK)
	{
		user_value = config.GetLongValue(group.c_str(), user_key.c_str(), def);
		return true;
	}
	return false;
}

bool Config::readInit(std::string path, std::string group, std::string user_key, double &user_value, double def)
{
	if (path.empty() || user_key.empty())
	{
		return false;
	}

	CSimpleIni config;
	if (config.LoadFile(path.c_str()) == SI_OK)
	{
		user_value = config.GetDoubleValue(group.c_str(), user_key.c_str(), def);
		return true;
	}
	return false;
}

bool Config::readInit(std::string path, std::string group, std::string user_key, bool &user_value, bool def)
{
	if (path.empty() || user_key.empty())
	{
		return false;
	}

	CSimpleIni config;
	if (config.LoadFile(path.c_str()) == SI_OK)
	{
		user_value = config.GetBoolValue(group.c_str(), user_key.c_str(), def);
		return true;
	}
	return false;
}