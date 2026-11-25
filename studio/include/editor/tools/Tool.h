#pragma once

#include <QWidget>
#include <QToolButton>
#include <string>
#include <QActionGroup>
#include <QComboBox>

class QToolBar;

namespace Engine {
	class StudioButtonTool : public QToolButton {
		Q_OBJECT

	public:
		explicit StudioButtonTool(QWidget* parent = nullptr, std::string_view name = "");
	};

	class StudioDropdownTool : public QComboBox {
	public:
		explicit StudioDropdownTool(QWidget* parent = nullptr, std::string_view name = "");
	};

	class StudioToolGroup : public QActionGroup {
		Q_OBJECT

	public:
		explicit StudioToolGroup(QToolBar* parent = nullptr, std::string_view name = "");
	};
}