/*
    Copyright (C) 2003 -2004 ITK
    Author  : Elena V. Kornilova <alena@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/  
#include <string.h>
#include "ci_hashcode.h"
#include "ci_clip.h"
#include "ci_clip-gtkcfg2.h"
 
#include <gtk/gtk.h>
#include <pango/pango-layout.h>
 
#include "ci_clip-gtk2.ch"
#include "ci_clip-gtk2.h"
 CLIP_DLLEXPORT GtkType _gtk_type_pango_layout() 
{
   return GTK_TYPE_PANGO_LAYOUT;
}

CLIP_DLLEXPORT GtkType _gtk_type_pango_context() 
{
   return GTK_TYPE_PANGO_CONTEXT;
}

long 
_clip_type_pango_layout() 
{
   return GTK_OBJECT_PANGO_LAYOUT;
}

long  
_clip_type_pango_context() 
{
   return GTK_OBJECT_PANGO_CONTEXT;
}

const char *
_clip_type_name_pango_layout() 
{
   return "GTK_OBJECT_PANGO_LAYOUT";
}

const char *
_clip_type_name_pango_context() 
{
   return "GTK_OBJECT_PANGO_CONTEXT";
}

int  
clip_INIT___PANGOLAYOUT(ClipMachine * ClipMachineMemory) 
{
   _wtype_table_put(_clip_type_pango_layout, _clip_type_name_pango_layout, _gtk_type_pango_layout, NULL, NULL);
   return 0;
}

int  
clip_INIT___PANGOCONTEXT(ClipMachine * ClipMachineMemory) 
{
   _wtype_table_put(_clip_type_pango_context, _clip_type_name_pango_context, _gtk_type_pango_context, NULL, NULL);
   return 0;
}

int 
clip_PANGOLAYOUTNEW(ClipMachine * ClipMachineMemory) 
{
   C_object * ccontext = _fetch_co_arg(ClipMachineMemory);
   PangoLayout * layout;
   C_object * clayout;
   CHECKCOBJ(ccontext, GTK_IS_PANGO_CONTEXT(ccontext));
   layout = pango_layout_new(PANGO_CONTEXT(ccontext->object));
   if (layout)
      
    {
       clayout = _list_get_cobject(ClipMachineMemory, layout);
       if (!clayout)
	  clayout = _register_object(ClipMachineMemory, layout, GTK_TYPE_PANGO_LAYOUT, NULL, NULL);
       if (clayout)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &clayout->obj);
    }
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTCOPY(ClipMachine * ClipMachineMemory) 
{
   C_object * csrc = _fetch_co_arg(ClipMachineMemory);
   PangoLayout * layout;
   C_object * clayout;
   CHECKCOBJ(csrc, GTK_IS_PANGO_LAYOUT(csrc));
   layout = pango_layout_copy(PANGO_LAYOUT(csrc->object));
   if (layout)
      
    {
       clayout = _list_get_cobject(ClipMachineMemory, layout);
       if (!clayout)
	  clayout = _register_object(ClipMachineMemory, layout, GTK_TYPE_PANGO_LAYOUT, NULL, NULL);
       if (clayout)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &clayout->obj);
    }
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETCONTEXT(ClipMachine * ClipMachineMemory) 
{
   C_object * csrc = _fetch_co_arg(ClipMachineMemory);
   PangoContext * context;
   C_object * ccontext;
   CHECKCOBJ(csrc, GTK_IS_PANGO_LAYOUT(csrc));
   context = pango_layout_get_context(PANGO_LAYOUT(csrc->object));
   if (context)
      
    {
       ccontext = _list_get_cobject(ClipMachineMemory, context);
       if (!ccontext)
	  ccontext = _register_object(ClipMachineMemory, context, GTK_TYPE_PANGO_CONTEXT, NULL, NULL);
       if (ccontext)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ccontext->obj);
    }
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTCONTEXTCHANGED(ClipMachine * ClipMachineMemory) 
{
   C_object * csrc = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(csrc, GTK_IS_PANGO_LAYOUT(csrc));
   pango_layout_context_changed(PANGO_LAYOUT(csrc->object));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTSETTEXT(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   gchar * text = _clip_parc(ClipMachineMemory, 2);
   gint len = _clip_parni(ClipMachineMemory, 3);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (len < 1)
      len = strlen(text);
   LOCALE_TO_UTF(text);
   pango_layout_set_text(PANGO_LAYOUT(clayout->object), text, len);
   FREE_TEXT(text);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETTEXT(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   gchar * text;
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   text = (gchar *) pango_layout_get_text(PANGO_LAYOUT(clayout->object));
   LOCALE_FROM_UTF(text);
   _clip_retc(ClipMachineMemory, text);
   FREE_TEXT(text);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTSETMARKUP(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   gchar * text = _clip_parc(ClipMachineMemory, 2);
   gint len = _clip_parni(ClipMachineMemory, 3);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   if (len < 1)
      len = strlen(text);
   LOCALE_TO_UTF(text);
   pango_layout_set_markup(PANGO_LAYOUT(clayout->object), text, len);
   FREE_TEXT(text);
   return 0;
 err:return 1;
}

int 
clip_PANGOLAYOUTSETMARKUPWITHACCEL(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   gchar * text = _clip_parc(ClipMachineMemory, 2);
   gint len = _clip_parni(ClipMachineMemory, 3);
   gchar * c = _clip_parc(ClipMachineMemory, 4);
   gunichar accel_marker, accel_char;
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, CHARACTER_type_of_ClipVarType);
   if (len < 1)
      len = strlen(text);
   accel_marker = *c;
   LOCALE_TO_UTF(text);
   pango_layout_set_markup_with_accel(PANGO_LAYOUT(clayout->object), text, len, accel_marker, &accel_char);
   FREE_TEXT(text);
   _clip_storc(ClipMachineMemory, (gchar *) (&accel_char), 5, 0);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTSETATTRIBUTES(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   C_object * cpattrs = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKCOBJ(cpattrs, GTK_IS_PANGO_ATTR_LIST(cpattrs));
   pango_layout_set_attributes(PANGO_LAYOUT(clayout->object), PANGO_ATTR_LIST(cpattrs->object));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETATTRIBUTES(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   PangoAttrList * pattrs;
   C_object * cpattrs;
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   pattrs = pango_layout_get_attributes(PANGO_LAYOUT(clayout->object));
   if (pattrs)
      
    {
       cpattrs = _list_get_cobject(ClipMachineMemory, pattrs);
       if (!cpattrs)
	  cpattrs = _register_object(ClipMachineMemory, pattrs, GTK_TYPE_PANGO_ATTR_LIST, NULL, NULL);
       if (cpattrs)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpattrs->obj);
    }
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTSETFONTDESCRIPTION(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   gchar * fontname = _clip_parc(ClipMachineMemory, 2);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, CHARACTER_type_of_ClipVarType) 
    pango_layout_set_font_description(PANGO_LAYOUT(clayout->object), pango_font_description_from_string  (fontname));
   return 0;
 err:return 1;
}

int 
clip_PANGOLAYOUTSETWIDTH(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   int     width = _clip_parni(ClipMachineMemory, 2);

   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKOPT(2, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 0) == 1)
      width = -1;
   pango_layout_set_width(PANGO_LAYOUT(clayout->object), width);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETWIDTH(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   _clip_retni(ClipMachineMemory, pango_layout_get_width(PANGO_LAYOUT(clayout->object)));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTSETWRAP(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   PangoWrapMode mode = _clip_parni(ClipMachineMemory, 2);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   pango_layout_set_wrap(PANGO_LAYOUT(clayout->object), mode);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETWRAP(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   _clip_retni(ClipMachineMemory, (int) pango_layout_get_wrap(PANGO_LAYOUT(clayout->object)));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTSETINDENT(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   int     indent = _clip_parni(ClipMachineMemory, 2);

   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   pango_layout_set_indent(PANGO_LAYOUT(clayout->object), indent);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETINDENT(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   _clip_retni(ClipMachineMemory, pango_layout_get_indent(PANGO_LAYOUT(clayout->object)));
   return 0;
 err:return 1;
}

int 
clip_PANGOLAYOUTSETSPACING(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   int     spacing = _clip_parni(ClipMachineMemory, 2);

   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   pango_layout_set_spacing(PANGO_LAYOUT(clayout->object), spacing);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETSPACING(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   _clip_retni(ClipMachineMemory, pango_layout_get_spacing(PANGO_LAYOUT(clayout->object)));
   return 0;
 err:return 1;
}

int 
clip_PANGOLAYOUTSETJUSTIFY(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   gboolean justify = _clip_parl(ClipMachineMemory, 2);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);
   pango_layout_set_justify(PANGO_LAYOUT(clayout->object), justify);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETJUSTIFY(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   _clip_retl(ClipMachineMemory, pango_layout_get_justify(PANGO_LAYOUT(clayout->object)));
   return 0;
 err:return 1;
}


#if (PANGO_VER_MAJOR >= 1) && (PANGO_VER_MINOR >= 4)
int      
clip_PANGOLAYOUTSETAUTODIR(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   gboolean autodir = _clip_parl(ClipMachineMemory, 2);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);
   pango_layout_set_auto_dir(PANGO_LAYOUT(clayout->object), autodir);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETAUTODIR(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   _clip_retl(ClipMachineMemory, pango_layout_get_auto_dir(PANGO_LAYOUT(clayout->object)));
   return 0;
 err:return 1;
}


#endif	/*
 */
