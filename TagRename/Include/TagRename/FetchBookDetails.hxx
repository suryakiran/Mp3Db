#ifndef FetchBookDetails_hxx_INCLUDED
#define FetchBookDetails_hxx_INCLUDED

#include <Stl/Vector.hxx>
#include <Stl/Map.hxx>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/URI.h>
#include <boost/shared_ptr.hpp>

namespace Poco {
  namespace Net {
    class HTMLForm;
    class HTTPRequest;
  }
}

class FetchBookDetails
{
private:
  struct BookDetails
  {
    std::string m_isbn10, m_isbn13;
    std::string m_title;
    std::string m_author;
  };

typedef boost::shared_ptr <Poco::Net::HTMLForm> FormPtr;
typedef boost::shared_ptr <Poco::Net::HTTPRequest> RequestPtr;
typedef boost::shared_ptr <Poco::Net::HTTPClientSession> SessionPtr;
typedef std::pair<FormPtr, RequestPtr> FormRequestPair;

  DEFINE_VECTOR(BookDetails, Results);
  DEFINE_VECTOR(FormPtr, FormPtrVec);
  DEFINE_MAP(SessionPtr, FormRequestPair, SessionDetails);

  FetchBookDetails();
  void createForm (const std::string& p_type, const std::string& p_search);

public:
  ~FetchBookDetails();
  static FetchBookDetails* instance ();
  void fetchIsbn  (const std::string& p_isbn);
  void fetchTitle (const std::string& p_title);
  void execute    ();
  
  const Results& results() const {
    return m_results;
  }

  void clearResults () {
    m_results.clear();
  }

  void readConfig (const std::string& p_confFile);
  
private:
  Results m_results;
  static FetchBookDetails* m_instance;
  Poco::URI m_uri;
  stl::StringVec m_isbnDbKeys;
  SessionDetails m_sessions;
};

#endif
