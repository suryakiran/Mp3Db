#include <TagRename/Curl.hxx>
#include <TagRename/IsbnDb.hxx>

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>

#include <tbb/parallel_for.h>

using namespace std;

IsbnDb::IsbnDb()
{
  curl_global_init(CURL_GLOBAL_ALL);
}

IsbnDb::~IsbnDb()
{
  curl_global_cleanup();
}

void
IsbnDb::addIsbn(const string& isbn)
{
  m_isbn.push_back(isbn);
}

void
IsbnDb::addTitle(const string& title)
{
  m_titles.push_back(title);
}

void
IsbnDb::fetch()
{
  boost::ptr_vector<Curl> curls, newCurls;
  for(const string& isbn: m_isbn)
  {
    Curl* c = new Curl("isbn", isbn);
    curls.push_back (c);
  }

  for(const string& title: m_titles)
  {
    Curl* c = new Curl("combined", title);
    curls.push_back(c);
  }

  tbb::parallel_for(tbb::blocked_range<size_t>(0, curls.size()),
                    [&](const tbb::blocked_range<size_t>& range) {
                      for(size_t i = range.begin(); i != range.end(); ++i) {
                        Curl& c = curls[i];
                        c();
                        Curl::ResultConstReference& res = c.results();
                        BOOST_FOREACH(auto& r, res["data"])
                        {
                          Book b(r);
                          if (b.valid) {
                            Results::accessor a;
                            m_results.insert(a, make_pair(b.isbn.first, b));
                          }
                        }
                        // if (!c.isDone()) {
                        //   int numPages = (c.numResults() / c.pageSize()) + 1;
                        //   for (int i = 2; i <= numPages; ++i) {
                        //     Curl* nc = new Curl(c);
                        //     nc->setPageNumber(i);
                        //     newCurls.push_back(nc);
                        //   }
                        // }
                      }
                    });

  tbb::parallel_for(tbb::blocked_range<size_t>(0, newCurls.size()),
                    [&](const tbb::blocked_range<size_t>& range) {
                      for(size_t i = range.begin(); i != range.end(); ++i) {
                        Curl& c = newCurls[i];
                        c();
                        // Curl::ResultReference& res = c.results();
                        // BOOST_FOREACH(auto& r, res)
                        // {
                        //   Book b(r.second);
                        //   if (b.valid) {
                        //     Results::accessor a;
                        //     m_results.insert(a, make_pair(b.isbn.first, b));
                        //   }
                        // }
                      }
                    });

}
