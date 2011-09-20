#include <TagRename/XQuery.hxx>
#include <TagRename/Zorba.hxx>
#include <TagRename/XQueryResult.hxx>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#include <zorba/iterator.h>

#include <ostream>
using namespace std;

XQuery::XQuery()
  : m_dynamicContext(0),
    m_resultMapper(0)
{
  zorba::Zorba* z = xml::Zorba::instance();
  m_query = z->createQuery();
  m_resultStream.str("");
  m_compilerHints.opt_level = ZORBA_OPT_LEVEL_O0;
  m_serializerOptions.reset(new Zorba_SerializerOptions_t);
  m_serializerOptions->ser_method = ZORBA_SERIALIZATION_METHOD_TEXT;
}

XQuery::~XQuery()
{
  m_query->close();
}

void
XQuery::setResultMapper (xquery::result_mapper::Result* p_mapper)
{
  m_resultMapper = p_mapper;

  if (m_resultMapper->parseFromXml())
  {
    m_serializerOptions->ser_method = ZORBA_SERIALIZATION_METHOD_XML;
  }
}

bool 
XQuery::compileString (const std::string& p_string)
{
  try
  {
    m_query->compile (p_string, m_compilerHints);
  }
  catch (zorba::ZorbaException& exc)
  {
    m_compileError = exc.what();
    return false;
  }

  return true;
}

bool
XQuery::compileFile (const fs::path& p_file)
{
  if (fs::exists (p_file))
  {
    fs::fstream fin;
    fin.open (p_file, std::ios_base::in);

    try 
    {
      m_query->compile (fin, m_compilerHints);
    }
    catch (zorba::ZorbaException& exc)
    {
      m_compileError = exc.what();
      return false;
    }

    fin.close();
    return true;
  }

  return false;
}

bool
XQuery::execute (xquery::result_mapper::Result* p_mapper)
{
  if (p_mapper) {
    setResultMapper (p_mapper);
  }

  if (m_query->isUpdating())
  {
    m_query->execute();
  }
  else
  {
    ostringstream oss;
    m_query->execute (oss, m_serializerOptions.get());
    if (m_resultMapper) {
      m_resultMapper->parse (oss.str());
    }
  }

  return true;
}

zorba::Zorba*
XQuery::getZorbaInstance()
{
  return xml::Zorba::instance();
}

void
XQuery::setVariable (const string& p_varName, const string& p_value)
{
  if(!m_dynamicContext) {
    m_dynamicContext = m_query->getDynamicContext();
  }
  m_dynamicContext->setVariable (p_varName, 
      getZorbaInstance()->getItemFactory()->createString(p_value));
}
