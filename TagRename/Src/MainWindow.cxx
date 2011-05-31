#include <TagRename/MainWindow.hpp>
#include <QtGui/QFileSystemModel>

MainWindow :: MainWindow(QWidget* p_parent)
  : QMainWindow(p_parent)
{
  setupUi(this);
  resize(1000, 600);

  m_dirModel = new QFileSystemModel(m_dirView);
  m_dirModel->setRootPath(QDir::homePath());
  m_dirView->setModel (m_dirModel);

  for (int i = 1; i < m_dirModel->columnCount(); ++i) {
    m_dirView->setColumnHidden(i, true);
  }

  connect(m_toggleFoldersDisplay, SIGNAL(toggled(bool)),
      this, SLOT(toggleFoldersDisplay(bool)));

  connect(m_toggleDetailsDisplay, SIGNAL(toggled(bool)),
      this, SLOT(toggleDetailsDisplay(bool)));
}

void
MainWindow::toggleDetailsDisplay(bool p_showDetails)
{
  if (p_showDetails) {
    m_detailsDockWidget->show();
  }
}

void
MainWindow::toggleFoldersDisplay(bool p_showFolders)
{
}
