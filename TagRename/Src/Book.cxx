#include <TagRename/Book.hxx>
#include <TagRename/PtreeUtils.hxx>
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/foreach.hpp>
#include <limits>

using namespace std;
namespace str = boost::algorithm;

Book::Book()
  :valid(false)
{
}

bool
Book::operator<(const Book& other)
{
  return isbn.second < other.isbn.second;
}

bool
Book::operator== (const Book& other)
{
  return (isbn.second == other.isbn.second);
}

size_t
Book::hash(const string& s)
{
  try {
    return boost::lexical_cast<size_t>(s);
  } catch (boost::bad_lexical_cast&) {
    return numeric_limits<size_t>::max();
  }
}

bool
Book::equal(const string& first, const string& second) const
{
  return first == second;
}

Book::Book(const YAML::Node& node)
  : valid (true)
{
  isbn.first = node["isbn10"].Scalar();
  isbn.second = node["isbn13"].Scalar();
  if (isbn.first.empty() && isbn.second.empty()) {
    valid = false;
    return;
  }

  title = node["title_long"].Scalar();
  if (title.empty()) {
    title = node["title"].Scalar();
    if (title.empty()) {
      title = node["title_latin"].Scalar();
    }
  }

  publisher = node["publisher_name"].Scalar();
  BOOST_FOREACH(auto& n, node["author_data"]) {
    authors.push_back(n["name"].Scalar());
  }
}
   
Book::Book (const boost::property_tree::ptree& pt)
  : valid(true)
{
  isbn.first = getAttributeValue<string>(pt, "<xmlattr>.isbn", "");
  isbn.second = getAttributeValue<string>(pt, "<xmlattr>.isbn13", "");
  if (isbn.first.empty() && isbn.second.empty())
  {
    valid = false;
    return;
  }

  title = pt.get<string>("TitleLong");
  if (title.empty()) {
    title = pt.get<string>("Title");
  }
  publisher = pt.get<string>("PublisherText");
  string authorsText(pt.get<string>("AuthorsText"));
  str::split(authors, authorsText, str::is_any_of(","), str::token_compress_on);
  boost::for_each(authors,
                  [](string& s){
                    str::trim(s);
                  });
  boost::remove_erase(authors, "");
}
