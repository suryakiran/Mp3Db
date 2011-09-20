#ifndef List_hxx_INCLUDED
#define List_hxx_INCLUDED

#include <TagRename/XQueryResultExtractor.hxx>
#include <TagRename/XQueryResultRule.hxx>
#include <sstream>
#include <iostream>

namespace xquery { namespace result_mapper {

template <class KeyType>
class List : public Result
{
  private:
    typedef std::list<KeyType> ResultType;
    ResultType m_result;

  public:
    List()
      : Result()
    {
    }

    List (const std::string& p_input)
      : Result(p_input)
    {
    }

    const ResultType& getResult() const {
      return m_result;
    }

  protected:
    virtual void parseImp ()
    {
      std::istringstream iss;
      iss.str (m_input);
      std::string l;

      using qi::_1;
      while (getline (iss, l, '\n'))
      {
        std::string::iterator beg (l.begin());

        xquery::result::list<KeyType> list_;

        qi::phrase_parse (
            beg,
            l.end(),
            list_.rule() % ';',
            qi::ascii::space,
            m_result
            );
      }
    }
};

}}

#endif
