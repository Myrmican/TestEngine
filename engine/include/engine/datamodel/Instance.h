#pragma once

#include <scripting/WasmRuntime.h>

#include <vector>
#include <string>
#include <map>
#include "boost/weak_ptr.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"
#include <boost/flyweight.hpp>

class WasmRuntime;

namespace Engine {

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
	public:
		bool internalLocked = false;

		virtual ~Instance() = default;

		virtual void destroy();
		void remove();
		void removeAllChildren();

		std::string getPath() const;

		Instance* getParent() { return parent; }
		const Instance* getParent() const { return parent; }

		void setParent(Instance* instance) { setParentInternal(instance, false); }
		void setParent(Instance* instance, bool ignoreLock) { setParentInternal(instance, ignoreLock); }

		const std::string& getName() const { return name.get(); }
		virtual std::string_view setName(std::string_view value);

		const std::vector<InstancePtr>& getChildren() const { return children; }
		const std::vector<InstancePtr>& getDescendants();
		const std::vector<InstancePtr>& getDescendants(std::string selector);

		bool isAncestorOf(const Instance* descendant) const;

		bool isDescendantOf(const Instance* ancestor);

		std::shared_ptr<Instance> clone();

		std::map<std::string, bool> getAttributes() { return attributes; }

		void setAttribute(const std::string& name, bool value);
		bool getAttribute(const std::string& name);

		std::string getClassName() { return className; }

		static void BindAPI(WasmRuntime& wasm);
	private:
		void setParentInternal(Instance* instance, bool ignoreLock);
	};

	class Createable : public Instance {
	public:

		Createable(const std::string name);
	};
}