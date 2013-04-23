#include <TagRename/FetchBookDetails.hxx>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPStreamFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <yaml-cpp/yaml.h>

FetchBookDetails* FetchBookDetails::m_instance = nullptr;

FetchBookDetails::FetchBookDetails()
{
  Poco::Net::HTTPStreamFactory::registerFactory();
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
  for(auto& iter: m_sessions)
  {
    std::ostream& os = iter.first->sendRequest(*(iter.second.second));
    Poco::Net::HTTPResponse response;
    std::istream& is = iter.first->receiveResponse(response);
  }
}

void
FetchBookDetails::readConfig(const std::string& p_confFile)
{
  YAML::Node config = YAML::LoadFile(p_confFile);
  std::string isbnDbUrl (config["Config"]["ISBNDb"]["Url"].as<std::string>());
  m_uri = Poco::URI(isbnDbUrl);
  m_isbnDbKeys = config["Config"]["ISBNDb"]["Keys"].as<stl::StringVec>();
}

void
FetchBookDetails::createForm (const std::string& p_type,
                              const std::string& p_search
  )
{
  SessionPtr session (new
                      Poco::Net::HTTPClientSession (m_uri.getHost(), m_uri.getPort()));
  RequestPtr request (new
    Poco::Net::HTTPRequest (Poco::Net::HTTPRequest::HTTP_POST,
                            m_uri.getPath(),
                            Poco::Net::HTTPMessage::HTTP_1_1));
  FormPtr form (new Poco::Net::HTMLForm);
  form->set("index1", p_type);
  form->set("value1", p_search);
  form->set("access_key", m_isbnDbKeys.front());
  form->prepareSubmit(*request);
  m_sessions[session] = std::make_pair(form, request);
}
