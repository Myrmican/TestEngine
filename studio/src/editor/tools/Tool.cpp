#include <editor/tools/Tool.h>
#include <QString>
#include <QToolBar>

namespace Engine {
	StudioTool::StudioTool(QToolBar* parent, std::string_view name) : QToolButton(parent) {
        this->setFixedSize(64, 64);

        //this->setIcon(icon);
        this->setIconSize(QSize(18, 18));

        this->setFont(QFont("Inter", 10, 500));

        this->setText(QString::fromStdString(std::string(name)));
        this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        this->setFocusPolicy(Qt::NoFocus);
        this->setCheckable(true);
        
        parent->addWidget(this);
    }

    StudioToolGroup::StudioToolGroup(QToolBar* parent, std::string_view name) : QActionGroup(parent) {

    }
}