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

static GtkTextAttributes _Attrs;
static GtkTextAttributes *Attrs = &_Attrs;

static GtkTextIter _MatchStart;
static GtkTextIter *MatchStart = &_MatchStart;

static GtkTextIter _MatchEnd;
static GtkTextIter *MatchEnd = &_MatchEnd;
/*********************** SIGNALS **************************/

/* TextIter has no signals */

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_text_iter()
{
   return GTK_TYPE_TEXT_ITER;
}

long
_clip_type_text_iter()
{
   return GTK_OBJECT_TEXT_ITER;
}

const char *
_clip_type_name_text_iter()
{
   return "GTK_OBJECT_TEXT_ITER";
}

int
clip_INIT___TEXTITER(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_text_iter, _clip_type_name_text_iter, _gtk_type_text_iter, NULL, NULL);
   return 0;
}

/******************************************************************************
* gtk_TextIterGetBuffer( textIter ) --> textBufferWidget
******************************************************************************/
int
clip_GTK_TEXTITERGETBUFFER(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   GtkTextBuffer *buffer;
   C_object *cbuffer;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   buffer = gtk_text_iter_get_buffer((const GtkTextIter *) (citer->object));

   if (buffer)
      {
	 cbuffer = _list_get_cobject(ClipMachineMemory, buffer);
	 if (!cbuffer)
	    cbuffer = _register_object(ClipMachineMemory, buffer, GTK_TYPE_TEXT_BUFFER, NULL, NULL);
	 if (cbuffer)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cbuffer->obj);
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterCopy( textIter ) --> textIterObject
******************************************************************************/
int
clip_GTK_TEXTITERCOPY(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   GtkTextIter *iter;
   C_object *citernew;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   iter = gtk_text_iter_copy((const GtkTextIter *) (citer->object));

   if (iter)
      {
	 citernew = _list_get_cobject(ClipMachineMemory, iter);
	 if (!citernew)
	    citernew = _register_object(ClipMachineMemory, iter, GTK_TYPE_TEXT_ITER, NULL, NULL);
	 if (citernew)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &citernew->obj);
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterFree( textIter ) --> NIL
******************************************************************************/
int
clip_GTK_TEXTITERFREE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   gtk_text_iter_free((GtkTextIter *) (citer->object));

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetOffset( textIter ) --> nOffset
******************************************************************************/
int
clip_GTK_TEXTITERGETOFFSET(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint offset;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   offset = gtk_text_iter_get_offset((const GtkTextIter *) (citer->object));

   _clip_retni(ClipMachineMemory, offset);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetLine( textIter ) --> nLine
******************************************************************************/
int
clip_GTK_TEXTITERGETLINE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint line;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   line = gtk_text_iter_get_line((const GtkTextIter *) (citer->object));

   _clip_retni(ClipMachineMemory, line);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetLineOffset( textIter ) --> nLineOffset
******************************************************************************/
int
clip_GTK_TEXTITERGETLINEOFFSET(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint offset;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   offset = gtk_text_iter_get_line_offset((const GtkTextIter *) (citer->object));

   _clip_retni(ClipMachineMemory, offset);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetLineIndex( textIter ) --> nLineIndex
******************************************************************************/
int
clip_GTK_TEXTITERGETLINEINDEX(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint index;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   index = gtk_text_iter_get_line_index((const GtkTextIter *) (citer->object));

   _clip_retni(ClipMachineMemory, index);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetVisibleLineIndex( textIter ) --> nLineIndex
******************************************************************************/
int
clip_GTK_TEXTITERGETVISIBLELINEINDEX(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint index;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   index = gtk_text_iter_get_visible_line_index((const GtkTextIter *) (citer->object));

   _clip_retni(ClipMachineMemory, index);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetVisibleLineOffset( textIter ) --> nLineOffset
******************************************************************************/
int
clip_GTK_TEXTITERGETVISIBLELINEOFFSET(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint offset;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   offset = gtk_text_iter_get_visible_line_offset((const GtkTextIter *) (citer->object));

   _clip_retni(ClipMachineMemory, offset);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetChar( textIter ) --> sChar || NIL
******************************************************************************/
int
clip_GTK_TEXTITERGETCHAR(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gchar *chr;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   chr = (gchar *) gtk_text_iter_get_char((const GtkTextIter *) (citer->object));

   if (chr != 0)
      {
	 LOCALE_FROM_UTF(chr);
	 _clip_retc(ClipMachineMemory, chr);
	 FREE_TEXT(chr);
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetSlice( textIter ) --> sString
******************************************************************************/
int
clip_GTK_TEXTITERGETSLICE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   C_object *citerend = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));
   gchar *str;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJ(citerend, GTK_IS_TEXT_ITER(citerend));

   str = gtk_text_iter_get_slice((const GtkTextIter *) (citer->object), (const GtkTextIter *) (citerend->object));

   LOCALE_FROM_UTF(str);
   _clip_retc(ClipMachineMemory, str);
   FREE_TEXT(str);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetText( textIter ) --> sString
******************************************************************************/
int
clip_GTK_TEXTITERGETTEXT(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   C_object *citerend = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));
   gchar *str;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJ(citerend, GTK_IS_TEXT_ITER(citerend));

   str = gtk_text_iter_get_text((const GtkTextIter *) (citer->object), (const GtkTextIter *) (citerend->object));

   LOCALE_FROM_UTF(str);
   _clip_retc(ClipMachineMemory, str);
   FREE_TEXT(str);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetVisibleSlice( textIter ) --> sString
******************************************************************************/
int
clip_GTK_TEXTITERGETVISIBLESLICE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   C_object *citerend = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));
   gchar *str;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJ(citerend, GTK_IS_TEXT_ITER(citerend));

   str = gtk_text_iter_get_visible_slice((const GtkTextIter *) (citer->object), (const GtkTextIter *) (citerend->object));

   LOCALE_FROM_UTF(str);
   _clip_retc(ClipMachineMemory, str);
   FREE_TEXT(str);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetVisibleText( textIter ) --> sString
******************************************************************************/
int
clip_GTK_TEXTITERGETVISIBLETEXT(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   C_object *citerend = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));
   gchar *str;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJ(citerend, GTK_IS_TEXT_ITER(citerend));

   str = gtk_text_iter_get_visible_text((const GtkTextIter *) (citer->object), (const GtkTextIter *) (citerend->object));

   LOCALE_FROM_UTF(str);
   _clip_retc(ClipMachineMemory, str);
   FREE_TEXT(str);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetPixbuf( textIter ) --> pixbufObject
******************************************************************************/
int
clip_GTK_TEXTITERGETPIXBUF(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   GdkPixbuf *pixbuf;
   C_object *cpixbuf;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   pixbuf = gtk_text_iter_get_pixbuf((const GtkTextIter *) (citer->object));

   cpixbuf = _register_object(ClipMachineMemory, pixbuf, GDK_TYPE_PIXBUF, NULL, NULL);
   if (cpixbuf)
      _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &cpixbuf->obj);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetMarks( textIter ) --> arrTextMarkObj
******************************************************************************/
int
clip_GTK_TEXTITERGETMARKS(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   GSList *marks;
   ClipVar *mk = 0;
   long n;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   marks = gtk_text_iter_get_marks((const GtkTextIter *) (citer->object));

   n = g_slist_length(marks);
   memset(mk, 0, sizeof(*mk));
   _clip_array(ClipMachineMemory, mk, 1, &n);
   if (n > 0)
      {
	 long i = 0;
	 while (marks)
	    {
	       ClipVar cv;
	       C_object *cobj;

	       memset(&cv, 0, sizeof(cv));
	       cv.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;

	       if (!marks->data)
		  goto err;
	       cobj = _register_object(ClipMachineMemory, GTK_TEXT_MARK(marks->data), GTK_TYPE_TEXT_MARK, NULL, NULL);
	       if (cobj)
		  {
		     _clip_mclone(ClipMachineMemory, &cv, &cobj->obj);
		     _clip_aset(ClipMachineMemory, mk, &cv, 1, &i);
		  }
	       marks = g_slist_next(marks);
	       _clip_destroy(ClipMachineMemory, &cv);
	       i++;
	    }
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetToggledTags( textIter, toggled_on ) --> arrTextTagObj
******************************************************************************/
int
clip_GTK_TEXTITERGETTOGGLEDTAGS(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean toggled = BOOL_OPTION(ClipMachineMemory, 2, 1);
   GSList *tags;
   ClipVar *mk = 0;
   long n;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, LOGICAL_type_of_ClipVarType) tags = gtk_text_iter_get_toggled_tags((const GtkTextIter *) (citer->object), toggled);

   n = g_slist_length(tags);
   memset(mk, 0, sizeof(*mk));
   _clip_array(ClipMachineMemory, mk, 1, &n);
   if (n > 0)
      {
	 long i = 0;
	 while (tags)
	    {
	       ClipVar cv;
	       C_object *cobj;

	       memset(&cv, 0, sizeof(cv));
	       cv.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;

	       if (!tags->data)
		  goto err;
	       cobj = _register_object(ClipMachineMemory, GTK_TEXT_TAG(tags->data), GTK_TYPE_TEXT_TAG, NULL, NULL);
	       if (cobj)
		  {
		     _clip_mclone(ClipMachineMemory, &cv, &cobj->obj);
		     _clip_aset(ClipMachineMemory, mk, &cv, 1, &i);
		  }
	       tags = g_slist_next(tags);
	       _clip_destroy(ClipMachineMemory, &cv);
	       i++;
	    }
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetChildAnchor( textIter ) --> TextChildAnchorObj
******************************************************************************/
int
clip_GTK_TEXTITERGETCHILDANCHOR(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   GtkTextChildAnchor *anchor;
   C_object *canchor;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   anchor = gtk_text_iter_get_child_anchor((const GtkTextIter *) (citer->object));

   if (anchor)
      {
	 canchor = _register_object(ClipMachineMemory, anchor, GTK_TYPE_TEXT_CHILD_ANCHOR, NULL, NULL);
	 if (canchor)
	    _clip_mclone(ClipMachineMemory, RETPTR(ClipMachineMemory), &canchor->obj);
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterBeginsTag( textIter, textTag ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERBEGINSTAG(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   C_object *ctag = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctag, GTK_IS_TEXT_TAG(citer->object));

   ret = gtk_text_iter_begins_tag((const GtkTextIter *) (citer->object), GTK_TEXT_TAG(ctag->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterEndsTag( textIter, textTag ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERENDSTAG(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   C_object *ctag = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctag, GTK_IS_TEXT_TAG(citer->object));

   ret = gtk_text_iter_ends_tag((const GtkTextIter *) (citer->object), GTK_TEXT_TAG(ctag->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterTogglesTag( textIter, textTag ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERTOGGLESTAG(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   C_object *ctag = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctag, GTK_IS_TEXT_TAG(citer->object));

   ret = gtk_text_iter_toggles_tag((const GtkTextIter *) (citer->object), GTK_TEXT_TAG(ctag->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterHasTag( textIter, textTag ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERHASTAG(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   C_object *ctag = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctag, GTK_IS_TEXT_TAG(citer->object));

   ret = gtk_text_iter_has_tag((const GtkTextIter *) (citer->object), GTK_TEXT_TAG(ctag->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetTags( textIter ) --> arrTextTagObj
******************************************************************************/
int
clip_GTK_TEXTITERGETTAGS(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   GSList *tags;
   ClipVar *mk = 0;
   long n;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   tags = gtk_text_iter_get_tags((const GtkTextIter *) (citer->object));

   n = g_slist_length(tags);
   memset(mk, 0, sizeof(*mk));
   _clip_array(ClipMachineMemory, mk, 1, &n);
   if (n > 0)
      {
	 long i = 0;
	 while (tags)
	    {
	       ClipVar cv;
	       C_object *cobj;

	       memset(&cv, 0, sizeof(cv));
	       cv.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;

	       if (!tags->data)
		  goto err;
	       cobj = _register_object(ClipMachineMemory, GTK_TEXT_TAG(tags->data), GTK_TYPE_TEXT_TAG, NULL, NULL);
	       if (cobj)
		  {
		     _clip_mclone(ClipMachineMemory, &cv, &cobj->obj);
		     _clip_aset(ClipMachineMemory, mk, &cv, 1, &i);
		  }
	       tags = g_slist_next(tags);
	       _clip_destroy(ClipMachineMemory, &cv);
	       i++;
	    }
      }
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterEditable( textIter, default_setting ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITEREDITABLE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean defset = _clip_parl(ClipMachineMemory, 2);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   ret = gtk_text_iter_editable((const GtkTextIter *) (citer->object), defset);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterCanInsert( textIter, default_editability ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERCANINSERT(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean defedt = _clip_parl(ClipMachineMemory, 2);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   ret = gtk_text_iter_can_insert((const GtkTextIter *) (citer->object), defedt);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterStartsWord( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERSTARTSWORD(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_starts_word((const GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterEndsWord( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERENDSWORD(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_ends_word((const GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterInsideWord( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERINSIDEWORD(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_inside_word((const GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterStartsLine( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERSTARTSLINE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_starts_line((const GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterEndsLine( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERENDSLINE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_ends_line((const GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterStartsSentence( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERSTARTSSENTENCE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_starts_sentence((const GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterEndsSentence( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERENDSSENTENCE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_ends_sentence((const GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterInsideSentence( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERINSIDESENTENCE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_inside_sentence((const GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterIsCursorPosition( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERISCURSORPOSITION(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_is_cursor_position((const GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetCharsInLine( textIter ) --> nChars
******************************************************************************/
int
clip_GTK_TEXTITERGETCHARSINLINE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_get_chars_in_line((const GtkTextIter *) (citer->object));

   _clip_retni(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetBytesInLine( textIter ) --> nChars
******************************************************************************/
int
clip_GTK_TEXTITERGETBYTESINLINE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_get_bytes_in_line((const GtkTextIter *) (citer->object));

   _clip_retni(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetAttributes( textIter, @attrs ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERGETATTRIBUTES(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   ClipVar *cvattrs = 0;
   C_object *cattrs = 0;
   gint ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   memset(Attrs, 0, sizeof(Attrs));
   ret = gtk_text_iter_get_attributes((const GtkTextIter *) (citer->object), Attrs);

   if (ret && Attrs)
      {
	 cattrs = _list_get_cobject(ClipMachineMemory, Attrs);
	 if (!cattrs)
	    cattrs = _register_object(ClipMachineMemory, Attrs, GTK_TYPE_TEXT_ATTRIBUTES, cvattrs, NULL);
	 if (cattrs)
	    _clip_mclone(ClipMachineMemory, cvattrs, &cattrs->obj);
      }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterGetLanguage( textIter ) --> sLanguage
******************************************************************************/
int
clip_GTK_TEXTITERGETLANGUAGE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   PangoLanguage *language;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   language = gtk_text_iter_get_language((const GtkTextIter *) (citer->object));

   _clip_retc(ClipMachineMemory, (char *) language);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterIsEnd( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERISEND(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_is_end((const GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterIsStart( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERISSTART(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_is_start((const GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterForwardChar( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERFORWARDCHAR(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_forward_char((GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterBackwardChar( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERBACKWARDCHAR(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_backward_char((GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterForwardChars( textIter, count ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERFORWARDCHARS(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint count = _clip_parni(ClipMachineMemory, 2);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_iter_forward_chars((GtkTextIter *) (citer->object), count);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterBackwardChars( textIter, count ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERBACKWARDCHARS(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint count = _clip_parni(ClipMachineMemory, 2);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_iter_backward_chars((GtkTextIter *) (citer->object), count);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterForwardLine( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERFORWARDLINE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_forward_line((GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterBackwardLine( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERBACKWARDLINE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_backward_line((GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterForwardLines( textIter, count ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERFORWARDLINES(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint count = _clip_parni(ClipMachineMemory, 2);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_iter_forward_lines((GtkTextIter *) (citer->object), count);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterBackwardLines( textIter, count ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERBACKWARDLINES(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint count = _clip_parni(ClipMachineMemory, 2);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_iter_backward_lines((GtkTextIter *) (citer->object), count);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterForwardWordEnds( textIter, count ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERFORWARDWORDENDS(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint count = _clip_parni(ClipMachineMemory, 2);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_iter_forward_word_ends((GtkTextIter *) (citer->object), count);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterBackwardWordStarts( textIter, count ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERBACKWARDWORDSTARTS(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint count = _clip_parni(ClipMachineMemory, 2);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_iter_backward_word_starts((GtkTextIter *) (citer->object), count);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterForwardWordEnd( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERFORWARDWORDEND(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_forward_word_end((GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterBackwardWordStart( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERBACKWARDWORDSTART(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_backward_word_start((GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterForwardCursorPosition( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERFORWARDCURSORPOSITION(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_forward_cursor_position((GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterBackwardCursorPosition( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERBACKWARDCURSORPOSITION(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_backward_cursor_position((GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterForwardCursorPositions( textIter, count ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERFORWARDCURSORPOSITIONS(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint count = _clip_parni(ClipMachineMemory, 2);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_iter_forward_cursor_positions((GtkTextIter *) (citer->object), count);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterBackwardCursorPositions( textIter, count ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERBACKWARDCURSORPOSITIONS(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint count = _clip_parni(ClipMachineMemory, 2);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_iter_backward_cursor_positions((GtkTextIter *) (citer->object), count);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterBackwardSentenceStart( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERBACKWARDSENTENCESTART(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_backward_sentence_start((GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterBackwardSentenceStarts( textIter, count ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERBACKWARDSENTENCESTARTS(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint count = _clip_parni(ClipMachineMemory, 2);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_iter_backward_sentence_starts((GtkTextIter *) (citer->object), count);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterForwardSentenceEnd( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERFORWARDSENTENCEEND(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_forward_sentence_end((GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterForwardSentenceEnds( textIter, count ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERFORWARDSENTENCEENDS(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint count = _clip_parni(ClipMachineMemory, 2);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   ret = gtk_text_iter_forward_sentence_ends((GtkTextIter *) (citer->object), count);

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterSetOffset( textIter, char_offset ) --> NIL
******************************************************************************/
int
clip_GTK_TEXTITERSETOFFSET(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint offset = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_text_iter_set_offset((GtkTextIter *) (citer->object), offset);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterSetLine( textIter, line_number ) --> NIL
******************************************************************************/
int
clip_GTK_TEXTITERSETLINE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint number = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_text_iter_set_line((GtkTextIter *) (citer->object), number);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterSetLIneOffset( textIter, char_on_line ) --> NIL
******************************************************************************/
int
clip_GTK_TEXTITERSETLINEOFFSET(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint offset = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_text_iter_set_line_offset((GtkTextIter *) (citer->object), offset);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterSetLIneIndex( textIter, byte_on_line ) --> NIL
******************************************************************************/
int
clip_GTK_TEXTITERSETLINEINDEX(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint index = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_text_iter_set_line_index((GtkTextIter *) (citer->object), index);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterSetVisibleLineIndex( textIter, byte_on_line ) --> NIL
******************************************************************************/
int
clip_GTK_TEXTITERSETVISIBLELINEINDEX(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint index = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_text_iter_set_visible_line_index((GtkTextIter *) (citer->object), index);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterSetVisibleLineOffset( textIter, char_on_line ) --> NIL
******************************************************************************/
int
clip_GTK_TEXTITERSETVISIBLELINEOFFSET(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gint offset = _clip_parni(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, NUMERIC_type_of_ClipVarType);

   gtk_text_iter_set_visible_line_offset((GtkTextIter *) (citer->object), offset);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterForwardToEnd( textIter ) --> NIL
******************************************************************************/
int
clip_GTK_TEXTITERFORWARDTOEND(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   gtk_text_iter_forward_to_end((GtkTextIter *) (citer->object));

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterForwardToLineEnd( textIter ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERFORWARDTOLINEEND(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));

   ret = gtk_text_iter_forward_to_line_end((GtkTextIter *) (citer->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterForwardToTagToggle( textIter, textTag ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERFORWARDTOTAGTOGGLE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   C_object *ctag = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctag, GTK_IS_TEXT_TAG(ctag->object));

   ret = gtk_text_iter_forward_to_tag_toggle((GtkTextIter *) (citer->object), GTK_TEXT_TAG(ctag->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterBackwardToTagToggle( textIter, textTag ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERBACKWARDTOTAGTOGGLE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   C_object *ctag = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJ(ctag, GTK_IS_TEXT_TAG(ctag->object));

   ret = gtk_text_iter_backward_to_tag_toggle((GtkTextIter *) (citer->object), GTK_TEXT_TAG(ctag->object));

   _clip_retl(ClipMachineMemory, 2);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterForwardSearch( textIter, str, flags, @mathTextIterStart,
* @matchTextIterEnd, textIterLimit ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERFORWARDSEARCH(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gchar *str = _clip_parc(ClipMachineMemory, 2);
   GtkTextSearchFlags flags = INT_OPTION(ClipMachineMemory, 3, 0);
   ClipVar *cvstart = _clip_par(ClipMachineMemory, 4);
   ClipVar *cvend = _clip_par(ClipMachineMemory, 5);
   C_object *climit = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 6));
   C_object *cstart;
   C_object *cend;
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, MAP_type_of_ClipVarType);
   CHECKOPT(5, MAP_type_of_ClipVarType);
   CHECKOPT(6, MAP_type_of_ClipVarType);
   CHECKCOBJOPT(climit, GTK_IS_TEXT_ITER(climit));

   memset(MatchStart, 0, sizeof(MatchStart));
   memset(MatchEnd, 0, sizeof(MatchEnd));

   ret = gtk_text_iter_forward_search((const GtkTextIter *) (citer->object), str, flags, MatchStart, MatchEnd, (climit ? (const GtkTextIter *) (climit->object) : NULL));

   if (ret && MatchStart && MatchEnd)
      {
	 cstart = _register_object(ClipMachineMemory, MatchStart, GTK_TYPE_TEXT_ITER, cvstart, NULL);
	 if (cstart)
	    _clip_mclone(ClipMachineMemory, cvstart, &cstart->obj);

	 cend = _register_object(ClipMachineMemory, MatchEnd, GTK_TYPE_TEXT_ITER, cvend, NULL);
	 if (cend)
	    _clip_mclone(ClipMachineMemory, cvend, &cend->obj);
      }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterBackwardSearch( textIter, str, flags, @mathTextIterStart,
* @matchTextIterEnd, textIterLimit ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERBACKWARDSEARCH(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   gchar *str = _clip_parc(ClipMachineMemory, 2);
   GtkTextSearchFlags flags = INT_OPTION(ClipMachineMemory, 3, 0);
   ClipVar *cvstart = _clip_par(ClipMachineMemory, 4);
   ClipVar *cvend = _clip_par(ClipMachineMemory, 5);
   C_object *climit = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 6));
   C_object *cstart;
   C_object *cend;
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, CHARACTER_type_of_ClipVarType);
   CHECKARG(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT(4, MAP_type_of_ClipVarType);
   CHECKOPT(5, MAP_type_of_ClipVarType);
   CHECKOPT(6, MAP_type_of_ClipVarType);
   CHECKCOBJOPT(climit, GTK_IS_TEXT_ITER(climit));

   memset(MatchStart, 0, sizeof(MatchStart));
   memset(MatchEnd, 0, sizeof(MatchEnd));

   ret = gtk_text_iter_backward_search((const GtkTextIter *) (citer->object), str, flags, MatchStart, MatchEnd, (climit ? (const GtkTextIter *) (climit->object) : NULL));

   if (ret && MatchStart && MatchEnd)
      {
	 cstart = _register_object(ClipMachineMemory, MatchStart, GTK_TYPE_TEXT_ITER, cvstart, NULL);
	 if (cstart)
	    _clip_mclone(ClipMachineMemory, cvstart, &cstart->obj);

	 cend = _register_object(ClipMachineMemory, MatchEnd, GTK_TYPE_TEXT_ITER, cvend, NULL);
	 if (cend)
	    _clip_mclone(ClipMachineMemory, cvend, &cend->obj);
      }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterEqual( textIter, anotherTextIter) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITEREQUAL(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   C_object *citer2 = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJOPT(citer2, GTK_IS_TEXT_ITER(citer2));

   ret = gtk_text_iter_equal((const GtkTextIter *) (citer->object), (const GtkTextIter *) (citer2->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterCompare( textIter, anotherTextIter) --> -1 || 1 || 0   (1<2, 1>2, 1==2)
******************************************************************************/
int
clip_GTK_TEXTITERCOMPARE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   C_object *citer2 = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));
   gint ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJOPT(citer2, GTK_IS_TEXT_ITER(citer2));

   ret = gtk_text_iter_compare((const GtkTextIter *) (citer->object), (const GtkTextIter *) (citer2->object));

   _clip_retni(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterInRange( textIter, startTextIter, endTextIter) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTITERINRANGE(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   C_object *citer2 = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));
   C_object *citer3 = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 3));
   gboolean ret;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJOPT(citer2, GTK_IS_TEXT_ITER(citer2));
   CHECKARG(3, MAP_type_of_ClipVarType);
   CHECKCOBJOPT(citer3, GTK_IS_TEXT_ITER(citer3));

   ret = gtk_text_iter_in_range((const GtkTextIter *) (citer->object), (const GtkTextIter *) (citer2->object), (const GtkTextIter *) (citer3->object));

   _clip_retl(ClipMachineMemory, ret);
   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextIterOrder( textIter, anotherTextIter) --> NIL
******************************************************************************/
int
clip_GTK_TEXTITERORDER(ClipMachine * ClipMachineMemory)
{
   C_object *citer = _fetch_co_arg(ClipMachineMemory);
   C_object *citer2 = _fetch_cobject(ClipMachineMemory, _clip_par(ClipMachineMemory, 2));

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(citer, GTK_IS_TEXT_ITER(citer));
   CHECKARG(2, MAP_type_of_ClipVarType);
   CHECKCOBJOPT(citer2, GTK_IS_TEXT_ITER(citer2));

   gtk_text_iter_order((GtkTextIter *) (citer->object), (GtkTextIter *) (citer2->object));

   return 0;
 err:
   return 1;
}
