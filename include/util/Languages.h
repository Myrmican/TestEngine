#include <yaml-cpp/yaml.h>
#include <QStringList>
#include <QString>
#include <QDebug>
#include <QFileInfo>
#include <string>

QStringList getExtensionsForLanguage(const std::string& languageName);

QString getLanguageFromFilePath(const QString& filePath);