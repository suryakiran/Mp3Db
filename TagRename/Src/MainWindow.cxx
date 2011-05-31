#include <TagRename/MainWindow.hpp>
#include <QtGui/QFileSystemModel>

MainWindow :: MainWindow(QWidget* p_parent)
  : QMainWindow(p_parent)
{
  setupUi(this);
  resize(1000, 600);
}
