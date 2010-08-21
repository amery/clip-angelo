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

/* Register ItemEntry in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_item_entry()
{
   return gtk_item_entry_get_type();
}

long
_clip_type_item_entry()
{
   return GTK_WIDGET_ITEM_ENTRY;
}

const char *
_clip_type_name_item_entry()
{
   return "GTK_WIDGET_ITEM_ENTRY";
}

int
clip_INIT___ITEM_ENTRY(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_item_entry, _clip_type_name_item_entry, _gtk_type_item_entry, _gtk_type_entry, NULL);
   return 0;
}

/**** ITEM ENTRY constructor ****/
int
clip_GTK_ITEMENTRYNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   guint16   max = _clip_parni(ClipMachineMemory, 2);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      wid = gtk_item_entry_new();
   else
      wid = gtk_item_entry_new_with_max_length(max);

   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

int
clip_GTK_ITEMENTRYSETTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cite = _fetch_cw_arg(ClipMachineMemory);

   const gchar *text = CHAR_OPTION(ClipMachineMemory, 2, "");

   GtkJustification justification = INT_OPTION(ClipMachineMemory, 3, GTK_JUSTIFY_LEFT);

   CHECKCWID(cite, GTK_IS_ITEM_ENTRY);
   CHECKOPT(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_item_entry_set_text(GTK_ITEM_ENTRY(cite->widget), text, justification);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ITEMENTRYGETTEXT(ClipMachine * ClipMachineMemory)
{
   C_widget *cite = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cite, GTK_IS_ENTRY);
   _clip_retc(ClipMachineMemory, gtk_entry_get_text(GTK_ENTRY(cite->widget)));
   return 0;
 err:
   return 1;
}

int
clip_GTK_ITEMENTRYSETJUSTIFICATION(ClipMachine * ClipMachineMemory)
{
   C_widget *cite = _fetch_cw_arg(ClipMachineMemory);

   GtkJustification justification = INT_OPTION(ClipMachineMemory, 2, GTK_JUSTIFY_LEFT);

   CHECKCWID(cite, GTK_IS_ITEM_ENTRY);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   gtk_item_entry_set_justification(GTK_ITEM_ENTRY(cite->widget), justification);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ITEMENTRYGETJUSTIFICATION(ClipMachine * ClipMachineMemory)
{
   C_widget *cite = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cite, GTK_IS_ITEM_ENTRY);
   _clip_retni(ClipMachineMemory, GTK_ITEM_ENTRY(cite->widget)->justification);
   return 0;
 err:
   return 1;
}

int
clip_GTK_ITEMENTRYGETTEXTMAXSIZE(ClipMachine * ClipMachineMemory)
{
   C_widget *cite = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(cite, GTK_IS_ITEM_ENTRY);
   _clip_retni(ClipMachineMemory, GTK_ITEM_ENTRY(cite->widget)->text_max_size);
   return 0;
 err:
   return 1;
}
