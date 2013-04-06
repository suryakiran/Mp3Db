#include <TagRename/DocFileDisplayWidget.hxx>
#include <QHeaderView>
#include <TagRename/FileModelColumn.hxx>
#include <TagRename/DocFileIterator.hxx>
#include <boost/assign/std/map.hpp>
#include <boost/assign/list_of.hpp>
#include <iostream>
#include <QtGui/QFileSystemModel>
using namespace std;
using namespace boost::assign;

namespace docs {
  COLUMN_DEFINE(FileType, 0, "File Type");
  COLUMN_DEFINE(FileName, 1, "File Name");
  COLUMN_DEFINE(DirName, 2, "Directory");
}

namespace {
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
    ;

  std::for_each(m_headerNameMap.begin(), m_headerNameMap.end(),
                [header](HeaderNameMapValue& value)
                { header->setText(value.first, value.second); }
    );
  setHeaderItem(header);
  
  m_pdf = newItem (this, "Pdf");
  m_epub = newItem (this, "Epub");
  m_mobi = newItem (this, "Mobi");
  m_djvu = newItem (this, "Djvu");
}

DocFileDisplayWidget::~DocFileDisplayWidget()
{
  
}

void DocFileDisplayWidget::readDirectory(const QModelIndex& p_index)
{
  qDeleteAll(m_pdf->takeChildren());
  qDeleteAll(m_epub->takeChildren());
  qDeleteAll(m_djvu->takeChildren());
  qDeleteAll(m_mobi->takeChildren());

  const QFileSystemModel* model = dynamic_cast<const QFileSystemModel*>(p_index.model());
  if(model)
  {
    DocFileIterator endIter;
    auto currentDir = model->filePath(p_index).toStdWString();
    for(DocFileIterator iter(currentDir); iter != endIter; ++iter)
    {
      QTreeWidgetItem* item = nullptr;
      
      if (iter.isPdf()) {
        item = new QTreeWidgetItem(m_pdf);
      } else if (iter.isDjvu()) {
        item = new QTreeWidgetItem(m_djvu);
      } else if (iter.isEpub()) {
        item = new QTreeWidgetItem(m_epub);
      } else if (iter.isMobi()) {
        item = new QTreeWidgetItem(m_mobi);
      }

        const fs::path& path (iter->path());
        item->setText(COLUMN_ID(docs::FileName), path.filename().string().c_str());
        item->setText(COLUMN_ID(docs::DirName), path.parent_path().string().c_str());
    }
  }
}
