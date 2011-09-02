#ifndef MusicFileDisplayWidget_hpp_INCLUDED
#define MusicFileDisplayWidget_hpp_INCLUDED

#include <QtGui/QTreeWidget>

class MusicFileDisplayWidget : public QTreeWidget
{
  Q_OBJECT

  public:
    MusicFileDisplayWidget (QWidget* p_parent = 0);
    ~MusicFileDisplayWidget();

  public Q_SLOTS:
    void readDirectory (const QModelIndex& p_index);
};

#endif
