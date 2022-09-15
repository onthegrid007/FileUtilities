#ifndef FILEUTILITIES_H
#define FILEUTILITIES_H

#include <functional>
#include <string>
#include <string_view>
#include <fstream>
#include <shared_mutex>
#include <filesystem>
#include <sstream>

namespace FileUtilities {
    namespace fs = std::filesystem;
    #ifdef WCHAR_FILENAME_OVERRIDE
        typedef string_view<wchar> FileNameT;
    #else
        typedef std::string_view<wchar> FileNameT;
    #endif
    // template<int name, int sfx>
    class HLFileHandle {
        public:
        enum FileState : char {
            Dead = 0,
            Opening,
            OpenRO,
            OpenRW,
            OpenWO,
            Closing
        };
        
        enum PathType : char {
            Full = 0,
            FullName,
            NameOnly,
            SFXOnly
        };
        
        private:
        std::tuple<std::string, std::string, std::string> m_fullPath;
        std::function<void()> m_onOpen;
        std::function<void()> m_onClose;
        std::fstream* m_fileStream;
        // std::shared_mutex* m_mtx;
        FileState m_state = FileState::Dead;
        
        public:        
        const std::string getPath(PathType type = PathType::Full) {
            switch(type) {
                case PathType::Full:
                    return m_fullPath[0] + m_fullPath[1] + m_fullPath[2];
                break;
                case PathType::FullName:
                    return m_fullPath[1] + m_fullPath[2];
                break;
                case PathType::NameOnly:
                    return m_fullPath[1];
                break;
                case PathType::SFXOnly:
                    return m_fullPath[2];
                break;
                default:
                return "";
            };
        }
        
        bool decomposePath(std::string file) {
            fs::path absPath = fs::absolute(file);
            m_fullPath[0] = absPath.parent_path();
            m_fullPath[1] = absPath.stem();
            m_fullPath[2] = absPath.extention();
            return fs::is_directory(m_fullPath[0]);
        }
        
        HLFileHandle(std::string filePath, std::function<void()> onOpen = [](){}, std::function<void()> onClose = [](){}) :
        m_onOpen(onOpen),
        m_onClose(onClose) {
            if(decomposePath(filePath)) {
                m_onOpen();
            }
        }
        
        void setOnOpen(std::function<void()> onOpen = [](){}) {
            m_onOpen = onOpen;
        }
        
        void setOnClose(std::function<void()> onClose = [](){}) {
            m_onClose = onClose;
        }
        
        bool open(const FileNameT file, bool truncate = false) {
            if(m_state > FileState::Opening) {
                return reopen(truncate);
            }
        }
        
        bool reopen(bool truncate = false) {
            switch(m_state) {
                case FileState::Dead:
                    goto jmpOpen;
                case FileState::Opening:
                    return false;
                    break;
            };
            
            if(m_state)
            
            jmpOpen:
            return open(getPath());
        }
        
        bool flush() {
            if(m_state > FileState::OpenRO) {
                m_fileStream.flush();
                return true;
            }
            return false;
        }
        
        bool close() {
            if(m_state != FileState::Closing) {
                
            }
        }
        
        std::string getContentStr() {
            
        }
        
        template<typename T>
        bool write(const T& buffer) {
            
        }
    };
}

#endif