#ifndef MainWindow_hxx_INCLUDED
#define MainWindow_hxx_INCLUDED

#include "ui_MainWindow.h"

class QFileSystemModel;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* p_parent = 0);

private:
  QFileSystemModel *m_dirModel;
  bool m_shown;

protected:
  virtual void showEvent(QShowEvent* p_event);
};

#endif
