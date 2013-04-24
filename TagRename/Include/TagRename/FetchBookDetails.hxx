#ifndef FetchBookDetails_hxx_INCLUDED
#define FetchBookDetails_hxx_INCLUDED

#include <Stl/Vector.hxx>
#include <Stl/Map.hxx>
#include <boost/shared_ptr.hpp>

class FetchBookDetails
{
private:
  struct BookDetails
  {
    std::string m_isbn10, m_isbn13;
    std::string m_title;
    std::string m_author;
  };

  DEFINE_VECTOR(BookDetails, Results);

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
  stl::StringVec m_isbnDbKeys;
};

#endif
