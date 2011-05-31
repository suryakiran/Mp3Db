#include <TagRename/DirView.hpp>
#include <QtGui/QFileSystemModel>

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
}

DirView::~DirView()
{
}
