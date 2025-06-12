/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2025, Peter Ferranti
*   All rights reserved.
*/
#ifndef FILEUTILITIES_HPP_
#define FILEUTILITIES_HPP_

#include "vendor/PlatformDetection/PlatformDetection.h"
#include <filesystem>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <functional>
#include <atomic>
#include <mutex>
#include <iostream>

#if defined(_BUILD_PLATFORM_WINDOWS)
    #define NOMINMAX
    #include <windows.h>
    #include <direct.h>
    #define mkdir(path, code) _mkdir(path)
    #define _DELETEFILE(path) (::DeleteFileA(path) != 0)
#elif defined(_BUILD_PLATFORM_LINUX)
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>
    #define _DELETEFILE(path) (unlink(path) == 0)
#endif

namespace FileUtilities {
    namespace fs = std::filesystem;
    using DirContents = fs::directory_iterator;
    using DirContentsRecursive = fs::recursive_directory_iterator;
    using TimeType = std::filesystem::file_time_type;
    class ParsedPath;
    static ParsedPath FindSelfExe();
    
    class ParsedPath {
        private:
        std::tuple<std::string, std::string, std::string> m_fullPath = {"", "", ""};
        
        public:
        enum PathType : std::uint8_t {
            FullPath = 0,
            ParentDir,
            Abstract,
            FullName,
            NameOnly,
            SFXOnly
        };
        struct REL{};
        struct ABS{};

        const bool setPathRelative(const std::string path) {
            const fs::path absPath{fs::absolute(FindSelfExe().getPath(PathType::ParentDir) + "/" + path)};
            std::get<0>(m_fullPath) = absPath.parent_path().string();
            std::get<1>(m_fullPath) = absPath.stem().string();
            std::get<2>(m_fullPath) = absPath.extension().string();
            return fs::is_directory(std::get<0>(m_fullPath)) && fs::exists(absPath);
        }
        
        const bool setPath(const std::string path) {
            const fs::path absPath{fs::absolute(path)};
            std::get<0>(m_fullPath) = absPath.parent_path().string();
            std::get<1>(m_fullPath) = absPath.stem().string();
            std::get<2>(m_fullPath) = absPath.extension().string();
            return fs::is_directory(std::get<0>(m_fullPath)) && fs::exists(absPath);
        }
        
        const std::string getPath(PathType type = PathType::FullPath) const {
            switch(type) {
                case PathType::FullPath:
                    return std::get<0>(m_fullPath) + "/" + std::get<1>(m_fullPath) + std::get<2>(m_fullPath);
                break;
                case PathType::ParentDir:
                    return std::get<0>(m_fullPath);
                break;
                case PathType::Abstract:
                    return std::get<0>(m_fullPath) + "/" + std::get<1>(m_fullPath);
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
        }
        
        ParsedPath() {}

        ParsedPath(const std::string path, const ParsedPath::REL r) {
            setPathRelative(path);
        }
        
        ParsedPath(const std::string path, const ParsedPath::ABS a) {
            setPath(path);
        }
    };

    inline bool operator==(const ParsedPath& lhs, const ParsedPath& rhs) {
        return lhs.getPath() == rhs.getPath();
    }

    inline ParsedPath operator+(const ParsedPath& lhs, const std::string& rhs) {
        return {lhs.getPath() + rhs, ParsedPath::ABS{}};
    }

    inline std::ostream& operator<<(std::ostream& os, const ParsedPath& parsedPath) {
        os << parsedPath.getPath();
        return os;
    }

    static ParsedPath FindSelfExe() {
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
            if(!havePath) return {"", ParsedPath::ABS{}};
            return {fs::canonical(std::string(&buf[0])).string(), ParsedPath::ABS{}};
        #elif defined(_BUILD_PLATFORM_LINUX)
            return {fs::canonical("/proc/self/exe").string(), ParsedPath::ABS{}};
        #else
            return {"", ParsedPath::ABS{}};
        #endif
    }

    static inline bool FileExists(const ParsedPath& file) {
        struct stat buffer;
        return (stat(file.getPath().c_str(), &buffer) == 0);
    }

    static inline bool DeleteFile(const ParsedPath& file) {
        return _DELETEFILE(file.getPath().c_str());
    }

