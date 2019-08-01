#ifndef LOGGER_H
#define	LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/msvc_sink.h"
#include <iostream>

class Logger
{

public:
	static Logger& Instance()
	{
		static Logger the_single_instance;
		return the_single_instance;
	}

	template<typename... Args> void info(const char* fmt, const Args& ... args)
	{
		logger_->info(fmt, args...);
	}

	template<typename T> void info(const T& msg)
	{
		logger_->info(msg);
	}

	template<typename... Args> void critical(const char* fmt, const Args& ... args)
	{
		logger_->info(fmt, args...);
	}

	template<typename T> void critical(const T& msg)
	{
		logger_->info(msg);
	}
private:

	std::shared_ptr<spdlog::sinks::msvc_sink_mt> sink_;
	std::shared_ptr<spdlog::logger> logger_;

	Logger();
	Logger(const Logger& root) = delete;
	Logger& operator=(const Logger&) = delete;
};

#endif // LOGGER_H