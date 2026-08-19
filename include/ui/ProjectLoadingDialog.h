#pragma once
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class ProjectLoadingDialog : public QDialog {
    Q_OBJECT
public:
    explicit ProjectLoadingDialog(const QString& projectName, QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("Loading Project");
        setFixedSize(400, 130);
        setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
        setModal(true);

        setStyleSheet(
            "QDialog { background-color: #1e1e1e; }"
            "QLabel { color: #f2f2f2; }"
        );

        auto* mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(16, 12, 16, 12);
        mainLayout->setSpacing(10);

        auto* headerLayout = new QHBoxLayout();
        auto* icon = new QLabel(this);
        //icon->setPixmap(QIcon(":/assets/icons/AppIcon.png").pixmap(24, 24));
        auto* nameLabel = new QLabel(projectName, this);
        nameLabel->setStyleSheet("font-weight: 600; font-size: 13px;");
        headerLayout->addWidget(icon);
        headerLayout->addWidget(nameLabel);
        headerLayout->addStretch();
        mainLayout->addLayout(headerLayout);

        auto* statusLayout = new QHBoxLayout();
        statusLabel = new QLabel("Creating project...", this);
        statusLabel->setStyleSheet("font-size: 12px; color: #cfcfcf;");
        statusLayout->addWidget(statusLabel);
        statusLayout->addStretch();
        mainLayout->addLayout(statusLayout);

        mainLayout->addStretch();

        auto* buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        auto* cancelButton = new QPushButton("Cancel", this);
        cancelButton->setStyleSheet(
            "QPushButton { background-color: #2f3039; color: #ffffff; border: none;"
            "padding: 6px 16px; border-radius: 4px; }"
            "QPushButton:hover { background-color: #3a3b46; }"
        );
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
        buttonLayout->addWidget(cancelButton);
        mainLayout->addLayout(buttonLayout);
    }

    void setStatus(const QString& text) { statusLabel->setText(text); }

private:
    QLabel* statusLabel;
};