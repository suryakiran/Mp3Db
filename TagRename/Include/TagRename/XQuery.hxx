#ifndef XQuery_hxx_INCLUDED
#define XQuery_hxx_INCLUDED

#include <zorba/zorba.h>
#include <zorba/options.h>
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
namespace fs = boost::filesystem;

class XQueryResultExtractor;

class XQuery
{
  public:
    XQuery ();
    ~XQuery ();

    void compileString (const std::string& p_string);
    void compileFile (const fs::path& p_file);

    void setResultExtractor(XQueryResultExtractor* p_extractor);

    bool execute (XQueryResultExtractor* p_extractor = 0);
    void setVariable (const std::string& p_varName, const std::string& p_value);

  private:
    zorba::Zorba* getZorbaInstance();

  private:
    zorba::XQuery_t m_query;
    Zorba_CompilerHints_t m_compilerHints;
    boost::shared_ptr<Zorba_SerializerOptions_t> m_serializerOptions;
    zorba::DynamicContext* m_dynamicContext;
    std::ostringstream m_resultStream;
    XQueryResultExtractor* m_resultExtractor;
};

#endif
