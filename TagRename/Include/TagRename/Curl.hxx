#ifndef Curl_hxx_INCLUDED
#define Curl_hxx_INCLUDED

#include <sys/socket.h>
#include <curl/curl.h>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <string>
#include <sstream>

class Curl
{
public:
  typedef CURL* Handle;
  typedef boost::property_tree::ptree Result;
  typedef boost::shared_ptr<boost::property_tree::ptree> ResultPtr;
  typedef boost::add_reference<Result>::type ResultReference;
  typedef const ResultReference ResultConstReference;

  struct Form
  {
  private:
    struct curl_httppost* m_formPost;
    struct curl_httppost* m_lastPtr;
    
  public:
    Form()
      : m_formPost(nullptr), m_lastPtr(nullptr)
      {}

    Form(const Form&)
      : m_formPost(nullptr), m_lastPtr(nullptr)
      {}

    ~Form()
      {
        curl_formfree(m_formPost);
      }
    
    struct curl_httppost* operator()() {
      return m_formPost;
    }

    template <class T>
    void set(const std::string& key, const T& value)
      {
        std::ostringstream os;
        os << value;
        curl_formadd (
          &m_formPost,
          &m_lastPtr,
          CURLFORM_COPYNAME, key.c_str(),
          CURLFORM_COPYCONTENTS, os.str().c_str(),
          CURLFORM_END
          );
      }
    
  };

public:
  Curl(const std::string& searchType, const std::string& url, const std::string& searchString);
  Curl(const Curl& other);

  void   setPageNumber(int pageNum);
  size_t writeData(void* buffer, size_t size, size_t nmemb);
  int    numResults();
  int    pageSize();
  bool   isDone();
  void   readData();

  ResultConstReference& results() const;
  void operator()();
  
private:
  void init();
  
private:
  boost::shared_ptr<void> m_handle;
  std::string m_url, m_searchString, m_searchType;
  ResultPtr m_result;
  std::string m_outputString;
  Form m_form;
};

#endif
