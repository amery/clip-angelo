/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk.ch"
#include "ci_clip-gtk.h"

/*********************** SIGNALS **************************/

/****            Misc has no own signal                ****/

/**********************************************************/

/* Register misc in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_misc()
{
   return GTK_TYPE_MISC;
}

long
_clip_type_misc()
{
   return GTK_WIDGET_MISC;
}

const char *
_clip_type_name_misc()
{
   return "GTK_WIDGET_MISC";
}

/* Register arrow in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_arrow()
{
   return GTK_TYPE_ARROW;
}

long
_clip_type_arrow()
{
   return GTK_WIDGET_ARROW;
}

const char *
_clip_type_name_arrow()
{
   return "GTK_WIDGET_ARROW";
}

int
clip_INIT___MISC(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_misc, _clip_type_name_misc, _gtk_type_misc, _gtk_type_widget, NULL);
   _wtype_table_put(_clip_type_arrow, _clip_type_name_arrow, _gtk_type_arrow, _gtk_type_misc, NULL);
   return 0;
}

/**********************************************************/

int
clip_GTK_MISCSETALIGNMENT(ClipMachine * ClipMachineMemory)
{
   C_widget *cmisc = _fetch_cw_arg(ClipMachineMemory);

   gfloat    xalign = _clip_parnd(ClipMachineMemory, 2);

   gfloat    yalign = _clip_parnd(ClipMachineMemory, 3);

   CHECKCWID(cmisc, GTK_IS_MISC);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_misc_set_alignment(GTK_MISC(cmisc->widget), xalign, yalign);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MISCSETPADDING(ClipMachine * ClipMachineMemory)
{
   C_widget *cmisc = _fetch_cw_arg(ClipMachineMemory);

   gfloat    xpad = _clip_parnd(ClipMachineMemory, 2);

   gfloat    ypad = _clip_parnd(ClipMachineMemory, 3);

   CHECKCWID(cmisc, GTK_IS_MISC);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_misc_set_padding(GTK_MISC(cmisc->widget), xpad, ypad);
   return 0;
 err:
   return 1;
}

/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/

/**** Arrow constructor ****/
int
clip_GTK_ARROWNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkArrowType arrow_type = _clip_parni(ClipMachineMemory, 2);

   GtkShadowType shadow_type = _clip_parni(ClipMachineMemory, 3);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);

   wid = gtk_arrow_new(arrow_type, shadow_type);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Sets the direction and style of the GtkArrow */
int
clip_GTK_ARROWSET(ClipMachine * ClipMachineMemory)
{
   C_widget *carrow = _fetch_cw_arg(ClipMachineMemory);

   GtkArrowType arrow_type = _clip_parni(ClipMachineMemory, 2);

   GtkShadowType shadow_type = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(carrow, GTK_IS_ARROW);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_arrow_set(GTK_ARROW(carrow->widget), arrow_type, shadow_type);
   return 0;
 err:
   return 1;
}
