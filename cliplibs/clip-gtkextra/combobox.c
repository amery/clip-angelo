/*
    Copyright (C) 2002  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkextracfg.h"

#include <gtkextra/gtkextra.h>
#include <string.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

#include "ci_clip-gtkextra.ch"
#include "ci_clip-gtkextra.h"

/**********************************************************/

/* Register ComboBox in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_combo_box()
{
   return gtk_combobox_get_type();
}

long
_clip_type_combo_box()
{
   return GTK_WIDGET_COMBO_BOX;
}

const char *
_clip_type_name_combo_box()
{
   return "GTK_WIDGET_COMBO_BOX";
}

int
clip_INIT___COMBO_BOX(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_combo_box, _clip_type_name_combo_box, _gtk_type_combo_box, _gtk_type_hbox, NULL);
   return 0;
}

/**** COMBO BOX constructor ****/
int
clip_GTK_COMBOBOXNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid, *cbutton, *carrow, *cpopup, *cpopwin, *cframe;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_combobox_new();

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   cbutton = _register_widget(ClipMachineMemory, GTK_COMBO_BOX(wid)->button, NULL);
   carrow = _register_widget(ClipMachineMemory, GTK_COMBO_BOX(wid)->arrow, NULL);
   cpopup = _register_widget(ClipMachineMemory, GTK_COMBO_BOX(wid)->popup, NULL);
   cpopwin = _register_widget(ClipMachineMemory, GTK_COMBO_BOX(wid)->popwin, NULL);
   cframe = _register_widget(ClipMachineMemory, GTK_COMBO_BOX(wid)->frame, NULL);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_BUTTON, &cbutton->obj);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_ARROW, &carrow->obj);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_POPUP, &cpopup->obj);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_POPWIN, &cpopwin->obj);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_FRAME, &cframe->obj);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

int
clip_GTK_COMBOBOXHIDEPOPDOWNWINDOW(ClipMachine * ClipMachineMemory)
{
   C_widget *ccbx = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ccbx, GTK_IS_COMBO_BOX);
   gtk_combobox_hide_popdown_window(GTK_COMBO_BOX(ccbx->widget));
   return 0;
 err:
   return 1;
}
