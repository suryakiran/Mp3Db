#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
namespace str = boost::algorithm;
using namespace std;

int main (int argc, char** argv) 
{
  string s = "mp3db";
  str::to_upper(s);
  cout << s << endl;
  return 0;
}
