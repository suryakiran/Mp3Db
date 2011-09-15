#include <TagRename/MusicFilePropertiesWidget.hxx>
#include <QTreeWidget>
#include <QPalette>
#include <QStandardItemModel>
#include <iostream>
#include <TagRename/FileModelColumn.hxx>
#include <TagRename/Mp3String.hxx>
#include <TagRename/Mp3Config.hxx>
#include <TagRename/ReturnPress.hxx>
#include <TagRename/QStringPrint.hxx>

#include <boost/foreach.hpp>
#include <iostream>

#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v2tag.h>
using namespace std;

#include <Signals/BoostSlots.hxx>

namespace {
  QPixmap createImage (const TagLib::ByteVector& bv)
  {
    QPixmap pixmap = QPixmap::fromImage (QImage::fromData((uchar*)bv.data(), bv.size()));
    return pixmap.scaledToHeight (100, Qt::SmoothTransformation);
  }
}

MusicFilePropertiesWidget::MusicFilePropertiesWidget (QWidget* p_parent)
  : QWidget (p_parent)
{
  setupUi (this);
  ReturnPress *rp = new ReturnPress (this);
  m_genre->installEventFilter (rp);

  m_mp3Config = Mp3Config::instance();
  updateGenresList();

  m_mp3Config->signal<Mp3Config::GenresModified>().connect(MEM_FUN(MusicFilePropertiesWidget, updateGenresList, 0));
  connect (m_genre, SIGNAL(editTextChanged(const QString&)), this, SLOT(updateGenresList(const QString&)));
  connect (rp, SIGNAL(returnPressed()), this, SLOT (genreNameEdited()));
}

void 
MusicFilePropertiesWidget::genreNameEdited()
{
  QString curText (m_genre->currentText());
  int idx (m_genre->findText (curText));
  if (idx == -1)
  {
    m_mp3Config->addGenre (curText.toStdString());
    idx = m_genre->findText (curText);
    m_genre->setCurrentIndex (idx);
  }
}

void
MusicFilePropertiesWidget::updateGenresList(const QString& p_genre)
{
}

void 
MusicFilePropertiesWidget::updateGenresList ()
{
  m_genre->clear();
  BOOST_FOREACH (const string& genre, m_mp3Config->getGenres())
  {
    m_genre->addItem (genre.c_str());
  }
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

  bool ok(false);
  idx = p_current.sibling (row, COLUMN_ID(Year));
  int year = idx.data().toString().toInt(&ok);
  if (ok) 
  {
    m_year->setValue(year);
  }

  ok = false;
  idx = p_current.sibling (row, COLUMN_ID(TrackNumber));
  int id = idx.data().toString().toInt(&ok);
  if (ok) 
  {
    m_trackId->setValue(id);
  }

  idx = p_current.sibling (row, COLUMN_ID(Genre));
  QString genre = idx.data().toString();
  int comboIdx (m_genre->findText(genre));
  if (comboIdx != -1)
  {
    m_genre->setCurrentIndex (comboIdx);
  }
  else
  {
    m_mp3Config->addGenre (genre.toStdString());
    comboIdx = m_genre->findText(genre);
    m_genre->setCurrentIndex (comboIdx);
  }

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
