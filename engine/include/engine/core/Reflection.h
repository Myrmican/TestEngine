#pragma once
#include <vector>
#include <string>

namespace Engine {

    inline std::vector<std::string>& GetCreateableClasses() {
        static std::vector<std::string> classes;
        return classes;
    }

    struct ReflectionHelper {
        ReflectionHelper(const std::string& className) {
            GetCreateableClasses().push_back(className);
        }
    };

}

#define REGISTER_CLASS(className) \
    inline const ::Engine::ReflectionHelper reflection_##className(#className)