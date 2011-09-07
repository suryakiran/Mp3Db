#include <TagRename/MusicFilePropertiesWidget.hpp>
#include <QTreeWidget>
#include <QPalette>
#include <QStandardItemModel>
#include <iostream>
#include <TagRename/FileModelColumn.hpp>
#include <TagRename/Mp3String.hpp>

#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v2tag.h>
using namespace std;

namespace {
  QPixmap createImage (const TagLib::ByteVector& bv)
  {
    cout << bv.size() << endl;
#if 0
    qint32 pos = 0;
    const char *data = bv.data();
    pos++;
    quint32 pictSize = *( quint32 *)(data + pos );
    pos += 4;
    QString mimeType = QString::fromUtf16( ( const ushort *)( data + pos ) );
    pos += 2 * ( mimeType.length() + 1 );
    QString description = QString::fromUtf16( ( const ushort *)( data + pos ) );
    pos += 2 * ( description.length() + 1 );
    QByteArray ba ( data + pos, pictSize );

    return QImage::fromData (ba);
#endif
    QPixmap pixmap = QPixmap::fromImage (QImage::fromData((uchar*)bv.data(), bv.size()));
    return pixmap.scaledToHeight (75, Qt::SmoothTransformation);
  }
}

MusicFilePropertiesWidget::MusicFilePropertiesWidget (QWidget* p_parent)
  : QWidget (p_parent)
{
  setupUi (this);
}

void
MusicFilePropertiesWidget::updateDetailsOfSelectedFile (const QModelIndex& p_current)
{
  m_fileArt->clear();
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

  TagLib::MPEG::File mp3File (m_fileName->text().toStdString().c_str());
  TagLib::ID3v2::Tag* tag = mp3File.ID3v2Tag();
  if (tag)
  {
    const TagLib::ID3v2::FrameList& flist = tag->frameListMap()["APIC"];
    if (!flist.isEmpty())
    {
      for (TagLib::ID3v2::FrameList::ConstIterator iter = flist.begin(); iter != flist.end(); ++iter)
      {
        if (TagLib::ID3v2::AttachedPictureFrame* picFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*> (*iter))
        {
          m_fileArt->setPixmap(createImage (picFrame->picture()));
        }
      }
    }
  }
}
