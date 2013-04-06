#ifndef DocFileDisplayWidget_hxx_INCLUDED
#define DocFileDisplayWidget_hxx_INCLUDED

#include <QtGui/QTreeWidget>
#include <Stl/Map.hxx>

class QTreeWidgetItem;

class DocFileDisplayWidget : public QTreeWidget
{
  Q_OBJECT

private:
  DEFINE_MAP(int, QString, HeaderNameMap);

private:
  HeaderNameMap m_headerNameMap;
  QTreeWidgetItem *m_pdf, *m_epub, *m_djvu, *m_mobi;

public:
  DocFileDisplayWidget (QWidget* p_parent = 0);
  ~DocFileDisplayWidget();

public Q_SLOTS: 
  void readDirectory (const QModelIndex& p_index);
};
#endif
