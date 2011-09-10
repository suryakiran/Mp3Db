#include <TagRename/XString.hxx>
#include <boost/scoped_array.hpp>

#include <iostream>
using namespace std;

#include <xercesc/util/XMLString.hpp>

XString::XString (const char* p_string)
  : std::basic_string<XMLCh> ()
{
  boost::scoped_array <XMLCh> ptr (xercesc::XMLString::transcode(p_string));
  assign (ptr.get());
}

XString::XString (const std::string& p_string)
  : std::basic_string<XMLCh> ()
{
  boost::scoped_array <XMLCh> ptr (xercesc::XMLString::transcode(p_string.c_str()));
  assign (ptr.get());
}

XString::XString (const XMLCh* p_string)
  : std::basic_string<XMLCh> (p_string)
{
}

XString::XString (const Mp3String& p_string)
  : std::basic_string<XMLCh>()
{
  TagLib::String s = p_string;
  boost::scoped_array <XMLCh> ptr (xercesc::XMLString::transcode(s.toCString(true)));
  assign (ptr.get());
}
