#include <TagRename/MainWindow.hpp>

MainWindow :: MainWindow(QWidget* p_parent)
  : QMainWindow(p_parent)
{
  setupUi(this);
  resize(1000, 600);

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
