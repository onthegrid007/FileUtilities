#include "FileDialogs.h"

namespace FileUtilities {
	namespace FileDialogs {
		#ifdef _WIN32
			#include <windows.h>
			#include <commdlg.h>
			// #include <GLFW/glfw3.h>
			// #define GLFW_EXPOSE_NATIVE_WIN32
			// #include <GLFW/glfw3native.h>
		#elif defined(__APPLE__) || defined(__MACH__)
			#include <TargetConditionals.h>
			#if TARGET_IPHONE_SIMULATOR == 1
				#error "IOS simulator is not supported!"
			#elif TARGET_OS_IPHONE == 1
				#error "IOS is not supported!"
			#elif TARGET_OS_MAC == 1
				#error "MacOS is not supported!"
			#else
				#error "Unknown Apple platform!"
			#endif
		#elif defined(__ANDROID__)
			#error "Android is not supported!"
		#elif defined(__linux__)
			static std::vector<HLFileHandle> Parse(std::string response) {
				constexpr char delimiter = ';';
				std::vector<HLFileHandle> rtn;
				size_t pos_s = 0, pos_e;
				std::string token;

				while((pos_e = response.find(delimiter, pos_s)) != std::string::npos) {
					token = response.substr(pos_s, pos_e - pos_s);
					pos_s = pos_e + 1;
					rtn.emplace_back(token);
				}

				rtn.emplace_back(std::move(response.substr(pos_s)));
				return rtn;
			}
			
			static const std::vector<HLFileHandle> Open(DialogProps props) {
				
			}
			
			static const HLFileHandle Save(DialogProps props) {
				
			}
		#else
			#error "Unknown platform!"
		#endif
	}
}