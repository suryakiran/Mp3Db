#include <TagRename/XQuery.hxx>
#include <TagRename/Zorba.hxx>
#include <TagRename/XQueryResultExtractor.hxx>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#include <zorba/iterator.h>

#include <ostream>
using namespace std;

XQuery::XQuery()
  : m_dynamicContext(0),
    m_resultExtractor(0)
{
  zorba::Zorba* z = xml::Zorba::instance();
  m_query = z->createQuery();
  m_resultStream.str("");
  m_compilerHints.opt_level = ZORBA_OPT_LEVEL_O0;
  m_serializerOptions.reset(new Zorba_SerializerOptions_t);
  m_serializerOptions->ser_method = ZORBA_SERIALIZATION_METHOD_TEXT;
}

void
XQuery::setResultExtractor (XQueryResultExtractor* p_extractor)
{
  m_resultExtractor = p_extractor;

  if (m_resultExtractor->extractResultFromXml())
  {
    m_serializerOptions->ser_method = ZORBA_SERIALIZATION_METHOD_XML;
  }
}

void 
XQuery::compileString (const std::string& p_string)
{
  m_query->compile (p_string, m_compilerHints);
}

void
XQuery::compileFile (const fs::path& p_file)
{
  if (fs::exists (p_file))
  {
    fs::fstream fin;
    fin.open (p_file, std::ios_base::in);

    m_query->compile (fin, m_compilerHints);

    fin.close();
  }
}

bool
XQuery::execute (XQueryResultExtractor* p_extractor)
{
  if (p_extractor) {
    setResultExtractor (p_extractor);
  }

  if (m_query->isUpdating())
  {
    m_query->execute();
  }
  else
  {
    ostringstream oss;
    m_query->execute (oss, m_serializerOptions.get());
    if (m_resultExtractor) {
      m_resultExtractor->extractResult (oss.str());
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
