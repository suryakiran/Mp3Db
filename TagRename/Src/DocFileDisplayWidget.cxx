#include <TagRename/DocFileDisplayWidget.hxx>

DocFileDisplayWidget::DocFileDisplayWidget(QWidget* p_parent)
  : QTreeWidget (p_parent)
{
  QTreeWidgetItem* header = new QTreeWidgetItem();
  insert(m_headerMap)
    (COLUMN_ID(FileName), COLUMN_LABEL(FileName))
}
