#ifndef ReturnPress_hxx_INCLUDED
#define ReturnPress_hxx_INCLUDED

#include <QObject>
class QEvent;

class ReturnPress : public QObject
{
  Q_OBJECT

  public:
    ReturnPress (QObject* p_parent);

  Q_SIGNALS:
    void returnPressed();

  protected:
    bool eventFilter (QObject* p_obj, QEvent* p_event);
};

#endif
