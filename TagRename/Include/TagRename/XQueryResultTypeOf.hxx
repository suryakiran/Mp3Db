#ifndef XQueryResultTypeOf_hxx_INCLUDED
#define XQueryResultTypeOf_hxx_INCLUDED

#include <boost/spirit/include/qi.hpp>
namespace qi = boost::spirit::qi;

namespace xquery { namespace result { namespace type_of {

  template <class T>
    struct rule
    {
      typedef typename qi::rule <
        std::string::iterator
        , T()
        , qi::ascii::space_type
        > type;
    };
}}}


#endif
