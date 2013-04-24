#include <TagRename/DocFileDisplayWidget.hxx>
#include <TagRename/IsbnDb.hxx>
#include <TagRename/FileModelColumn.hxx>
#include <TagRename/DocFileIterator.hxx>
#include <TagRename/QStringPrint.hxx>

#include <QHeaderView>
#include <QtGui/QFileSystemModel>

#include <boost/assign/std/map.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <sstream>
#include <iostream>

using namespace std;
using namespace boost::assign;

namespace docs {
  COLUMN_DEFINE(FileType, 0, "File Type");
  COLUMN_DEFINE(Select, 1, "Select");
  COLUMN_DEFINE(FileName, 2, "File Name");
  COLUMN_DEFINE(Title, 3, "Title");
  COLUMN_DEFINE(DirName, 4, "Directory");
}

namespace {
  boost::regex u_isbnReg ("(\\d{9}[\\d|X])");
  boost::regex u_titleReg("\\b([a-z])");


  void guessTitle(string& title)
  {
    boost::regex reg("\\.|[ _-]|(?<!^)(?=[A-Z][a-z])");
    boost::regex nonEmptyString ("^\\S+$");
    stl::StringVec sv;
    fs::path p(title);
    string extn(p.extension().string());
    str::split_regex(sv, p.stem().string(), reg);
    for(string& s: sv) {
      str::trim(s);
      str::replace_regex(s, u_titleReg, std::string("\\U\\1"));
    }
    string final = str::join_if(sv, " ", nonEmptyString);
    final += extn;
    title = final;
  }
  
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
    (COLUMN_ID(docs::Title),  COLUMN_LABEL(docs::Title))
    ;

  for(auto& i: m_headerNameMap) {
    header->setText(i.first, i.second);
  }
  setHeaderItem(header);

  insert(m_fileTypeItems)
    ("pdf", newItem(this, "Pdf"))
    ("epub", newItem(this, "Epub"))
    ("djvu", newItem(this, "Djvu"))
    ("mobi", newItem(this, "Mobi"))
    ;
  
  connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
          this, SLOT(onItemChanged(QTreeWidgetItem*, int)));
  
  connect(this, SIGNAL(itemExpanded(QTreeWidgetItem*)),
          this, SLOT(resizeColumns()));
  
  connect(this, SIGNAL(itemExpanded(QTreeWidgetItem*)),
          this, SLOT(resizeColumns()));
}

void
DocFileDisplayWidget::onFetchTitles()
{
  for(auto& iter: m_fileTypeItems)
  {
  }
}

void
DocFileDisplayWidget::onRenameToTitle()
{
  for(auto& iter: m_fileTypeItems)
  {
    QTreeWidgetItem* item = iter.second;
    for(int i = 0; i < item->childCount(); ++i)
    {
      QTreeWidgetItem* childItem = item->child(i);
      Qt::CheckState state = childItem->checkState(COLUMN_ID(docs::Select));
      if (state == Qt::Checked) {
        QString oldName(childItem->text(COLUMN_ID(docs::FileName)));
        QString newName(childItem->text(COLUMN_ID(docs::Title)));
        if (newName.isEmpty()) {
          continue;
        }
        cout << oldName << endl;
        cout << newName << endl;
        cout << "======" << endl;
      }
    }
  }
}

void
DocFileDisplayWidget::onItemChanged(QTreeWidgetItem* item, int column)
{
  bool found(false);
  for(auto& i: m_fileTypeItems)
  {
    if (i.second == item) {
      found = true;
    }
  }

  if(!found) {
    return;
  }

  if (column == COLUMN_ID(docs::Select)) {
    Qt::CheckState state = item->checkState(column);
    for (int i = 0; i < item->childCount(); ++i) {
      item->child(i)->setCheckState(COLUMN_ID(docs::Select), state);
    }
  }
}

DocFileDisplayWidget::~DocFileDisplayWidget()
{
  
}

void
DocFileDisplayWidget::resizeColumns()
{
  for(int i = 0; i < columnCount(); ++i) {
    resizeColumnToContents(i);
  }
}

void DocFileDisplayWidget::guessTitles()
{
  stl::StringVec isbns;
  for(auto& iter: m_fileTypeItems)
  {
    QTreeWidgetItem* item = iter.second;
    for (int i = 0; i < item->childCount(); ++i)
    {
      QTreeWidgetItem* childItem = item->child(i);
      string title = childItem->text(COLUMN_ID(docs::FileName)).toStdString();
      auto match = str::find_regex(title, u_isbnReg);
      if (!match) {
        guessTitle(title);
        childItem->setText(COLUMN_ID(docs::Title), title.c_str());
      } else {
        ostringstream os ;
        os << match;
        string s(os.str());
        isbns.push_back(s);
        childItem->setData(COLUMN_ID(docs::Title), Qt::UserRole, s.c_str());
      }
    }
  }

  IsbnDb db;
  boost::for_each(isbns, [&](const string& s) { db.addIsbn(s); });
  db.fetch();
  IsbnDb::Results& results = db.results();

  for(auto& iter: m_fileTypeItems)
  {
    QTreeWidgetItem* item = iter.second;
    for (int i = 0; i < item->childCount(); ++i) {
      QTreeWidgetItem* childItem = item->child(i);
      QVariant v = childItem->data(COLUMN_ID(docs::Title), Qt::UserRole);
      if (!v.isNull()) {
        IsbnDb::Results::const_accessor a;
        if(results.find(a, v.toString().toStdString())) {
          Book b = a->second;
          childItem->setText(COLUMN_ID(docs::Title), b.title.c_str());
        }
      }
    }
  }
  
}

void DocFileDisplayWidget::readDirectory(const QModelIndex& p_index)
{
  for(auto& i: m_fileTypeItems) {
    qDeleteAll(i.second->takeChildren());
  }

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

    for(auto& i: m_fileTypeItems) {
      setFileTypeDefaults(i.second);
    }

    guessTitles();
  }
}
