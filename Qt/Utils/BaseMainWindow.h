#ifndef BASEMAINWINDOW_H
#define BASEMAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QQuickWidget>
#include "QtBridge/QtCppBridge.h"

class BaseMainWindow : public QMainWindow
{
    Q_OBJECT
#ifdef __APPLE__
    void setWindowBorderColor(QColor windowBorderColor);
#endif
protected:
    QtCppBridge *cpp;
public:
    BaseMainWindow(QColor windowBorderColor);

    QQuickWidget *createQQuickWidget(const QString& qmlFile);
    QQuickWidget *createQQuickWidget(const QString& qmlFile, QWidget* parent);
};

#endif // BASEMAINWINDOW_H