// MIT License Terms (http://en.wikipedia.org/wiki/MIT_License)
// 
// Copyright (C) 2011 by Oleg Alexandrov
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qmenubar.h>
#include <qapplication.h>
#include <qimage.h>
#ifdef ADD_PREF_INI
#include <QSettings>
#endif // ADD_PREF_INI
#include <iostream>
#include <stdlib.h>
#include "appWindow.h"
#include "polyView.h"
#include "utils.h"

#ifndef PV2D_VERSION
#define PV2D_VERSION "1.0.2"
#endif
#ifndef PV2D_DATE
#define PV2D_DATE "2016/06/22"  // This is epock date for this option
#endif

using namespace std;
using namespace utils;

int main(int argc, char** argv){

  char * exeName = argv[0];

  int windowWidX, windowWidY;
  cmdLineOptions options;
#ifdef ADD_PREF_INI
	QApplication::setOrganizationName("Polyview");
	QApplication::setOrganizationDomain("sites.google.com/site/polyview2d");
	QApplication::setApplicationName("Polyview-2D");
	QApplication::setApplicationVersion(PV2D_VERSION);
    QSettings startsettings; // create settings
#endif // ADD_PREF_INI
  
  parseCmdOptions(// inputs
                  argc, argv, exeName,
                  // outputs
                  windowWidX, windowWidY, options
                  );
  
  QApplication app(argc, argv);
  string progName = "PolyView-2D";
  
  appWindow m(NULL, progName, options, windowWidX, windowWidY);
#ifdef _MSC_VER
  QIcon icon(":/pv.ico"); 
  m.setWindowIcon(icon);
#endif
#ifdef USE_QT4_DEFS
  m.setCaption(progName.c_str());
#else
  // m.setCaption(...)
  m.setWindowTitle(progName.c_str());
#endif
  m.show();
  
  QObject::connect( qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()) );
  
  return app.exec();
}
