/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/**********************************************************/

/****           Radio button has no signals            ****/

/**********************************************************/

/* Register radio button in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_radio_button()
{
   return GTK_TYPE_RADIO_BUTTON;
}

long
_clip_type_radio_button()
{
   return GTK_WIDGET_RADIO;
}

const char *
_clip_type_name_radio_button()
{
   return "GTK_WIDGET_RADIO";
}

CLIP_DLLEXPORT GtkType
_gtk_type_radio_group()
{
   return GTK_WIDGET_RADIO_GROUP;
}

long
_clip_type_radio_group()
{
   return GTK_WIDGET_RADIO_GROUP;
}

const char *
_clip_type_name_radio_group()
{
   return "GTK_WIDGET_RADIO_GROUP";
}

int
clip_INIT___RADIO_BUTTON(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_radio_button, _clip_type_name_radio_button, _gtk_type_radio_button, _gtk_type_toggle_button, NULL);
   _wtype_table_put(_clip_type_radio_group, _clip_type_name_radio_group, _gtk_type_radio_group, NULL, NULL);
   return 0;
}

/**** Radio button constructor ****/
int
clip_GTK_RADIOBUTTONNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   C_widget *cgrp = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   char *label = _clip_parc(ClipMachineMemory, 3);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   GtkRadioButton *rb = NULL;
   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);

   if (cgrp && cgrp->type != GTK_WIDGET_RADIO_GROUP)
      goto err;
   if (cgrp && cgrp->data)
      rb = ((GSList *) (cgrp->data))->data;

   if (_clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType)
      {
	 LOCALE_TO_UTF(label);
	 wid = gtk_radio_button_new_with_label_from_widget(rb, label);
	 if (cgrp && !cgrp->data)
	    cgrp->data = gtk_radio_button_get_group(GTK_RADIO_BUTTON(wid));
	 FREE_TEXT(label);
      }
   else
      {
	 wid = gtk_radio_button_new_from_widget(rb);
	 if (cgrp && !cgrp->data)
	    cgrp->data = gtk_radio_button_get_group(GTK_RADIO_BUTTON(wid));
      }
   if (!wid)
      goto err;

   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/**** Radio group constructor ****/
int
clip_GTK_RADIOGROUPNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);

   cwid = _register_widget(ClipMachineMemory, NULL, cv);
   cwid->type = _clip_type_radio_group();
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Sets a RadioButton's group. It should be noted that this does not
 * change the layout of your interface in any way, so if you are
 * changing the group, it is likely you will need to re-arrange the
 * user interface to reflect these changes. */

int
clip_GTK_RADIOBUTTONSETGROUP(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cgrp = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   GSList *group = NULL;
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cbtn, GTK_IS_RADIO_BUTTON);
   if (cgrp && cgrp->type != GTK_WIDGET_RADIO_GROUP)
      goto err;
   if (cgrp && cgrp->data)
      group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(((GSList *) (cgrp->data))->data));
   gtk_radio_button_set_group(GTK_RADIO_BUTTON(cbtn->widget), group);
   if (cgrp && cgrp->data)
      cgrp->data = gtk_radio_button_get_group(GTK_RADIO_BUTTON(cbtn->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_RADIOBUTTONSETSTYLE(ClipMachine * ClipMachineMemory)
{
   C_widget *cbtn = _fetch_cw_arg(ClipMachineMemory);
   ClipVar *mstyle = _clip_spar(ClipMachineMemory, 2);
   GtkStyle *style;
   GtkButton *button;
   CHECKCWID(cbtn, GTK_IS_TOGGLE_BUTTON);
   CHECKARG(2, MAP_type_of_ClipVarType);

   button = &(GTK_TOGGLE_BUTTON(cbtn->widget)->button);
   style = gtk_style_copy(GTK_BIN(&(button->bin))->child->style);
  //gtk_style_unref(GTK_BIN(&(button->bin))->child->style);
   _map_to_style(ClipMachineMemory, mstyle, style);
   gtk_widget_set_style(GTK_BIN(&(button->bin))->child, style);
   return 0;
 err:
   return 1;
}
