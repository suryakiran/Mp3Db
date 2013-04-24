#ifndef DirView_hxx_INCLUDED
#define DirView_hxx_INCLUDED

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

private Q_SLOTS:
  void selectCurrentDirectory();

protected:
  void showEvent (QShowEvent*);

private:
  bool m_shown;
};

#endif
