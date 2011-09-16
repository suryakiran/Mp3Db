#ifndef XQuery_hxx_INCLUDED
#define XQuery_hxx_INCLUDED

#include <TagRename/XQueryResult.hxx>

#include <zorba/zorba.h>
#include <zorba/options.h>
#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;

class XQuery
{
  public:
    XQuery ();

    XQuery& fromString (const std::string& p_string);
    XQuery& fromFile (const fs::path& p_file);
    XQuery& fromFile (const std::string& p_string);

    bool execute ();

    template <class T>
      void setVariable (const std::string& p_varName, const T& p_value)
      {
      }

  private:
    zorba::XQuery_t m_query;
    Zorba_CompilerHints_t m_compilerHints;
    zorba::DynamicContext* m_dynamicContext;
    std::ostringstream m_resultStream;
    XQueryResultPtr m_result;
};

#endif
