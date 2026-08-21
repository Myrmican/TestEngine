#pragma once

#include <QString>
#include <QStandardPaths>
#include <filesystem>

namespace Asset {
    /**
     * @brief Extracts an embedded Qt resource file to a target disk path.
     * @param resourcePath Qt resource path (e.g., ":/embedded/jvm/jre.zip")
     * @param outputPath Target destination path on disk.
     * @param overwrite If true, replaces existing files at destination.
     * @return True if extraction succeeded.
     */
    bool extract(const QString& resourcePath, const QString& outputPath, bool overwrite = true);

    /**
     * @brief Extracts a resource file directly into the engine's temp folder.
     * @param resourcePath Qt resource path (e.g., ":/embedded/jvm/jre.zip")
     * @param relativeTempSubpath Destination relative to engine's temp directory.
     * @return Full path to the extracted file, or empty path on failure.
     */
    QString extractToTemp(const QString& resourcePath, const QString& relativeTempSubpath);

    /**
     * @brief Returns the root temp directory path for TestEngine.
     */
    QString getEngineTempDir();
}