#ifndef FetchBookDetails_hxx_INCLUDED
#define FetchBookDetails_hxx_INCLUDED

#include <curl/curl.h>
#include <Stl/Vector.hxx>

DEFINE_VECTOR(CURL*, CurlHandleVec);

struct BookDetails
{
  std::string m_isbn10, m_isbn13;
  std::string m_title;
  std::string m_author;
};

DEFINE_VECTOR(BookDetails, BookFetchResults);

class FetchBookDetails
{
private:
  typedef struct curl_httppost* CurlPostPtr;
  typedef std::pair<CurlPostPtr, CurlPostPtr> Form;

  FetchBookDetails();
  void curlCreate(const std::string& p_type, const std::string& p_value);

public:
  ~FetchBookDetails();
  static FetchBookDetails* instance ();
  void fetchIsbn  (const std::string& p_isbn);
  void fetchTitle (const std::string& p_title);
  void execute    ();
  
  const BookFetchResults& results() const {
    return m_results;
  }

  void clearResults () {
    m_results.clear();
  }

  void readConfig (const std::string& p_confFile);
  
private:
  BookFetchResults m_results;
  static FetchBookDetails* m_instance;
  CurlHandleVec m_handles;
  std::string m_isbnDbUrl;
  stl::StringVec m_isbnDbKeys;
};

#endif