int     
clip_PANGOLAYOUTSETALIGNMENT(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   PangoAlignment align = _clip_parni(ClipMachineMemory, 2);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   pango_layout_set_alignment(PANGO_LAYOUT(clayout->object), align);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETALIGNMENT(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   _clip_retni(ClipMachineMemory, (int) pango_layout_get_alignment(PANGO_LAYOUT(clayout->object)));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTSETTABS(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   C_object * ctabs = _fetch_cobject(ClipMachineMemory, _clip_spar(ClipMachineMemory, 2));
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKCOBJ(ctabs, GTK_IS_PANGO_TAB_ARRAY(ctabs));
   pango_layout_set_tabs(PANGO_LAYOUT(clayout->object), PANGO_TAB_ARRAY(ctabs->object));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETTABS(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   PangoTabArray * tabs;
   C_object * ctabs;
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   tabs = pango_layout_get_tabs(PANGO_LAYOUT(clayout->object));
   if (tabs)
      
    {
       ctabs = _list_get_cobject(ClipMachineMemory, tabs);
       if (!ctabs)
	  ctabs = _register_object(ClipMachineMemory, tabs, GTK_TYPE_PANGO_TAB_ARRAY, NULL, NULL);
       if (ctabs)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ctabs->obj);
    }
   return 0;
 err:return 1;
}

int 
clip_PANGOLAYOUTSETSINGLEPARAGRAPHMODE(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   gboolean setting = _clip_parl(ClipMachineMemory, 2);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);
   pango_layout_set_single_paragraph_mode(PANGO_LAYOUT(clayout->object), setting);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETSINGLEPARAGRAPHMODE(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   _clip_retl(ClipMachineMemory, pango_layout_get_single_paragraph_mode(PANGO_LAYOUT  (clayout->object)));
   return 0;
 err:return 1;
}

int 
clip_PANGOLAYOUTGETLOGATTRS(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   ClipVar * cv = RETPTR(ClipMachineMemory);
   PangoLogAttr * attr;
   gint item;
   long    l;

   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   pango_layout_get_log_attrs(PANGO_LAYOUT(clayout->object), &attr, &item);
   l = item;
   _clip_array(ClipMachineMemory, cv, 1, &l);
   for (l = 0; l < item; l++)
      
    {
       C_object * cattr;
       cattr = _list_get_cobject(ClipMachineMemory, &attr[l]);
       if (!cattr)
	  cattr = _register_object(ClipMachineMemory, &attr[l], GTK_TYPE_PANGO_LOG_ATTR, NULL, NULL);
       if (cattr)
	  _clip_aset(ClipMachineMemory, cv, &cattr->obj, 1, &l);
    }
   g_free(attr);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTXYTOINDEX(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   gint x = _clip_parni(ClipMachineMemory, 2);
   gint y = _clip_parni(ClipMachineMemory, 3);
   gint index, trailing;
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   _clip_retl(ClipMachineMemory, pango_layout_xy_to_index(PANGO_LAYOUT(clayout->object), x, y, &index, &trailing));
   _clip_storni(ClipMachineMemory, index, 4, 0);
   _clip_storni(ClipMachineMemory, trailing, 5, 0);
   return 0;
 err:return 1;
}

CLIP_DLLEXPORT int 
_map_to_pango_rectangle(ClipMachine * ClipMachineMemory, ClipVar * map, PangoRectangle * pos) 
{
   if (map && map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType && pos)
      
    {
       double  x, y, width, height;

       _clip_mgetn(ClipMachineMemory, map, HASH_X, &x);
       _clip_mgetn(ClipMachineMemory, map, HASH_Y, &y);
       _clip_mgetn(ClipMachineMemory, map, HASH_WIDTH, &width);
       _clip_mgetn(ClipMachineMemory, map, HASH_HEIGHT, &height);
       pos->x = x;
       pos->y = y;
       pos->width = width;
       pos->height = height;
       return 0;
    }
   return -1;
}

CLIP_DLLEXPORT int 
_pango_rectangle_to_map(ClipMachine * ClipMachineMemory, ClipVar * map, PangoRectangle * pos) 
{
   if (map && map->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType && pos)
      
    {
       _clip_mputn(ClipMachineMemory, map, HASH_X, pos->x);
       _clip_mputn(ClipMachineMemory, map, HASH_Y, pos->y);
       _clip_mputn(ClipMachineMemory, map, HASH_WIDTH, pos->width);
       _clip_mputn(ClipMachineMemory, map, HASH_HEIGHT, pos->height);
       return 0;
    }
   return -1;
}

int  
clip_PANGOLAYOUTINDEXTOPOS(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   gint index = _clip_parni(ClipMachineMemory, 2);
   ClipVar * cv = 0;
   PangoRectangle pos;
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   pango_layout_index_to_pos(PANGO_LAYOUT(clayout->object), index, &pos);
   _clip_map(ClipMachineMemory, cv);
   _pango_rectangle_to_map(ClipMachineMemory, cv, &pos);
   _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), cv);
   return 0;
 err:return 1;
}

int 
clip_PANGOLAYOUTGETCURSORPOS(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   gint index = _clip_parni(ClipMachineMemory, 2);
   PangoRectangle strong_pos, weak_pos;
   ClipVar * cv1 = _clip_spar(ClipMachineMemory, 3);
   ClipVar * cv2 = _clip_spar(ClipMachineMemory, 4);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   pango_layout_get_cursor_pos(PANGO_LAYOUT(clayout->object), index, &strong_pos, &weak_pos);
   _clip_map(ClipMachineMemory, cv1);
   _clip_map(ClipMachineMemory, cv2);
   if (&strong_pos)
      _pango_rectangle_to_map(ClipMachineMemory, cv1, &strong_pos);
   if (&weak_pos)
      _pango_rectangle_to_map(ClipMachineMemory, cv2, &weak_pos);
   return 0;
 err:return 1;
}

int 
clip_PANGOLAYOUTMOVECURSORVISUALLY(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   gboolean strong = _clip_parl(ClipMachineMemory, 2);
   gint old_index = _clip_parni(ClipMachineMemory, 3);
   gint old_trailing = _clip_parni(ClipMachineMemory, 4);
   gint direction = _clip_parni(ClipMachineMemory, 5);
   gint new_index;
   gint new_trailing;
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKARG(4, NUMERIC_type_of_ClipVarType);
   CHECKARG(5, NUMERIC_type_of_ClipVarType);
   pango_layout_move_cursor_visually(PANGO_LAYOUT(clayout->object), strong, old_index, old_trailing, direction,
					&new_index, &new_trailing);
   _clip_storni(ClipMachineMemory, new_index, 6, 0);
   _clip_storni(ClipMachineMemory, new_trailing, 7, 0);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETEXTENTS(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   PangoRectangle inc_rect, logical_rect;
   ClipVar * cv1 = _clip_spar(ClipMachineMemory, 2);
   ClipVar * cv2 = _clip_spar(ClipMachineMemory, 3);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   pango_layout_get_extents(PANGO_LAYOUT(clayout->object), &inc_rect, &logical_rect);
   _clip_map(ClipMachineMemory, cv1);
   _clip_map(ClipMachineMemory, cv2);
   if (&logical_rect)
      _pango_rectangle_to_map(ClipMachineMemory, cv1, &inc_rect);
   if (&logical_rect)
      _pango_rectangle_to_map(ClipMachineMemory, cv2, &logical_rect);
   return 0;
 err:return 1;
}

int 
clip_PANGOLAYOUTGETPIXELEXTENTS(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   PangoRectangle inc_rect, logical_rect;
   ClipVar * cv1 = _clip_spar(ClipMachineMemory, 2);
   ClipVar * cv2 = _clip_spar(ClipMachineMemory, 3);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   pango_layout_get_pixel_extents(PANGO_LAYOUT(clayout->object), &inc_rect, &logical_rect);
   _clip_map(ClipMachineMemory, cv1);
   _clip_map(ClipMachineMemory, cv2);
   if (&logical_rect)
      _pango_rectangle_to_map(ClipMachineMemory, cv1, &inc_rect);
   if (&logical_rect)
      _pango_rectangle_to_map(ClipMachineMemory, cv2, &logical_rect);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETSIZE(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   int     width, height;

   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   pango_layout_get_size(PANGO_LAYOUT(clayout->object), &width, &height);
   _clip_storni(ClipMachineMemory, width, 2, 0);
   _clip_storni(ClipMachineMemory, height, 3, 0);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETPIXELSIZE(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   int     width, height;

   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   pango_layout_get_pixel_size(PANGO_LAYOUT(clayout->object), &width, &height);
   _clip_storni(ClipMachineMemory, width, 2, 0);
   _clip_storni(ClipMachineMemory, height, 3, 0);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETLINECOUNT(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   _clip_retni(ClipMachineMemory, pango_layout_get_line_count(PANGO_LAYOUT  (clayout->object)));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETLINE(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   gint line_n = _clip_parni(ClipMachineMemory, 2);
   PangoLayoutLine * line;
   C_object * cline;
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   line_n--;
   line = pango_layout_get_line(PANGO_LAYOUT(clayout->object), line_n);
   if (line)
      
    {
       cline = _list_get_cobject(ClipMachineMemory, line);
       if (!cline)
	  cline = _register_object(ClipMachineMemory, line, GTK_TYPE_PANGO_LAYOUT_LINE, NULL, NULL);
       if (cline)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cline->obj);
    }
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETLINES(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   GSList * list;
   ClipVar * cv = RETPTR(ClipMachineMemory);
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   list = pango_layout_get_lines(PANGO_LAYOUT(clayout->object));
   if (list)
      
    {
       int     i = 0;

       long    l;

       l = g_slist_length(list);
       i = l;
       _clip_array(ClipMachineMemory, cv, 1, &l);
       for (l = 0; l < i; l++, list = g_slist_next(list))
	  
	{
	   PangoLayoutLine * line = (PangoLayoutLine *) (list->data);
	   C_object * cline;
	   cline = _list_get_cobject(ClipMachineMemory, line);
	   if (!cline)
	      cline = _register_object(ClipMachineMemory, line, GTK_TYPE_PANGO_LAYOUT_LINE, NULL, NULL);
	   if (cline)
	      _clip_aset(ClipMachineMemory, cv, &cline->obj, 1, &l);
	}
    }
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTGETITER(ClipMachine * ClipMachineMemory) 
{
   C_object * clayout = _fetch_co_arg(ClipMachineMemory);
   PangoLayoutIter * iter;
   C_object * citer;
   CHECKCOBJ(clayout, GTK_IS_PANGO_LAYOUT(clayout));
   iter = pango_layout_get_iter(PANGO_LAYOUT(clayout->object));
   citer = _list_get_cobject(ClipMachineMemory, iter);
   if (!citer)
      citer = _register_object(ClipMachineMemory, iter, GTK_TYPE_PANGO_LAYOUT_ITER, NULL, NULL);
   if (citer)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &citer->obj);
   return 0;
 err:return 1;
}

int 
clip_PANGOLAYOUTITERFREE(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   pango_layout_iter_free(PANGO_LAYOUT_ITER(citer->object));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTITERNEXTRUN(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   _clip_retl(ClipMachineMemory, pango_layout_iter_next_run(PANGO_LAYOUT_ITER  (citer->object)));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTITERNEXTCHAR(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   _clip_retl(ClipMachineMemory, pango_layout_iter_next_char(PANGO_LAYOUT_ITER  (citer->object)));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTITERNEXTCLUSTER(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   _clip_retl(ClipMachineMemory, pango_layout_iter_next_cluster(PANGO_LAYOUT_ITER  (citer->object)));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTITERNEXTLINE(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   _clip_retl(ClipMachineMemory, pango_layout_iter_next_line(PANGO_LAYOUT_ITER  (citer->object)));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTITERATLASTLINE(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   _clip_retl(ClipMachineMemory, pango_layout_iter_at_last_line(PANGO_LAYOUT_ITER  (citer->object)));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTITERGETINDEX(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   _clip_retni(ClipMachineMemory, pango_layout_iter_get_index(PANGO_LAYOUT_ITER  (citer->object)));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTITERGETBASELINE(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   _clip_retni(ClipMachineMemory, pango_layout_iter_get_baseline(PANGO_LAYOUT_ITER  (citer->object)));
   return 0;
 err:return 1;
}

int 
clip_PANGOLAYOUTITERGETRUN(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   PangoLayoutRun * run;
   C_object * crun;
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   run = pango_layout_iter_get_run(PANGO_LAYOUT_ITER(citer->object));
   if (run)
      
    {
       crun = _list_get_cobject(ClipMachineMemory, run);
       if (!crun)
	  crun = _register_object(ClipMachineMemory, run, GTK_TYPE_PANGO_LAYOUT_RUN, NULL, NULL);
       if (crun)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &crun->obj);
    }
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTITERGETLINE(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   PangoLayoutLine * line;
   C_object * cline;
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   line = pango_layout_iter_get_line(PANGO_LAYOUT_ITER(citer->object));
   if (line)
      
    {
       cline = _list_get_cobject(ClipMachineMemory, line);
       if (!cline)
	  cline = _register_object(ClipMachineMemory, line, GTK_TYPE_PANGO_LAYOUT_LINE, NULL, NULL);
       if (cline)
	  _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cline->obj);
    }
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTITERGETCHAREXTENTS(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   ClipVar * cv = _clip_spar(ClipMachineMemory, 2);
   PangoRectangle logical_rect;
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   pango_layout_iter_get_char_extents(PANGO_LAYOUT_ITER(citer->object), &logical_rect);
   _clip_map(ClipMachineMemory, cv);
   _pango_rectangle_to_map(ClipMachineMemory, cv, &logical_rect);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTITERGETCLUSTEREXTENTS(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   ClipVar * cv = _clip_spar(ClipMachineMemory, 2);
   ClipVar * cv2 = _clip_spar(ClipMachineMemory, 3);
   PangoRectangle ink_rect, logical_rect;
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   pango_layout_iter_get_cluster_extents(PANGO_LAYOUT_ITER(citer->object), &ink_rect, &logical_rect);
   _clip_map(ClipMachineMemory, cv);
   _clip_map(ClipMachineMemory, cv2);
   _pango_rectangle_to_map(ClipMachineMemory, cv, &ink_rect);
   _pango_rectangle_to_map(ClipMachineMemory, cv2, &logical_rect);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTITERGETRUNEXTENTS(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   ClipVar * cv = _clip_spar(ClipMachineMemory, 2);
   ClipVar * cv2 = _clip_spar(ClipMachineMemory, 3);
   PangoRectangle ink_rect, logical_rect;
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   pango_layout_iter_get_run_extents(PANGO_LAYOUT_ITER(citer->object), &ink_rect, &logical_rect);
   _clip_map(ClipMachineMemory, cv);
   _clip_map(ClipMachineMemory, cv2);
   _pango_rectangle_to_map(ClipMachineMemory, cv, &ink_rect);
   _pango_rectangle_to_map(ClipMachineMemory, cv2, &logical_rect);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTITERGETLINEYRANGE(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   int     y0_, y1_;

   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   pango_layout_iter_get_line_yrange(PANGO_LAYOUT_ITER(citer->object), &y0_, &y1_);
   _clip_storni(ClipMachineMemory, y0_, 2, 0);
   _clip_storni(ClipMachineMemory, y1_, 3, 0);
   return 0;
 err:return 1;
}

int 
clip_PANGOLAYOUTITERGETLINEEXTENTS(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   ClipVar * cv = _clip_spar(ClipMachineMemory, 2);
   ClipVar * cv2 = _clip_spar(ClipMachineMemory, 3);
   PangoRectangle ink_rect, logical_rect;
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   pango_layout_iter_get_line_extents(PANGO_LAYOUT_ITER(citer->object), &ink_rect, &logical_rect);
   _clip_map(ClipMachineMemory, cv);
   _clip_map(ClipMachineMemory, cv2);
   _pango_rectangle_to_map(ClipMachineMemory, cv, &ink_rect);
   _pango_rectangle_to_map(ClipMachineMemory, cv2, &logical_rect);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTITERGETLAYOUTEXTENTS(ClipMachine * ClipMachineMemory) 
{
   C_object * citer = _fetch_co_arg(ClipMachineMemory);
   ClipVar * cv = _clip_spar(ClipMachineMemory, 2);
   ClipVar * cv2 = _clip_spar(ClipMachineMemory, 3);
   PangoRectangle ink_rect, logical_rect;
   CHECKCOBJ(citer, GTK_IS_PANGO_LAYOUT_ITER(citer));
   pango_layout_iter_get_layout_extents(PANGO_LAYOUT_ITER(citer->object), &ink_rect, &logical_rect);
   _clip_map(ClipMachineMemory, cv);
   _clip_map(ClipMachineMemory, cv2);
   _pango_rectangle_to_map(ClipMachineMemory, cv, &ink_rect);
   _pango_rectangle_to_map(ClipMachineMemory, cv2, &logical_rect);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTLINEREF(ClipMachine * ClipMachineMemory) 
{
   C_object * cline = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(cline, GTK_IS_PANGO_LAYOUT_LINE(cline));
   pango_layout_line_ref(PANGO_LAYOUT_LINE(cline->object));
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTLINEUNREF(ClipMachine * ClipMachineMemory) 
{
   C_object * cline = _fetch_co_arg(ClipMachineMemory);
   CHECKCOBJ(cline, GTK_IS_PANGO_LAYOUT_LINE(cline));
   pango_layout_line_unref(PANGO_LAYOUT_LINE(cline->object));
   return 0;
 err:return 1;
}

int 
clip_PANGOLAYOUTLINEGETEXTENTS(ClipMachine * ClipMachineMemory) 
{
   C_object * cline = _fetch_co_arg(ClipMachineMemory);
   ClipVar * cv = _clip_spar(ClipMachineMemory, 2);
   ClipVar * cv2 = _clip_spar(ClipMachineMemory, 3);
   PangoRectangle ink_rect, logical_rect;
   CHECKCOBJ(cline, GTK_IS_PANGO_LAYOUT_LINE(cline));
   pango_layout_line_get_extents(PANGO_LAYOUT_LINE(cline->object), &ink_rect, &logical_rect);
   _clip_map(ClipMachineMemory, cv);
   _clip_map(ClipMachineMemory, cv2);
   _pango_rectangle_to_map(ClipMachineMemory, cv, &ink_rect);
   _pango_rectangle_to_map(ClipMachineMemory, cv2, &logical_rect);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTLINEGETPIXELEXTENTS(ClipMachine * ClipMachineMemory) 
{
   C_object * cline = _fetch_co_arg(ClipMachineMemory);
   ClipVar * cv = _clip_spar(ClipMachineMemory, 2);
   ClipVar * cv2 = _clip_spar(ClipMachineMemory, 3);
   PangoRectangle ink_rect, logical_rect;
   CHECKCOBJ(cline, GTK_IS_PANGO_LAYOUT_LINE(cline));
   pango_layout_line_get_pixel_extents(PANGO_LAYOUT_LINE(cline->object), &ink_rect, &logical_rect);
   _clip_map(ClipMachineMemory, cv);
   _clip_map(ClipMachineMemory, cv2);
   _pango_rectangle_to_map(ClipMachineMemory, cv, &ink_rect);
   _pango_rectangle_to_map(ClipMachineMemory, cv2, &logical_rect);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTLINEINDEXTOX(ClipMachine * ClipMachineMemory) 
{
   C_object * cline = _fetch_co_arg(ClipMachineMemory);
   int     index_ = _clip_parni(ClipMachineMemory, 2);

   gboolean trailing = _clip_parl(ClipMachineMemory, 3);
   int     x_pos;

   CHECKCOBJ(cline, GTK_IS_PANGO_LAYOUT_LINE(cline));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, LOGICAL_type_of_ClipVarType);
   pango_layout_line_index_to_x(PANGO_LAYOUT_LINE(cline->object), index_, trailing, &x_pos);
   _clip_storni(ClipMachineMemory, x_pos, 4, 0);
   return 0;
 err:return 1;
}

int  
clip_PANGOLAYOUTLINEXTOINDEX(ClipMachine * ClipMachineMemory) 
{
   C_object * cline = _fetch_co_arg(ClipMachineMemory);
   int     x_pos = _clip_parni(ClipMachineMemory, 2);

   int     trailing;

   int     index_;

   CHECKCOBJ(cline, GTK_IS_PANGO_LAYOUT_LINE(cline));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   _clip_retl(ClipMachineMemory, pango_layout_line_x_to_index(PANGO_LAYOUT_LINE 
								  (cline->object), x_pos, &index_, &trailing));
   return 0;
 err:return 1;
}

int 
clip_PANGOLAYOUTLINEGETXRANGES(ClipMachine * ClipMachineMemory) 
{
   C_object * cline = _fetch_co_arg(ClipMachineMemory);
   int     start_index = _clip_parni(ClipMachineMemory, 2);

   int     end_index = _clip_parni(ClipMachineMemory, 3);

   ClipVar * cv = _clip_spar(ClipMachineMemory, 4);
   int    *ranges;

   int     n_ranges;

   long    l;

   CHECKCOBJ(cline, GTK_IS_PANGO_LAYOUT_LINE(cline));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   pango_layout_line_get_x_ranges(PANGO_LAYOUT_LINE(cline->object), start_index, end_index, &ranges, &n_ranges);
   l = n_ranges;
   _clip_array(ClipMachineMemory, cv, 1, &l);
   for (l = 0; l < n_ranges; l++)
      
    {
       ClipVar cn;
       memset(&cn, 0, sizeof(cn));
       cn.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
       cn.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = ranges[l];
       _clip_aset(ClipMachineMemory, cv, &cn, 1, &l);
       _clip_destroy(ClipMachineMemory, &cn);
    }
   g_free(ranges);
   return 0;
 err:return 1;
}


