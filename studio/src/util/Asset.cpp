#include <util/Asset.h>
#include <QFileInfo>
#include <QDir>

namespace Asset {

	bool extract(const QString& resourcePath, const QString& outputPath, bool overwrite) {
		QFileInfo fileInfo(outputPath);
		QDir().mkpath(fileInfo.absolutePath());

		if (QFile::exists(outputPath)) {
			if (!overwrite) {
				return true;
			}
			QFile::remove(outputPath);
		}

		if (QFile::copy(resourcePath, outputPath)) {
			QFile::setPermissions(outputPath, QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ReadUser | QFileDevice::WriteUser);
			return true;
		}
		else {
			qWarning("Failed to extract resource: %s to %s", qUtf8Printable(resourcePath), qUtf8Printable(outputPath));
			return false;
		}
	}
}