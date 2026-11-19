#include <string>
#include <QString>
#include <QFileDialog>
#include <QMainWindow>
#include <QStandardPaths>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include <QComboBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QStackedWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QTimer>
#include <QApplication>
#include <editor/EditorWindow.h>
#include <project/Project.h>
#include <ui/ProjectLoadingDialog.h>
#include <engine/serialization/PlaceDeserializer.h>
#include <boost/algorithm/string.hpp>
#include <memory>

Project::Project(const QFileInfo fileInfo, const QString projectName) {
    name = projectName.isEmpty() ? fileInfo.baseName() : projectName;

    try {
        engine = std::make_unique<Engine::EngineInstance>();
        engine->init();

        scriptEngine = std::make_unique<Engine::ScriptEngine>(engine->getDataModel());

        QFile file(fileInfo.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly)) {
			std::cout << "Failed to open file: " << fileInfo.absoluteFilePath().toStdString() << std::endl;
        }

        const QByteArray rawData = file.readAll();

		Engine::PlaceDeserializer::loadFromFile(std::string_view(rawData.constData(), static_cast<size_t>(rawData.size())), engine->getDataModel());
    } catch (const std::exception& e) {
        qCritical() << "Failed to initialize EngineInstance:" << e.what();
    }
}

Project::~Project() = default;

namespace ProjectManager {
    Project* ProjectManager::createNew(QMainWindow* parent) {
        QDialog dialog(parent);
        dialog.setWindowTitle("Create Project");
        dialog.setMinimumSize(480, 250);
        dialog.setModal(true);

        QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);
        mainLayout->setContentsMargins(20, 20, 20, 20);
        mainLayout->setSpacing(25);

        QFormLayout* formLayout = new QFormLayout();
        formLayout->setLabelAlignment(Qt::AlignLeft);
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

        QLineEdit* nameEdit = new QLineEdit("Untitled Game");
        formLayout->addRow("Project Name", nameEdit);

        QWidget* pathContainer = new QWidget(&dialog);
        QHBoxLayout* pathLayout = new QHBoxLayout(pathContainer);
        pathLayout->setContentsMargins(0, 0, 0, 0);
        pathLayout->setSpacing(0);

        QLineEdit* pathEdit = new QLineEdit(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
        QPushButton* browseButton = new QPushButton("Browse...");

        pathLayout->addWidget(pathEdit);
        pathLayout->addWidget(browseButton);

        formLayout->addRow("Project Path", pathContainer);

        QComboBox* dimensionBox = new QComboBox();
        dimensionBox->addItem("3D");
        dimensionBox->addItem("2D");
        formLayout->addRow("Dimension", dimensionBox);

        QPushButton* importRobloxButton = new QPushButton(&dialog);
        importRobloxButton->setText("Import RBXLX");
        formLayout->addRow(importRobloxButton);

        mainLayout->addLayout(formLayout);
        mainLayout->addStretch();

        QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

        QString selectedRobloxFile;

        QObject::connect(browseButton, &QPushButton::clicked, &dialog, [pathEdit, &dialog]() {
            QString dir = QFileDialog::getExistingDirectory(&dialog, "Select Project Directory", pathEdit->text());
            if (!dir.isEmpty()) {
                pathEdit->setText(dir);
            }
            });

        QObject::connect(importRobloxButton, &QPushButton::pressed, [&dialog, parent, &selectedRobloxFile]() {
            QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
            QString filePath = QFileDialog::getOpenFileName(parent, "Select Place File", path, "Roblox Place (*.rbxlx)");
            if (!filePath.isEmpty()) {
                selectedRobloxFile = filePath;
                dialog.accept();
            }
            });

        QObject::connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if (QPushButton* saveButton = buttonBox->button(QDialogButtonBox::Save)) {
            saveButton->setText("Create");
        }

        mainLayout->addWidget(buttonBox);

        nameEdit->setFocus();

        if (dialog.exec() != QDialog::Accepted) {
            return nullptr;
        }

        if (!selectedRobloxFile.isEmpty()) {
            QFileInfo fileInfo(selectedRobloxFile);
            return new Project(fileInfo);
        }

        QFileInfo fileInfo("studio/content/Place1.rbxlx");
        Project* newProject = new Project(fileInfo, nameEdit->text().trimmed());
        newProject->is2D = dimensionBox->currentText() == "2D";

        std::string projectPath = pathEdit->text().toUtf8().constData();
        newProject->projectPath = boost::trim_copy(projectPath);

        return newProject;
    }

	Project* openFile(QMainWindow* parent) {
		QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

		QString filePath = QFileDialog::getOpenFileName(parent, "Open Project", path, "Engine Files (*.tep);;Roblox Place (*.rbxlx)");
        if (filePath.isEmpty()) return nullptr;

        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.completeBaseName();

        QFile* projectFile = new QFile(filePath);
		
        Project* newProject = new Project(fileInfo);
        return newProject;
	}

    Project* openProject(QString filePath, QStackedWidget* workspaceStack) {
		QFileInfo fileInfo(filePath);

		QMainWindow* window = qobject_cast<QMainWindow*>(workspaceStack->parentWidget());

		Project* newProject = new Project(fileInfo);

        auto loadingDialog = new ProjectLoadingDialog(newProject->name, window);
        loadingDialog->setAttribute(Qt::WA_DeleteOnClose);
        loadingDialog->show();

        QTimer::singleShot(0, window, [window, workspaceStack, newProject, loadingDialog]() {
            loadingDialog->setStatus("Building editor...");
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

            new Engine::EditorWindow(newProject, workspaceStack->widget(1), window);

            workspaceStack->setCurrentIndex(1);
            loadingDialog->close();
            });

        return newProject;
    }

    Project* getProject(QWidget* contextWidget) {
        QWidget* current = contextWidget;

        while (current) {
            QVariant prop = current->property("projectInstance");
            if (prop.isValid()) {
                return static_cast<Project*>(prop.value<void*>());
            }
            current = current->parentWidget();
        }

        return nullptr;
    }
}