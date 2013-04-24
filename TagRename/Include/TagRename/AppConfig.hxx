#ifndef AppConfig_hxx_INCLUDED
#define AppConfig_hxx_INCLUDED

#include <yaml-cpp/yaml.h>
#include <Stl/Set.hxx>
#include <Stl/Vector.hxx>

class AppConfig
{
public:
  static AppConfig& instance();
  void readConfig(const std::string& configFile);
  stl::StringSet getGenres();
  void addGenre(const std::string& genre);
  stl::StringVec isbnDbKeys();
  std::string isbnDbUrl();
  
private:
  static AppConfig* m_instance;
  AppConfig();
  YAML::Node m_config;
};

#endif
