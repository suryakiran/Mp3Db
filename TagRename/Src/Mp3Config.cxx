#include <TagRename/Mp3Config.hxx>
#include <TagRename/Zorba.hxx>
#include <TagRename/XString.hxx>
#include <TagRename/XQueryMapResultExtractor.hxx>
#include <TagRename/XQueryListResultExtractor.hxx>
#include <TagRename/XQuery.hxx>

#include <Stl/Map.hxx>
#include <Stl/List.hxx>

#include <sstream>
#include <boost/format.hpp>
using boost::format;

#include <zorba/zorba.h>
#include <zorba/store_manager.h>
#include <zorba/zorba_exception.h>
#include <zorba/iterator.h>

#include <iostream>
using namespace std;

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/foreach.hpp>
namespace str = boost::algorithm;

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
using namespace xercesc;

Mp3Config* Mp3Config::m_instance = (Mp3Config*) 0;

Mp3Config::Mp3Config()
{
  fusion::for_each(m_signals, initialize());
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

const StringSet&
Mp3Config::getGenres() const
{
  return m_genres;
}

void Mp3Config::writeGenres()
{
}

void Mp3Config::readConfig (const fs::path& p_fileName)
{
  m_fileName = p_fileName.parent_path().parent_path()/"Mp3Config.xml";
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

    XQueryMapResultExtractor<string, string> mapResultExtractor;
    XQuery query;
    query.compileString(oss.str());
    query.setVariable ("file", p_fileName.string());
    query.execute (&mapResultExtractor);
    BOOST_FOREACH (const StringMapValue& smv, mapResultExtractor.getResult())
    {
      m_queryFileMap[smv.first] = m_xqDir/smv.second;
    }
  }

  readGenres();
}

void 
Mp3Config::readGenres()
{
  const fs::path& queryFile = m_queryFileMap["Read Genres"];

  XQueryListResultExtractor<string> listResultExtractor;
  XQuery query;
  query.compileFile (queryFile);
  query.setVariable("context", m_fileName.string());
  query.execute(&listResultExtractor);
  const stl::StringList& results (listResultExtractor.getResult());
  std::copy (results.begin(), results.end(), std::inserter(m_genres, m_genres.begin()));
}

void Mp3Config::addGenre (const string& p_genre)
{
  const fs::path& queryFile = m_queryFileMap ["Write Genres"];

  XQuery query;
  query.compileFile (queryFile);
  query.setVariable("context", m_fileName.string());
  query.setVariable("genreName", p_genre);

  if (query.execute())
  {
    m_genres.insert (p_genre);
    emitSignal<GenresModified>();
  }
}
