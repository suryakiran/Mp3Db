#ifndef Curl_hxx_INCLUDED
#define Curl_hxx_INCLUDED

#include <sys/socket.h>
#include <curl/curl.h>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <sstream>
#include <Stl/Vector.hxx>
#include <boost/algorithm/string/join.hpp>
#include <iostream>
#include <yaml-cpp/yaml.h>
using namespace std;

class Curl
{
public:
  typedef CURL* Handle;
  typedef YAML::Node Result;
  typedef Result& ResultReference;
  typedef const Result& ResultConstReference;

  typedef enum {
    SingleResult,
    MultipleResults
  } ResultsType;

  struct Url
  {
    Url(const std::string& url)
      : m_url(url)
      {
      }

    Url() { }

    Url& append(const std::string& field)
      {
        m_url.append("/");
        m_url.append(field);
        return *this;
      }

    const char* operator()() const
      {
        return m_url.c_str();
      }

  private:
    std::string m_url;
  };

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

    operator bool() const {
      return m_formPost != nullptr;
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
  Curl(const std::string& searchType, const std::string& searchString);
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
  std::string m_searchString, m_searchType;
  Url m_url;
  Result m_result;
  std::string m_outputString;
  Form m_form;
  stl::StringVec m_keys;
  ResultsType m_resultType;
};

#endif
