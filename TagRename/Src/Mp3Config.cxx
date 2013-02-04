#include <TagRename/Mp3Config.hxx>
#include <TagRename/XString.hxx>

#include <Stl/Map.hxx>
#include <Stl/List.hxx>

#include <sstream>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
using boost::format;

#include <iostream>
using namespace std;

#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
using namespace xercesc;

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
#if 0
  YAML::Node config = YAML::LoadFile (p_fileName.string());
  m_fileName = p_fileName.parent_path().parent_path()/"Mp3Config.yml";
  m_xqDir = p_fileName.parent_path();
  zorba::Zorba* z = xml::Zorba::instance();

  if (fs::exists (p_fileName))
  {
    ostringstream oss;

    oss 
      << "declare variable $file external;"
      << "for $x in doc($file)/Queries/Query\n"
      << "return concat('[\"', data($x/@name), '\"=', data($x), ']\n')" << endl
      ;

    xquery::result_mapper::Map<string, string> resultMapper;
    XQuery query;
    if (query.compileString(oss.str()))
    {
      query.setVariable ("file", p_fileName.string());
      query.execute (&resultMapper);
      const stl::StringMap& results (resultMapper.getResult());
      BOOST_FOREACH (const stl::StringMapValue& smv, results)
      {
        m_queryFileMap[smv.first] = m_xqDir/smv.second;
      }
    }
  }
  readGenres();
#endif
}

void 
Mp3Config::readGenres()
{
#if 0
  const fs::path& queryFile = m_queryFileMap["Read Genres"];

  xquery::result_mapper::List<string> resultMapper;
  XQuery query;
  if (query.compileFile (queryFile))
  {
    query.setVariable("context", m_fileName.string());
    query.execute(&resultMapper);
    const stl::StringList& results (resultMapper.getResult());
    std::copy (results.begin(), results.end(), std::inserter(m_genres, m_genres.begin()));
  }
#endif
}

void Mp3Config::addGenre (const string& p_genre)
{
#if 0
  const fs::path& queryFile = m_queryFileMap ["Write Genres"];

  XQuery query;
  if (query.compileFile (queryFile))
  {
    query.setVariable("context", m_fileName.string());
    query.setVariable("genreName", p_genre);

    if (query.execute())
    {
      m_genres.insert (p_genre);
      emitSignal<signal::mp3::config::GenresModified>();
    }
  }
#endif
}
