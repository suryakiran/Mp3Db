#include <TagRename/MainWindow.hpp>
#include <QtGui/QFileSystemModel>

MainWindow :: MainWindow(QWidget* p_parent)
  : QMainWindow(p_parent)
{
  setupUi(this);
  resize(1000, 600);

  connect(m_dirView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), 
      m_musicFileDisplayWidget, SLOT(readDirectory(const QModelIndex&)));
}
