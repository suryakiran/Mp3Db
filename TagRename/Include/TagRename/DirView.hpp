#ifndef DirView_hpp_INCLUDED
#define DirView_hpp_INCLUDED

#include <QtGui/QTreeView>

class QFileSystemModel;

class DirView : public QTreeView
{
  Q_OBJECT

  public:
    DirView (QWidget* p_parent = 0);
    ~DirView();

  private:
      QFileSystemModel* m_model;
};

#endif
