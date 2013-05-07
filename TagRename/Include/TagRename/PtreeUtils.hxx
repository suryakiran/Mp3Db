#ifndef PtreeUtils_hxx_INCLUDED
#define PtreeUtils_hxx_INCLUDED

#include <boost/property_tree/ptree.hpp>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>

template <class T>
T getAttributeValue(const boost::property_tree::ptree& pt,
                    const std::string& attr,
                    const T& defaultValue)
{
  std::ostringstream os;
  os << defaultValue ;
  auto key = pt.get_child_optional(attr);
  if (!key) {
    return defaultValue;
  }

  T value(defaultValue);
  try {
    value = boost::lexical_cast<T>(key->get_value<std::string>(os.str()));
  } catch (boost::bad_lexical_cast&) { }
  return value;
}

#endif
