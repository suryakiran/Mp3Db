#ifndef XQueryResultRule_hxx_INCLUDED
#define XQueryResultRule_hxx_INCLUDED

#include <TagRename/XQueryResultTypeOf.hxx>
#include <TagRename/XQueryResultString.hxx>

#include <string>
#include <boost/spirit/include/qi.hpp>
namespace qi = boost::spirit::qi;

namespace xquery { namespace parser {
  template <class T> struct map;

  template <> struct map <std::string>
  {
    typedef type_of::rule<std::string>::type rule_type;
    typedef std::string::value_type char_type;

    rule_type key_rule() {
      return string<char_type, '"'> ().rule();
    }

    rule_type value_rule() {
      return string<char_type, ']'> ().rule();
    }
  };
}}

namespace xquery { namespace parser {
  template <class T> struct list;

  template <> struct list<std::string>
  {
    typedef type_of::rule<std::string>::type rule_type;
    typedef std::string::value_type char_type;

    rule_type rule() {
      return string<char_type, ';'>().rule();
    }
  };
}}
#endif
