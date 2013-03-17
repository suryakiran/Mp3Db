#ifndef Mp3FileIterator_hxx_INCLUDED
#define Mp3FileIterator_hxx_INCLUDED

#include <iostream>
#include <string>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

struct is_mp3_file
{
  bool operator() (const fs::path& p_filePath)
  {
    return (p_filePath.extension() == ".mp3");
  }
};

typedef boost::filter_iterator<is_mp3_file, fs::directory_iterator> Mp3Filter;

class Mp3FileIterator : public Mp3Filter
{
  public:
    Mp3FileIterator ()
      : Mp3Filter()
    {
    }

    Mp3FileIterator (const std::wstring& p_dirName)
      : Mp3Filter(m_predicate, fs::directory_iterator(fs::path(p_dirName)))
    {
    }

    Mp3FileIterator (const std::string& p_dirName)
      : Mp3Filter(m_predicate, fs::directory_iterator(fs::path(p_dirName)))
    {
    }

    Mp3FileIterator (const fs::path& p_path)
      : Mp3Filter(m_predicate, fs::directory_iterator(p_path))
    {
    }

  private:
    is_mp3_file m_predicate;
};

#endif
