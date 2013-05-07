#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <TagRename/Curl.hxx>
#include <TagRename/PtreeUtils.hxx>
#include <TagRename/AppConfig.hxx>

using namespace std;

namespace {
  
  size_t writeFunction (void* buffer, size_t size, size_t nmemb, void* userp)
  {
    Curl* curl = reinterpret_cast<Curl*> (userp);
    if (curl) {
      return curl->writeData(buffer, size, nmemb);
    }
    return 0;
  }
  
}

Curl::Curl(const string& searchType,
           const string& searchString)
  : m_searchString(searchString),
    m_searchType(searchType),
    m_result(new Result)
{
  init();
}

Curl::Curl(const Curl& other)
  : m_searchString(other.m_searchString),
    m_searchType(other.m_searchType),
    m_result(new Result)
{
  init();
}

void
Curl::setPageNumber(int pageNum)
{
  m_form.set("page_number", pageNum);
}

size_t
Curl::writeData(void* buffer, size_t size, size_t nmemb)
{
  size_t count = size * nmemb;
  fstream fout;
  ostringstream os;
  os.write(reinterpret_cast<const char*>(buffer), count);
  m_outputString += os.str();
    
  return count;
}

int
Curl::numResults()
{
  return getAttributeValue(*m_result, "ISBNdb.BookList.<xmlattr>.total_results", 0);
}

int
Curl::pageSize()
{
  return (getAttributeValue(*m_result, "ISBNdb.BookList.<xmlattr>.page_size", 10));
}
  
bool
Curl::isDone()
{
  int pageNumber(getAttributeValue(*m_result, "ISBNdb.BookList.<xmlattr>.page_number", 1));
  return (numResults() <= (pageSize() * pageNumber));
}

void
Curl::readData()
{
  istringstream is;
  is.str(m_outputString);
  boost::property_tree::read_xml(is, *m_result, 
                                 boost::property_tree::xml_parser::trim_whitespace |
                                 boost::property_tree::xml_parser::no_comments);
}

Curl::ResultConstReference&
Curl::results() const
{
  return m_result->get_child("ISBNdb.BookList");
}

void
Curl::operator()()
{
  CURLcode res = curl_easy_perform(m_handle.get());
  if (res == CURLE_OK){
    readData();
    m_outputString.clear();
  }
}

void
Curl::init()
{
  m_url = AppConfig::instance().isbnDbUrl();
  m_keys = AppConfig::instance().isbnDbKeys();
  m_handle.reset(curl_easy_init());
  m_form.set("access_key", m_keys.front().c_str());
  m_form.set("index1", m_searchType);
  m_form.set("value1", m_searchString);

  curl_easy_setopt(m_handle.get(), CURLOPT_URL, m_url.c_str());
  curl_easy_setopt(m_handle.get(), CURLOPT_WRITEFUNCTION, writeFunction);
  curl_easy_setopt(m_handle.get(), CURLOPT_WRITEDATA, this);
  curl_easy_setopt(m_handle.get(), CURLOPT_HTTPPOST, m_form());
}
