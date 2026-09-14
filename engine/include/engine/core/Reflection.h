#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <datamodel/Instance.h>
#include <datamodel/ClassDescriptor.h>

namespace Engine {

    using FactoryFunc = std::function<std::unique_ptr<Instance>()>;

    inline auto& GetReflectionRegistry() {
        static struct {
            std::vector<std::string> names;
            std::unordered_map<std::string, FactoryFunc> factories;
            std::unordered_map<std::string, std::unique_ptr<ClassDescriptor>> descriptors;
        } registry;
        return registry;
    }

    inline std::vector<std::string>& GetCreatableClasses() {
        return GetReflectionRegistry().names;
    }

    inline std::unique_ptr<Creatable> CreateInstance(const std::string& className) {
        auto& factories = GetReflectionRegistry().factories;
        auto it = factories.find(className);
        if (it != factories.end()) {
            std::unique_ptr<Instance> inst = it->second();

            return std::unique_ptr<Creatable>(static_cast<Creatable*>(inst.release()));
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
        ReflectionHelper(
            const std::string& className,
            const std::string& parentClassName,
            FactoryFunc factory,
            std::function<void(ClassDescriptor*)> reflectFunc,
            bool isEditorVisible = true
        ) {
            auto& reg = GetReflectionRegistry();

            // 1. Store factory for instantiation
            if (factory) {
                reg.names.push_back(className);
                reg.factories[className] = factory;
            }

            // 2. Fetch parent ClassDescriptor if it exists
            ClassDescriptor* parentDesc = parentClassName.empty() ? nullptr : GetClassDescriptor(parentClassName);

            // 3. Create ClassDescriptor and execute property reflections
            auto desc = std::make_unique<ClassDescriptor>(className, parentDesc, isEditorVisible);
            if (reflectFunc) {
                reflectFunc(desc.get());
            }

            reg.descriptors[className] = std::move(desc);
        }
    };

}

#define REGISTER_INSTANCE(className) \
    inline const ::Engine::ReflectionHelper reflection_##className( \
        #className, \
        "Instance", \
        nullptr, \
        [](::Engine::ClassDescriptor* desc) { className::reflectProperties(desc); }, \
        true \
    )

#define REGISTER_CREATABLE(className) \
    inline const ::Engine::ReflectionHelper reflection_##className( \
        #className, \
        "Instance", \
        []() -> std::unique_ptr<::Engine::Creatable> { \
            return std::make_unique<className>(); \
        }, \
        [](::Engine::ClassDescriptor* desc) { className::reflectProperties(desc); }, \
        true \
    )

#define REGISTER_INTERNAL(className) \
    inline const ::Engine::ReflectionHelper reflection_##className( \
        #className, \
        "Instance", \
        []() -> std::unique_ptr<::Engine::Instance> { \
            return std::make_unique<className>(); \
        }, \
        [](::Engine::ClassDescriptor* desc) { className::reflectProperties(desc); }, \
        false \
    )