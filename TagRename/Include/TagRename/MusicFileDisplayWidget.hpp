#ifndef MusicFileDisplayWidget_hpp_INCLUDED
#define MusicFileDisplayWidget_hpp_INCLUDED

#include <QtGui/QTreeWidget>
#include <map>

class MusicFileDisplayWidget : public QTreeWidget
{
  Q_OBJECT

  private:
    typedef enum {
      FileName = 0,
      AlbumName,
      TrackNumber,
      TrackName,
      ArtistNames,
      Genre,
      Composer,
      BitRate,
      Duration,
    } Details;

    typedef enum {
      HiddenDataRole = Qt::UserRole + 1
    } ItemRole;

    typedef std::map<Details, QString> HeaderNameMap;
    typedef HeaderNameMap::value_type HeaderNameMapValue;
    HeaderNameMap m_headerNameMap;

  public:
    MusicFileDisplayWidget (QWidget* p_parent = 0);
    ~MusicFileDisplayWidget();

  public Q_SLOTS:
    void readDirectory (const QModelIndex& p_index);
};

#endif
