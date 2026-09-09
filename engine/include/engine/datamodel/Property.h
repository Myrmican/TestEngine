#pragma once

#include <string>
#include <any>

namespace Engine {
	class Instance;

	class Property {
	public:
		std::string m_name;
		std::string m_category;

		Property(std::string, std::string category);

		virtual ~Property() = default;

		virtual std::any getValue(const Instance* instance) const = 0;
		virtual void setValue(Instance* instance, const std::any& value) const = 0;
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
        }

        std::any getValue(const Instance* instance) const override {
            auto typedInst = static_cast<const ClassType*>(instance);
            return (typedInst->*m_getter)();
        }

        void setValue(Instance* instance, const std::any& value) const override {
            auto typedInst = static_cast<ClassType*>(instance);
            (typedInst->*m_setter)(std::any_cast<ValueType>(value));
        }
    };
}