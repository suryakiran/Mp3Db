#include <TagRename/MainWindow.hxx>
#include <QtGui/QFileSystemModel>

MainWindow :: MainWindow(QWidget* p_parent)
  : QMainWindow(p_parent)
{
  setupUi(this);
  resize(1000, 600);

  m_musicFilePropertiesFrame->setVisible(false);
  m_saveAndPrev->setEnabled(false);
  m_saveAndNext->setEnabled(false);

  connect(m_dirView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), 
          m_musicFileDisplayWidget, SLOT(readDirectory(const QModelIndex&)));
  connect(m_musicFileDisplayWidget, SIGNAL(hasItemSelection(bool)),
          m_musicFilePropertiesFrame, SLOT(setVisible(bool)));
  connect(m_musicFileDisplayWidget, SIGNAL(hasItemSelection(bool)),
          m_saveAndPrev, SLOT(setEnabled(bool)));
  connect(m_musicFileDisplayWidget, SIGNAL(hasItemSelection(bool)),
          m_saveAndNext, SLOT(setEnabled(bool)));
  connect(m_musicFileDisplayWidget->selectionModel(),
          SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
          m_musicFilePropertiesWidget,
          SLOT(updateDetailsOfSelectedFile(const QModelIndex&)));
  connect(m_saveAndNext, SIGNAL(clicked()),
          m_musicFileDisplayWidget, SLOT(saveCurrentAndGotoNextItem()));
  connect(m_saveAndPrev, SIGNAL(clicked()),
          m_musicFileDisplayWidget, SLOT(saveCurrentAndGotoPrevItem()));

  buttonGroup->setId(musicButton, 0);
  buttonGroup->setId(docButton, 1);
}
