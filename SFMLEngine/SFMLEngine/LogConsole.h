#pragma once
#include <imgui.h>
#include <imgui-SFML.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
using namespace std;
namespace Engine
{
	namespace Console
	{

		enum logType
		{
			error = 1,
			info,
			fatal,
			system,
			script,
			script_result
		};
		constexpr std::string_view logType_s[] =
		{
			 "all", "error", "info", "fatal", "system" ,"script", "script_result"
		};
		/*!
		\brief Structure that stores the log data
		\bug
		\warning
		\example
		\todo
		*/
		struct Log
		{
			Log(std::string s, logType t);
			std::string text;
			std::string pervText;
			void count_update(int count);
			int log_count;
			ImVec4 color;
			logType type;
		};

		/*!
		\brief The structure of which draws and adds the log
		\bug
		\warning
		\example
		\todo
		*/

		class AppLog
		{
		private:
			static bool ScrollToBottom; ///< Flag, true if the scrollbar
			static std::vector<std::string> current_input;
			static vector<Log> Buffer;

		public:
			AppLog() = default;
			~AppLog() { saveLog("Data/log.txt"); }
			static void Clear()
			{
				Buffer.clear();
				Buffer.shrink_to_fit();
			}
			static void addLog(Log log);
			static void addLog(std::string s, logType t);
			static void saveLog(std::string_view path);
			static void Draw(const char* title, bool* p_open);
		};
	} // namespace Console
}