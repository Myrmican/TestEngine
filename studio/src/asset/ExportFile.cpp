#include <asset/ExportFile.h>
#include <engine/datamodel/Instance.h>
#include <QString>
#include <QFileDialog>
#include <QTreeWidget>
#include <QStandardPaths>

namespace Engine {
    void onSaveRequest(Instance* instance, QTreeWidget* parent) {
        QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        QString filePath = QFileDialog::getSaveFileName(
            parent,
            "Save Instance to File",
            path,
            "Instance Files (*.xml);;All Files (*)"
        );

        if (filePath.isEmpty()) {
            return;
        }

        bool success = false; //Call engine method that uses native C++ to serialize the instance in a file at the path Qt gave us

        if (!success) {

        }
	};

}