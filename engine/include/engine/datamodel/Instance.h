#pragma once

#include <string>

class Instance;

struct ChildAdded {

public:
	shared_ptr<Instance> const child;
	ChildAdded(Instance* child);
	ChildAdded(const ChildAdded& event);
};

class Instance {
private:
	std::string name;
	Instance* parent;
protected:
	Instance();
	Instance(const char* name);

	virtual ~Instance();

public:
	void destroy();
	void removeAllChildren();

	std::string getPath() const;

	Instance* getParent() { return parent; }
	const Instance* getParent() { return parent; }

	void setParent(Instance* instance) { setParentInternal(instance) };

	shared_ptr<Instance> getChildren() const { return children; }

	bool isAncestorOf(const Instance* descendant) const {
		if (descendant == NULL) {
			return false;
		}
		else if (descendant->getParent() == this) {
			return true;
		}
		else {
			return descendant->isAncestorOf(descendant->getParent());
		}
	}

	shared_ptr<Instance> clone();

private:
	bool setParentInternal(Instance* instance);
};