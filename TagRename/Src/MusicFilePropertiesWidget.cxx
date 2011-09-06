#include <TagRename/MusicFilePropertiesWidget.hpp>
#include <QTreeWidget>
#include <QStandardItemModel>
#include <iostream>
#include <TagRename/FileModelColumn.hpp>
using namespace std;

MusicFilePropertiesWidget::MusicFilePropertiesWidget (QWidget* p_parent)
  : QWidget (p_parent)
{
  setupUi (this);
}

void
MusicFilePropertiesWidget::updateDetailsOfSelectedFile (const QModelIndex& p_current)
{
  int row (p_current.row());
  const QAbstractItemModel* model = p_current.model();
  QModelIndex idx;

  idx = p_current.sibling (row, COLUMN_ID(FileName));
  m_fileName->setText (idx.data(ItemDataRole::Hidden).toString());

  idx = p_current.sibling (row, COLUMN_ID(AlbumName));
  m_album->setText (idx.data().toString());

  idx = p_current.sibling (row, COLUMN_ID(Composer));
  m_composer->setText (idx.data().toString());

  idx = p_current.sibling (row, COLUMN_ID(TrackName));
  m_trackName->setText (idx.data().toString());

  idx = p_current.sibling (row, COLUMN_ID(ArtistNames));
  m_artist->setText (idx.data().toString());
}
