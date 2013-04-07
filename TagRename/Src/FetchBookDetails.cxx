#include <TagRename/FetchBookDetails.hxx>
#include <yaml-cpp/yaml.h>

FetchBookDetails* FetchBookDetails::m_instance = nullptr;

FetchBookDetails::FetchBookDetails()
{
  curl_global_init(CURL_GLOBAL_ALL);
}

FetchBookDetails::~FetchBookDetails()
{
  curl_global_cleanup();
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
  curlCreate("isbn", p_isbn);
}

void
FetchBookDetails::fetchTitle(const std::string& p_title)
{
  curlCreate("combined", p_title);
}

void
FetchBookDetails::execute()
{
  
}

void
FetchBookDetails::readConfig(const std::string& p_confFile)
{
  YAML::Node config = YAML::LoadFile(p_confFile);
  m_isbnDbUrl = config["Config"]["ISBNDb"]["Url"].as<std::string>();
  m_isbnDbKeys = config["Config"]["ISBNDb"]["Keys"].as<stl::StringVec>();
}

void
FetchBookDetails::curlCreate(const std::string& p_type, const std::string& p_value)
{
  Form form (nullptr, nullptr);
  auto handle = curl_easy_init();
  curl_formadd(&form.first,
               &form.second,
               CURLFORM_COPYNAME, "access_key",
               CURLFORM_COPYCONTENTS, "ZB4GTWMM",
               CURLFORM_END);
  curl_formadd(&form.first,
               &form.second,
               CURLFORM_COPYNAME, "value1",
               CURLFORM_COPYCONTENTS, p_value.c_str(),
               CURLFORM_END);
  curl_formadd(&form.first,
               &form.second,
               CURLFORM_COPYNAME, "index1",
               CURLFORM_COPYCONTENTS, p_type.c_str(),
               CURLFORM_END);
  curl_easy_setopt(handle, CURLOPT_URL, m_isbnDbUrl.c_str());
  curl_easy_setopt(handle, CURLOPT_HTTPPOST, form.first);
  m_handles.push_back(handle);
}
