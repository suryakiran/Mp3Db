#include <TagRename/DocFileDisplayWidget.hxx>
#include <QHeaderView>
#include <TagRename/FileModelColumn.hxx>
#include <TagRename/DocFileIterator.hxx>
#include <boost/assign/std/map.hpp>
#include <boost/assign/list_of.hpp>
#include <iostream>
#include <TagRename/QStringPrint.hxx>
#include <QtGui/QFileSystemModel>
#include <boost/range/algorithm/for_each.hpp>
using namespace std;
using namespace boost::assign;

namespace docs {
  COLUMN_DEFINE(FileType, 0, "File Type");
  COLUMN_DEFINE(FileName, 1, "File Name");
  COLUMN_DEFINE(DirName, 2, "Directory");
  COLUMN_DEFINE(Select, 3, "Select");
}

namespace {
  void setFileTypeDefaults (QTreeWidgetItem* item)
  {
    QString format("%1 items");
    item->setText(COLUMN_ID(docs::FileName), format.arg(item->childCount()));
    item->setCheckState(COLUMN_ID(docs::Select), Qt::Unchecked);
  }
                  
  QTreeWidgetItem* newItem (QTreeWidget* parent, const QString& itemName)
  {
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, itemName);
    return item;
  }
}

DocFileDisplayWidget::DocFileDisplayWidget(QWidget* p_parent)
  : QTreeWidget (p_parent)
{
  QTreeWidgetItem* header = new QTreeWidgetItem();
  insert(m_headerNameMap)
    (COLUMN_ID(docs::FileType), COLUMN_LABEL(docs::FileType))
    (COLUMN_ID(docs::FileName), COLUMN_LABEL(docs::FileName))
    (COLUMN_ID(docs::DirName),  COLUMN_LABEL(docs::DirName))
    (COLUMN_ID(docs::Select),  COLUMN_LABEL(docs::Select))
    ;

  boost::range::for_each(m_headerNameMap,
                         [header](HeaderNameMapValue& value) {
                           header->setText(value.first, value.second);
                         });
  setHeaderItem(header);

  insert(m_fileTypeItems)
    ("pdf", newItem(this, "Pdf"))
    ("epub", newItem(this, "Epub"))
    ("djvu", newItem(this, "Djvu"))
    ("mobi", newItem(this, "Mobi"))
    ;
  
  connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
          this, SLOT(onItemChanged(QTreeWidgetItem*, int)));
}

void
DocFileDisplayWidget::onItemChanged(QTreeWidgetItem* item, int column)
{
}

DocFileDisplayWidget::~DocFileDisplayWidget()
{
  
}

void DocFileDisplayWidget::readDirectory(const QModelIndex& p_index)
{
  boost::range::for_each(m_fileTypeItems,
                         [] (FileTypeItemMapValue& value) {
                           qDeleteAll(value.second->takeChildren());
                         });

  const QFileSystemModel* model = dynamic_cast<const QFileSystemModel*>(p_index.model());
  if(model)
  {
    DocFileIterator endIter;
    auto currentDir = model->filePath(p_index).toStdWString();
    for(DocFileIterator iter(currentDir); iter != endIter; ++iter)
    {
      auto findIter = m_fileTypeItems.find(iter.extension());
      if (findIter == m_fileTypeItems.end()) {
        continue;
      }

      QTreeWidgetItem* item = new QTreeWidgetItem(findIter->second);
      
      const fs::path& path (iter->path());
      item->setText(COLUMN_ID(docs::FileName), path.filename().string().c_str());
      item->setText(COLUMN_ID(docs::DirName), path.parent_path().string().c_str());
      item->setCheckState(COLUMN_ID(docs::Select), Qt::Unchecked);
    }

    boost::range::for_each(m_fileTypeItems,
                           [](FileTypeItemMapValue& value){
                             setFileTypeDefaults(value.second);
                           });
  }
}
