#ifndef Mp3FileIterator_hpp_INCLUDED
#define Mp3FileIterator_hpp_INCLUDED

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

class Mp3FileIterator : public boost::filter_iterator <is_mp3_file, fs::directory_iterator>
{
  public:
    Mp3FileIterator ()
      : boost::filter_iterator<is_mp3_file, fs::directory_iterator>()
    {
    }

    Mp3FileIterator (const std::wstring& p_dirName)
      : boost::filter_iterator<is_mp3_file, fs::directory_iterator> 
        (m_predicate, fs::directory_iterator(fs::path(p_dirName)))
    {
    }

    Mp3FileIterator (const fs::path& p_path)
      : boost::filter_iterator<is_mp3_file, fs::directory_iterator> 
        (m_predicate, fs::directory_iterator(p_path))
    {
    }

  private:
    is_mp3_file m_predicate;
};

#endif
