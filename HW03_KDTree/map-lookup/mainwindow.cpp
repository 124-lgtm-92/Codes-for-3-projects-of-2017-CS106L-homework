/**********************************************************
 * File: mainwindow.cpp
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * Implementation of the window and widget classes for the
 * Map Lookup program.  Comments have been added to the
 * relevant sections.
 */

#include "mainwindow.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <fstream>
#include <sstream>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QStatusBar>
#include <QCoreApplication>
#include <QMessageBox>
using namespace std;

/***** Module Constants and Functions *****/

static const int kImageHeight = 513;  // Dimensions of the image
static const int kImageWidth  = 1024;

static const double kPi = 3.14159265358979323; // Pi,  used in coordinate transforms

/* Converts a click from a point in a window to a point in the unit box. */
static Point<2> GetNormalizedClickLocation(const QPoint& where) {
  /* Normalize everything into [-1, 1] x [-1, 1] */
  Point<2> result;
  result[0] = (where.x() - kImageWidth / 2)  / double(kImageWidth / 2);
  result[1] = (where.y() - kImageHeight / 2) / double(kImageHeight / 2);
  return result;
}

/* This code is based on math from Wolfram MathWorld at
 *
 * http://mathworld.wolfram.com/MollweideProjection.html
 *
 * This converts from a click on the world map to a concrete
 * longitude and latitude.
 */
static void InvertMollwideProjection(Point<2>& location) {
  /* Compute theta = arcsin(y) */
  double theta = asin(location[1]);

  /* Longitude is pi x / cos(theta) */
  double longitude = kPi * location[0] / cos(theta);

  /* Latitude is arcsin((2theta + sin(2theta)) / pi).  However, our coordinate axis
   * is flipped since y grows down, so we negate the result.
   */
  double latitude = -asin((2.0 * theta + sin(2.0 * theta)) / kPi);

  /* Convert from rads to degrees. */
  longitude *= 180.0 / kPi;
  latitude  *= 180.0 / kPi;

  /* Update the vector. */
  location[0] = longitude;
  location[1] = latitude;
}

/* Given a FIPS 10-4 name (i.e. US04), return the display name of that location
 * (i.e Arizona, United States).
 */
static string NameForLocation(const string& name, const map<string, string>& geoCodes) {
  /* Case one: We know what this is. */
  map<string, string>::const_iterator itr = geoCodes.find(name);
  if (itr != geoCodes.end()) return itr->second;

  /* Case two: We don't know what this is, but we know the country code. */
  itr = geoCodes.find(name.substr(0, 2));
  if (itr != geoCodes.end()) return itr->second;

  /* Case three: No idea.  Say so. */
  return "Unknown Location (FIPS 10-4 code: " + name + ")";
}

/***** MainWindow Implementation *****/

/* Constructor sets up the window and fires the loading thread. */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    /* Create the world map. */
    worldMapPic = new PictureDisplay("world-map.bmp");
    setCentralWidget(worldMapPic);

    /* Connect signals so we can handle mouse clicks. */
    connect(worldMapPic, SIGNAL(clicked(QPoint)), this, SLOT(onMapClick(QPoint)));

    /* 直接在主线程加载数据，最简单安全 */
    statusBar()->showMessage("Loading...");

    try {
        // 加载地理编码
        ifstream geoCodes("geo-codes.txt", ios::binary);
        if (!geoCodes) throw runtime_error("Could not open geo-codes.txt");

        string line;
        while (getline(geoCodes, line)) {
            stringstream parser(line);
            string code, name;
            parser >> code;
            parser.get();
            getline(parser, name, '\r');

            if (code.size() == 2) {
                geoLookup[code] = name;
            } else if (code.size() == 4) {
                geoLookup[code] = name + ", " + geoLookup[code.substr(0, 2)];
            }
        }

        // 加载地图数据
        ifstream input("place-data.txt");
        if (!input) throw runtime_error("Could not open place-data.txt");

        size_t totalNumber;
        input >> totalNumber;
        size_t count = 0;
        Point<2> pt;
        string label;

        while (input >> pt[0] >> pt[1] >> label) {
            kd.insert(pt, label);
            if (++count % 10000 == 0) {
                statusBar()->showMessage(("Loaded " + to_string(count) + " data points...").c_str());
            }
        }

        statusBar()->showMessage("Ready");
    } catch (const exception& e) {
        QMessageBox::critical(this, "Error", e.what());
        QCoreApplication::quit();
    }
}

MainWindow::~MainWindow() {
  /* Leak everything; the app quits when we finish. */
}

/* When the map is clicked on, convert the click to a longitude/latitude and
 * process.
 */
void MainWindow::onMapClick(const QPoint& where) {
  /* Figure out where the user clicked and give up if the click location
   * is out of the unit circle.
   */
  Point<2> location = GetNormalizedClickLocation(where);
  if (inner_product(location.begin(), location.end(), location.begin(), 0.0) >= 1)
    return;

  /* Convert from a point in the unit circle to a longitude and latitude. */
  InvertMollwideProjection(location);

  /* Report the nearest neighbor. */
  statusBar()->showMessage(NameForLocation(kd.kNNValue(location, 1), geoLookup).c_str());
}

/************** PictureDisplay Implementation *****************/

PictureDisplay::PictureDisplay(const string& filename) {
  setMinimumSize(1024, 513);
  if (!pict.load(filename.c_str()))
    throw runtime_error("Couldn't load file " + filename);
}

void PictureDisplay::paintEvent(QPaintEvent *) {
  QPainter(this).drawImage(QPoint(0, 0), pict);
}

void PictureDisplay::mousePressEvent(QMouseEvent* event) {
  emit clicked(event->pos());
}
