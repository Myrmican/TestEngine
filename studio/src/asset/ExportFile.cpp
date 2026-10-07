#include <asset/ExportFile.h>
#include <engine/datamodel/Instance.h>
#include <QString>
#include <QFileDialog>
#include <QTreeWidget>

namespace Engine {
    void onSaveRequest(Instance* instance, QTreeWidget* parent) {
       QString filePath = QFileDialog::getSaveFileName(
            parent,
            "Save Instance to File",
            QString(),
            "Instance Files (*.xml);;All Files (*)"
        );

       if (filePath.isEmpty()) {
           return;
       }
	};

}