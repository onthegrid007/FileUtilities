#ifndef FILEDIALOGS_H_
#define FILEDIALOGS_H_

#include "vendor/STDExtras/STDExtras.hpp"
#include "FileUtilities.hpp"

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
	}
}

#endif