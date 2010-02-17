/*
    Copyright (C) 2002  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/*********************** SIGNALS **************************/

/***          Tooltips have no own signals              ***/

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_tooltips()
{
   return GTK_TYPE_TOOLTIPS;
}

long
_clip_type_tooltips()
{
   return GTK_WIDGET_TOOLTIPS;
}

const char *
_clip_type_name_tooltips()
{
   return "GTK_OBJECT_TOOLTIPS";
}

/* Register toolbar in global table */
int
clip_INIT___TOOLTIPS(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_tooltips, _clip_type_name_tooltips, _gtk_type_tooltips, _gtk_type_object, NULL);
   return 0;
}

/**** ------------------ ****/
/**** TOOLTIPS constructor ****/
int
clip_GTK_TOOLTIPSNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar *cv = _clip_spar(ClipMachineMemory, 1);
   GtkTooltips *wid = NULL;
   C_widget *cwid;
   CHECKOPT(1, MAP_type_of_ClipVarType);
   wid = gtk_tooltips_new();
   if (!wid)
      goto err;
   cwid = _register_widget(ClipMachineMemory, (GtkWidget *) wid, cv);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cwid->obj);
   return 0;
 err:
   return 1;
}

/**** ------------------ ****/

/* Allows the user to see your tooltips as they navigate your application. */
int
clip_GTK_TOOLTIPSENABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctt = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ctt, GTK_IS_TOOLTIPS);
   gtk_tooltips_enable(GTK_TOOLTIPS(ctt->widget));
   return 0;
 err:
   return 1;
}

/* Causes all tooltips in tooltips to become inactive. Any widgets that have
 * tips associated with that group will no longer display their tips until
 * they are enabled again with gtk_tooltips_enable(). */
int
clip_GTK_TOOLTIPSDISABLE(ClipMachine * ClipMachineMemory)
{
   C_widget *ctt = _fetch_cw_arg(ClipMachineMemory);
   CHECKCWID(ctt, GTK_IS_TOOLTIPS);
   gtk_tooltips_disable(GTK_TOOLTIPS(ctt->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLTIPSSETDELAY(ClipMachine * ClipMachineMemory)
{
   C_widget *ctt = _fetch_cw_arg(ClipMachineMemory);
   guint delay = _clip_parni(ClipMachineMemory, 2);

   CHECKCWID(ctt, GTK_IS_TOOLTIPS);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   gtk_tooltips_set_delay(GTK_TOOLTIPS(ctt->widget), delay);
   return 0;
 err:
   return 1;
}

/* Adds a tooltip containing the message tip_text to the specified GtkWidget. */
int
clip_GTK_TOOLTIPSSETTIP(ClipMachine * ClipMachineMemory)
{
   C_widget *ctt = _fetch_cw_arg(ClipMachineMemory);
   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   gchar *tip_text = _clip_parc(ClipMachineMemory, 3);
   gchar *tip_private = _clip_parc(ClipMachineMemory, 4);
   CHECKCWID(ctt, GTK_IS_TOOLTIPS);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT(4, CHARACTER_type_of_ClipVarType);
   LOCALE_TO_UTF(tip_text);
   LOCALE_TO_UTF(tip_private);
   gtk_tooltips_set_tip(GTK_TOOLTIPS(ctt->widget), cwid->widget, tip_text, tip_private);
   FREE_TEXT(tip_text);
   FREE_TEXT(tip_private);
   return 0;
 err:
   return 1;
}

/* Retrieves any GtkTooltipsData previously associated with the given widget. */
int
clip_GTK_TOOLTIPSDATAGET(ClipMachine * ClipMachineMemory)
{
   C_widget *cwid = _fetch_cw_arg(ClipMachineMemory);
   GtkTooltipsData *data;
   CHECKCWID(cwid, GTK_IS_WIDGET);
   data = gtk_tooltips_data_get(cwid->widget);
   if (data)
      {
	 C_widget *ctt = _list_get_cwidget(ClipMachineMemory, data->tooltips);
	 ClipVar *ret = RETPTR(ClipMachineMemory);
	 char *tip_text = data->tip_text ? data->tip_text : "";
	 char *tip_private = data->tip_private ? data->tip_private : "";
	 memset(ret, 0, sizeof(*ret));
	 _clip_map(ClipMachineMemory, ret);
	 if (!ctt)
	    ctt = _register_widget(ClipMachineMemory, (GtkWidget *) (data->tooltips), NULL);
	 if (ctt)
	    _clip_madd(ClipMachineMemory, ret, HASH_TOOLTIPS, &ctt->obj);

	 _clip_madd(ClipMachineMemory, ret, HASH_WIDGET, &cwid->obj);
	 LOCALE_FROM_UTF(tip_text);
	 LOCALE_FROM_UTF(tip_private);
	 _clip_mputc(ClipMachineMemory, ret, HASH_TIPTEXT, tip_text, strlen(tip_text));
	 _clip_mputc(ClipMachineMemory, ret, HASH_TIPPRIVATE, tip_private, strlen(tip_private));
	 FREE_TEXT(tip_text);
	 FREE_TEXT(tip_private);
      }
   return 0;
 err:
   return 1;
}

int
clip_GTK_TOOLTIPSFORCEWINDOW(ClipMachine * ClipMachineMemory)
{
   C_widget *ctt = _fetch_cw_arg(ClipMachineMemory);

   CHECKCWID(ctt, GTK_IS_TOOLTIPS);

   gtk_tooltips_force_window(GTK_TOOLTIPS(ctt->widget));

   return 0;
 err:
   return 1;
}
