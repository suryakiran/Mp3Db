#include <TagRename/MusicFileDisplayWidget.hxx>
#include <TagRename/Mp3FileIterator.hxx>
#include <TagRename/Mp3String.hxx>
#include <TagRename/FileModelColumn.hxx>

#include <QtCore/QTextCodec>
#include <QtGui/QFileSystemModel>

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>

#include <string>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/map.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::assign;

namespace bt = boost::posix_time;
namespace id3 = TagLib::ID3v2;
namespace mpeg = TagLib::MPEG;

#include <iostream>
using namespace std;

namespace {
  void setItemValue (QTreeWidgetItem* p_item, int p_id, const TagLib::String& p_string, Mp3String& p_target)
  {
    p_target = p_string;
    p_item->setText (p_id, p_target);
  }
}

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
    (COLUMN_ID(Year),        COLUMN_LABEL(Year))
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
  m_tags.clear();
  if (const QFileSystemModel* model = dynamic_cast<const QFileSystemModel*> (p_index.model()))
  {
    Mp3FileIterator endIter;
    m_currentDirectory = model->filePath(p_index).toStdWString();
    for (Mp3FileIterator iter (m_currentDirectory); iter != endIter; ++iter)
    {
      fs::path p = iter->path();
      Mp3Tags& tags = m_tags[p];

      TagLib::FileRef fileRef (p.string().c_str());
      QTreeWidgetItem* item = new QTreeWidgetItem(this);
      TagLib::Tag* tag = fileRef.tag();
      TagLib::AudioProperties* audioProperties = fileRef.audioProperties();

      int length (audioProperties->length());
      bt::time_duration td = bt::seconds (length);

      if (mpeg::File* mpegFile = dynamic_cast<mpeg::File*>(fileRef.file()))
      {
        const id3::Tag* id3v2Tag = mpegFile->ID3v2Tag();
        if (id3v2Tag) 
        {
          const id3::FrameListMap& frameList = id3v2Tag->frameListMap();
          id3::FrameListMap::ConstIterator iter = frameList.find("TCOM");
          if (iter != frameList.end())
          {
            setItemValue (item, COLUMN_ID(Composer), iter->second.front()->toString(), tags.composer);
          }

          iter = frameList.find("USLT");
          if (iter != frameList.end())
          {
            tags.lyrics = iter->second.front()->toString();
          }
        }
      }

      item->setData (COLUMN_ID(FileName), ItemDataRole::Hidden, p.string().c_str());
      item->setText (COLUMN_ID(FileName), _M(p.filename()));
      item->setText (COLUMN_ID(BitRate),  _M(audioProperties->bitrate()));
      item->setText (COLUMN_ID(Duration), _M(bt::to_simple_string(td)));
      item->setData (COLUMN_ID(Duration), ItemDataRole::Hidden, length); 

      setItemValue (item, COLUMN_ID(AlbumName),   tag->album(),  tags.album);
      setItemValue (item, COLUMN_ID(TrackName),   tag->title(),  tags.track);
      setItemValue (item, COLUMN_ID(Genre),       tag->genre(),  tags.genre);
      setItemValue (item, COLUMN_ID(ArtistNames), tag->artist(), tags.artist);
      setItemValue (item, COLUMN_ID(TrackNumber), _M(tag->track()),  tags.id);
      setItemValue (item, COLUMN_ID(Year),        _M(tag->year()),  tags.year);

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

void MusicFileDisplayWidget::saveCurrentAndGotoNextItem()
{
  if (!saveCurrentItem())
  {
    return ;
  }

  QTreeWidgetItem* curItem (currentItem());
  QTreeWidgetItem* nextItem (itemBelow(curItem));
  
  if (nextItem)
  {
    curItem->setSelected (false);
    nextItem->setSelected(true);
    setCurrentItem (nextItem);
  }
}

void MusicFileDisplayWidget::saveCurrentAndGotoPrevItem()
{
  if (!saveCurrentItem())
  {
    return;
  }

  QTreeWidgetItem* curItem (currentItem());
  QTreeWidgetItem* prevItem (itemAbove(curItem));

  if (prevItem)
  {
    curItem->setSelected (false);
    prevItem->setSelected(true);
    setCurrentItem (prevItem);
  }
}

bool MusicFileDisplayWidget::saveCurrentItem()
{
  return true;
}
