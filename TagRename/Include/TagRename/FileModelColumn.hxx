#ifndef FileModelColumn_hxx_INCLUDED
#define FileModelColumn_hxx_INCLUDED

#include <string>

#define COLUMN_DEFINE(ColumnName, ColumnId, StringRep) \
  struct ColumnName                                    \
  {                                                    \
    int id() const                                     \
    {                                                  \
      return ColumnId;                                 \
    }                                                  \
                                                       \
    QString operator() () const                        \
    {                                                  \
      return StringRep;                                \
    }                                                  \
  }

COLUMN_DEFINE (FileName,    0, "File Name");
COLUMN_DEFINE (AlbumName,   1, "Album Name");
COLUMN_DEFINE (TrackNumber, 2, "#");
COLUMN_DEFINE (TrackName,   3, "Track Name");
COLUMN_DEFINE (ArtistNames, 4, "Artist(s)");
COLUMN_DEFINE (Genre,       5, "Genre");
COLUMN_DEFINE (Composer,    6, "Composer");
COLUMN_DEFINE (BitRate,     7, "Bit Rate");
COLUMN_DEFINE (Duration,    8, "Duration");
COLUMN_DEFINE (Year,        9, "Year");

#define COLUMN_ID(ColumnName) Column<ColumnName>::id()
#define COLUMN_LABEL(ColumnName) Column<ColumnName>::label()

template <class T>
  class Column 
  {
    public:
      static int id()
      {
        T col;
        return col.id();
      }

      static QString label()
      {
        T col;
        return col();
      }
  };

struct ItemDataRole
{
  static const int Hidden = Qt::UserRole + 1;
};

#endif                   
