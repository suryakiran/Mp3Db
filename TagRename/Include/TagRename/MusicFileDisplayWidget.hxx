#ifndef MusicFileDisplayWidget_hxx_INCLUDED
#define MusicFileDisplayWidget_hxx_INCLUDED

#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;

#include <QtGui/QTreeWidget>
#include <map>
#include <TagRename/Mp3String.hxx>

class MusicFileDisplayWidget : public QTreeWidget
{
  Q_OBJECT

  private:
    struct Mp3Tags
    {
      Mp3String track, composer, id;
      Mp3String album, lyrics, artist, genre;
    };

  private:
    typedef std::map<int, QString> HeaderNameMap;
    typedef HeaderNameMap::value_type HeaderNameMapValue;

    typedef std::map <fs::path, Mp3Tags> Mp3TagMap;
    typedef Mp3TagMap::value_type Mp3TagMapValue;
    typedef Mp3TagMap::iterator Mp3TagMapIter;
    typedef Mp3TagMap::const_iterator Mp3TagMapIterC;

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
    void writeXML ();

  private Q_SLOTS:
    void emitSignalIfItemsSelected();

  private:
    bool saveCurrentItem();

  Q_SIGNALS:
    void hasItemSelection(bool);
};

#endif
