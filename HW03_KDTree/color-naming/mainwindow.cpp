/**********************************************************
 * File: mainwindow.cpp
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 */

#include "mainwindow.h"
#include <QStatusBar>
#include <QCoreApplication>
#include <QMessageBox>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

static string IntegerToString(int val) {
  stringstream converter;
  converter << val;
  return converter.str();
}

/***** MainWindow Implementation *****/
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(QString("Color Lookup"));

    /* 颜色选择器 */
    colorChooser = new QColorDialog();
    colorChooser->setOptions(QColorDialog::NoButtons | QColorDialog::DontUseNativeDialog);
    setCentralWidget(colorChooser);

    connect(colorChooser, SIGNAL(currentColorChanged(QColor)), this, SLOT(handleColorChange(QColor)));

    statusBar()->setSizeGripEnabled(false);
    statusBar()->showMessage(tr("Loading colors..."));

    /* ------------- 单线程加载（修复所有线程错误）------------- */
    try {
        ifstream input("colors.txt", ios::binary);
        if (!input) throw runtime_error("Cannot open colors.txt");

        size_t count;
        if (!(input >> count)) throw runtime_error("File format error");
        input.ignore(1);

        size_t read = 0;
        while (true) {
            char colorBuffer[3];
            input.read(colorBuffer, 3);

            char bufferSize = input.get();
            size_t toRead = (unsigned char)bufferSize;

            char nameBuffer[256];
            input.read(nameBuffer, (streamsize)toRead);

            if (!input) break;

            Point<3> pt;
            pt[0] = (unsigned char)colorBuffer[0];
            pt[1] = (unsigned char)colorBuffer[1];
            pt[2] = (unsigned char)colorBuffer[2];

            lookup.insert(pt, string(nameBuffer, nameBuffer + toRead));

            if (++read % 10000 == 0)
                statusBar()->showMessage(("Loaded " + IntegerToString(read) + " points...").c_str());
        }

        if (read != count) throw runtime_error("Incomplete file");

        statusBar()->showMessage("Ready");

    } catch (const exception& e) {
        QMessageBox::critical(0, "Error", e.what());
        QCoreApplication::quit();
    }
}

MainWindow::~MainWindow() {
}

void MainWindow::handleColorChange(const QColor& c) {
    Point<3> colorVector;
    colorVector[0] = c.red();
    colorVector[1] = c.green();
    colorVector[2] = c.blue();

    statusBar()->showMessage(QString(lookup.kNNValue(colorVector, 3).c_str()));
}

void MainWindow::handleDataLoaded(int) {}
void MainWindow::handleDoneIndexing() {}
