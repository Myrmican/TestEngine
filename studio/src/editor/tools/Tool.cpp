#include <editor/tools/Tool.h>
#include <QString>
#include <QToolBar>
#include <QPalette>
#include <QAbstractItemView>
#include <QListView>
#include <QPainter>

void applyComboStyle(QComboBox* comboBox, bool darkMode) {
    QPalette palette = comboBox->palette();

    QColor background;
    QColor hoverBackground;
    QColor textColor;
    QColor borderColor;
    if (darkMode) {
        background = QColor("#242424");
        hoverBackground = QColor("#3a3a3a");
        textColor = QColor("#d8d8d8");
        borderColor = QColor("#383838");
    }

    palette.setColor(QPalette::Window, background);
    palette.setColor(QPalette::Base, background);
    palette.setColor(QPalette::Button, background);
    palette.setColor(QPalette::ButtonText, textColor);
    palette.setColor(QPalette::WindowText, textColor);
    palette.setColor(QPalette::Text, textColor);
    palette.setColor(QPalette::Highlight, hoverBackground);
    palette.setColor(QPalette::HighlightedText, Qt::white);
    palette.setColor(QPalette::Mid, borderColor);

    comboBox->setPalette(palette);

    if (QAbstractItemView* popupView = comboBox->view()) {
        popupView->setPalette(palette);
        if (QWidget* popupWin = popupView->window()) {
            popupWin->setPalette(palette);
        }
    }
}

namespace Engine {
    StudioButtonTool::StudioButtonTool(QWidget* parent, std::string_view name) : QToolButton(parent) {
        this->setFixedSize(64, 64);

        //this->setIcon(icon);
        this->setIconSize(QSize(18, 18));

        this->setFont(QFont("Inter", 10, 500));

        this->setText(QString::fromStdString(std::string(name)));
        this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        this->setFocusPolicy(Qt::NoFocus);
        this->setCheckable(true);
    }

    StudioDropdownTool::StudioDropdownTool(QWidget* parent, std::string_view name) : QComboBox(parent) {
        this->setFixedSize(84, 28);
        this->setIconSize(QSize(20, 20));
        this->setFont(QFont("Inter", 10, QFont::Medium));

        auto* listView = new QListView(this);
        listView->setMouseTracking(true);
        listView->setFrameShape(QFrame::NoFrame);
        this->setView(listView);

        listView->setStyleSheet(
            "QListView {"
            "   background-color: rgb(30, 30, 30)"
            "}"
        );

        applyComboStyle(this, true);
    }

    StudioToolGroup::StudioToolGroup(QToolBar* parent, std::string_view name) : QActionGroup(parent) {

    }
}