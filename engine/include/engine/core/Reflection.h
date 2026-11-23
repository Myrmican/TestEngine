#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <type_traits>
#include <datamodel/Instance.h>
#include <datamodel/ClassDescriptor.h>

namespace Engine {

    using FactoryFunc = std::function<InstancePtr()>;

    template <typename T>
    FactoryFunc MakeDefaultFactoryIfPossible() {
        if constexpr (std::is_default_constructible_v<T>) {
            return []() -> InstancePtr {
                return std::make_shared<T>();
                };
        }
        else {
            return nullptr;
        }
    }

    inline auto& GetReflectionRegistry() {
        static struct {
            std::vector<std::string> creatables;
            std::vector<std::string> classes;
            std::unordered_map<std::string, FactoryFunc> factories;
            std::unordered_map<std::string, std::unique_ptr<ClassDescriptor>> descriptors;
        } registry;
        return registry;
    }

    inline std::vector<std::string>& GetCreatableClasses() {
        return GetReflectionRegistry().creatables;
    }

    inline std::vector<std::string>& GetClasses() {
        return GetReflectionRegistry().classes;
    }

    inline InstancePtr CreateInstance(const std::string& className) {
		auto& factories = GetReflectionRegistry().factories;
        auto iterator = factories.find(className);
        if (iterator == factories.end()) return nullptr;

		return iterator->second();
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
            bool isEditorVisible = true,
            bool isCreatable = false
        ) {
            auto& reg = GetReflectionRegistry();
            reg.classes.push_back(className);

            if (factory) {
                reg.factories[className] = factory;

                if (isCreatable) {
                    reg.creatables.push_back(className);
                }
            }

            // Create ClassDescriptor and execute property reflections
            auto desc = std::make_unique<ClassDescriptor>(className, parentClassName, isEditorVisible);
            if (reflectFunc) {
                reflectFunc(desc.get());
            }

            reg.descriptors[className] = std::move(desc);
        }
    };
}

#define REGISTER_INSTANCE(className, parentClassName) \
    inline const ::Engine::ReflectionHelper reflection_##className( \
        #className, \
        #parentClassName, \
        ::Engine::MakeDefaultFactoryIfPossible<className>(), \
        [](::Engine::ClassDescriptor* desc) { className::properties(desc); }, \
        true, \
        false \
    )

#define REGISTER_CREATABLE(className, parentClassName) \
    inline const ::Engine::ReflectionHelper reflection_##className( \
        #className, \
        #parentClassName, \
        []() -> ::Engine::InstancePtr { \
            return std::make_shared<className>(); \
        }, \
        [](::Engine::ClassDescriptor* desc) { className::properties(desc); }, \
        true, \
        true \
    )

#define REGISTER_INTERNAL(className) \
    inline const ::Engine::ReflectionHelper reflection_##className( \
        #className, \
        "Instance", \
        []() -> ::Engine::InstancePtr { \
            return std::make_shared<className>(); \
        }, \
        [](::Engine::ClassDescriptor* desc) { className::properties(desc); }, \
        false, \
        false \
    )