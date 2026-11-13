#pragma once

class QGridLayout;
class QFrame;
class QString;

namespace MainMenu {
	void displayProjectsGrid(QGridLayout* contentContainer);
	QFrame* createProjectCard(const QString& name, const QString& path, const QString& creator, const QString& publication, const QString& modified);
}