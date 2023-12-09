/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2023, Peter Ferranti
*   All rights reserved.
*/

#ifndef FILEUTILITIES_HPP_
#define FILEUTILITIES_HPP_

#include "vendor/STDExtras/vendor/ThreadPool/vendor/PlatformDetection/PlatformDetection.h"
#include <filesystem>
// #include <iostream>

namespace FileUtilities {
    namespace fs = std::filesystem;
    
    static const fs::path FindSelfExe() {
        #ifdef _BUILD_PLATFORM_WINDOWS
            return fs::canonical(GetModuleFileName(NULL));
        #endif
        #ifdef _BUILD_PLATFORM_LINUX
            return fs::canonical("/proc/self/exe");
        #endif
    }
    
    enum PathType : std::uint8_t {
        FullPath = 0,
        FullName,
        NameOnly,
        SFXOnly
    };
    
    class ParsedPath {
        private:
        std::tuple<std::string, std::string, std::string> m_fullPath = {"", "", ""};
        
        public:
        struct RELATIVE{};
        struct ABS{};
        const bool setPathRelative(const std::string file) {
            fs::path absPath = fs::absolute(std::string(FindSelfExe().parent_path()) + "/" + file);
            std::get<0>(m_fullPath) = absPath.parent_path();
            std::get<1>(m_fullPath) = absPath.stem();
            std::get<2>(m_fullPath) = absPath.extension();
            // std::cout << absPath << std::endl;
            return fs::is_directory(std::get<0>(m_fullPath)) && fs::exists(absPath);
        }
        
        const bool setPath(const std::string file) {
            fs::path absPath = fs::absolute(file);
            std::get<0>(m_fullPath) = absPath.parent_path();
            std::get<1>(m_fullPath) = absPath.stem();
            std::get<2>(m_fullPath) = absPath.extension();
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
        
        operator const std::string() {
            return getPath();
        };
        
        ParsedPath() {}

        ParsedPath(const std::string filepath, RELATIVE) {
            setPathRelative(filepath);
        }
        
        ParsedPath(const std::string filepath, ABS) {
            setPath(filepath);
        }
    };
}
#endif