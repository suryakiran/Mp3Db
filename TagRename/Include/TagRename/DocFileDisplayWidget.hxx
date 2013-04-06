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
  DEFINE_MAP(std::string, QTreeWidgetItem*, FileTypeItemMap);

private:
  HeaderNameMap m_headerNameMap;
  FileTypeItemMap m_fileTypeItems;

public:
  DocFileDisplayWidget (QWidget* p_parent = 0);
  ~DocFileDisplayWidget();

public Q_SLOTS: 
  void readDirectory (const QModelIndex& p_index);
  void onItemChanged (QTreeWidgetItem* item, int column);
};
#endif
