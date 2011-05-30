#ifndef MainWindow_hpp_INCLUDED
#define MainWindow_hpp_INCLUDED

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget* p_parent = 0);

  private Q_SLOTS:
    void toggleDetailsDisplay (bool);
    void toggleFoldersDisplay (bool);
};

#endif
