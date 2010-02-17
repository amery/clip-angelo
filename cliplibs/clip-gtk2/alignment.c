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
/****            Alignment has no signals              ****/
/**********************************************************/

/* Register menu item signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_alignment()
{
   return GTK_TYPE_ALIGNMENT;
}

long
_clip_type_alignment()
{
   return GTK_WIDGET_ALIGNMENT;
}

const char *
_clip_type_name_alignment()
{
   return "GTK_WIDGET_ALIGNMENT";
}

int
clip_INIT___ALIGNMENT(ClipMachine * ClipMachineMemory)
{
//      _wtype_table_put(_clip_type_alignment, _clip_type_name_alignment, _gtk_type_alignment, _gtk_type_container, NULL);
   _wtype_table_put(_clip_type_alignment, _clip_type_name_alignment, _gtk_type_alignment, _gtk_type_bin, NULL);
   return 0;
}

/**********************************************************/

/****  Alignment constructor ****/
int
clip_GTK_ALIGNMENTNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   gfloat xalign = _clip_parnd(ClipMachineMemory, 2);
   gfloat yalign = _clip_parnd(ClipMachineMemory, 3);
   gfloat xscale = _clip_parnd(ClipMachineMemory, 4);
   gfloat yscale = _clip_parnd(ClipMachineMemory, 5);
   GtkWidget *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      xscale = 1;
   if (_clip_parinfo(ClipMachineMemory, 5) == UNDEF_type_of_ClipVarType)
      yscale = 1;

   wid = gtk_alignment_new(xalign, yalign, xscale, yscale);
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/****  ------------------ ****/

int
clip_GTK_ALIGNMENTSET(ClipMachine * ClipMachineMemory)
{
   C_widget *cali = _fetch_cw_arg(ClipMachineMemory);
   gfloat xalign = _clip_parnd(ClipMachineMemory, 2);
   gfloat yalign = _clip_parnd(ClipMachineMemory, 3);
   gfloat xscale = _clip_parnd(ClipMachineMemory, 4);
   gfloat yscale = _clip_parnd(ClipMachineMemory, 5);
   CHECKCWID(cali, GTK_IS_ALIGNMENT);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      xscale = 1;
   if (_clip_parinfo(ClipMachineMemory, 5) == UNDEF_type_of_ClipVarType)
      yscale = 1;

   gtk_alignment_set(GTK_ALIGNMENT(cali->widget), xalign, yalign, xscale, yscale);
   return 0;
 err:
   return 1;
}

#if (GTK2_VER_MAJOR >= 2) && (GTK2_VER_MINOR >= 4)
int
clip_GTK_ALIGNMENTGETPADDING(ClipMachine * ClipMachineMemory)
{
   C_widget *cali = _fetch_cw_arg(ClipMachineMemory);
   guint top, bottom, left, right;

   CHECKCWID(cali, GTK_IS_ALIGNMENT);

   gtk_alignment_get_padding(GTK_ALIGNMENT(cali->widget), &top, &bottom, &left, &right);

   _clip_storni(ClipMachineMemory, top, 2, 0);
   _clip_storni(ClipMachineMemory, bottom, 3, 0);
   _clip_storni(ClipMachineMemory, left, 4, 0);
   _clip_storni(ClipMachineMemory, right, 5, 0);

   return 0;
 err:
   return 1;
}

int
clip_GTK_ALIGNMENTSETPADDING(ClipMachine * ClipMachineMemory)
{
   C_widget *cali = _fetch_cw_arg(ClipMachineMemory);
   guint top = _clip_parni(ClipMachineMemory, 2);
   guint bottom = _clip_parni(ClipMachineMemory, 3);
   guint left = _clip_parni(ClipMachineMemory, 4);
   guint right = _clip_parni(ClipMachineMemory, 5);

   CHECKCWID(cali, GTK_IS_ALIGNMENT);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);

   gtk_alignment_set_padding(GTK_ALIGNMENT(cali->widget), top, bottom, left, right);

   return 0;
 err:
   return 1;
}
#endif
