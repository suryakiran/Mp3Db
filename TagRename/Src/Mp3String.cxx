#include <TagRename/Mp3String.hxx>

namespace {
  struct QStringVisitor : public boost::static_visitor<QString>
  {
    QString operator() (const TagLib::String& p_string) const
    {
      return TStringToQString (p_string);
    }

    QString operator() (const QString& p_string) const
    {
      return p_string;
    }

    QString operator() (const std::string& p_string) const
    {
      return QString::fromStdString(p_string);
    }

    QString operator() (const std::wstring& p_string) const
    {
      return QString::fromStdWString(p_string);
    }

    QString operator() (int p_int) const
    {
      return QString("%1").arg(p_int);
    }
  };

  struct TaglibStringVisitor : public boost::static_visitor<TagLib::String>
  {
    TagLib::String operator() (const TagLib::String& p_string) const
    {
      return p_string;
    }

    TagLib::String operator() (const QString& p_string) const
    {
      return TagLib::String (p_string.toUtf8().data(), TagLib::String::UTF8);
    }

    TagLib::String operator() (const std::string& p_string) const
    {
      return TagLib::String (p_string);
    }

    TagLib::String operator() (const std::wstring& p_string) const
    {
      return TagLib::String (p_string);
    }

    TagLib::String operator() (int p_int) const
    {
      return TagLib::String::number(p_int);
    }
  };
}

Mp3String::Mp3String()
  : m_null (true)
{
}

Mp3String::Mp3String (int p_int)
  : m_string (p_int), m_null(false)
{
}

Mp3String::Mp3String (const std::string& p_string)
  : m_string(p_string), m_null(false)
{
}

Mp3String::Mp3String (const std::wstring& p_string)
  : m_string(p_string), m_null(false)
{
}

Mp3String::Mp3String (const TagLib::String& p_string)
  : m_string(p_string), m_null(false)
{
}

Mp3String::Mp3String (const QString& p_string)
  : m_string(p_string), m_null(false)
{
}

Mp3String::Mp3String (const char* p_string)
  : m_string(QString(p_string)), m_null(false)
{
}

Mp3String::Mp3String (const wchar_t* p_string)
  : m_string(TagLib::String(p_string)), m_null(false)
{
}

Mp3String::Mp3String (const boost::filesystem::path& p_path)
  : m_string (TagLib::String(p_path.string())), m_null(false)
{
}

Mp3String::operator QString() const
{
  if (m_null) {
    return QString();
  }

  return boost::apply_visitor (QStringVisitor(), m_string);
}

Mp3String::operator TagLib::String() const
{
  if (m_null) {
    return TagLib::String();
  }

  return boost::apply_visitor (TaglibStringVisitor(), m_string);
}
