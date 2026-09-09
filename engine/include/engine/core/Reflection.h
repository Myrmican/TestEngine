#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <datamodel/Instance.h>
#include <datamodel/ClassDescriptor.h>

namespace Engine {

    using FactoryFunc = std::function<std::unique_ptr<Engine::Createable>()>;

    inline auto& GetReflectionRegistry() {
        static struct {
            std::vector<std::string> names;
            std::unordered_map<std::string, FactoryFunc> factories;
            std::unordered_map<std::string, std::unique_ptr<ClassDescriptor>> descriptors;
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

    inline ClassDescriptor* GetClassDescriptor(const std::string& className) {
        auto& descriptors = GetReflectionRegistry().descriptors;
        auto it = descriptors.find(className);
        if (it != descriptors.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    struct ReflectionHelper {
        ReflectionHelper(const std::string& className, const std::string& parentClassName, FactoryFunc factory, std::function<void(ClassDescriptor*)> reflectFunc) {
            auto& reg = GetReflectionRegistry();

            // 1. Store factory for instantiation
            if (factory) {
                reg.names.push_back(className);
                reg.factories[className] = factory;
            }

            // 2. Fetch parent ClassDescriptor if it exists
            ClassDescriptor* parentDesc = parentClassName.empty() ? nullptr : GetClassDescriptor(parentClassName);

            // 3. Create ClassDescriptor and execute property reflections
            auto desc = std::make_unique<ClassDescriptor>(className, parentDesc);
            if (reflectFunc) {
                reflectFunc(desc.get());
            }

            reg.descriptors[className] = std::move(desc);
        }
    };

}

#define REGISTER_CLASS(className) \
    inline const ::Engine::ReflectionHelper reflection_##className( \
        #className, \
        "Instance", \
        nullptr, \
        [](::Engine::ClassDescriptor* desc) { className::reflectProperties(desc); } \
    )

#define REGISTER_CLASS_DERIVED(className, parentClassName) \
    inline const ::Engine::ReflectionHelper reflection_##className( \
        #className, \
        #parentClassName, \
        []() -> std::unique_ptr<::Engine::Createable> { return std::make_unique<className>(); }, \
        [](::Engine::ClassDescriptor* desc) { className::reflectProperties(desc); } \
    )