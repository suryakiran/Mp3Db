#include <TagRename/MusicFileDisplayWidget.hpp>
#include <TagRename/Mp3FileIterator.hpp>

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
    (FileName, "File Name")
    (AlbumName, "Album Name")
    (TrackNumber, "#")
    (TrackName, "Track Name")
    (ArtistNames, "Artist(s)")
    (Genre, "Genre")
    (Composer, "Composer")
    (BitRate, "Bit Rate")
    (Duration, "Duration")
    ;

  BOOST_FOREACH (HeaderNameMapValue himv, m_headerNameMap)
  {
    header->setText (himv.first, himv.second);
  }
  setHeaderItem (header);
  setAlternatingRowColors (true);
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
      TagLib::String album = tag->album();

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
            item->setText (Composer, TStringToQString(iter->second.front()->toString()));
          }
        }
      }
      else
      {
      }

      item->setText (FileName, p.filename().c_str());
      item->setText (AlbumName, TStringToQString(tag->album()));
      item->setText (TrackName, TStringToQString(tag->title()));
      item->setText (Genre, TStringToQString(tag->genre()));
      item->setText (ArtistNames, TStringToQString(tag->artist()));
      item->setText (TrackNumber, QString("%1").arg(tag->track()));
      item->setText (BitRate, QString("%1").arg(audioProperties->bitrate()));
      item->setText (Duration, QString("%1").arg(bt::to_simple_string(td).c_str()));
      item->setData (Duration, HiddenDataRole, length); 
    }
  }

  sortItems (0, Qt::AscendingOrder);
  for (int i = 0; i < columnCount(); ++i)
  {
    resizeColumnToContents(i);
  }
}
