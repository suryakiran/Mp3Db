#ifndef Mp3Config_hxx_INCLUDED
#define Mp3Config_hxx_INCLUDED

#include <Signals/Signals.hxx>
#include <Stl/Set.hxx>
#include <Stl/Map.hxx>

#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;

class Mp3Config : public SignalBase
{
  public:
    struct GenresModified;

  private:
    typedef fusion::map <
      fusion::pair <GenresModified, BOOST_SIGNAL(void())>
      > SignalsMap;

#include <Signals/SignalMacros.hxx>

  private:
    DEFINE_MAP(std::string, fs::path, StringPathMap);

  public:
    static Mp3Config* instance();
    void readConfig (const fs::path& p_fileName);
    const stl::StringSet& getGenres() const;

    void addGenre (const std::string& p_genre);

  private:
    static Mp3Config* m_instance;
    stl::StringSet m_genres;
    StringPathMap m_queryFileMap;
    fs::path m_fileName;
    fs::path m_xqDir;

  private:
    Mp3Config();
    void readGenres();
};

#endif
