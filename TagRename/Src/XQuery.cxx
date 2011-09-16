#include <TagRename/XQuery.hxx>
#include <TagRename/Zorba.hxx>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#include <zorba/iterator.h>

XQuery::XQuery()
{
  zorba::Zorba* z = xml::Zorba::instance();
  m_query = z->createQuery();
  m_dynamicContext = m_query->getDynamicContext();
  m_resultStream.str("");
  m_compilerHints.opt_level = ZORBA_OPT_LEVEL_O0;
}

XQuery& 
XQuery::fromString (const std::string& p_string)
{
  m_query->compile (p_string, m_compilerHints);
  return *this;
}

XQuery&
XQuery::fromFile (const std::string& p_file)
{
  return fromFile (fs::path(p_file));
}

XQuery&
XQuery::fromFile (const fs::path& p_file)
{
  if (fs::exists (p_file))
  {
    fs::fstream fin;
    fin.open (p_file, std::ios_base::in);

    m_query->compile (fin, m_compilerHints);

    fin.close();
  }
  return *this;
}

bool
XQuery::execute ()
{
  if (m_query->isUpdating())
  {
    m_query->execute();
  }
  else
  {
    zorba::Iterator_t iter (m_query->iterator());
    zorba::Item item;
    while (iter->next(item));
  }

  return true;
}
