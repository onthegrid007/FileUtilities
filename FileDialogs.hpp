/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2024, Peter Ferranti
*   All rights reserved.
*/
#ifndef FILEDIALOGS_HPP_
#define FILEDIALOGS_HPP_

#include "vendor/PlatformDetection/PlatformDetection.h"
#include "FileUtilities.hpp"

namespace FileUtilities {
	namespace FileDialogs {
		#define Bit(x) (std::int32_t(1) << (x))
		typedef struct {
			enum Flags : std::int32_t {
				// Multi-Platform
				MULTI_SELECT = Bit(0),
				FORCE_READONLY = Bit(1),
				FORCE_WRITABLE = Bit(2),
				FILE_MUST_EXIST = Bit(3),
				PATH_MUST_EXIST = Bit(4),
				PROMPT_CREATE = Bit(5),
				PROMPT_OVERWRITE = Bit(6),
				DISABLE_CD = Bit(7),
				
				// Windows Only
				NATIVE_EXPLORER = Bit(8),
				SHOW_HIDDEN = Bit(9),
				ENABLE_RESIZING = Bit(10),
				DONT_ADD_TO_USER_RECENT = Bit(11),
				DONT_DEREFERENCE_LINKS = Bit(12),
				ALLOW_ALTERNATE_EXTENSIONS = Bit(13),
				HIDE_READONLY_CHECKBOX = Bit(14),
				HIDE_NETWORK_BUTTON = Bit(15),
				DISABLE_WRITE_PROTECTION = Bit(16),
				IGNORE_NETWORK_SHARE_VIOLATION = Bit(17)
			};
			std::string Title;
			std::string InitialPath;
			std::int32_t Flags;
			std::vector<std::string> EXTWhitelist;
			std::vector<std::string> EXTBlacklist;
		} DialogProps;
		static const std::vector<ParsedPath> Open(DialogProps props);
		static const ParsedPath Save(DialogProps props);
		#if _BUILD_PLATFORM_WINDOWS
			#include <windows.h>
			#include <commdlg.h>
			constexpr std::string_view nl_delim{"\r\n"};
			constexpr std::string_view parse_delim{"\0"};
		#elif _BUILD_PLATFORM_LINUX
			constexpr std::string_view nl_delim = "\n";
			constexpr std::string_view parse_delim = ";";
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
			std::vector<ParsedPath> rtn;
			
			return rtn;
		}
		
		static const ParsedPath Save(DialogProps props) {
			
		}
	}
}
#endif