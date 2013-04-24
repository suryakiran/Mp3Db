#include <TagRename/FetchBookDetails.hxx>
#include <yaml-cpp/yaml.h>

FetchBookDetails* FetchBookDetails::m_instance = nullptr;

FetchBookDetails::FetchBookDetails()
{
}

FetchBookDetails::~FetchBookDetails()
{
}

FetchBookDetails*
FetchBookDetails::instance()
{
  if (!m_instance) {
    m_instance = new FetchBookDetails;
  }

  return m_instance;
}

void
FetchBookDetails::fetchIsbn(const std::string& p_isbn)
{
  createForm("isbn", p_isbn);
}

void
FetchBookDetails::fetchTitle(const std::string& p_title)
{
  createForm("combined", p_title);
}

void
FetchBookDetails::execute()
{
}

void
FetchBookDetails::readConfig(const std::string& p_confFile)
{
  YAML::Node config = YAML::LoadFile(p_confFile);
  std::string isbnDbUrl (config["Config"]["ISBNDb"]["Url"].as<std::string>());
  // m_uri = Poco::URI(isbnDbUrl);
  m_isbnDbKeys = config["Config"]["ISBNDb"]["Keys"].as<stl::StringVec>();
}

void
FetchBookDetails::createForm (const std::string& p_type,
                              const std::string& p_search
  )
{
}
