#include <yaml-cpp/yaml.h>
#include <QStringList>
#include <QString>
#include <QDebug>
#include <QFileInfo>
#include <string>

QStringList getExtensionsForLanguage(const std::string& languageName) {
    QStringList extensions;

    try {
        YAML::Node config = YAML::LoadFile(LANGUAGES_YML_PATH);

        if (config[languageName] && config[languageName]["extensions"]) {
            for (const auto& ext : config[languageName]["extensions"]) {
                extensions.append(QString::fromStdString(ext.as<std::string>()));
            }
        }
    }
    catch (const YAML::Exception& e) {
        qWarning() << "YAML Error:" << e.what();
    }

    return extensions;
}

QString getLanguageFromFilePath(const QString& filePath) {
    QFileInfo fileInfo(filePath);
    std::string fileExt = "." + fileInfo.suffix().toStdString();

    try {
        YAML::Node config = YAML::LoadFile(LANGUAGES_YML_PATH);

        for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
            std::string langName = it->first.as<std::string>();
            YAML::Node langNode = it->second;

            if (langNode["extensions"]) {
                for (const auto& extNode : langNode["extensions"]) {
                    if (extNode.as<std::string>() == fileExt) {
                        return QString::fromStdString(langName);
                    }
                }
            }
        }
    }
    catch (const YAML::Exception& e) {
        qWarning() << "YAML Parsing Error:" << e.what();
    }

    return "Unknown";
}