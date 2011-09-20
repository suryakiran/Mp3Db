#ifndef XQuery_hxx_INCLUDED
#define XQuery_hxx_INCLUDED

#include <zorba/zorba.h>
#include <zorba/options.h>
#include <boost/filesystem/path.hpp>
#include <boost/scoped_ptr.hpp>
namespace fs = boost::filesystem;

namespace xquery { namespace result_mapper {
  class Result;
}}

class XQuery
{
  public:
    XQuery ();
    ~XQuery ();

    bool compileString (const std::string& p_string);
    bool compileFile (const fs::path& p_file);

    void setResultMapper(xquery::result_mapper::Result* p_extractor);

    bool execute (xquery::result_mapper::Result* p_extractor = 0);
    void setVariable (const std::string& p_varName, const std::string& p_value);

  private:
    zorba::Zorba* getZorbaInstance();

  private:
    zorba::XQuery_t m_query;
    Zorba_CompilerHints_t m_compilerHints;
    boost::scoped_ptr<Zorba_SerializerOptions_t> m_serializerOptions;
    zorba::DynamicContext* m_dynamicContext;
    std::ostringstream m_resultStream;
    std::string m_compileError;
    xquery::result_mapper::Result* m_resultMapper;
};

#endif
