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

/*********************** SIGNALS **************************/

/* TextMark has no signals */

/**********************************************************/
CLIP_DLLEXPORT GtkType
_gtk_type_text_mark()
{
   return GTK_TYPE_TEXT_MARK;
}

long
_clip_type_text_mark()
{
   return GTK_OBJECT_TEXT_MARK;
}

const char *
_clip_type_name_text_mark()
{
   return "GTK_OBJECT_TEXT_MARK";
}

int
clip_INIT___TEXTMARK(ClipMachine * ClipMachineMemory)
{
   _wtype_table_put(_clip_type_text_mark, _clip_type_name_text_mark, _gtk_type_text_mark, NULL, NULL);
   return 0;
}

/******************************************************************************
* gtk_TextMarkSetVisible( textMark, visibility ) --> NIL
******************************************************************************/
int
clip_GTK_TEXTMARKSETVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_object *cmark = _fetch_co_arg(ClipMachineMemory);

   gboolean  visibility = _clip_parl(ClipMachineMemory, 2);

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cmark, GTK_IS_TEXT_MARK(cmark->object));
   CHECKARG(2, LOGICAL_type_of_ClipVarType);

   gtk_text_mark_set_visible(GTK_TEXT_MARK(cmark->object), visibility);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextMarkGetVisible( textMark ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTMARKGETVISIBLE(ClipMachine * ClipMachineMemory)
{
   C_object *cmark = _fetch_co_arg(ClipMachineMemory);

   gboolean  visibility;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cmark, GTK_IS_TEXT_MARK(cmark->object));

   visibility = gtk_text_mark_get_visible(GTK_TEXT_MARK(cmark->object));

   _clip_retl(ClipMachineMemory, visibility);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextMarkGetDeleted( textMark ) --> TRUE || FALSE
******************************************************************************/
int
clip_GTK_TEXTMARKGETDELETED(ClipMachine * ClipMachineMemory)
{
   C_object *cmark = _fetch_co_arg(ClipMachineMemory);

   gboolean  deleted;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cmark, GTK_IS_TEXT_MARK(cmark->object));

   deleted = gtk_text_mark_get_deleted(GTK_TEXT_MARK(cmark->object));

   _clip_retl(ClipMachineMemory, deleted);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextMarkGetName( textMark ) --> sMarkName
******************************************************************************/
int
clip_GTK_TEXTMARKGETNAME(ClipMachine * ClipMachineMemory)
{
   C_object *cmark = _fetch_co_arg(ClipMachineMemory);

   gchar    *name;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cmark, GTK_IS_TEXT_MARK(cmark->object));

   name = (gchar *) gtk_text_mark_get_name(GTK_TEXT_MARK(cmark->object));

   _clip_retc(ClipMachineMemory, name);

   return 0;
 err:
   return 1;
}

/******************************************************************************
* gtk_TextMarkGetBuffer( textMark ) --> textBufferObj
******************************************************************************/
int
clip_GTK_TEXTMARKGETBUFFER(ClipMachine * ClipMachineMemory)
{
   C_object *cmark = _fetch_co_arg(ClipMachineMemory);

   GtkTextBuffer *buffer;

   C_object *cbuffer;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cmark, GTK_IS_TEXT_MARK(cmark->object));

   buffer = gtk_text_mark_get_buffer(GTK_TEXT_MARK(cmark->object));

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
* gtk_TextMarkGetLeftGravity( textMark ) --> TRUE || FALSE
* "left" and "right" here refer to logical direction
* (left is the toward the start of the buffer);
* in some languages such as Hebrew the logically-leftmost text
* is not actually on the left when displayed.
******************************************************************************/
int
clip_GTK_TEXTMARKGETLEFTGRAVITY(ClipMachine * ClipMachineMemory)
{
   C_object *cmark = _fetch_co_arg(ClipMachineMemory);

   gboolean  gravity;

   CHECKARG(1, MAP_type_of_ClipVarType);
   CHECKCOBJ(cmark, GTK_IS_TEXT_MARK(cmark->object));

   gravity = gtk_text_mark_get_left_gravity(GTK_TEXT_MARK(cmark->object));

   _clip_retl(ClipMachineMemory, gravity);

   return 0;
 err:
   return 1;
}
