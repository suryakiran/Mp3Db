#include <TagRename/DirView.hpp>
#include <QtGui/QFileSystemModel>
#include <QtCore/QTimer>

#include <iostream>
using namespace std;

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;

namespace {
  ostream&
    operator<< (ostream& p_os, const QString& p_string)
    {
      p_os << p_string.toStdString() ;
      return p_os;
    }

  wostream&
    operator<< (wostream& p_os, const QString& p_string)
    {
      p_os << p_string.toStdWString() ;
      return p_os;
    }
}

DirView::DirView(QWidget* p_parent)
  : QTreeView(p_parent), m_shown(false)
{
  m_model = new QFileSystemModel(this);

  m_model->setFilter (QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Drives | QDir::Hidden);
  m_model->setRootPath(QDir::homePath());
  setModel (m_model);

  for (int i = 1; i < m_model->columnCount(); ++i) {
    setColumnHidden(i, true);
  }
}

DirView::~DirView()
{
}

void 
DirView::selectCurrentDirectory()
{
  QModelIndex idx (m_model->index(fs::current_path().c_str()));
  selectionModel()->select(idx, QItemSelectionModel::Select);
  setExpanded(idx, true);
  setCurrentIndex(idx);
  scrollTo(idx, QAbstractItemView::PositionAtCenter);
}

void
DirView::showEvent (QShowEvent* p_event)
{
  QTreeView::showEvent(p_event);
  if (!m_shown) {
    m_shown = true;
    QTimer::singleShot(500, this, SLOT(selectCurrentDirectory()));
  }
}
