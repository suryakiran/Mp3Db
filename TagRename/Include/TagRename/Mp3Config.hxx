#ifndef Mp3Config_hxx_INCLUDED
#define Mp3Config_hxx_INCLUDED

#include <set>
#include <string>

#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;

class Mp3Config
{
  public:
    static Mp3Config* instance();
    void readFile (const std::string& p_fileName);
    const std::set<std::string>& getGenres() const;

  private:
    static Mp3Config* m_instance;
    std::set<std::string> m_genres;
    fs::path m_fileName;
    fs::path m_xqDir;

  private:
    Mp3Config();
};

#endif
