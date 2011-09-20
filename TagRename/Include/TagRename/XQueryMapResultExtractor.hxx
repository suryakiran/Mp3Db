#ifndef XQueryMapResultExtractor_hxx_INCLUDED
#define XQueryMapResultExtractor_hxx_INCLUDED

#include <TagRename/XQueryResultExtractor.hxx>
#include <TagRename/XQueryResultRule.hxx>
#include <sstream>
#include <iostream>

template <class KeyType, class ValueType>
class XQueryMapResultExtractor : public XQueryResultExtractor
{
  private:
    typedef std::map<KeyType, ValueType> ResultType;
    ResultType m_result;

  public:
    XQueryMapResultExtractor()
      : XQueryResultExtractor()
    {
    }

    XQueryMapResultExtractor (const std::string& p_input)
      : XQueryResultExtractor(p_input)
    {
    }

    const ResultType& getResult() const {
      return m_result;
    }

  protected:
    virtual void extractResultImp ()
    {
      std::istringstream iss;
      iss.str (m_input);
      std::string l;

      while (getline (iss, l, '\n'))
      {
        std::string::iterator beg (l.begin());
        KeyType key;
        ValueType value;

        xquery::result::map<KeyType> map_key;
        xquery::result::map<ValueType> map_value;

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

#endif
