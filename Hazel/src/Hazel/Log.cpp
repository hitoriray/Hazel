#include "hzpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Hazel {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	 
	Log::Log()
	{

	}

	Log::~Log()
	{

	}

	void Log::Init()
	{
		// 设置记录信息的模式
		spdlog::set_pattern("%^[%T] %n: %v%$"); // 时间戳  日志的名字(core or cilent) 实际的信息
		s_CoreLogger = spdlog::stdout_color_mt("HAZEL"); // 创建一个彩色多线程记录器，并命名为HAZEL
		s_CoreLogger->set_level(spdlog::level::trace); // 设置用来打印信息的级别（trace：打印所有信息）

		s_ClientLogger = spdlog::stderr_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}