#include <TagRename/DirView.hpp>
#include <QtGui/QFileSystemModel>

#include <iostream>
using namespace std;

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;

DirView::DirView(QWidget* p_parent)
  : QTreeView(p_parent)
{
  m_model = new QFileSystemModel(this);

  m_model->setFilter (m_model->filter() | QDir::Hidden);
  m_model->setRootPath(QDir::homePath());
  setModel (m_model);

  for (int i = 1; i < m_model->columnCount(); ++i) {
    setColumnHidden(i, true);
  }

  QModelIndex idx (m_model->index(fs::current_path().c_str()));
  selectionModel()->select(idx, QItemSelectionModel::Select);
  setExpanded(idx, true);
}

DirView::~DirView()
{
}
