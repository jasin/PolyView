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
#ifndef EXAMPLE_H
#define EXAMPLE_H

#ifdef USE_QT4_DEFS
#include <q3mainwindow.h>
#else
#include <qmainwindow.h>
#endif // #ifdef USE_QT4_DEFS
#include <qlineedit.h>
#include <QEvent>
//Added by qt3to4:
#include <QCloseEvent>
#include <string>
#include <vector>

#ifndef USE_QT4_DEFS
QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE
#endif

class polyView;
class QCloseEvent;
class cmdLineOptions;
class QTextEdit;

class cmdLine : public QLineEdit {
  Q_OBJECT

public:
  cmdLine(QWidget* parent);
  virtual ~cmdLine();
};

#ifdef USE_QT4_DEFS
class docWindow: public Q3MainWindow{
#else // ! #ifdef USE_QT4_DEFS
class docWindow: public QMainWindow{
#endif 
  Q_OBJECT

public:
  docWindow(QWidget * parent = NULL);
  virtual ~docWindow();
  void setText(const std::string & docText);
  QTextEdit * m_textArea;
};

#ifdef USE_QT4_DEFS
class appWindow: public Q3MainWindow{
#else // ! #ifdef USE_QT4_DEFS
class appWindow : public QMainWindow {
#endif 
  Q_OBJECT

public:
  appWindow(QWidget* parent, std::string progName,
            const cmdLineOptions & options,
            int windowWidX, int windowWidY
            );
  ~appWindow();

protected:
  bool eventFilter(QObject *obj, QEvent *event);

private slots:
  void createMenusAndMainWidget(const cmdLineOptions & opt);
  void showDoc();
  void about();
  void procCmdLine();
  void shiftUp();
  void shiftDown();
  void forceQuit();

private:
  void closeEvent(QCloseEvent *);
  void insertCmdFromHist();

  polyView       * m_poly;
  cmdLine        * m_cmdLine;
  std::string      m_progName;
  std::vector<std::string> m_cmdHist;
  int m_histPos;
  docWindow m_docWindow;
#ifndef USE_QT4_DEFS
     QMenu *fileMenu;

    QAction *openAct;
    QAction *saveoneAct;
    QAction *savemultAct;
    QAction *exitAct;
    QAction *writeAct;
#ifdef ADD_IMAGE_SAVE
    QAction *imageAct;
#endif

// #### GEN HEADER CODE ############################################## 
// gen Qt5 header code, gen by qt42qt5.pl, on 2016/06/21 00:31:55 UTC.
// Add 8 QMenu items to header
 QMenu *viewMenu;
 QMenu *editMenu;
 QMenu *selectionMenu;
 QMenu *transformMenu;
 QMenu *gridMenu;
 QMenu *diffMenu;
 QMenu *optionsMenu;
 QMenu *helpMenu;
// Add 46 QAction items to header
 QAction *chooseFilesToShowAct;
 QAction *zoomOutAct;
 QAction *zoomInAct;
 QAction *shiftLeftAct;
 QAction *shiftRightAct;
 QAction *shiftUpAct;
 QAction *shiftDownAct;
 QAction *resetViewAct;
 QAction *changeOrderAct;
 QAction *toggleAnnoAct;
 QAction *toggleFilledAct;
 QAction *togglePEAct;
 QAction *toggleVertIndexAnnoAct;
 QAction *toggleLayerAnnoAct;
 QAction *undoAct;
 QAction *redoAct;
 QAction *create45DegreeIntPolyAct;
 QAction *enforce45Act;
 QAction *createArbitraryPolyAct;
 QAction *mergePolysAct;
 QAction *createHltAct;
 QAction *cutToHltAct;
 QAction *deleteSelectedPolysAct;
 QAction *pasteSelectedPolysAct;
 QAction *moveSelectedPolysAct;
 QAction *deselectPolysDeleteHltsAct;
 QAction *translateSelectedPolysAct;
 QAction *rotateSelectedPolysAct;
 QAction *scaleSelectedPolysAct;
 QAction *transformSelectedPolysAct;
 QAction *translatePolysAct;
 QAction *rotatePolysAct;
 QAction *scalePolysAct;
 QAction *toggleShowGridAct;
 QAction *enforce45AndSnapToGridAct;
 QAction *setGridSizeAct;
 QAction *setGridWidthAct;
 QAction *setGridColorAct;
 QAction *toggleDifferentColorsAct;
 QAction *toggleShowPolyDiffAct;
 QAction *plotNextDiffAct;
 QAction *plotPrevDiffAct;
 QAction *setLineWidthAct;
 QAction *setBgColorAct;
 QAction *showDocAct;
 QAction *aboutAct;
// #### END GEN HEADER CODE ############################################## 

#endif

};


#endif
// eof
