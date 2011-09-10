#include <TagRename/MusicFileDisplayWidget.hxx>
#include <TagRename/Mp3FileIterator.hxx>
#include <TagRename/Mp3String.hxx>

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

#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>

#include <TagRename/XString.hpp>

using namespace xercesc;
using namespace boost::assign;

namespace bt = boost::posix_time;

#include <iostream>
using namespace std;

namespace {
  void createChildElement (DOMNode* p_node, const std::string& p_nodeName, const Mp3String& p_nodeText)
  {
    DOMDocument* doc = p_node->getOwnerDocument();
    DOMElement* node = doc->createElement (_X(p_nodeName));
    DOMText* text = doc->createTextNode (_X(p_nodeText));
    node->appendChild (text);
    p_node->appendChild (node);
  }

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
      item->setText (COLUMN_ID(BitRate),     _M(audioProperties->bitrate()));
      item->setText (COLUMN_ID(Duration),    _M(bt::to_simple_string(td)));
      item->setData (COLUMN_ID(Duration),    ItemDataRole::Hidden, length); 

      setItemValue (item, COLUMN_ID(AlbumName),   tag->album(),  tags.album);
      setItemValue (item, COLUMN_ID(TrackName),   tag->title(),  tags.track);
      setItemValue (item, COLUMN_ID(Genre),       tag->genre(),  tags.genre);
      setItemValue (item, COLUMN_ID(ArtistNames), tag->artist(), tags.artist);
      setItemValue (item, COLUMN_ID(TrackNumber), _M(tag->track()),  tags.id);

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

void MusicFileDisplayWidget::writeXML ()
{
  if (m_tags.empty()) {
    return;
  }

  const XMLCh ls_id [] = {chLatin_L, chLatin_S, chNull};
  DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(ls_id);

  DOMDocument* doc = impl->createDocument (0, _X("Songs"), 0);
  DOMElement* songs = doc->getDocumentElement();

  BOOST_FOREACH (const Mp3TagMapValue& tag, m_tags)
  {
    DOMElement* song = doc->createElement(_X("Song"));
    song->setAttribute (_X("id"), _X(tag.second.id));

    createChildElement (song, "Title", tag.second.track);
    createChildElement (song, "Composer", tag.second.composer);
    createChildElement (song, "Album", tag.second.album);
    createChildElement (song, "Artists", tag.second.artist);
    createChildElement (song, "Genre", tag.second.genre);
    createChildElement (song, "Lyrics", tag.second.lyrics);

    songs->appendChild (song);
  }

  DOMLSSerializer* ser = impl->createLSSerializer();
  DOMConfiguration* serializerConfig=ser->getDomConfig();
  serializerConfig->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
  serializerConfig->setParameter(XMLUni::fgDOMWRTXercesPrettyPrint, false);
  serializerConfig->setParameter(XMLUni::fgDOMXMLDeclaration, true);

  DOMLSOutput* output = impl->createLSOutput();
  output->setEncoding (_X("UTF-8"));
  XMLFormatTarget *myFormTarget=new StdOutFormatTarget(); 
  output->setByteStream (myFormTarget);

  ser->write (doc, output);
  output->release();
  ser->release();
  delete myFormTarget;
}
