#include <iostream>
#include <string>

#include <TagRename/MainWindow.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
using namespace std;

namespace po = boost::program_options;

namespace {
  void parseArgs (int argc, char** argv)
  {
    po::options_description desc ("Allowed Options");

    desc.add_options()
      ("help", "Display this message")
      ("init-dir", po::value<std::string>(), "Initial directory to start with")
      ;

    po::variables_map vmap;
    po::store(po::parse_command_line(argc, argv, desc), vmap);
    po::notify(vmap);
  }
}

int main (int argc, char** argv) 
{
  parseArgs (argc, argv);

  QApplication app (argc, argv);

  MainWindow *mainw = new MainWindow;
  mainw->show();

  app.exec();
}
