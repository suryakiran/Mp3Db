#include <iostream>
#include <string>

#include <TagRename/MainWindow.hpp>

int main (int argc, char** argv) 
{
  QApplication app (argc, argv);

  MainWindow *mainw = new MainWindow;
  mainw->show();

  app.exec();
}
