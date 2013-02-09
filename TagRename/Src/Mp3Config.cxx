#include <TagRename/Mp3Config.hxx>

#include <Stl/Vector.hxx>

#include <sstream>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
using boost::format;

#include <iostream>
#include <algorithm>
using namespace std;

#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>

#include <yaml-cpp/yaml.h>

namespace YAML {
  template <typename T>
  struct convert< std::set<T> >
  {
    static Node encode (const std::set<T>& p_set)
    {
      Node node (NodeType::Sequence);
      BOOST_FOREACH(const T& item, p_set)
        node.push_back(item);
      return node;
    }

    static bool decode (const Node& node, std::set<T>& p_set)
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

Mp3Config* Mp3Config::m_instance = (Mp3Config*) 0;

Mp3Config::Mp3Config()
  : config(0)
{
}

Mp3Config*
Mp3Config::instance()
{
  if (!m_instance)
  {
    m_instance = new Mp3Config;
  }

  return m_instance;
}

const stl::StringSet&
Mp3Config::getGenres() const
{
  return m_genres;
}

void Mp3Config::readConfig (const fs::path& p_fileName)
{
  YAML::Node config = YAML::LoadFile(p_fileName.string());
  m_genres = config["Config"]["Genres"].as<stl::StringSet>();
}

void Mp3Config::addGenre (const string& p_genre)
{
  return;
}
