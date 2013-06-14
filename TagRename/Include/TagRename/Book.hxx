#ifndef Book_hxx_INCLUDED
#define Book_hxx_INCLUDED

#include <boost/property_tree/ptree.hpp>
#include <Stl/Vector.hxx>
#include <yaml-cpp/yaml.h>

struct Book
{
  std::string title;
  stl::StringVec authors;
  std::pair<std::string, std::string> isbn; //(isbn, isbn13)
  std::string publisher;
  bool valid;

  Book();
  Book(const boost::property_tree::ptree& pt);
  Book(const YAML::Node& node);
  bool operator<  (const Book& other);
  bool operator== (const Book& other);
  size_t hash (const std::string& s);
  bool equal (const std::string& first, const std::string& second) const;
  
};

#endif
