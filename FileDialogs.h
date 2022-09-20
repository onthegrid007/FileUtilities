#ifndef FILEDIALOGS_H_
#define FILEDIALOGS_H_

#include <string>

namespace FileDialogs {
	typedef struct {
		enum Flags {
			MULTI_SELECT,
			PROMPT_CREATE,
			PROMPT_OVERWRITE,
			DONT_ADD_TO_USER_RECENT,
			DONT_DEREFERENCE_LINKS,
			ENABLE_RESIZING,
			NATIVE_EXPLORER,
			FILE_MUST_EXIST,
			ALLOW_ALTERNATE_EXTENSIONS,
			SHOW_HIDDEN,
			FORCE_WRITABLE,
			FORCE_READONLY,
			HIDE_READONLY_CHECKBOX,
			HIDE_NETWORK_BUTTON,
			DISABLE_CD,
			DISABLE_WRITE_PROTECTION,
			FORCE_PATH_MUST_EXIST,
			IGNORE_NETWORK_SHARE_VIOLATION
		};
		std::string Title;
		std::string StartPath;
		int Flags;
	} Props;
	static const std::string Open(Props props);
	static const std::string Save(Props props);
}

#endif