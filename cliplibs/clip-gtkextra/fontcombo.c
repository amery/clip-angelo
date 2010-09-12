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
/* Signals table */
static SignalTable font_combo_signals[] = {
   {"changed", GSF(widget_signal_handler), ESF(object_emit_signal), GTK_CHANGED_SIGNAL},
   {"", NULL, NULL, 0}
};

/**********************************************************/

/* Register FontCombo in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_font_combo()
{
   return gtk_font_combo_get_type();
}

long
_clip_type_font_combo()
{
   return GTK_WIDGET_FONT_COMBO;
}

const char *
_clip_type_name_font_combo()
{
   return "GTK_WIDGET_FONT_COMBO";
}

int
clip_INIT___FONT_COMBO(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_font_combo, _clip_type_name_font_combo, _gtk_type_font_combo, _gtk_type_toolbar,
		    font_combo_signals);
   return 0;
}

/**** FONT COMBO constructor ****/
int
clip_GTK_FONTCOMBONEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid, *cname_combo, *csize_combo, *cbold_btn, *citalic_btn;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_font_combo_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, wid, cv);
   cname_combo = _register_widget(ClipMachineMemory, GTK_FONT_COMBO(wid)->name_combo, NULL);
   csize_combo = _register_widget(ClipMachineMemory, GTK_FONT_COMBO(wid)->size_combo, NULL);
   cbold_btn = _register_widget(ClipMachineMemory, GTK_FONT_COMBO(wid)->bold_button, NULL);
   citalic_btn = _register_widget(ClipMachineMemory, GTK_FONT_COMBO(wid)->italic_button, NULL);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_NAMECOMBO, &cname_combo->obj);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_SIZECOMBO, &csize_combo->obj);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_BOLDBUTTON, &cbold_btn->obj);
   _clip_madd(ClipMachineMemory, &cwid->obj, HASH_ITALICBUTTON, &citalic_btn->obj);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);

   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

int
clip_GTK_FONTCOMBOSELECT(ClipMachine * ClipMachineMemory)
{
   C_widget *cfnc = _fetch_cw_arg(ClipMachineMemory);

   const gchar *family = CHAR_OPTION(ClipMachineMemory, 2, "");

   gboolean  bold = BOOL_OPTION(ClipMachineMemory, 3, FALSE);

   gboolean  italic = BOOL_OPTION(ClipMachineMemory, 4, FALSE);

   gint      height = _clip_parni(ClipMachineMemory, 5);

   CHECKCWID(cfnc, GTK_IS_FONT_COMBO);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT(4, LOGICAL_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   gtk_font_combo_select(GTK_FONT_COMBO(cfnc->widget), family, bold, italic, height);
   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTCOMBOSELECTNTH(ClipMachine * ClipMachineMemory)
{
   C_widget *cfnc = _fetch_cw_arg(ClipMachineMemory);

   gint      n = INT_OPTION(ClipMachineMemory, 2, 1);

   gboolean  bold = BOOL_OPTION(ClipMachineMemory, 3, FALSE);

   gboolean  italic = BOOL_OPTION(ClipMachineMemory, 4, FALSE);

   gint      height = _clip_parni(ClipMachineMemory, 5);

   CHECKCWID(cfnc, GTK_IS_FONT_COMBO);
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT(4, LOGICAL_type_of_ClipVarType);
   CHECKOPT(5, NUMERIC_type_of_ClipVarType);
   gtk_font_combo_select_nth(GTK_FONT_COMBO(cfnc->widget), n, bold, italic, height);
   return 0;
 err:
   return 1;
}

int
clip_GTK_FONTCOMBOGETFONT(ClipMachine * ClipMachineMemory)
{
   C_widget *cfnc = _fetch_cw_arg(ClipMachineMemory);

   C_object *cfont;

   CHECKCWID(cfnc, GTK_IS_FONT_COMBO);
   cfont = _list_get_cobject(ClipMachineMemory, GTK_FONT_COMBO(cfnc->widget)->font);
   if (!cfont)
      cfont = _register_object(ClipMachineMemory, GTK_FONT_COMBO(cfnc->widget)->font,
			       GDK_OBJECT_FONT, NULL, (coDestructor) gdk_object_font_destructor);
   if (cfont)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cfont->obj);
   return 0;
 err:
   return 1;
}
