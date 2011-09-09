#ifndef XString_hpp_INCLUDED
#define XString_hpp_INCLUDED

#include <xercesc/util/XMLChar.hpp>
#include <string>

#include <TagRename/Mp3String.hpp>

class XString : public std::basic_string <XMLCh>
{
  public:
    XString (const char* p_string);
    XString (const std::string& p_string);
    XString (const XMLCh* p_string);
    XString (const Mp3String& p_string);

    operator const XMLCh* ()
    {
      return c_str();
    }
};

#ifdef _X
#undef _X
#endif

#define _X(s) XString(s)

#endif
