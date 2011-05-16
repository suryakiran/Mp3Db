#ifndef MainWindow_hpp_INCLUDED
#define MainWindow_hpp_INCLUDED

#include "ui_MainWindow.h"

class MainWindow : public Ui::MainWindow, public QMainWindow
{
  public:
    MainWindow(QWidget* p_parent = 0);
};

#endif
