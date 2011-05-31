#ifndef MainWindow_hpp_INCLUDED
#define MainWindow_hpp_INCLUDED

#include "ui_MainWindow.h"

class QFileSystemModel;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget* p_parent = 0);

  private:
    QFileSystemModel *m_dirModel;
};

#endif
