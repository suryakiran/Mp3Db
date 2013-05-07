#include <TagRename/Book.hxx>
#include <TagRename/PtreeUtils.hxx>
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/range/algorithm/for_each.hpp>
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
