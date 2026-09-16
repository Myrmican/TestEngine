#pragma once

#include <scripting/WasmRuntime.h>
#include <core/Event.h>
#include <vector>
#include <string>
#include <map>
#include <any>
#include <memory>
#include <boost/flyweight.hpp>

class WasmRuntime;

namespace Engine {

	class ClassDescriptor;
	class Instance;

	using InstancePtr = std::unique_ptr<Instance>;

	class Instance {
	private:
		boost::flyweight<std::string> name;

		std::vector<InstancePtr> children;
		Instance* parent;

		std::string className;
		std::map<std::string, bool> attributes;
	protected:
		Instance(std::string name);

		virtual void onChildAdded(Instance* child) {};
	public:
		bool internalLocked = false;

		virtual ~Instance() = default;

		Event<std::string, std::any> changed;

		virtual void destroy();
		void remove();
		void removeAllChildren();

		std::string getPath() const;

		Instance* getParent() const { return parent; }

		// Reparent an instance that ALREADY belongs to some parent's children vector.
		void setParent(Instance* instance) { setParent(instance, false); }
		void setParent(Instance* instance, bool ignoreLock);

		// Take ownership of a freshly created (parentless) instance.
		void addChild(InstancePtr child);

		std::string_view getName() const { return name.get(); }
		virtual void setName(std::string_view value);

		const std::vector<InstancePtr>& getChildren() const { return children; }
		std::vector<Instance*> getDescendants();
		std::vector<Instance*> getDescendants(std::string_view selector);

		Instance* findChild(std::string) const;
		Instance* findChildClass(std::string) const;

		bool isAncestorOf(const Instance* descendant) const;
		bool isDescendantOf(const Instance* ancestor);

		std::unique_ptr<Instance> clone();

		std::map<std::string, bool> getAttributes() { return attributes; }

		void setAttribute(const std::string& name, bool value);
		bool getAttribute(const std::string& name);

		std::string_view getClassName() const { return className; }

		static void BindAPI(WasmRuntime& wasm);
		static void reflectProperties(ClassDescriptor* desc);
	private:
		void setParentInternal(Instance* instance, bool ignoreLock);

		void collectDescendants(Instance* current, std::vector<Instance*>& out) {
			for (const InstancePtr& child : current->getChildren()) {
				out.push_back(child.get());
				collectDescendants(child.get(), out);
			}
		}
	};

	class Creatable : public Instance {
	public:
		Creatable(const std::string name);
	};
}