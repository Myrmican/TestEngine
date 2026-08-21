#pragma comment(lib, "dwmapi.lib")

#include <QApplication>
#include <QFile>
#include <QWindow>
#include <QMainWindow>
#include <QMenuBar>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QStackedWidget>
#include <QUrl>
#include <QVBoxLayout>
#include <QWKWidgets/widgetwindowagent.h>
#include <ui/menus/FileMenu.h>
#include <ui/menus/PluginsMenu.h>
#include <ui/menus/HelpMenu.h>
#include <ui/menus/ViewMenu.h>
#include <ui/menus/EditMenu.h>
#include <ui/menus/TestMenu.h>
#include <ui/menus/WindowMenu.h>

#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <windows.h>
#endif

QIcon makeRoundIcon(const QIcon& originalIcon, int size) {
    QPixmap srcPixmap = originalIcon.pixmap(originalIcon.actualSize(QSize(512, 512)));
    QPixmap squarePixmap = srcPixmap.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPixmap targetPixmap(size, size);
    targetPixmap.fill(Qt::transparent);

    QPainter painter(&targetPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPainterPath path;
    path.addEllipse(0, 0, size, size);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, squarePixmap);

    return QIcon(targetPixmap);
}

void createLeftDropdownsMain(QMenuBar* menuBar, QMainWindow* window, QStackedWidget* workspaceStack, QWidget* editorPage) {
    new FileMenu(menuBar, window, workspaceStack, editorPage);
    new PluginsMenu(menuBar);
	new HelpMenu(menuBar);
}

void createLeftDropdownsEditor(QMenuBar* menuBar, QMainWindow* window, QStackedWidget* workspaceStack, QWidget* editorPage) {
    new FileMenu(menuBar, window, workspaceStack, editorPage);
    new EditMenu(menuBar);
    new ViewMenu(menuBar, window);
    new PluginsMenu(menuBar);
	new TestMenu(menuBar);
    new WindowMenu(menuBar);
	new HelpMenu(menuBar);
}

namespace Window {
    void initialize(QMainWindow* window, QApplication& app) {
        auto* agent = new QWK::WidgetWindowAgent(window);
        agent->setup(window);

        auto* titleBarPanel = new QWidget(window);
        titleBarPanel->setObjectName("TitleBarPanel");
        titleBarPanel->setFixedHeight(30);

        auto* titleLayout = new QHBoxLayout(titleBarPanel);
        titleLayout->setContentsMargins(30, 0, 0, 0);
        titleLayout->setSpacing(0);

        auto* workspaceStack = new QStackedWidget(window);
        workspaceStack->addWidget(new QWidget(workspaceStack));
        auto* editorPage = new QWidget(workspaceStack);
        workspaceStack->addWidget(editorPage);

        auto* leftMenuToolBars = new QStackedWidget(titleBarPanel);
        titleLayout->addWidget(leftMenuToolBars, 0, Qt::AlignVCenter);

        auto createSubMenuBar = [&](auto setupFunc) {
            auto* menuBar = new QMenuBar(leftMenuToolBars);
            menuBar->setObjectName("TitleMenuBar");
            menuBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
            setupFunc(menuBar, window, workspaceStack, editorPage);
            leftMenuToolBars->addWidget(menuBar);
            agent->setHitTestVisible(menuBar, true);
            };

        createSubMenuBar(createLeftDropdownsMain);
        createSubMenuBar(createLeftDropdownsEditor);

        QObject::connect(workspaceStack, &QStackedWidget::currentChanged, leftMenuToolBars, &QStackedWidget::setCurrentIndex);

        auto* rightMenuBar = new QMenuBar(titleBarPanel);
        rightMenuBar->setObjectName("TitleMenuBar");
        rightMenuBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        rightMenuBar->addMenu(QIcon(":/assets/icons/bell.png"), "");

        QMenu* userProfileMenu = rightMenuBar->addMenu(makeRoundIcon(QIcon(":/assets/icons/profileTest.png"), 30), "");
        userProfileMenu->setObjectName("UserProfileMenu");
        userProfileMenu->addAction("Add account");
        userProfileMenu->addAction("Log out");

        titleLayout->addWidget(rightMenuBar, 0, Qt::AlignVCenter);
        titleLayout->addSpacing(20);

        QFont sysIconFont("Segoe Fluent Icons", 10, QFont::Thin);
        auto createSysBtn = [&](const QString& text, const QString& objName, QWK::WindowAgentBase::SystemButton type) {
            auto* btn = new QPushButton(text, titleBarPanel);
            btn->setObjectName(objName);
            btn->setFont(sysIconFont);
            btn->setFixedSize(43, 30);
            titleLayout->addWidget(btn);
            agent->setSystemButton(type, btn);
            return btn;
            };

        auto* minBtn = createSysBtn("\uE921", "minButton", QWK::WindowAgentBase::Minimize);
        auto* maxBtn = createSysBtn("\uE923", "maxButton", QWK::WindowAgentBase::Maximize);
        auto* closeBtn = createSysBtn("\uE8BB", "closeButton", QWK::WindowAgentBase::Close);

        agent->setSystemButton(QWK::WindowAgentBase::Minimize, minBtn);
        agent->setSystemButton(QWK::WindowAgentBase::Maximize, maxBtn);
        agent->setSystemButton(QWK::WindowAgentBase::Close, closeBtn);

        QObject::connect(minBtn, &QPushButton::clicked, window, &QMainWindow::showMinimized);
        QObject::connect(maxBtn, &QPushButton::clicked, [window, maxBtn]() {
            if (window->isMaximized()) {
                window->showNormal();
            }
            else {
                window->showMaximized();
            }
            });
        QObject::connect(closeBtn, &QPushButton::clicked, window, &QMainWindow::close);

        if (QWindow* nativeWindow = window->windowHandle()) {
            QObject::connect(nativeWindow, &QWindow::windowStateChanged, maxBtn, [maxBtn](Qt::WindowState state) {
                if (state & Qt::WindowMaximized) {
                    maxBtn->setText("\uE923");
                }
                else {
                    maxBtn->setText("\uE922");
                }
                });
        }

        window->setMenuWidget(titleBarPanel);
        window->setCentralWidget(workspaceStack);

        agent->setHitTestVisible(leftMenuToolBars, true);
        agent->setHitTestVisible(rightMenuBar, true);
        agent->setTitleBar(titleBarPanel);

        if (QFile file(":/style.qss"); file.open(QFile::ReadOnly | QFile::Text)) {
            app.setStyleSheet(QTextStream(&file).readAll());
        }
    }
}