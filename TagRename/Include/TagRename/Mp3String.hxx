#ifndef Mp3String_hxx_INCLUDED
#define Mp3String_hxx_INCLUDED

#include <string>
#include <QString>
#include <taglib/tstring.h>
#include <boost/variant.hpp>
#include <boost/filesystem/path.hpp>

class Mp3String
{
  typedef 
    boost::variant <
        std::string
      , std::wstring
      , QString
      , TagLib::String
      , int
      > String;

  public:
    Mp3String ();
    Mp3String (int p_int);
    Mp3String (const std::string& p_string);
    Mp3String (const std::wstring& p_string);
    Mp3String (const QString& p_string);
    Mp3String (const TagLib::String& p_string);
    Mp3String (const char* p_string);
    Mp3String (const wchar_t* p_string);
    Mp3String (const boost::filesystem::path& p_path);

    bool isNull () const { 
      return m_null;
    }

    operator QString() const;
    operator TagLib::String() const;

  private:
    String m_string;
    bool m_null;
};

#ifdef _M
#undef _M
#endif

#define _M(s) Mp3String(s)

#endif
