#include <Environment.h>
#include <project/Project.h>
#include <core/Logger.h>
#include <util/File.h>
#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QFileDialog>
#include <QObject>
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QLabel>

bool SaveDialog(Project* project, QWidget* parent) {
	QDialog* dialog = new QDialog(parent);
	dialog->setWindowTitle("Save Project");
	dialog->setModal(true);
	dialog->resize(500, 500);

	QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
	mainLayout->setContentsMargins(20, 20, 20, 20);
	mainLayout->setSpacing(25);

	QFormLayout* formLayout = new QFormLayout();
	formLayout->setLabelAlignment(Qt::AlignLeft);
	formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

	QHBoxLayout* checkboxLayout = new QHBoxLayout();
	checkboxLayout->setSpacing(15);

	QCheckBox* saveButtonCloud = new QCheckBox("Cloud");
	QCheckBox* saveButtonLocal = new QCheckBox("Local");

	QButtonGroup* optionsGroup = new QButtonGroup(dialog);
	optionsGroup->setExclusive(true);

	optionsGroup->addButton(saveButtonCloud);
	optionsGroup->addButton(saveButtonLocal);

	saveButtonLocal->setChecked(true);

	checkboxLayout->addWidget(saveButtonCloud);
	checkboxLayout->addWidget(saveButtonLocal);
	checkboxLayout->addStretch();

	formLayout->addRow("Save location", checkboxLayout);

	QWidget* pathContainer = new QWidget(dialog);
	QHBoxLayout* pathLayout = new QHBoxLayout(pathContainer);
	pathLayout->setContentsMargins(0, 0, 0, 0);
	pathLayout->setSpacing(0);

	QLineEdit* pathEdit = new QLineEdit(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
	QPushButton* browseButton = new QPushButton("Browse...");

	pathLayout->addWidget(pathEdit);
	pathLayout->addWidget(browseButton);

	formLayout->addRow("Project Path", pathContainer);

	QObject::connect(browseButton, &QPushButton::clicked, dialog, [pathEdit, &dialog]() {
		QString dir = QFileDialog::getExistingDirectory(dialog, "Select Project Directory", pathEdit->text());
		if (!dir.isEmpty()) {
			pathEdit->setText(dir);
		}
		});

	QObject::connect(saveButtonLocal, &QCheckBox::toggled, dialog, [formLayout, pathContainer](bool checked) {
		formLayout->setRowVisible(pathContainer, checked);
		});

	mainLayout->addLayout(formLayout);
	mainLayout->addStretch();

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

	QObject::connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
	QObject::connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

	if (QPushButton* saveButton = buttonBox->button(QDialogButtonBox::Save)) {
		saveButton->setObjectName("saveButton");
		saveButton->setAutoDefault(false);
		saveButton->setDefault(false);
		saveButton->setText("Save");
	}

	if (QPushButton* cancelButton = buttonBox->button(QDialogButtonBox::Cancel)) {
		cancelButton->setObjectName("cancelButton");
	}

	mainLayout->addWidget(buttonBox);

	if (dialog->exec() == QDialog::Accepted) {
		std::string projectPath = pathEdit->text().toStdString();
		project->projectPath = projectPath;
		return true;
	}

	return false;
}

namespace SaveProject {
	bool initSave(Project* project, QWidget* parent) {
		if (project == nullptr) return false;

		Logger* logger = project->logger;

		if (!System::IsConnectedToNetwork()) {
			logger->Error("You are not connected to the internet.");
			return false;
		}

		//Check if the project has already been saved, true for now - run if it hasn't
		bool savedProject;

		if (true) {
			savedProject = SaveDialog(project, parent);
			if (!savedProject) return false;

			QString projectPath = QString::fromStdString(project->projectPath);

			QFile* projectFile = File::CreateProjectFile(project->name, projectPath);
			project->projectFile = projectFile;
		}

		logger->Info("Saving the current project...");

		logger->Info("Saved current changes.");

		return true;
	}
}