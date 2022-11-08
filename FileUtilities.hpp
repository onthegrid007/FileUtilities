#ifndef FILEUTILITIES_H_
#define FILEUTILITIES_H_

#include <fstream>
#include <shared_mutex>
#include <filesystem>

// namespace std {
//     #define WITH_LOCK_DEFINITION(func, rtn) \
//         template<typename ... Args> \
//         decltype(rtn) func(Args... args) { \
//             LType lock(m_mtx); \
//             this->T::func(args...); \
//             return rtn; \
//         }
//     template<typename T>
//     class ts_ostream : public T {
//         private:
//         typedef std::shared_mutex MType;
//         typedef std::lock_guard<MType> LType;
//         MType m_mtx;
        
//         public:
//         template<typename ... Args>
//         ts_ostream(Args... args) :
//             T(args...) {};
        
//         WITH_LOCK_DEFINITION(open, void())
//         WITH_LOCK_DEFINITION(close, void())
//         WITH_LOCK_DEFINITION(flush, void())
//         WITH_LOCK_DEFINITION(write, void())
//         WITH_LOCK_DEFINITION(operator<<, *this)
//     };
    
//     template<typename T>
//     class ts_istream : public T {
//         private:
//         typedef std::shared_mutex MType;
//         typedef std::lock_guard<MType> LType;
//         MType m_mtx;
        
//         public:
//         template<typename ... Args>
//         ts_istream(Args... args) :
//             T(args...) {};
        
//         WITH_LOCK_DEFINITION(open, void())
//         WITH_LOCK_DEFINITION(close, void())
//         WITH_LOCK_DEFINITION(flush, void())
//         WITH_LOCK_DEFINITION(read, *this)
//         WITH_LOCK_DEFINITION(operator>>, *this)
//     };
//     typedef ts_ostream<std::ofstream> ts_ofstream;
//     typedef ts_istream<std::ifstream> ts_ifstream;
// }

namespace FileUtilities {
    namespace fs = std::filesystem;
    enum PathType : char {
        FullPath = 0,
        FullName,
        NameOnly,
        SFXOnly
    };
    
    class ParsedPath {
        private:
        std::tuple<std::string, std::string, std::string> m_fullPath = {"", "", ""};
        
        public:
        const bool setPath(const std::string file) {
            fs::path absPath = fs::absolute(file);
            std::get<0>(m_fullPath) = absPath.parent_path();
            std::get<1>(m_fullPath) = absPath.stem();
            std::get<2>(m_fullPath) = absPath.extension();
            // std::cout << getPath() << std::endl;
            return fs::is_directory(std::get<0>(m_fullPath)) && fs::exists(absPath);
        }
        
        const std::string getPath(PathType type = PathType::FullPath) {
            switch(type) {
                case PathType::FullPath:
                    return std::get<0>(m_fullPath) + "/" + std::get<1>(m_fullPath) + std::get<2>(m_fullPath);
                break;
                case PathType::FullName:
                    return std::get<1>(m_fullPath) + std::get<2>(m_fullPath);
                break;
                case PathType::NameOnly:
                    return std::get<1>(m_fullPath);
                break;
                case PathType::SFXOnly:
                    return std::get<2>(m_fullPath);
                break;
                default:
                return "";
            };
        }
        
        operator std::string() {
            return getPath();
        };
        
        ParsedPath() {}

        ParsedPath(const std::string filepath) {
            setPath(filepath);
        } 
        
        ParsedPath(const char* filepath) {
            setPath(filepath);
        }
    };
}
#endif