#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <TagRename/MainWindow.hxx>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/algorithm/string/join.hpp>
using namespace std;

#include <TagRename/AppConfig.hxx>
#include <TagRename/FetchBookDetails.hxx>

namespace po = boost::program_options;

namespace {
  string styleSheetFile;
  string confFile;
  string dataDir;

  void parseArgs (int argc, char** argv)
  {
    po::options_description desc ("Allowed Options");

    desc.add_options()
      ("help", "Display this message")
      ("init-dir", po::value<std::string>(), "Initial directory to start with")
      ("style-sheet", po::value<std::string>(), "Style Sheet File")
      ("conf-file", po::value<std::string>(), "Mp3 Configuration File")
      ("data-dir", po::value<std::string>(), "Mp3 Data Directory")
      ;

    po::variables_map vmap;
    po::store(po::parse_command_line(argc, argv, desc), vmap);
    po::notify(vmap);

    if (vmap.count("style-sheet"))
    {
      styleSheetFile = vmap["style-sheet"].as<string>();
    }

    if (vmap.count("conf-file"))
    {
      confFile = vmap["conf-file"].as<string>();
    }

    if (vmap.count("data-dir"))
    {
      dataDir = vmap["data-dir"].as<string>();
    }

    AppConfig& config = AppConfig::instance();
    config.readConfig(confFile);

    FetchBookDetails* fbd = FetchBookDetails::instance();
    fbd->readConfig(confFile);
  }

  void setApplicationStyleSheet()
  {
    if (styleSheetFile.empty())
    {
      return;
    }

    fstream fin;
    fin.open (styleSheetFile.c_str(), ios_base::in);

    vector<string> lines;
    string l;

    while (getline (fin, l, '\n'))
    {
      lines.push_back (l);
    }

    fin.close();
    qApp->setStyleSheet (boost::algorithm::join(lines, "\n").c_str());
  }
}

int main (int argc, char** argv) 
{
  parseArgs (argc, argv);

  QApplication app (argc, argv);
  setApplicationStyleSheet();

  MainWindow *mainw = new MainWindow;
  mainw->show();

  app.exec();
}
