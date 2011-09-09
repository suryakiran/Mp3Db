#include <TagRename/MainWindow.hpp>
#include <QtGui/QFileSystemModel>

MainWindow :: MainWindow(QWidget* p_parent)
  : QMainWindow(p_parent)
{
  setupUi(this);
  resize(1000, 600);

  m_musicFilePropertiesFrame->setVisible(false);

  connect(m_dirView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), 
      m_musicFileDisplayWidget, SLOT(readDirectory(const QModelIndex&)));

  connect(m_musicFileDisplayWidget, SIGNAL(hasItemSelection(bool)), m_musicFilePropertiesFrame, SLOT(setVisible(bool)));
  connect(m_musicFileDisplayWidget->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
          m_musicFilePropertiesWidget, SLOT(updateDetailsOfSelectedFile(const QModelIndex&)));
  connect(m_writeXml, SIGNAL(clicked()), m_musicFileDisplayWidget, SLOT(writeXML()));
}
