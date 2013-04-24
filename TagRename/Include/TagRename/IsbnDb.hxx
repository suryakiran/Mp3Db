#ifndef IsbnDb_hxx_INCLUDED
#define IsbnDb_hxx_INCLUDED

#include <TagRename/Book.hxx>
#include <Stl/Vector.hxx>
#include <tbb/concurrent_hash_map.h>

class IsbnDb
{
public:
  typedef tbb::concurrent_hash_map<std::string, Book, Book> Results;
  IsbnDb();
  ~IsbnDb();

  void addIsbn(const std::string& isbn);
  void addTitle(const std::string& title);
  void fetch();
  Results& results() {
    return m_results;
  }

private:
  stl::StringVec m_isbn, m_titles;
  Results m_results;
};

#endif
