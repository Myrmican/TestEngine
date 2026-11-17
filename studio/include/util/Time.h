#pragma once

#include <QDateTime>
#include <QString>

namespace TimeUtils {
	QString formatTime(std::time_t time) {
		QDateTime currenTime = QDateTime::currentDateTime();
		QDateTime pastTime = QDateTime::fromSecsSinceEpoch(time);

		qint64 secs = pastTime.secsTo(currenTime);

		if (secs < 60) {
			return QString::number(secs) + " seconds ago";
		}
		else if (secs < 3600) {
			return QString::number(secs / 60) + " minutes ago";
		}
		else if (secs < 86400) {
			return QString::number(secs / 3600) + " hours ago";
		}
		else if (secs < 604800) {
			return QString::number(secs / 86400) + " days ago";
		}
		else if (secs < 2419200) {
			return QString::number(secs / 604800) + " weeks ago";
		}
		else if (secs < 29030400) {
			return QString::number(secs / 2419200) + " months ago";
		}
		else {
			return QString::number(secs / 29030400) + " years ago";
		}

		//Return the date if none of the above conditions are met
		return pastTime.toString("MM/dd/yyyy hh:mm");
	}
}