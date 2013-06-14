#include <TagRename/AppConfig.hxx>
#include <yaml-cpp/yaml.h>
#include <set>
#include <boost/foreach.hpp>

using namespace std;

AppConfig* AppConfig::m_instance = (AppConfig*)0;

namespace YAML {
  template <typename T>
  struct convert< set<T> >
  {
    static Node encode (const set<T>& p_set)
    {
      Node node (NodeType::Sequence);
      BOOST_FOREACH(const T& item, p_set)
        node.push_back(item);
      return node;
    }

    static bool decode (const Node& node, set<T>& p_set)
    {
      if(!node.IsSequence()) {
        return false;
      }

      p_set.clear();
      for(const_iterator it = node.begin(); it != node.end(); ++it) {
        p_set.insert(it->as<T>());
      }
      return true;
    }

  };
}

AppConfig::AppConfig()
{
  
}

AppConfig&
AppConfig::instance()
{
  if(!m_instance) {
    m_instance = new AppConfig;
  }

  return *m_instance;
}

void
AppConfig::readConfig(const string& configFile)
{
  m_config = YAML::LoadFile(configFile);
}

stl::StringSet
AppConfig::getGenres()
{
  return m_config["Config"]["Genres"].as<stl::StringSet>();
}

void
AppConfig::addGenre(const string& genre)
{
  
}

std::string
AppConfig::isbnDbUrl()
{
  return m_config["Config"]["ISBNDb"]["Api"]["V2"]["Url"].as<string>();
}

stl::StringVec
AppConfig::isbnDbKeys()
{
  return m_config["Config"]["ISBNDb"]["Keys"].as<stl::StringVec>();
}
