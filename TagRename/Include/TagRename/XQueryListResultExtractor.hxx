#ifndef XQueryListResultExtractor_hxx_INCLUDED
#define XQueryListResultExtractor_hxx_INCLUDED

#include <TagRename/XQueryResultExtractor.hxx>
#include <TagRename/XQueryResultRule.hxx>
#include <sstream>
#include <iostream>

template <class KeyType>
class XQueryListResultExtractor : public XQueryResultExtractor
{
  private:
    typedef std::list<KeyType> ResultType;
    ResultType m_result;

  public:
    XQueryListResultExtractor()
      : XQueryResultExtractor()
    {
    }

    XQueryListResultExtractor (const std::string& p_input)
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

#endif
