#ifndef XQueryResult_hxx_INCLUDED
#define XQueryResult_hxx_INCLUDED

#include <boost/shared_ptr.hpp>

class XQueryResult
{
  public:
    XQueryResult();
    ~XQueryResult();
};

typedef boost::shared_ptr<XQueryResult> XQueryResultPtr;

#endif
