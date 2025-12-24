/*!
  \file   graphic_gui.h
  \author Fichier moodle
  \date   Modification en Mai 2025
  \brief  Fichier moodle
*/

#ifndef GRAPHIC_GUI_H
#define GRAPHIC_GUI_H
//i changed <gtkmm/drawingarea.h>
#include <gtkmm.h>
#include "graphic.h"

void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr);

#endif
