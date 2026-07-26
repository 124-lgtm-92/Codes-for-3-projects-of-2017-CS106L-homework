/**********************************************************
 * File: mainwindow.h
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include "KDTree.h"
#include <QMainWindow>
using namespace std;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    QColorDialog* colorChooser;
    KDTree<3, string> lookup;

private slots:
    void handleColorChange(const QColor& color);
    void handleDataLoaded(int);
    void handleDoneIndexing();
};

#endif
