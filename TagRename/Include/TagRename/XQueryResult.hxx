#ifndef xquery_result_mapper_Result_hxx_INCLUDED
#define xquery_result_mapper_Result_hxx_INCLUDED

#include <string>
#include <typeinfo>
#include <map>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_const.hpp>

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

namespace xquery { namespace result_mapper { namespace traits {

template <class T> struct result;

}}}

namespace xquery { namespace result_mapper {

template <class Derived>
  struct ResultBase 
  {
    typedef typename xquery::result_mapper::traits::result <Derived>::type result_type;
    typedef typename boost::add_reference<result_type>::type result_type_reference;
    typedef typename boost::add_const<result_type_reference>::type result_type_const_reference;
    typedef ResultBase<Derived> base_type;
    result_type m_result;

    result_type_const_reference getResult()
    {
      return m_result;
    }
  };

}}

#endif
