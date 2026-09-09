#pragma once

#include <scripting/WasmRuntime.h>
#include <core/Event.h>
#include <vector>
#include <string>
#include <map>
#include <any>
#include "boost/weak_ptr.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"
#include <boost/flyweight.hpp>

class WasmRuntime;

namespace Engine {

	class ClassDescriptor;

	class Instance;

	using InstancePtr = std::shared_ptr<Instance>;
	using InstanceWeakPtr = std::weak_ptr<Instance>;

	struct ChildAdded {

	public:
		std::shared_ptr<Instance> const child;
		ChildAdded(Instance* child);
		ChildAdded(const ChildAdded& event);
	};

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

		Event<std::string, std::any> changed;

		virtual void destroy();
		void remove();
		void removeAllChildren();

		std::string getPath() const;

		Instance* getParent() { return parent; }
		const Instance* getParent() const { return parent; }

		void setParent(Instance* instance) { setParentInternal(instance, false); }
		void setParent(Instance* instance, bool ignoreLock) { setParentInternal(instance, ignoreLock); }

		std::string_view getName() const { return name.get(); }
		virtual void setName(std::string_view value);

		const std::vector<InstancePtr>& getChildren() const { return children; }
		const std::vector<InstancePtr>& getDescendants();
		const std::vector<InstancePtr>& getDescendants(std::string_view selector);

		bool isAncestorOf(const Instance* descendant) const;

		bool isDescendantOf(const Instance* ancestor);

		std::shared_ptr<Instance> clone();

		std::map<std::string, bool> getAttributes() { return attributes; }

		void setAttribute(const std::string& name, bool value);
		bool getAttribute(const std::string& name);

		std::string getClassName() { return className; }

		static void BindAPI(WasmRuntime& wasm);

		static void reflectProperties(ClassDescriptor* desc);
	private:
		void setParentInternal(Instance* instance, bool ignoreLock);

		void collectDescendants(Instance* current, std::vector<InstancePtr>& out) {
			for (const InstancePtr& child : current->getChildren()) {
				out.push_back(child);
				collectDescendants(child.get(), out);
			}
		}
	};

	class Createable : public Instance {
	public:

		Createable(const std::string name);
	};
}