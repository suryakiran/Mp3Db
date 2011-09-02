#include <TagRename/MusicFileDisplayWidget.hpp>
#include <TagRename/Mp3FileIterator.hpp>
#include <QFileSystemModel>

#include <iostream>
using namespace std;

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
      QTreeWidgetItem* item = new QTreeWidgetItem(this);
      item->setText (0, iter->path().filename().c_str());
    }
  }

  sortItems (0, Qt::AscendingOrder);
  for (int i = 0; i < columnCount(); ++i)
  {
    resizeColumnToContents(i);
  }
}
