#include "Log.h"



Log& Log::Get()
{
	static Log log;
	return log;
}
Log::Log()
{
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
	m_Logger = spdlog::stdout_color_mt("Server");
	m_Logger->set_level(spdlog::level::trace);
}
Log::~Log()
{
}