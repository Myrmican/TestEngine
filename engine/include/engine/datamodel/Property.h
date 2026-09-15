#pragma once

#include <core/math/Color3.h>
#include <core/math/Vector3.h>
#include <string>
#include <variant>
#include <any>

namespace Engine {
	class Instance;

    using PropertyValue = std::variant<
        std::monostate,
        std::string,
        std::string_view,
        Instance*,
        Color3,
        Vector3,
        float,
        int,
        bool
    >;

	class Property {
	public:
		std::string m_name;
        std::string m_category;
        bool readOnly = false;

		Property(std::string, std::string category);

		virtual ~Property() = default;

		virtual Engine::PropertyValue getValue(const Instance* instance) const = 0;
		virtual void setValue(Instance* instance, const std::any& value) const = 0;
        virtual std::string getFormatted(const Instance* instance) const;
	};

    template<typename ClassType, typename ValueType>
    class TypedProperty : public Property {
    public:
        using Getter = ValueType(ClassType::*)() const;
        using Setter = void (ClassType::*)(ValueType);

        Getter m_getter;
        Setter m_setter;

        TypedProperty(std::string name, std::string category, Getter getter, Setter setter)
            : Property(std::move(name), std::move(category)), m_getter(getter), m_setter(setter) {
            readOnly = m_setter == NULL;
        }

        Engine::PropertyValue getValue(const Instance* instance) const override {
            auto typedInst = static_cast<const ClassType*>(instance);

            if constexpr (std::is_convertible_v<ValueType, Engine::Instance*>) {
                return static_cast<Engine::Instance*>((typedInst->*m_getter)());
            }
            else if constexpr (std::is_constructible_v<Engine::PropertyValue, ValueType>) {
                return (typedInst->*m_getter)();
            }
            else {
                return std::monostate{};
            }
        }

        void setValue(Instance* instance, const std::any& value) const override {
            if (!m_setter) return;

            auto typedInst = static_cast<ClassType*>(instance);
            (typedInst->*m_setter)(std::any_cast<ValueType>(value));
        }
    };
}