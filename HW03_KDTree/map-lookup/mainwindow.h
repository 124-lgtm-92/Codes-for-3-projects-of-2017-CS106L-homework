/**********************************************************
 * File: mainwindow.h
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * Defines the main window for the Map Lookup program.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <map>
#include "../KDTree.h"
using namespace std;

/* Forward-declare the class responsible for displaying the world map. */
class PictureDisplay;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  PictureDisplay* worldMapPic;   // The widget that draws the earth.
  KDTree<2, string> kd;          // The kd-tree that does 1-NN lookup.
  map<string, string> geoLookup; // Mapping from FIPS 10-4 codes to place names

private slots:
  void onMapClick(const QPoint& where); // Triggered when the user clicks on the map.
};

/* This code is based on an example from
 *
 * http://www.vision.ee.ethz.ch/computing/sepp-irix/qt-2.3.1-mo/picture-picture-cpp.html
 *
 * However, it contains numerous fixes to make it work correctly in our context.
 */
class PictureDisplay : public QWidget {
  Q_OBJECT

public:
  explicit PictureDisplay(const string& filename);

protected:
  void paintEvent(QPaintEvent *);     // Draw the world
  void mousePressEvent(QMouseEvent*); // Report the click back to the main window

private:
  QImage pict;                        // The world map

signals:
  void clicked(const QPoint& where);  // Emitted on a click
};

#endif // MAINWINDOW_H
