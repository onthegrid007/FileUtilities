/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2023, Peter Ferranti
*   All rights reserved.
*/
#ifndef FILEDIALOGS_HPP_
#define FILEDIALOGS_HPP_

#include "vendor/STDExtras/vendor/ThreadPool/vendor/PlatformDetection/PlatformDetection.h"
#include "FileUtilities.hpp"
#include "vendor/STDExtras/STDExtras.hpp"

namespace FileUtilities {
	namespace FileDialogs {
		typedef struct {
			enum Flags : std::I32 {
				// Multi-Platform
				MULTI_SELECT = BIT(0),
				FORCE_READONLY = BIT(1),
				FORCE_WRITABLE = BIT(2),
				FILE_MUST_EXIST = BIT(3),
				PATH_MUST_EXIST = BIT(4),
				PROMPT_CREATE = BIT(5),
				PROMPT_OVERWRITE = BIT(6),
				DISABLE_CD = BIT(7),
				
				// Windows Only
				NATIVE_EXPLORER = BIT(8),
				SHOW_HIDDEN = BIT(9),
				ENABLE_RESIZING = BIT(10),
				DONT_ADD_TO_USER_RECENT = BIT(11),
				DONT_DEREFERENCE_LINKS = BIT(12),
				ALLOW_ALTERNATE_EXTENSIONS = BIT(13),
				HIDE_READONLY_CHECKBOX = BIT(14),
				HIDE_NETWORK_BUTTON = BIT(15),
				DISABLE_WRITE_PROTECTION = BIT(16),
				IGNORE_NETWORK_SHARE_VIOLATION = BIT(17)
			};
			std::string Title;
			std::string InitialPath;
			std::I32 Flags;
			std::vector<std::string> EXTWhitelist;
			std::vector<std::string> EXTBlacklist;
		} DialogProps;
		static const std::vector<ParsedPath> Open(DialogProps props);
		static const ParsedPath Save(DialogProps props);
		#if _BUILD_PLATFORM_WINDOWS == 2
			#include <windows.h>
			#include <commdlg.h>
			constexpr auto nl_delim = "\r\n";
			constexpr auto parse_delim = "\0";
		#elif _BUILD_PLATFORM_LINUX == 1
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
#endif