#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <TagRename/Curl.hxx>
#include <TagRename/PtreeUtils.hxx>
#include <TagRename/AppConfig.hxx>
#include <iostream>
#include <boost/foreach.hpp>

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
    m_searchType(searchType), m_resultType(SingleResult)
{
  init();
}

Curl::Curl(const Curl& other)
  : m_searchString(other.m_searchString),
    m_searchType(other.m_searchType), m_resultType(other.m_resultType)
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
  ostringstream os;
  os.write(reinterpret_cast<const char*>(buffer), count);
  m_outputString += os.str();
    
  return count;
}

int
Curl::numResults()
{
  if (m_resultType == SingleResult) {
    return 1;
  }
  // return getAttributeValue(*m_result, "ISBNdb.BookList.<xmlattr>.total_results", 0);
  return 0;
}

int
Curl::pageSize()
{
  if (m_resultType == SingleResult) {
    return 1;
  }
  // return (getAttributeValue(*m_result, "ISBNdb.BookList.<xmlattr>.page_size", 10));
  return 0;
}
  
bool
Curl::isDone()
{
  if (m_resultType == SingleResult) {
    return true;
  }
  // int pageNumber(getAttributeValue(*m_result, "ISBNdb.BookList.<xmlattr>.page_number", 1));
  // return (numResults() <= (pageSize() * pageNumber));
  return true;
}

void
Curl::readData()
{
  m_result = YAML::Load(m_outputString);
}

Curl::ResultConstReference
Curl::results() const
{
  return m_result;
}

void
Curl::operator()()
{
  CURLcode res = curl_easy_perform(m_handle.get());
  auto iter = m_result.begin();
  if (res == CURLE_OK){
    readData();
  }
}

void
Curl::init()
{
  m_url = AppConfig::instance().isbnDbUrl();
  m_keys = AppConfig::instance().isbnDbKeys();
  m_handle.reset(curl_easy_init());

  if (m_searchType != "isbn") {
    m_resultType = MultipleResults;
  }

  if (m_resultType == SingleResult) {
    m_url.append(m_keys.front()).append("book").append(m_searchString);
  } else {
    m_url = m_keys.front() + "books";
    m_form.set("q", m_searchString);
    m_form.set("i", m_searchType);
  }

  curl_easy_setopt(m_handle.get(), CURLOPT_URL, m_url());
  curl_easy_setopt(m_handle.get(), CURLOPT_WRITEFUNCTION, writeFunction);
  curl_easy_setopt(m_handle.get(), CURLOPT_WRITEDATA, this);

  if (m_form) {
    curl_easy_setopt(m_handle.get(), CURLOPT_HTTPPOST, m_form());
  }
}
