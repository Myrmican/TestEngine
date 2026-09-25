#include <string_view>
#include <chrono>
#include <format>
#include <iostream>
#include <QDateTime>
#include <core/Logger.h>
#include <project/Project.h>
#include <ui/docks/Output.h>
#include <engine/services/logging/Logging.h>

using namespace std::chrono;

Logger::Logger(Output* parent, Project* project) {
    this->outputDock = parent;
    this->project = project;

    auto services = project->engine->getProvider()->m_services;

    Engine::Logging* logging = nullptr;

    for (const auto& [name, instance] : services) {
        if (name == "Logging") {
            logging = dynamic_cast<Engine::Logging*>(instance);
            break;
        }
    }

    logging->messageOut.connect([this](std::string_view message, Engine::LoggerMessageType type) {
        int messageLevel = static_cast<int>(type);
        switch (messageLevel) {
        case 0:
            Logger::Info(message);
            break;
        case 1:
            qDebug() << messageLevel;
            break;
        case 2:
            Logger::Error(message);
            break;
        }

        });
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