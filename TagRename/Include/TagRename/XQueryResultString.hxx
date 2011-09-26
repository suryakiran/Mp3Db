#ifndef XQueryResultString_hxx_INCLUDED
#define XQueryResultString_hxx_INCLUDED

#include <TagRename/XQueryResultTypeOf.hxx>

namespace xquery { namespace parser { 

template < class CharType, CharType ch >
struct string
{
  typedef typename std::basic_string <CharType> string_type;
  typedef typename type_of::rule <string_type>::type rule_type;

  rule_type rule() {
    return qi::lexeme[+(qi::char_ - ch)];
  }
};

}}

#endif
