#ifndef xquery_result_mapper_Map_hxx_INCLUDED
#define xquery_result_mapper_Map_hxx_INCLUDED

#include <TagRename/XQueryResult.hxx>
#include <TagRename/XQueryResultRule.hxx>
#include <sstream>
#include <iostream>

namespace xquery { namespace result_mapper {

template <class KeyType, class ValueType>
class Map : public Result
{
  private:
    typedef std::map<KeyType, ValueType> ResultType;
    ResultType m_result;

  public:
    Map()
      : Result()
    {
    }

    Map (const std::string& p_input)
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

      while (getline (iss, l, '\n'))
      {
        std::string::iterator beg (l.begin());
        KeyType key;
        ValueType value;

        xquery::parser::map<KeyType> map_key;
        xquery::parser::map<ValueType> map_value;

        if (
            qi::phrase_parse (
              beg,
              l.end(),
              qi::lit("[\"") >> 
              map_key.key_rule() >>
              '"' >> '=' >> 
              map_value.value_rule() >> ']',
              qi::ascii::space,
              key, value
              )
           )
        {
          m_result[key] = value;
        }
      }
    }
};

}}

#endif
