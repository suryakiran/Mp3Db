#ifndef QStringPrint_hxx_INCLUDED
#define QStringPrint_hxx_INCLUDED

#include <iostream>
#include <QtCore/QString>

std::ostream& operator<< (std::ostream& p_os, const QString& p_string);
std::wostream& operator<< (std::wostream& p_os, const QString& p_string);

#endif
