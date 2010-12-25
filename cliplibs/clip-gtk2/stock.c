/*
    Copyright (C) 2004  ITK
    Author  : Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"

#include <gtk/gtk.h>

#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_stock_item()
{
   return GTK_TYPE_STOCK_ITEM;
}

long
_clip_type_stock_item()
{
   return GTK_OBJECT_STOCK_ITEM;
}

const char *
_clip_type_name_stock_item()
{
   return "GTK_OBJECT_STOCK_ITEM";
}

/* Register in global table */
int
clip_INIT___STOCKITEM(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_stock_item, _clip_type_name_stock_item, _gtk_type_stock_item, NULL, NULL);
   return 0;
}

/* gtk_StockAdd(aStocks, nStocks) --> NUL                                     */
/* aStocks - array of map with attibutes:                                     */
/*        STOCK_ID - string, stock identifier                                 */
/*           LABEL - string, label                                            */
/*        MODIFIER - numeric, the GdkModifierType                             */
/*          KEYVAL - numeric, key value                                       */
/*   TRANSL_DOMAIN - string,                                                  */

int
clip_GTK_STOCKADD(ClipMachine * ClipMachineMemory)
{
   ClipArrVar *cv = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 1));

   guint     n_items = _clip_parni(ClipMachineMemory, 2);

   GtkStockItem *items;

   gint      i;

   CHECKARG(1, ARRAY_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   if (cv->count_of_ClipArrVar < n_items)
      n_items = cv->count_of_ClipArrVar;
   items = malloc(n_items * sizeof(GtkStockItem));
   memset(items, 0, n_items * sizeof(GtkStockItem));
   for (i = 0; i < n_items; i++)
    {
       GtkStockItem it;

       _map_to_stock_item(ClipMachineMemory, &cv->ClipVar_items_of_ClipArrVar[i], &it);
       items[i] = it;
    }

   gtk_stock_add(items, n_items);

   free(items);

   return 0;
 err:
   return 1;
}

int
clip_GTK_STOCKADDSTATIC(ClipMachine * ClipMachineMemory)
{
   ClipArrVar *cv = (ClipArrVar *) _clip_vptr(_clip_spar(ClipMachineMemory, 1));

   guint     n_items = _clip_parni(ClipMachineMemory, 2);

   GtkStockItem *items;

   gint      i;

   CHECKARG(1, ARRAY_type_of_ClipVarType);
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   if (cv->count_of_ClipArrVar < n_items)
      n_items = cv->count_of_ClipArrVar;
   items = malloc(n_items * sizeof(GtkStockItem));
   memset(items, 0, n_items * sizeof(GtkStockItem));
   for (i = 0; i < n_items; i++)
    {
       GtkStockItem it;

       _map_to_stock_item(ClipMachineMemory, &cv->ClipVar_items_of_ClipArrVar[i], &it);
       items[i] = it;
    }

   gtk_stock_add_static(items, n_items);

   free(items);

   return 0;
 err:
   return 1;
}

int
clip_GTK_STOCKITEMCOPY(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   ClipVar  *newcv = 0;

   GtkStockItem item;

   GtkStockItem *newitem;

   CHECKARG(1, MAP_type_of_ClipVarType);

   _map_to_stock_item(ClipMachineMemory, cv, &item);

   newitem = gtk_stock_item_copy(&item);

   _stock_item_to_map(ClipMachineMemory, newcv, newitem);

   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), newcv);

   return 0;
 err:
   return 1;
}

int
clip_GTK_STOCKITEMFREE(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = _clip_spar(ClipMachineMemory, 1);

   GtkStockItem item;

   CHECKARG(1, MAP_type_of_ClipVarType);

   _map_to_stock_item(ClipMachineMemory, cv, &item);

   gtk_stock_item_free(&item);

   return 0;
 err:
   return 1;
}

int
clip_GTK_STOCKLISTIDS(ClipMachine * ClipMachineMemory)
{
   ClipVar  *cv = RETPTR(ClipMachineMemory);

   GSList   *list;

   long      i, n;

   list = gtk_stock_list_ids();

   n = g_slist_length(list);
   if (n > 0)
    {
       _clip_array(ClipMachineMemory, cv, 1, &n);
       for (i = 0; list; list = g_slist_next(list), i++)
	{
	   ClipVar   c;

	   gchar    *str;

	   str = (gchar *) list->data;
	   _clip_var_str(str, strlen(str), &c);
	   _clip_aset(ClipMachineMemory, cv, &c, 1, &i);
	   _clip_destroy(ClipMachineMemory, &c);
	   g_free(str);
	}
       g_slist_free(list);
    }

   return 0;
}

int
clip_GTK_STOCKLOOKUP(ClipMachine * ClipMachineMemory)
{
   gchar    *stock_id = _clip_parc(ClipMachineMemory, 1);

   ClipVar  *cv = _clip_spar(ClipMachineMemory, 2);

   GtkStockItem item;

   CHECKARG(1, CHARACTER_type_of_ClipVarType);
   CHECKARG(2, MAP_type_of_ClipVarType);

   _map_to_stock_item(ClipMachineMemory, cv, &item);

   _clip_retl(ClipMachineMemory, gtk_stock_lookup(stock_id, &item));

   return 0;
 err:
   return 1;
}
