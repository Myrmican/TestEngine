#pragma once

#include <scripting/WasmRuntime.h>

#include <vector>
#include <string>
#include "boost/weak_ptr.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"
#include <boost/static_assert.hpp>
#include <boost/flyweight.hpp>

class WasmRuntime;

using namespace boost;

namespace Engine {

	class Instance;

	struct ChildAdded {

	public:
		std::shared_ptr<Instance> const child;
		ChildAdded(Instance* child);
		ChildAdded(const ChildAdded& event);
	};

	typedef std::vector<shared_ptr<Instance> > Instances;

	class Instance {
	private:
		flyweight<std::string> name;

		shared_ptr<Instances> children;
		Instance* parent;
	protected:
		Instance();
		Instance(std::string name);

		virtual ~Instance();

	public:
		bool internalLocked = false;

		virtual void destroy();
		void remove();
		void removeAllChildren();

		std::string getPath() const;

		Instance* getParent() { return parent; }
		const Instance* getParent() const { return parent; }

		void setParent(Instance* instance) { setParentInternal(instance); }

		const std::string& getName() const { return name.get(); }
		virtual void setName(const std::string& value);

		shared_ptr<Instances> getChildren() const { return children; }

		bool isAncestorOf(const Instance* descendant) const {
			if (!descendant) return false;
			else if (descendant->getParent() == this) return true;

			return isAncestorOf(descendant->getParent());
		}

		bool isDescendantOf(const Instance* ancestor) {
			
		}

		shared_ptr<Instance> clone();

		static void BindAPI(WasmRuntime& wasm);
	private:
		bool setParentInternal(Instance* instance);
	};

	class Createable : public Instance {
	public:
		Createable(const std::string name);
	};
}