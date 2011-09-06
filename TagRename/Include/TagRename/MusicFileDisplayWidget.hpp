#ifndef MusicFileDisplayWidget_hpp_INCLUDED
#define MusicFileDisplayWidget_hpp_INCLUDED

#include <QtGui/QTreeWidget>
#include <map>

class MusicFileDisplayWidget : public QTreeWidget
{
  Q_OBJECT

  private:
    typedef std::map<int, QString> HeaderNameMap;
    typedef HeaderNameMap::value_type HeaderNameMapValue;
    HeaderNameMap m_headerNameMap;

  public:
    MusicFileDisplayWidget (QWidget* p_parent = 0);
    ~MusicFileDisplayWidget();

  public Q_SLOTS:
    void readDirectory (const QModelIndex& p_index);

  private Q_SLOTS:
    void emitSignalIfItemsSelected();

  Q_SIGNALS:
    void hasItemSelection(bool);
};

#endif
