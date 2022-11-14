#include "../vendor/PlatformDetection/PlatformDetection.h"
#include "FileDialogs.h"

namespace FileUtilities {
	namespace FileDialogs {
		#if _BUILD_PLATFORM_WINDOWS == 2
			#include <windows.h>
			#include <commdlg.h>
			constexpr auto nl_delim = "\r\n";
			constexpr auto parse_delim = "\0";
		#elif _BUILD_PLATFORM_LINUX == 1
			// #define ADVCLOCK_STATIC_INSTANCE_TYPE_OVERRIDE high_resolution_clock
			constexpr auto nl_delim = "\n";
			constexpr auto parse_delim = ";";
		#else
			#error "Unknown or Unsupported Platform!"
		#endif
		static std::vector<ParsedPath> Parse(std::string response) {
			std::vector<ParsedPath> rtn;
			size_t pos_s = 0, pos_e;
			std::string token;
			while((pos_e = response.find(parse_delim, pos_s)) != std::string::npos) {
				token = response.substr(pos_s, pos_e - pos_s);
				pos_s = pos_e + 1;
				rtn.emplace_back(token);
			}
			rtn.emplace_back(std::move(response.substr(pos_s)));
			return rtn;
		}
		
		static const std::vector<ParsedPath> Open(DialogProps props) {
			
		}
		
		static const ParsedPath Save(DialogProps props) {
			
		}
	}
}