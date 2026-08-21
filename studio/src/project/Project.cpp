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
#include <QPushButton>
#include <QButtonGroup>
#include <project/Project.h>

using namespace std;

Project::Project(const std::string& projectName) {
    name = QString::fromStdString(projectName);
    savingInCloud = false;  
}

Project::~Project() = default;

namespace ProjectManager {

    Project* onNewProject(QMainWindow* parent) {
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
        nameEdit->setMinimumHeight(2);

        formLayout->addRow("Project Name", nameEdit);

        QHBoxLayout* languageLayout = new QHBoxLayout();
        languageLayout->setSpacing(10);

        QPushButton* javaButton = new QPushButton("Java");
        QPushButton* kotlinButton = new QPushButton("Kotlin");

        javaButton->setCheckable(true);
        kotlinButton->setCheckable(true);
        kotlinButton->setChecked(true);

        QButtonGroup* languageGroup = new QButtonGroup(&dialog);
        languageGroup->setExclusive(true);
        languageGroup->addButton(javaButton);
        languageGroup->addButton(kotlinButton);

        languageLayout->addWidget(javaButton);
        languageLayout->addWidget(kotlinButton);
        languageLayout->addStretch();

        formLayout->addRow("Language", languageLayout);

        QComboBox* dimensionBox = new QComboBox();
        dimensionBox->addItem("3D");
        dimensionBox->addItem("2D");

        formLayout->addRow("Dimension", dimensionBox);

        mainLayout->addLayout(formLayout);
        mainLayout->addStretch();

        QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

        QObject::connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if (QPushButton* saveButton = buttonBox->button(QDialogButtonBox::Save)) {
            saveButton->setObjectName("saveButton");
            saveButton->setAutoDefault(false);
            saveButton->setDefault(false);
            saveButton->setText("Create");
        }

        if (QPushButton* cancelButton = buttonBox->button(QDialogButtonBox::Cancel)) {
            cancelButton->setObjectName("cancelButton");
        }

        mainLayout->addWidget(buttonBox);

        QFile file(":/styles.qss");
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            dialog.setStyleSheet(stream.readAll());
            file.close();
        }

        if (dialog.exec() == QDialog::Accepted) {
            std::string projectName = nameEdit->text().toStdString();
            //std::string projectPath = pathEdit->text().toStdString();

			Project* newProject = new Project(projectName);
			//newProject->savingInCloud = saveButtonCloud->isChecked();
			newProject->primaryLanguage = javaButton->isChecked() ? "Java" : "Kotlin";
            return newProject;
        }

        return nullptr;
    }

	void onOpenProject(QMainWindow* parent) {
		QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

		QString dir = QFileDialog::getOpenFileName(parent, "Open Project", path, "Engine Files (*.xml)");
		
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