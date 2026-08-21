#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <QObject>
#include <QEvent>
#include <project/Project.h>

class QTreeWidgetItem;

class Properties : public QObject {
    Q_OBJECT

public:
    QDockWidget* dockWidget;
    QTreeWidget* treeWidget;

    Properties(QMainWindow* window, Project* project);
    void AddProperty(const QString& category, const QString& property, const QString& propertyType, const QString& defaultValue);
    QTreeWidgetItem* GetOrCreateCategory(const QString& categoryName);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};