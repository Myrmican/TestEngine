#pragma once

#include <QTreeWidget>

namespace Engine {
	class Instance;

	void onSaveRequest(Instance* instance, QTreeWidget* parent);

}