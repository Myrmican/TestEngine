#pragma once

#include <editor/tools/Tool.h>

namespace Engine::Tools {
	class Scene : public StudioDropdownTool {
	public:
		Scene(QWidget* parent) : StudioDropdownTool(parent, "Scene") {

			this->setFixedWidth(100);

			this->addItem("Main");

			this->insertSeparator(this->count());

			//TODO: Make this text look disabled if the user has no connection-A tooltip telling them they need a connection
			this->addItem("Create scene");

			connect(this, QComboBox::currentIndexChanged, this, [=](int index) {
				 
				});
		};
	};
}