#include <TagRename/ReturnPress.hxx>
#include <QtGui/QKeyEvent>

ReturnPress::ReturnPress (QObject* p_parent)
  : QObject(p_parent)
{
}

bool
ReturnPress::eventFilter (QObject* p_object, QEvent* p_event)
{
  if (p_event->type() == QEvent::KeyPress)
  {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*> (p_event);
    if (
        keyEvent->key() == Qt::Key_Return || 
        keyEvent->key() == Qt::Key_Enter
       )
    {
      Q_EMIT returnPressed();
    }
  }

  return QObject::eventFilter (p_object, p_event);
}
