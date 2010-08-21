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

/*********************** SIGNALS **************************/

/****            Menubar has no signals                ****/

/**********************************************************/

/* Register menu bar signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_menu_bar()
{
   return GTK_TYPE_MENU_BAR;
}

long
_clip_type_menu_bar()
{
   return GTK_WIDGET_MENU_BAR;
}

const char *
_clip_type_name_menu_bar()
{
   return "GTK_WIDGET_MENU_BAR";
}

int
clip_INIT___MENU_BAR(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_menu_bar, _clip_type_name_menu_bar, _gtk_type_menu_bar, _gtk_type_menu_shell, NULL);
   return 0;
}

/**********************************************************/

/****  MENU BAR constructor ****/
int
clip_GTK_MENUBARNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkWidget *wid = NULL;

   C_widget *cwid;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   wid = gtk_menu_bar_new();
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
clip_GTK_MENUBARAPPEND(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cmnb, GTK_IS_MENU_BAR);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_menu_bar_append(GTK_MENU_BAR(cmnb->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUBARPREPEND(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKCWID(cmnb, GTK_IS_MENU_BAR);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   gtk_menu_bar_prepend(GTK_MENU_BAR(cmnb->widget), cwid->widget);
   return 0;
 err:
   return 1;
}

int
clip_GTK_MENUBARINSERT(ClipMachine * ClipMachineMemory)
{
   C_widget *cmnb = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cwid = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   int       position = _clip_parni(ClipMachineMemory, 3);

   CHECKCWID(cmnb, GTK_IS_MENU_BAR);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cwid, GTK_IS_WIDGET);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   gtk_menu_bar_insert(GTK_MENU_BAR(cmnb->widget), cwid->widget, position);
   return 0;
 err:
   return 1;
}

/*
int
clip_GTK_MENUBARSETSHADOWTYPE(ClipMachine * ClipMachineMemory)
{
	C_widget *cmnb  = _fetch_cw_arg(ClipMachineMemory);
        int shadow_type = _clip_parni(ClipMachineMemory,2);
        CHECKCWID(cmnb,GTK_IS_MENU_BAR);
        CHECKOPT(2,NUMERIC_type_of_ClipVarType);
        gtk_menu_bar_set_shadow_type(GTK_MENU_BAR(cmnb->widget), shadow_type);
	return 0;
err:
	return 1;
}
*/
