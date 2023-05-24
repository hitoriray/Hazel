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
		// ���ü�¼��Ϣ��ģʽ
		spdlog::set_pattern("%^[%T] %n: %v%$"); // ʱ���  ��־������(core or cilent) ʵ�ʵ���Ϣ
		s_CoreLogger = spdlog::stdout_color_mt("HAZEL"); // ����һ����ɫ���̼߳�¼����������ΪHAZEL
		s_CoreLogger->set_level(spdlog::level::trace); // ����������ӡ��Ϣ�ļ���trace����ӡ������Ϣ��

		s_ClientLogger = spdlog::stderr_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}