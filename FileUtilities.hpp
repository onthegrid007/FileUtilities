/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2024, Peter Ferranti
*   All rights reserved.
*/
#ifndef FILEUTILITIES_HPP_
#define FILEUTILITIES_HPP_

#include "vendor/PlatformDetection/PlatformDetection.h"
#include <filesystem>
#include <string>

namespace FileUtilities {
    namespace fs = std::filesystem;
    static const fs::path FindSelfExe() {
        #if defined(_BUILD_PLATFORM_WINDOWS)
            std::vector<char> buf(1024, 0);
            auto size{buf.size()};
            bool havePath{false};
            bool shouldContinue{true};
            while(shouldContinue) {
                const auto result{GetModuleFileNameA(nullptr, &buf[0], size)};
                const auto lastError{GetLastError()};
                if(result == 0)
                    shouldContinue = false;
                else if(result < size) {
                    havePath = true;
                    shouldContinue = false;
                }
                else if((result == size) && ((lastError == ERROR_INSUFFICIENT_BUFFER) || (lastError == ERROR_SUCCESS)))
                    buf.resize(size *= 2);
            }
            if(!havePath) return "";
            return fs::canonical(std::string(&buf[0]));
        #elif defined(_BUILD_PLATFORM_LINUX)
            return fs::canonical("/proc/self/exe");
        #else
            return "";
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
        struct REL{};
        struct ABS{};
        const bool setPathRelative(const std::string file) {
            fs::path absPath = fs::absolute(FindSelfExe().parent_path().generic_string() + "/" + file);
            std::get<0>(m_fullPath) = absPath.parent_path().generic_string();
            std::get<1>(m_fullPath) = absPath.stem().generic_string();
            std::get<2>(m_fullPath) = absPath.extension().generic_string();
            return fs::is_directory(std::get<0>(m_fullPath)) && fs::exists(absPath);
        }
        
        const bool setPath(const std::string file) {
            fs::path absPath = fs::absolute(file);
            std::get<0>(m_fullPath) = absPath.parent_path().generic_string();
            std::get<1>(m_fullPath) = absPath.stem().generic_string();
            std::get<2>(m_fullPath) = absPath.extension().generic_string();
            return fs::is_directory(std::get<0>(m_fullPath)) && fs::exists(absPath);
        }
        
        const std::string getPath(PathType type = PathType::FullPath) const {
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
        
        operator const std::string() const {
            return getPath();
        };
        
        ParsedPath() {}

        ParsedPath(const std::string filepath, ParsedPath::REL r) {
            setPathRelative(filepath);
        }
        
        ParsedPath(const std::string filepath, ParsedPath::ABS a) {
            setPath(filepath);
        }
    };
}
#endif