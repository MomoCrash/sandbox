#pragma once

#include <chrono>
#include <string>
#include <unordered_map>

namespace US {
    struct Task
    {
        std::string Name;
        std::chrono::high_resolution_clock::time_point Start;

        Task(std::string name, std::chrono::high_resolution_clock::time_point start) : Name(name), Start(start) {}
    };

    class Profiler
    {
    public:
        Profiler();
        static void NewTask(std::string id, std::string message = "");
        static void EndTask(std::string id);
    private:
        static std::unordered_map<std::string, Task> m_tasks;
    };
}

