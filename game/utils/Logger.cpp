#include "Logger.h"
#include <iostream>

Logger::Logger()
{
    sink_ = std::make_shared<spdlog::sinks::msvc_sink_mt>();
    logger_ = std::make_shared<spdlog::logger>("msvc_logger", sink_);
    logger_->set_pattern("[%H:%M:%S %z] [thread:%t] %v");
}
