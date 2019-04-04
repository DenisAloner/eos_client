#include "Logger.h"

Logger::Logger()
{
	sink_ = std::make_shared<spdlog::sinks::msvc_sink_mt>();
	logger_ = std::make_shared<spdlog::logger>("msvc_logger", sink_);
}
