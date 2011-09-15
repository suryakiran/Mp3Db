#include <TagRename/QStringPrint.hxx>
using namespace std;

ostream&
operator<< (ostream& p_os, const QString& p_string)
{
  p_os << p_string.toStdString() ;
  return p_os;
}

wostream&
operator<< (wostream& p_os, const QString& p_string)
{
  p_os << p_string.toStdWString() ;
  return p_os;
}
