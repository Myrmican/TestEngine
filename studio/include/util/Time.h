#pragma once

#include <QDateTime>
#include <QString>

namespace TimeUtils {
    QString formatTime(std::time_t time) {
        QDateTime currentTime = QDateTime::currentDateTime();
        QDateTime pastTime = QDateTime::fromSecsSinceEpoch(time);

        qint64 secs = pastTime.secsTo(currentTime);

        if (secs < 60) {
            return QString::number(secs) + (secs == 1 ? " second ago" : " seconds ago");
        }
        else if (secs < 3600) {
            qint64 val = secs / 60;
            return QString::number(val) + (val == 1 ? " minute ago" : " minutes ago");
        }
        else if (secs < 86400) {
            qint64 val = secs / 3600;
            return QString::number(val) + (val == 1 ? " hour ago" : " hours ago");
        }
        else if (secs < 604800) {
            qint64 val = secs / 86400;
            return QString::number(val) + (val == 1 ? " day ago" : " days ago");
        }
        else if (secs < 2419200) {
            qint64 val = secs / 604800;
            return QString::number(val) + (val == 1 ? " week ago" : " weeks ago");
        }
        else if (secs < 29030400) {
            qint64 val = secs / 2419200;
            return QString::number(val) + (val == 1 ? " month ago" : " months ago");
        }
        else {
            qint64 val = secs / 29030400;
            return QString::number(val) + (val == 1 ? " year ago" : " years ago");
        }
    }
}