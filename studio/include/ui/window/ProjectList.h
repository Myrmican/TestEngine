#pragma once

class QVBoxLayout;
class QFrame;
class QString;

namespace MainMenu {
	void displayProjectsList(QVBoxLayout* contentContainer);
	QFrame* createProjectRow(const QString& name, const QString& path, const QString& creator, const QString& publication, const QString& modified);
}