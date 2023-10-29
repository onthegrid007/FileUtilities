/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2023, Peter Ferranti
*   All rights reserved.
*/

#ifndef FILEUTILITIES_HPP_
#define FILEUTILITIES_HPP_

#include <filesystem>

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