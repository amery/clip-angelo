/*
    Copyright (C) 2004  ITK
    Author  : Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/**********************************************************/
/*   no owner signals                                     */
/**********************************************************/

/* Register image menu item signals table in global table */
CLIP_DLLEXPORT GtkType
_gtk_type_image_menu_item()
{
   return GTK_TYPE_IMAGE_MENU_ITEM;
}

long
_clip_type_image_menu_item()
{
   return GTK_WIDGET_IMAGE_MENU_ITEM;
}

const char *
_clip_type_name_image_menu_item()
{
   return "GTK_WIDGET_IMAGE_MENU_ITEM";
}

int
clip_INIT___IMAGE_MENU_ITEM(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_image_menu_item,
		    _clip_type_name_image_menu_item, _gtk_type_image_menu_item, _gtk_type_menu_item, NULL);
   return 0;
}

/**********************************************************/

int
clip_GTK_IMAGEMENUITEMSETIMAGE(ClipMachine * ClipMachineMemory)
{
   C_widget *cimgitem = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cimage = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cimgitem, GTK_IS_IMAGE_MENU_ITEM);
   CHECKARG2(2, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cimage, GTK_IS_WIDGET);

   gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(cimgitem->widget), GTK_WIDGET(cimage->widget));
   return 0;
 err:
   return 1;
}

int
clip_GTK_IMAGEMENUITEMGETIMAGE(ClipMachine * ClipMachineMemory)
{
   C_widget *cimgitem = _fetch_cw_arg(ClipMachineMemory);

   C_widget *cimage;

   GtkWidget *image;

   CHECKARG2(1, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWID(cimgitem, GTK_IS_IMAGE_MENU_ITEM);

   image = gtk_image_menu_item_get_image(GTK_IMAGE_MENU_ITEM(cimgitem->widget));

   if (image)
    {
       cimage = _list_get_cwidget(ClipMachineMemory, image);
       if (!cimage)
	  cimage = _register_widget(ClipMachineMemory, image, NULL);
       if (cimage)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cimage->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_IMAGEMENUITEMNEW(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   C_widget *cimage;

   GtkWidget *image;

   CHECKOPT(1, MAP_type_of_ClipVarType);

   image = gtk_image_menu_item_new();

   if (image)
    {
       cimage = _list_get_cwidget(ClipMachineMemory, image);
       if (!cimage)
	  cimage = _register_widget(ClipMachineMemory, image, cv);
       if (cimage)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cimage->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_IMAGEMENUITEMNEWFROMSTOCK(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gchar    *stock_id = _clip_parc(ClipMachineMemory, 2);

   C_widget *accelgr = _fetch_cwidget(ClipMachineMemory, _clip_spar(ClipMachineMemory, 3));

   C_widget *cimage;

   GtkWidget *image;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(3, MAP_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKCWIDOPT(accelgr, GTK_IS_ACCEL_GROUP);

   image = gtk_image_menu_item_new_from_stock(stock_id, (accelgr != NULL) ? GTK_ACCEL_GROUP(accelgr->widget) : NULL);

   if (image)
    {
       cimage = _list_get_cwidget(ClipMachineMemory, image);
       if (!cimage)
	  cimage = _register_widget(ClipMachineMemory, image, cv);
       if (cimage)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cimage->obj);
    }
   return 0;
 err:
   return 1;
}

int
clip_GTK_IMAGEMENUITEMNEWWITHLABEL(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gchar    *label = _clip_parc(ClipMachineMemory, 2);

   C_widget *cimage;

   GtkWidget *image;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(label);
   image = gtk_image_menu_item_new_with_label(label);

   if (image)
    {
       cimage = _list_get_cwidget(ClipMachineMemory, image);
       if (!cimage)
	  cimage = _register_widget(ClipMachineMemory, image, cv);
       if (cimage)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cimage->obj);
    }
   FREE_TEXT(label);
   return 0;
 err:
   return 1;
}

int
clip_GTK_IMAGEMENUITEMNEWWITHMNEMONIC(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   gchar    *label = _clip_parc(ClipMachineMemory, 2);

   C_widget *cimage;

   GtkWidget *image;

   CHECKOPT(1, MAP_type_of_ClipVarType);
   CHECKARG(2, CHARACTER_type_of_ClipVarType);

   LOCALE_TO_UTF(label);
   image = gtk_image_menu_item_new_with_mnemonic(label);

   if (image)
    {
       cimage = _list_get_cwidget(ClipMachineMemory, image);
       if (!cimage)
	  cimage = _register_widget(ClipMachineMemory, image, cv);
       if (cimage)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cimage->obj);
    }
   FREE_TEXT(label);
   return 0;
 err:
   return 1;
}
