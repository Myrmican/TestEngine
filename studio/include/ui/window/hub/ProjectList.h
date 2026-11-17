#pragma once

class QVBoxLayout;
class QFrame;
class QString;
class QStackedWidget;

namespace MainMenu {
	void displayProjectsList(QVBoxLayout* contentContainer, QStackedWidget* workspaceStack);
	QFrame* createProjectRow(const QString& name, const QString& path, const QString& creator, const QString& publication, const std::time_t modified);
}