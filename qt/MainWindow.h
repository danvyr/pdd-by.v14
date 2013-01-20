#pragma once

#include <QMainWindow>

class MainWindowImpl;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);

private:
    MainWindowImpl* m_impl;
};
