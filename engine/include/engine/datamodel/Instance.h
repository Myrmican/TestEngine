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

	using InstancePtr = std::shared_ptr<Instance>;

	class Instance : public std::enable_shared_from_this<Instance> {
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

		Event<std::string, std::any> changing;
		Event<std::string> changed;

		virtual void destroy();
		void removeAllChildren();

		std::string getPath() const;

		Instance* getParent() const { return parent; }

		void setParent(Instance* instance) { setParentInternal(instance, false); }
		void setLockedParent(Instance* instance) { setParentInternal(instance, true); }

		std::string_view getName() const { return name.get(); }
		virtual void setName(std::string_view value);

		const std::vector<InstancePtr>& getChildren() const { return children; }
		std::vector<Instance*> getDescendants();
		std::vector<Instance*> getDescendants(std::string_view selector);

		Instance* findChild(std::string) const;
		Instance* findChildClass(std::string) const;

		bool isAncestorOf(const Instance* descendant) const;
		bool isDescendantOf(const Instance* ancestor);

		InstancePtr clone();

		std::map<std::string, bool> getAttributes() { return attributes; }

		void setAttribute(const std::string& name, bool value);
		bool getAttribute(const std::string& name);

		std::string_view getClassName() const { return className; }

		static void BindAPI(WasmRuntime& wasm);
		static void properties(ClassDescriptor* desc);
	private:
		bool setParentInternal(Instance* instance, bool ignoreLock);

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