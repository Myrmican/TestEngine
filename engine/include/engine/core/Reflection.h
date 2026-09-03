#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

namespace Engine {

    class Createable;

    using FactoryFunc = std::function<std::unique_ptr<Createable>()>;

    inline auto& GetReflectionRegistry() {
        static struct {
            std::vector<std::string> names;
            std::unordered_map<std::string, FactoryFunc> factories;
        } registry;
        return registry;
    }

    inline std::vector<std::string>& GetCreateableClasses() {
        return GetReflectionRegistry().names;
    }

    inline std::unique_ptr<Createable> CreateInstance(const std::string& className) {
        auto& factories = GetReflectionRegistry().factories;
        auto it = factories.find(className);
        if (it != factories.end()) {
            return it->second();
        }
        return nullptr;
    }

    struct ReflectionHelper {
        ReflectionHelper(const std::string& className, FactoryFunc factory) {
            GetReflectionRegistry().names.push_back(className);
            GetReflectionRegistry().factories[className] = factory;
        }
    };

}

#define REGISTER_CLASS(className) \
    inline const ::Engine::ReflectionHelper reflection_##className( \
        #className, \
        []() -> std::unique_ptr<::Engine::Createable> { \
            return std::make_unique<className>(); \
        } \
    )