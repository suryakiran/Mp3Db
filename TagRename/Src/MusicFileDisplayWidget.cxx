#include <TagRename/MusicFileDisplayWidget.hpp>
#include <TagRename/Mp3FileIterator.hpp>
#include <TagRename/Mp3String.hpp>
#include <TagRename/FileModelColumn.hpp>

#include <QtCore/QTextCodec>
#include <QtGui/QFileSystemModel>

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>

#include <boost/assign/list_of.hpp>
#include <boost/assign/std/map.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::assign;

namespace bt = boost::posix_time;

#include <iostream>
using namespace std;

MusicFileDisplayWidget::MusicFileDisplayWidget (QWidget* p_parent)
  : QTreeWidget(p_parent)
{
  QTreeWidgetItem* header = new QTreeWidgetItem();
  insert (m_headerNameMap)
    (COLUMN_ID(FileName),    COLUMN_LABEL(FileName))
    (COLUMN_ID(AlbumName),   COLUMN_LABEL(AlbumName))
    (COLUMN_ID(TrackNumber), COLUMN_LABEL(TrackNumber))
    (COLUMN_ID(TrackName),   COLUMN_LABEL(TrackName))
    (COLUMN_ID(ArtistNames), COLUMN_LABEL(ArtistNames))
    (COLUMN_ID(Genre),       COLUMN_LABEL(Genre))
    (COLUMN_ID(Composer),    COLUMN_LABEL(Composer))
    (COLUMN_ID(BitRate),     COLUMN_LABEL(BitRate))
    (COLUMN_ID(Duration),    COLUMN_LABEL(Duration))
    ;

  BOOST_FOREACH (HeaderNameMapValue himv, m_headerNameMap)
  {
    header->setText (himv.first, himv.second);
  }
  setHeaderItem (header);

  connect (this, SIGNAL(itemSelectionChanged()), this, SLOT(emitSignalIfItemsSelected()));
}

MusicFileDisplayWidget::~MusicFileDisplayWidget()
{
}

void MusicFileDisplayWidget::readDirectory (const QModelIndex& p_index)
{
  clear();
  if (const QFileSystemModel* model = dynamic_cast<const QFileSystemModel*> (p_index.model()))
  {
    Mp3FileIterator endIter;
    for (Mp3FileIterator iter (model->filePath(p_index).toStdWString()); 
        iter != endIter; ++iter)
    {
      fs::path p = iter->path();
      TagLib::FileRef fileRef (p.string().c_str());
      QTreeWidgetItem* item = new QTreeWidgetItem(this);
      TagLib::Tag* tag = fileRef.tag();
      TagLib::AudioProperties* audioProperties = fileRef.audioProperties();

      int length (audioProperties->length());
      bt::time_duration td = bt::seconds (length);

      if (TagLib::MPEG::File* mpegFile = dynamic_cast<TagLib::MPEG::File*>(fileRef.file()))
      {
        const TagLib::ID3v2::Tag* id3v2Tag = mpegFile->ID3v2Tag();
        if (id3v2Tag) 
        {
          const TagLib::ID3v2::FrameListMap& frameList = id3v2Tag->frameListMap();
          TagLib::ID3v2::FrameListMap::ConstIterator iter = frameList.find("TCOM");
          if (iter != frameList.end())
          {
            item->setText (COLUMN_ID(Composer), _M(iter->second.front()->toString()));
          }
        }
      }

      item->setData (COLUMN_ID(FileName),    ItemDataRole::Hidden, p.string().c_str());
      item->setText (COLUMN_ID(FileName),    _M(p.filename()));
      item->setText (COLUMN_ID(AlbumName),   _M(tag->album()));
      item->setText (COLUMN_ID(TrackName),   _M(tag->title()));
      item->setText (COLUMN_ID(Genre),       _M(tag->genre()));
      item->setText (COLUMN_ID(ArtistNames), _M(tag->artist()));
      item->setText (COLUMN_ID(TrackNumber), _M(tag->track()));
      item->setText (COLUMN_ID(BitRate),     _M(audioProperties->bitrate()));
      item->setText (COLUMN_ID(Duration),    _M(bt::to_simple_string(td)));
      item->setData (COLUMN_ID(Duration),    ItemDataRole::Hidden, length); 
    }
  }

  sortItems (0, Qt::AscendingOrder);
  for (int i = 0; i < columnCount(); ++i)
  {
    resizeColumnToContents(i);
  }
}

void MusicFileDisplayWidget::emitSignalIfItemsSelected ()
{
  Q_EMIT hasItemSelection(!selectedItems().isEmpty());
}
