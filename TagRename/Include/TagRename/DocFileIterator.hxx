#ifndef DocFileIterator_hxx_INCLUDED
#define DocFileIterator_hxx_INCLUDED

#include <typeinfo>
#include <iostream>
#include <string>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
namespace fs = boost::filesystem;
namespace str = boost::algorithm;
using namespace std;

struct is_doc_file
{
  bool operator() (const fs::path& p_filePath)
    {
      std::string extn(p_filePath.extension().string());
      return (
        fs::is_regular_file(p_filePath) && (
          str::iequals(extn, ".pdf") ||
          str::iequals(extn, ".djvu") ||
          str::iequals(extn, ".epub") ||
          str::iequals(extn, ".mobi")
          )
        );
    }
};

typedef boost::filter_iterator<is_doc_file, fs::recursive_directory_iterator> DocFilter;

class DocFileIterator : public DocFilter
{
public:
  DocFileIterator ()
    : DocFilter()
    {
    }

  DocFileIterator (const std::wstring& p_dirName)
    : DocFilter(m_predicate, fs::recursive_directory_iterator(fs::path(p_dirName)))
    {
    }

  DocFileIterator (const std::string& p_dirName)
    : DocFilter(m_predicate, fs::recursive_directory_iterator(fs::path(p_dirName)))
    {
    }

  DocFileIterator (const fs::path& p_path)
    : DocFilter(m_predicate, fs::recursive_directory_iterator(p_path))
    {
    }

  bool isPdf() const {
    return isFileExtension(".pdf");
  }

  bool isDjvu() const {
    return isFileExtension(".djvu");
  }

  bool isEpub() const {
    return isFileExtension(".epub");
  }

  bool isMobi() const {
    return isFileExtension(".mobi");
  }

private:
  bool isFileExtension (const std::string& extn) const
    {
      const fs::path& path = *(base());
      return str::iequals(path.extension().string(), extn);
    }
  
private:
  is_doc_file m_predicate;
};

#endif
