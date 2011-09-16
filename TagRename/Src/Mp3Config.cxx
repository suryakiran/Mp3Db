#include <TagRename/Mp3Config.hxx>
#include <TagRename/Zorba.hxx>
#include <TagRename/XString.hxx>

#include <sstream>
#include <boost/format.hpp>
using boost::format;

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
using namespace boost::spirit;

#include <zorba/zorba.h>
#include <zorba/store_manager.h>
#include <zorba/zorba_exception.h>
#include <zorba/iterator.h>
using namespace zorba;

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
  Zorba* z = xml::Zorba::instance();

  if (fs::exists (p_fileName))
  {
    ostringstream oss;
    istringstream iss;

    oss 
      << "declare variable $file external;"
      << "for $x in doc($file)/Queries/Query\n"
      << "return concat('[', data($x/@name), '=', data($x), ']')" << endl
      ;

    XQuery_t query = z->compileQuery (oss.str());
    DynamicContext* ctx = query->getDynamicContext();
    ctx->setVariable("file", z->getItemFactory()->createString(p_fileName.string()));
    Zorba_SerializerOptions_t* options = new Zorba_SerializerOptions_t;
    options->ser_method = ZORBA_SERIALIZATION_METHOD_TEXT;

    oss.str("");
    query->execute(oss, options);
    iss.str(oss.str());
    string l;
    while (getline (iss, l, '\n'))
    {
//      cout << l << endl;
      //vector<string> vs;
      //string key, value;
      //str::split (vs, l, boost::is_any_of("[-]"), boost::token_compress_on);
      //string::iterator b (l.begin());
      //const bool result = qi::phrase_parse (b, l.end(),
      //    *(qi::lit("[")) >> *(qi::char_-'=') >> '=' >> *(qi::char_-']') >> ']',
      //      ascii::space, key, value);
      //m_queryFileMap[key] = m_xqDir/value;
    }
  }

  readGenres();
}

void 
Mp3Config::readGenres()
{
  const fs::path& queryFile = m_queryFileMap["ReadGenres"];

  fs::fstream fin;
  fin.open (queryFile, ios_base::in);

  try 
  {
    Zorba* z = xml::Zorba::instance();
    cout << "HE HE HE" << endl;
    XQuery_t query = z->compileQuery (fin);
    DynamicContext* ctx = query->getDynamicContext();
    ctx->setVariable("context", z->getItemFactory()->createString(m_fileName.string()));
    Iterator_t iter (query->iterator());
    iter->open();
    Item item;
    while (iter->next(item))
    {
      m_genres.insert (item.getStringValue().str());
    }

    iter->close();
    fin.close();
  }
  catch (ZorbaException& exc)
  {
    cout << exc.what() << endl;
  }
}

void Mp3Config::addGenre (const string& p_genre)
{
  const fs::path& queryFile = m_queryFileMap ["WriteGenres"];
  fs::fstream fin;
  fin.open (queryFile, ios_base::in);
  bool success (true);

  try
  {
    Zorba* z = xml::Zorba::instance();
    XQuery_t query = z->compileQuery (fin);
    DynamicContext* ctx = query->getDynamicContext();
    ctx->setVariable("context", z->getItemFactory()->createString(m_fileName.string()));
    ctx->setVariable("genreName", z->getItemFactory()->createString(p_genre));

    Iterator_t iter (query->iterator());
    iter->open();
    Item item;
    while (iter->next(item));
    iter->close();

    query->close();
  }
  catch (ZorbaException& exc)
  {
    success = false;
  }

  fin.close();

  if(success)
  {
    m_genres.insert (p_genre);
    emitSignal<GenresModified>();
  }
}
