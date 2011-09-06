#ifndef MusicFilePropertiesWidget_hpp_INCLUDED
#define MusicFilePropertiesWidget_hpp_INCLUDED

#include "ui_MusicFilePropertiesWidget.h"

class MusicFilePropertiesWidget : public QWidget, private Ui::MusicFilePropertiesWidget
{
  public:
    MusicFilePropertiesWidget (QWidget* p_parent);
};

#endif
