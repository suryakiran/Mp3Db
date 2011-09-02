#include <TagRename/MusicFileDisplayWidget.hpp>
#include <TagRename/Mp3FileIterator.hpp>

#include <QtCore/QTextCodec>
#include <QtGui/QFileSystemModel>

#include <taglib/fileref.h>
#include <taglib/tag.h>

#include <iostream>
using namespace std;

namespace {
  QString toQString(TagLib::String s)
  {
    if (s.isLatin1()) //respect the tradition of writing data in system locale when no encoding is defined
    {
      QByteArray result;
      result.resize(s.size());
      for (int i=0;i<result.size();i++)
        result[i]=s[i];

      return QTextCodec::codecForLocale()->toUnicode(result);
    }

    QString result;
    result.resize(s.size());
    for (int i=0;i<result.size();i++)
      result[i]=s[i];

    return result;
  }
}

MusicFileDisplayWidget::MusicFileDisplayWidget (QWidget* p_parent)
  : QTreeWidget(p_parent)
{
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
      TagLib::FileName fileName (iter->path().filename().c_str());
      TagLib::FileRef fileRef (fileName);
      QTreeWidgetItem* item = new QTreeWidgetItem(this);
      TagLib::Tag* tag = fileRef.tag();
      TagLib::AudioProperties* audioProperties = fileRef.audioProperties();
      //TagLib::String album = tag->album();

      item->setText (0, fileName);
      if (fileRef.isNull())
      {
        item->setText (1, "Surya");
      }
    }
  }

  sortItems (0, Qt::AscendingOrder);
  for (int i = 0; i < columnCount(); ++i)
  {
    resizeColumnToContents(i);
  }
}
