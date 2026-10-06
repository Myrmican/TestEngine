#include <editor/file/FileManager.h>
#include <editor/file/CodeEditor.h>
#include <editor/file/FileEditor.h>
#include <engine/datamodel/Instance.h>
#include <QString>

namespace Engine {
	namespace FileManager {
		void openFile(Engine::Instance* file, QTabWidget* documentTabs) {
			for (int i = 0; i < documentTabs->count(); ++i) {
				QWidget* widget = documentTabs->widget(i);
				auto openInstance = widget->property("targetInstance").value<Engine::Instance*>();
				if (openInstance == file) {
					documentTabs->setCurrentIndex(i);
					return;
				}
			}

			//FileEditor* fileEditor = new FileEditor(documentTabs);
			CodeEditor* fileEditor = new CodeEditor(documentTabs);
			fileEditor->setProperty("targetInstance", QVariant::fromValue(file));

			QString instanceName = QString::fromStdString(std::string(file->getName()));

			int newTabIndex = documentTabs->addTab(fileEditor, instanceName);
			documentTabs->setTabToolTip(newTabIndex, QString::fromStdString(file->getPath()));
			documentTabs->setCurrentIndex(newTabIndex);

			

			/*QObject::connect(file, &Instance::nameChanged, fileEditor, [documentTabs, fileEditor](const std::string& newName) {
				int idx = documentTabs->indexOf(fileEditor);
				if (idx != -1) {
				    documentTabs->setTabText(idx, QString::fromStdString(newName));
				}
			});*/

			/*QObject::connect(file, &Instance::aboutToBeDestroyed, fileEditor, [documentTabs, fileEditor]() {
				int idx = documentTabs->indexOf(fileEditor);
				if (idx != -1) {
					documentTabs->removeTab(idx);
					fileEditor->deleteLater();
				}
			});*/
		}
	}
}