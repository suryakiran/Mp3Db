#ifndef Mp3Config_hxx_INCLUDED
#define Mp3Config_hxx_INCLUDED

#include <Stl/Set.hxx>
#include <Stl/Map.hxx>

#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;

class Mp3Config
{
  public:
    static Mp3Config* instance();
    void readFile (const std::string& p_fileName);
    const StringSet& getGenres() const;

  private:
    static Mp3Config* m_instance;
    StringSet m_genres;
    StringMap m_queryFileMap;
    fs::path m_fileName;
    fs::path m_xqDir;

  private:
    Mp3Config();
};

#endif
