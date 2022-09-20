#ifndef FILEUTILITIES_H_
#define FILEUTILITIES_H_

#include <fstream>
#include <shared_mutex>
#include <filesystem>

namespace std {
    #define WITH_LOCK_DEFINITION(func, rtn) \
        template<typename ... Args> \
        decltype(rtn) func(Args... args) { \
            LType lock(m_mtx); \
            this->T::func(args...); \
            return rtn; \
        }
    template<typename T>
    class ts_ostream : public T {
        private:
        typedef std::shared_mutex MType;
        typedef std::lock_guard<MType> LType;
        MType m_mtx;
        
        public:
        template<typename ... Args>
        ts_ostream(Args... args) :
            T(args...) {};
        
        WITH_LOCK_DEFINITION(open, void())
        WITH_LOCK_DEFINITION(close, void())
        WITH_LOCK_DEFINITION(flush, void())
        WITH_LOCK_DEFINITION(write, void())
        WITH_LOCK_DEFINITION(operator<<, *this)
    };
    
    template<typename T>
    class ts_istream : public T {
        private:
        typedef std::shared_mutex MType;
        typedef std::lock_guard<MType> LType;
        MType m_mtx;
        
        public:
        template<typename ... Args>
        ts_istream(Args... args) :
            T(args...) {};
        
        WITH_LOCK_DEFINITION(open, void())
        WITH_LOCK_DEFINITION(close, void())
        WITH_LOCK_DEFINITION(flush, void())
        WITH_LOCK_DEFINITION(read, *this)
        WITH_LOCK_DEFINITION(operator>>, *this)
    };
    typedef ts_ostream<std::ofstream> ts_ofstream;
    typedef ts_istream<std::ifstream> ts_ifstream;
}

namespace FileUtilities {
    namespace fs = std::filesystem;
    enum PathType : char {
        FullPath = 0,
        FullName,
        NameOnly,
        SFXOnly
    };
    
    class HLFileHandle {
        private:
        std::tuple<std::string, std::string, std::string> m_fullPath;
        
        public:
        const bool setPath(const std::string file) {
            fs::path absPath = fs::absolute(file);
            m_fullPath[0] = absPath.parent_path();
            m_fullPath[1] = absPath.stem();
            m_fullPath[2] = absPath.extention();
            return fs::is_directory(m_fullPath[0]) && fs::exists(absPath);
        }
        
        const std::string getPath(PathType type = PathType::FullPath) {
            switch(type) {
                case PathType::FullPath:
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
        
        HLFileHandle(std::string filepath) {
            setPath(filepath);
        }
    };
}

/*
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
        
        
        
        
        std::function<void()> m_onOpen;
        std::function<void()> m_onClose;
        std::fstream* m_fileStream;
        // std::shared_mutex* m_mtx;
        FileState m_state = FileState::Dead;
        
        
        
        
        
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
*/
#endif