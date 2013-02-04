#ifndef MusicFileDisplayWidget_hxx_INCLUDED
#define MusicFileDisplayWidget_hxx_INCLUDED

#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;
#include <Stl/Map.hxx>

#include <QtGui/QTreeWidget>
#include <TagRename/Mp3String.hxx>

class MusicFileDisplayWidget : public QTreeWidget
{
  Q_OBJECT

  private:
    struct Mp3Tags
    {
      Mp3String track, composer, id, year;
      Mp3String album, lyrics, artist, genre;
    };

  private:
    DEFINE_MAP(int, QString, HeaderNameMap);
    DEFINE_MAP(fs::path, Mp3Tags, Mp3TagMap);

  private:
    HeaderNameMap m_headerNameMap;
    Mp3TagMap m_tags;
    fs::path m_currentDirectory;

  public:
    MusicFileDisplayWidget (QWidget* p_parent = 0);
    ~MusicFileDisplayWidget();

  public Q_SLOTS:
    void readDirectory (const QModelIndex& p_index);
    void saveCurrentAndGotoPrevItem ();
    void saveCurrentAndGotoNextItem ();
    // void writeXML ();

  private Q_SLOTS:
    void emitSignalIfItemsSelected();

  private:
    bool saveCurrentItem();

  Q_SIGNALS:
    void hasItemSelection(bool);
};

#endif
