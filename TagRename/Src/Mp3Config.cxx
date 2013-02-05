#include <TagRename/Mp3Config.hxx>

#include <Stl/Vector.hxx>

#include <sstream>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
using boost::format;

#include <iostream>
#include <algorithm>
using namespace std;

#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>

#include <yaml-cpp/yaml.h>

Mp3Config* Mp3Config::m_instance = (Mp3Config*) 0;

Mp3Config::Mp3Config()
{
}

Mp3Config*
Mp3Config::instance()
{
  if (!m_instance)
  {
    m_instance = new Mp3Config;
  }

  return m_instance;
}

const stl::StringSet&
Mp3Config::getGenres() const
{
  return m_genres;
}

void Mp3Config::readConfig (const fs::path& p_fileName)
{
  YAML::Node config = YAML::LoadFile(p_fileName.string());
  stl::StringVec genres = config["Config"]["Genres"].as<stl::StringVec>();
  copy(genres.begin(), genres.end(), inserter(m_genres, m_genres.begin()));
  cout << m_genres.size() << endl;
  cout << genres.size() << endl;
  BOOST_FOREACH (string s, m_genres)
    cout << s << endl;
  
}

void Mp3Config::addGenre (const string& p_genre)
{
  // const fs::path& queryFile = m_queryFileMap ["Write Genres"];

  // XQuery query;
  // if (query.compileFile (queryFile))
  // {
  //   query.setVariable("context", m_fileName.string());
  //   query.setVariable("genreName", p_genre);

  //   if (query.execute())
  //   {
  //     m_genres.insert (p_genre);
  //     emitSignal<signal::mp3::config::GenresModified>();
  //   }
  // }
}
