#pragma once

#include <QWidget>
#include <QToolButton>
#include <string>
#include <QActionGroup>

class QToolBar;

namespace Engine {
	class StudioTool : public QToolButton {
		Q_OBJECT

	public:
		explicit StudioTool(QToolBar* parent = nullptr, std::string_view name = "");
	};

	class StudioToolGroup : public QActionGroup {
		Q_OBJECT

	public:
		explicit StudioToolGroup(QToolBar* parent = nullptr, std::string_view name = "");
	};
}