#include <TagRename/Mp3Config.hxx>
#include <TagRename/Zorba.hxx>
#include <TagRename/XString.hxx>

#include <sstream>
#include <boost/format.hpp>
using boost::format;

#include <zorba/zorba.h>
#include <zorba/store_manager.h>
#include <zorba/zorba_exception.h>
#include <zorba/iterator.h>
using namespace zorba;

#include <iostream>
#include <string>
using namespace std;

#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string/trim.hpp>
namespace str = boost::algorithm;

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
using namespace xercesc;

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

const set<string>&
Mp3Config::getGenres() const
{
  return m_genres;
}

void 
Mp3Config::readFile(const string& p_fileName)
{
  m_fileName = p_fileName;
  m_xqDir = m_fileName.parent_path()/"xq";
  const fs::path& queryConfigFile = (m_xqDir/"ConfigQueries.xml");

  Zorba* z = xml::Zorba::instance();

  if (fs::exists (queryConfigFile))
  {
    ostringstream oss;
    istringstream iss;

    oss 
      << "declare variable $file external;"
      << "for $x in doc($file)/Queries/Query "
      << "return (\"[\", data($x/@name), \"-\", data($x),\"]\n\")" << endl;

    XQuery_t query = z->compileQuery (oss.str());
    DynamicContext* ctx = query->getDynamicContext();
    ctx->setVariable("file", z->getItemFactory()->createString(queryConfigFile.string()));
    Zorba_SerializerOptions_t* options = new Zorba_SerializerOptions_t;
    options->ser_method = ZORBA_SERIALIZATION_METHOD_TEXT;

    oss.clear();
    query->execute(oss, options);
    iss.str(oss.str());
    string l;
    while (getline (iss, l, '\n'))
    {
      str::trim(l);
      cout << l << endl;
    }
  }


  format fmt ("for $x in doc('%1%')/Config/Genres/Genre return data($x)");
  XQuery_t query = z->compileQuery ((fmt % p_fileName).str());
  Iterator_t iter (query->iterator());
  iter->open();
  Item item;
  while (iter->next(item))
  {
    m_genres.insert (item.getStringValue().str());
  }
  iter->close();
}
