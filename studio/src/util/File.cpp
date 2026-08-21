#include <QFile>
#include <QString>
#include <iostream>
#include <QDebug>

namespace File {
	QFile* CreateProjectFile(QString name, QString dir) {
		QFile* file = new QFile(dir + "/" + name + ".xml");

		if (!file->open(QIODevice::WriteOnly | QIODevice::Text)) {
			qWarning() << "Failed to create file:" << file->errorString();
		}

		return file;
	}
}
