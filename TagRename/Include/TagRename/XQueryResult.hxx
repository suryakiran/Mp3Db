#ifndef xquery_result_mapper_Result_hxx_INCLUDED
#define xquery_result_mapper_Result_hxx_INCLUDED

#include <string>

namespace xquery { namespace result_mapper {

class Result
{
  public:
    Result ()
    {
    }

    Result (const std::string& p_input)
      : m_input(p_input)
    {
    }

    void parse(const std::string& p_input = std::string("")) {
      if (!p_input.empty()) {
        m_input = p_input;
      }
      parseImp() ;
    }

    virtual bool parseFromXml() {
      return false;
    }

  protected:
    virtual void parseImp() = 0;

  protected:
    std::string m_input;
};

}}

#endif
