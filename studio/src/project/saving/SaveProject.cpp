#include <util/System.h>
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

	/*QHBoxLayout* checkboxLayout = new QHBoxLayout();
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
	checkboxLayout->addStretch();*/

	//formLayout->addRow("Save location", checkboxLayout);

	//QObject::connect(saveButtonLocal, &QCheckBox::toggled, dialog, [formLayout](bool checked) {
	//	formLayout->setRowVisible(pathContainer, checked);
	//	});

	mainLayout->addLayout(formLayout);
	mainLayout->addStretch();

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Save);

	QObject::connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
	QObject::connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);

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
		return true;
	}

	return false;
}

namespace SaveProject {
	bool initSave(Project* project, QWidget* parent) {
		if (project == nullptr) return false;

		Logger* logger = project->logger;

		/*if (!System::IsConnectedToNetwork()) {
			logger->Error("You are not connected to the internet.");
			return false;
		}*/

		bool needsSaveDialog = (project->projectPath.empty());

		if (needsSaveDialog) {
			bool isSaved = SaveDialog(project, parent);
			if (!isSaved) return false;

			QString projectPath = QString::fromStdString(project->projectPath);

			QFile* projectFile = File::CreateProjectFile(project->name, projectPath);

			project->projectPath = projectFile->fileName().toUtf8().constData();
		}

		logger->Info("Saving the current project...");

		logger->Info("Saved current changes.");

		return true;
	}
}