    static inline bool DirectoryExists(const ParsedPath& path) {
        struct stat info;
        if (stat(path.getPath().c_str(), &info) != 0) {
            return false;
        }
        else if (info.st_mode & S_IFDIR) {
            return true;
        }
        return false;
    }

    static inline bool MakeDirectory(const ParsedPath& path) {
        if (!DirectoryExists(path)) {
            if (mkdir(path.getPath().c_str(), 0755) == 0) return true;
            return false;
        }
        return true;
    }
}

namespace std {
    template<>
    struct hash<FileUtilities::ParsedPath> {
        std::size_t operator()(const FileUtilities::ParsedPath& obj) const {
            return std::hash<std::string>{}(obj.getPath());
        }
    };
}

namespace FileUtilities {
    class Watcher {
        public:
        typedef std::chrono::milliseconds DelayType;
        typedef std::function<void(const ParsedPath&)> CallbackType;
        enum Depth : bool {
            SHALLOW = false,
            RECURSIVE = true
        };

        Watcher(const ParsedPath& basePath, const CallbackType onCreate, const CallbackType onModify, const CallbackType onDelete, const Depth& depth, const bool rebuildStructure = true, const DelayType& delay = std::chrono::milliseconds(1000)) :
            m_running(true),
            m_rebuildStructure(rebuildStructure),
            m_basePath(basePath),
            m_onCreate(onCreate),
            m_onModify(onModify),
            m_onDelete(onDelete),
            m_depth(depth),
            m_delay(delay) {
            start(basePath, rebuildStructure);
        }
        
        void reload() {
            const auto reloadDelay{getDelay() * 2};
            setDelay(std::chrono::milliseconds(1));
            m_running = false;
            std::this_thread::sleep_for(reloadDelay);
            start(m_basePath, m_rebuildStructure);
        }

        void setDelay(const DelayType& delay = std::chrono::milliseconds(1000)) {
            m_delay = delay;
        }

        const DelayType getDelay() const {
            return m_delay;
        }

        private:
        std::atomic<bool> m_running;
        const bool m_rebuildStructure;
        const ParsedPath m_basePath;
        const CallbackType m_onCreate;
        const CallbackType m_onModify;
        const CallbackType m_onDelete;
        const Depth m_depth;
        std::atomic<DelayType> m_delay;

        void start(const ParsedPath& path, const bool& rebuildStructure) {
            m_running = true;
            if (rebuildStructure) {
                if (!MakeDirectory(path)) return;
            }

            if (m_depth) {
                for (const auto& content : DirContents(path.getPath())) {
                    if (content.is_directory()) {
                        start({content.path().string(), ParsedPath::ABS{}}, rebuildStructure);
                    }
                }
            }

            std::thread(
                [this, toWatch = path, rebuildStructure](){
                    std::unordered_map<ParsedPath, TimeType> m_times;
                    for (const auto& content : DirContents(toWatch.getPath()))
                        m_times[{content.path().string(), ParsedPath::ABS{}}] = std::filesystem::last_write_time(content);
                    
                    while(m_running) {
                        std::this_thread::sleep_for(m_delay.load());
                        if (!DirectoryExists(toWatch)) {
                            if (rebuildStructure && DirectoryExists({toWatch.getPath(ParsedPath::PathType::ParentDir), FileUtilities::ParsedPath::ABS{}})) MakeDirectory(toWatch);
                            else continue;
                        }

                        // Delete
                        auto it{m_times.begin()};
                        while (it != m_times.end()) {
                            if (!FileExists(it->first)) {
                                m_onDelete(it->first);
                                it = m_times.erase(it);
                            }
                            else {
                                it++;
                            }
                        }

                        for (const auto& content : DirContents(toWatch.getPath())) {
                            if (content.is_regular_file()) {
                                const auto lastModifyTime{fs::last_write_time(content)};
                                const ParsedPath pp{content.path().string(), ParsedPath::ABS{}};
                                const auto it{m_times.find(pp)};

                                // Create
                                if (it == m_times.end()) {
                                    m_times[pp] = lastModifyTime;
                                    m_onCreate(pp);
                                }
                                // Modify
                                else if (m_times[pp] != lastModifyTime) {
                                    m_times[pp] = lastModifyTime;
                                    m_onModify(pp);
                                }
                            }
                        }
                    }
                }
            ).detach();
        }
    };
}
#endif