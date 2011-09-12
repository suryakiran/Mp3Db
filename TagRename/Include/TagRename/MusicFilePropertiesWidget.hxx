#ifndef MusicFilePropertiesWidget_hxx_INCLUDED
#define MusicFilePropertiesWidget_hxx_INCLUDED

#include "ui_MusicFilePropertiesWidget.h"

class MusicFilePropertiesWidget : public QWidget, private Ui::MusicFilePropertiesWidget
{
  Q_OBJECT

  public:
    MusicFilePropertiesWidget (QWidget* p_parent);

  public Q_SLOTS:
    void updateDetailsOfSelectedFile (const QModelIndex& p_current);
};

#endif