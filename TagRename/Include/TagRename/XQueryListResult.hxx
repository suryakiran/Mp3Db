#ifndef xquery_result_mapper_List_hxx_INCLUDED
#define xquery_result_mapper_List_hxx_INCLUDED

#include <TagRename/XQueryResult.hxx>
#include <TagRename/XQueryResultRule.hxx>
#include <sstream>
#include <iostream>
#include <list>

namespace xquery { namespace result_mapper {

  template <class T> class List;

  namespace traits {

    template <class K>
      struct result < xquery::result_mapper::List<K> >
      {
        typedef std::list <K> type;
      };
  }

  template <class KeyType>
  class List : public Result, public ResultBase < List <KeyType> >
  {
    typedef typename List::base_type base_type;
    using base_type::m_result;

    public:
      List()
        : Result()
      {
      }
  
      List (const std::string& p_input)
        : Result(p_input)
      {
      }
  
    private:
      virtual void parseImp ()
      {
        std::istringstream iss;
        iss.str (m_input);
        std::string l;

        using qi::_1;
        while (getline (iss, l, '\n'))
        {
          std::string::iterator beg (l.begin());

          xquery::parser::list<KeyType> list_;

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
