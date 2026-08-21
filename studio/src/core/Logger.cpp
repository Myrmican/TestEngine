#include <string_view>
#include <chrono>
#include <format>
#include <iostream>
#include <QDateTime>
#include <core/Logger.h>
#include <ui/docks/Output.h>

using namespace std::chrono;

Logger::Logger(Output* parent) {
    this->outputDock = parent;
}

void Logger::Info(std::string_view message) {
    auto now = system_clock::now();

    QString logTimestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString logMessage = QString::fromUtf8(message.data(), static_cast<qsizetype>(message.size()));

    QString formattedLog = QString("<span style=\"color: gray;\">%1</span> %2")
        .arg(logTimestamp.toHtmlEscaped(), logMessage.toHtmlEscaped());

    outputDock->textEdit->append(formattedLog);
}

void Logger::Error(std::string_view message) {
    auto now = system_clock::now();

    QString logTimestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString logMessage = QString::fromUtf8(message.data(), static_cast<qsizetype>(message.size()));

    QString formattedLog = QString("<span style=\"color: gray;\">%1</span> <span style=\"color: #cc6666;\">%2</span>")
        .arg(logTimestamp.toHtmlEscaped(), logMessage.toHtmlEscaped());

    outputDock->textEdit->append(formattedLog);
}