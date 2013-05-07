#include <TagRename/MainWindow.hxx>
#include <QtGui/QFileSystemModel>

MainWindow :: MainWindow(QWidget* p_parent)
  : QMainWindow(p_parent), m_shown(false)
{
  setupUi(this);
  resize(1500, 600);

  m_musicFilePropertiesFrame->setVisible(false);
  m_saveAndPrev->setEnabled(false);
  m_saveAndNext->setEnabled(false);

  connect(m_dirView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), 
          m_musicFileDisplayWidget, SLOT(readDirectory(const QModelIndex&)));
  connect(m_dirView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), 
          m_docFileDisplayWidget, SLOT(readDirectory(const QModelIndex&)));
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
  connect(m_renameToTitle, SIGNAL(clicked()),
          m_docFileDisplayWidget, SLOT(onRenameToTitle()));
  connect(m_fetchTitles, SIGNAL(clicked()),
          m_docFileDisplayWidget, SLOT(onFetchTitles()));
  connect(m_docFileDisplayWidget, SIGNAL(filesRenamed()),
          this, SLOT(readCurrentDirectory()));
  
  buttonGroup->setId(musicButton, 0);
  buttonGroup->setId(docButton, 1);
}

void
MainWindow::showEvent(QShowEvent* p_event)
{
  if (!m_shown) {
    m_shown = true;
    buttonGroup->button(0)->click();
  }
  
  QMainWindow::showEvent(p_event);
}

void
MainWindow::readCurrentDirectory()
{
  QItemSelectionModel* model = m_dirView->selectionModel();
  if (!model) {
    return;
  }

  if (model->hasSelection()) {
    m_docFileDisplayWidget->readDirectory(model->currentIndex());
  }
}
