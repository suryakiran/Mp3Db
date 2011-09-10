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

#include <xercesc/util/PlatformUtils.hpp>
using namespace xercesc;

namespace po = boost::program_options;

namespace {
  string styleSheetFile;

  void parseArgs (int argc, char** argv)
  {
    po::options_description desc ("Allowed Options");

    desc.add_options()
      ("help", "Display this message")
      ("init-dir", po::value<std::string>(), "Initial directory to start with")
      ("style-sheet", po::value<std::string>(), "Style Sheet File")
      ;

    po::variables_map vmap;
    po::store(po::parse_command_line(argc, argv, desc), vmap);
    po::notify(vmap);

    if (vmap.count("style-sheet"))
    {
      styleSheetFile = vmap["style-sheet"].as<string>();
    }
  }

  void setApplicationStyleSheet()
  {
    if (!styleSheetFile.empty())
    {
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

}

int main (int argc, char** argv) 
{
  XMLPlatformUtils::Initialize();
  parseArgs (argc, argv);

  QApplication app (argc, argv);
  setApplicationStyleSheet();

  MainWindow *mainw = new MainWindow;
  mainw->show();

  app.exec();
  XMLPlatformUtils::Terminate();
}
