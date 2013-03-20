#ifndef DocFileDisplayWidget_hxx_INCLUDED
#define DocFileDisplayWidget_hxx_INCLUDED

class DocFileDisplayWidget : public QTreeWidget
{
  Q_OBJECT

private:
  DEFINE_MAP(int, QString, HeaderNameMap);

private:
  HeaderNameMap m_headerNameMap;

public:
  DocFileDisplayWidget (QWidget* p_parent = 0);
  ~DocFileDisplayWidget();

public Q_SLOTS:
  void readDirectory (const QModelIndex& p_index);
};
#endif
