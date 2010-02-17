/*
	Copyright (C) 2001  ITK
	Author   : Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>

#include "ci_clip.h"
#include "ci_clipcfg.h"

#ifdef OS_CYGWIN
#include <io.h>
#endif

#include "ci_set.ch"
#include "ci_screen.h"
#include "ci_hashcode.h"
#include "ci_rational.h"
#include "ci_charset.h"

extern char *_get_unix_name(ClipMachine * ClipMemoryStore, char *dname);
static void set_printer_charset(ClipMachine * ClipMemoryStore);

static void
set_flag0(ClipMachine * ClipMemoryStore, int flag, int beg)
{
   _clip_retl(ClipMemoryStore, ClipMemoryStore->flags & flag);
   if (ClipMemoryStore->argc > beg)
      {
	 int lp = -1;
	 ClipVar *vp;

	 vp = _clip_par(ClipMemoryStore, beg + 1);
	switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	    {
	    case LOGICAL_type_of_ClipVarType:
			 lp = vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	       break;
	    case NUMERIC_type_of_ClipVarType:
	       lp = _clip_double(vp);
	       lp = (lp != 0);
	       break;
	    case CHARACTER_type_of_ClipVarType:
	       {
		  char *s = _clip_parc(ClipMemoryStore, beg + 1);

		  if (!strcasecmp(s, "on"))
		     lp = 1;
		  else
		     lp = 0;
	       }
	       break;
	    default:
	       break;
	    }

	 if (lp != -1)
	    {
	       if (lp)
		  ClipMemoryStore->flags |= flag;
	       else
		  ClipMemoryStore->flags &= ~flag;
	    }
      }
}

static void
set_flag(ClipMachine * ClipMemoryStore, int flag)
{
   return set_flag0(ClipMemoryStore, flag, 1);
}

static void
set_flag1(ClipMachine * ClipMemoryStore, int flag, int beg, int inverse)
{
   _clip_retl(ClipMemoryStore, ClipMemoryStore->flags1 & flag);
   if (ClipMemoryStore->argc > beg)
      {
	 int lp = -1;
	 ClipVar *vp;

	 vp = _clip_par(ClipMemoryStore, beg + 1);
	switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	    {
	    case LOGICAL_type_of_ClipVarType:
			 lp = vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	       break;
	    case NUMERIC_type_of_ClipVarType:
	       lp = _clip_double(vp);
	       lp = (lp != 0);
	       break;
	    case CHARACTER_type_of_ClipVarType:
	       {
		  char *s = _clip_parc(ClipMemoryStore, beg + 1);

		  if (!strcasecmp(s, "on"))
		     lp = 1;
		  else
		     lp = 0;
	       }
	       break;

	    default:
	       break;
	    }

	 if (lp != -1)
	    {
	       if (inverse)
		  {
		     if (lp)
			ClipMemoryStore->flags1 &= ~flag;
		     else
			ClipMemoryStore->flags1 |= flag;
		  }
	       else
		  {
		     if (lp)
			ClipMemoryStore->flags1 |= flag;
		     else
			ClipMemoryStore->flags1 &= ~flag;
		  }
	    }
      }
}

static int
_clip_addExtToFile(char *buf, int maxlen, const char *sExt)
{
   int len = strlen(buf);
   int len2 = strlen(sExt);
   int i = len, j;
   int extExist = 0;
  /* filename begin from end string */
   for (; i >= 0; i--)
      {
	 if (buf[i] == '/')
	    break;
	 if (buf[i] == '\\')
	    break;
	 if (buf[i] == ':')
	    break;
      }
  /* ext exist ? */
   for (; i < len; i++)
      {
	 if (buf[i] == '.')
	    extExist = 1;
      }
   i = len;
   if (!extExist && i < (maxlen - (len2 + 1)))
      {
	 buf[i] = '.';
	 for (j = 0, i++; j < len2; j++, i++)
	    buf[i] = sExt[j];
	 buf[i] = 0;
	 extExist = 1;
      }
   else
      extExist = 0;
   return extExist;
}

static void
set_color(ClipMachine * ClipMemoryStore, char *str)
{
   char *s;
   int no = 0, attr, ls, l;

   if (strchr(str, 'X') || strchr(str, 'x'))
      {
	 ClipMemoryStore->attr.standard_of_ClipAttr = 0;
	 ClipMemoryStore->attr.enhanced_of_ClipAttr = 0;
	 ClipMemoryStore->attr.border_of_ClipAttr = 0;
	 ClipMemoryStore->attr.background_of_ClipAttr = 0;
	 ClipMemoryStore->attr.unselected_of_ClipAttr = 0;
	 ClipMemoryStore->attr.u1_of_ClipAttr = 0;
	 ClipMemoryStore->attr.u2_of_ClipAttr = 0;
	 ClipMemoryStore->attr.u3_of_ClipAttr = 0;
	 ClipMemoryStore->attr.u4_of_ClipAttr = 0;
	 ClipMemoryStore->attr.u5_of_ClipAttr = 0;
      }
   for (s = str; s && *s && no < 10; ++no)
      {
	 ls = strspn(s, " \t\n\r");
	 l = strcspn(s, ",");

	 if (l != ls)
	    {
	       attr = _clip_str2attr(s, l);
	       s += l;
	       if (*s)
		  ++s;
	    }
	 else
	    {
	       s += l;
	       if (*s)
		  ++s;
	       continue;
	    }

	 switch (no)
	    {
	    case 0:
	       ClipMemoryStore->attr.standard_of_ClipAttr = attr;
	       break;
	    case 1:
	       ClipMemoryStore->attr.enhanced_of_ClipAttr = attr;
	       ClipMemoryStore->attr.unselected_of_ClipAttr = attr;
	       break;
	    case 2:
	       ClipMemoryStore->attr.border_of_ClipAttr = attr;
	       break;
	    case 3:
	       ClipMemoryStore->attr.background_of_ClipAttr = attr;
	       break;
	    case 4:
	       ClipMemoryStore->attr.unselected_of_ClipAttr = attr;
	       break;
	    case 5:
	       ClipMemoryStore->attr.u1_of_ClipAttr = attr;
	       break;
	    case 6:
	       ClipMemoryStore->attr.u2_of_ClipAttr = attr;
	       break;
	    case 7:
	       ClipMemoryStore->attr.u3_of_ClipAttr = attr;
	       break;
	    case 8:
	       ClipMemoryStore->attr.u4_of_ClipAttr = attr;
	       break;
	    case 9:
	       ClipMemoryStore->attr.u5_of_ClipAttr = attr;
	       break;
	    }
      }
}

static void
get_color(ClipMachine * ClipMemoryStore, char *buf, int buflen, int get_num)
{
   int no;
   char *s, *e;
   int l;

   for (no = 0, s = buf, e = buf + buflen - 1; s < e && no < 10; ++no)
      {
	 int attr = 0;

	 if (no)
	    *s++ = ',';

	 switch (no)
	    {
	    case 0:
	       attr = ClipMemoryStore->attr.standard_of_ClipAttr;
	       break;
	    case 1:
	       attr = ClipMemoryStore->attr.enhanced_of_ClipAttr;
	       break;
	    case 2:
	       attr = ClipMemoryStore->attr.border_of_ClipAttr;
	       break;
	    case 3:
	       attr = ClipMemoryStore->attr.background_of_ClipAttr;
	       break;
	    case 4:
	       attr = ClipMemoryStore->attr.unselected_of_ClipAttr;
	       break;
	    case 5:
	       attr = ClipMemoryStore->attr.u1_of_ClipAttr;
	       break;
	    case 6:
	       attr = ClipMemoryStore->attr.u2_of_ClipAttr;
	       break;
	    case 7:
	       attr = ClipMemoryStore->attr.u3_of_ClipAttr;
	       break;
	    case 8:
	       attr = ClipMemoryStore->attr.u4_of_ClipAttr;
	       break;
	    case 9:
	       attr = ClipMemoryStore->attr.u5_of_ClipAttr;
	       break;
	    }

	 l = _clip_attr2str(attr, s, e - s, get_num);

	 s += l;
      }
   *s = 0;
}

int
_clip_close_printer(ClipMachine * ClipMemoryStore)
{
   FILE *printer = (FILE *) ClipMemoryStore->printer;

   if (!printer)
      return 0;

   fclose(printer);
   ClipMemoryStore->printer = 0;
   if (ClipMemoryStore->real_prfile != ClipMemoryStore->prfile)
      {
	 char buf[256], *sp = ClipMemoryStore->prfile;
	 int prno = 0;

	 char *p_prog;
	/*setenv("CLIP_PRINT_PROG","lpr",0); */
	 p_prog = getenv("CLIP_PRINT_PROG");
	 if (!p_prog || !*p_prog)
	    p_prog = "lpr";

#if 0
	 if (!strcasecmp(sp, "lpt2") || !strcasecmp(sp, "lpt2:"))
	    prno = 2;
	 else if (!strcasecmp(sp, "lpt3") || !strcasecmp(sp, "lpt3:"))
	    prno = 3;
/*
		elseif (!strcasecmp(sp, "lpt1") || !strcasecmp(sp, "lpt1:"))
			prno = 1;
*/
#else
	 if (!strncasecmp(sp, "lpt", 3) && strlen(sp) > 3 && sp[3] >= '1' && sp[3] <= '9')
	    prno = atoi(sp + 3);
	 if (prno <= 1)
	    prno = 0;
#endif
#ifndef _WIN32
	 if (prno)
	    snprintf(buf, sizeof(buf), "%s -P lp%d %s 2>&1 >/dev/null", p_prog, prno, ClipMemoryStore->real_prfile);
	 else
	    snprintf(buf, sizeof(buf), "%s %s 2>&1 >/dev/null", p_prog, ClipMemoryStore->real_prfile);
#else
	 snprintf(buf, sizeof(buf), "copy /b %s %s >null", ClipMemoryStore->real_prfile, ClipMemoryStore->prfile);
#endif
	 system(buf);
	 _clip_logg(2, "close printer:%s", buf);
	 remove(ClipMemoryStore->real_prfile);
	 free(ClipMemoryStore->real_prfile);
      }
   free(ClipMemoryStore->prfile);

   ClipMemoryStore->prfile = 0;
   ClipMemoryStore->real_prfile = 0;
   return 0;
}

int
clip___SETCENTURY(ClipMachine * ClipMemoryStore)
{
   int c_flag = (ClipMemoryStore->flags & CENTURY_FLAG);
   char *s, *e, *new_format, ch = 'y';
   int i, j, pos[10];

   set_flag0(ClipMemoryStore, CENTURY_FLAG, 0);

  /* remake data of SET DATE FORMAT */
   new_format = calloc(1, 11);
   e = ClipMemoryStore->date_format + strlen(ClipMemoryStore->date_format);
   for (i = 0, s = ClipMemoryStore->date_format; s < e; s++)
      {
	 if (*s != 'y' && *s != 'Y')
	    continue;
	 if (*s == 'Y')
	    ch = 'Y';
	 pos[i] = s - ClipMemoryStore->date_format;
	 i++;
      }
  /* new CENTURY setting -> ON */
   if (ClipMemoryStore->flags & CENTURY_FLAG)
      {
	 if (i != 2)
	    {
	       _clip_retl(ClipMemoryStore, c_flag);
	       return 0;
	    }
	 strcpy(new_format, ClipMemoryStore->date_format);
	 new_format[pos[1] + 1] = ch;
	 new_format[pos[1] + 2] = ch;
	 for (i = pos[1] + 3, s = ClipMemoryStore->date_format + pos[1] + 1; s < e; s++, i++)
	    new_format[i] = *s;
	 new_format[i] = 0;
      }
  /* new CENTURY setting -> OFF */
   else
      {
	 if (i < 4)
	    {
	       _clip_retl(ClipMemoryStore, c_flag);
	       return 0;
	    }
	 for (i = 0, j = 0, s = ClipMemoryStore->date_format; s < e; s++, i++)
	    {
	       if (i == pos[2] || i == pos[3])
		  continue;

	       new_format[j] = *s;
	       j++;
	    }
	 new_format[j] = 0;
      }

   if (ClipMemoryStore->date_format != NULL)
      free(ClipMemoryStore->date_format);

   ClipMemoryStore->date_format = new_format;
   _clip_retl(ClipMemoryStore, c_flag);
   return 0;
}

int
clip_SETCANCEL(ClipMachine * ClipMemoryStore)
{
   int l = -1;
   char *str = NULL;

   _clip_retl(ClipMemoryStore, !ClipMemoryStore->notcancel);

   if (_clip_parinfo(ClipMemoryStore, 1) == LOGICAL_type_of_ClipVarType)
      l = _clip_parl(ClipMemoryStore, 1);
   if (_clip_parinfo(ClipMemoryStore, 1) == CHARACTER_type_of_ClipVarType)
      str = _clip_parc(ClipMemoryStore, 1);

   if (_clip_parinfo(ClipMemoryStore, 2) == LOGICAL_type_of_ClipVarType)
      l = _clip_parl(ClipMemoryStore, 2);
   if (_clip_parinfo(ClipMemoryStore, 2) == CHARACTER_type_of_ClipVarType)
      str = _clip_parc(ClipMemoryStore, 2);

   if (str != NULL)
      l = !strcasecmp(str, "on");

   if (l >= 0)
      {
	 if (ClipMemoryStore->fullscreen)
	    setCtrlBreak_Screen(ClipMemoryStore->screen, l);
	 ClipMemoryStore->notcancel = !l;
      }

   return 0;
}

int
clip_SET(ClipMachine * ClipMemoryStore)
{
   int opt = _clip_parni(ClipMemoryStore, 1);
   int lp;
   int argc = ClipMemoryStore->argc;
   char *sp;

   switch (opt)
      {
      case _SET_LOGLEVEL:
	 if (_clip_parinfo(ClipMemoryStore, 2) == NUMERIC_type_of_ClipVarType)
	    _clip_retni(ClipMemoryStore, _clip_loglevel(_clip_parni(ClipMemoryStore, 2)));
	 else
	    _clip_retni(ClipMemoryStore, _clip_loglevel(-1));
	 break;
      case _SET_LOGFILE:
	 if (_clip_parinfo(ClipMemoryStore, 2) == CHARACTER_type_of_ClipVarType)
	    {
	       sp = _get_unix_name(ClipMemoryStore, _clip_parc(ClipMemoryStore, 2));
	       _clip_log_init(sp);
	       free(sp);
	       _clip_retl(ClipMemoryStore, 1);
	    }
	 else
	    _clip_retl(ClipMemoryStore, 0);

	 break;
      case _SET_EXACT:
	 set_flag(ClipMemoryStore, EXACT_FLAG);
	 break;
      case _SET_FIXED:
	 set_flag(ClipMemoryStore, FIXED_FLAG);
	 break;
      case _SET_DECIMALS:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->decimals);
	 if (argc > 1)
	    {
	       lp = _clip_parni(ClipMemoryStore, 2);
	       ClipMemoryStore->decimals = lp;
	    }
	 break;
      case _SET_FILECREATEMODE:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->fileCreateMode);
	 lp = -1;
	 if (argc > 1)
	    {
	       if (_clip_parinfo(ClipMemoryStore, 2) == CHARACTER_type_of_ClipVarType)
		  lp = _clip_fileStrModeToNumMode(_clip_parc(ClipMemoryStore, 2));
	       if (_clip_parinfo(ClipMemoryStore, 2) == NUMERIC_type_of_ClipVarType)
		  lp = _clip_parni(ClipMemoryStore, 2);
	       if (lp > 0)
		  ClipMemoryStore->fileCreateMode = lp;
	    }
	 break;
      case _SET_DIRCREATEMODE:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->dirCreateMode);
	 lp = -1;
	 if (argc > 1)
	    {
	       if (_clip_parinfo(ClipMemoryStore, 2) == CHARACTER_type_of_ClipVarType)
		  lp = _clip_fileStrModeToNumMode(_clip_parc(ClipMemoryStore, 2));
	       if (_clip_parinfo(ClipMemoryStore, 2) == NUMERIC_type_of_ClipVarType)
		  lp = _clip_parni(ClipMemoryStore, 2);
	       if (lp > 0)
		  ClipMemoryStore->dirCreateMode = lp;
	    }
	 break;
      case _SET_DATEFORMAT:
	 _clip_retc(ClipMemoryStore, ClipMemoryStore->date_format);
	 if (argc > 1 && (sp = _clip_parc(ClipMemoryStore, 2)))
	    {
	       int i, ycount = 0;

	       free(ClipMemoryStore->date_format);
	       ClipMemoryStore->date_format = strdup(sp);
	       for (i = 0; sp[i] != 0; i++)
		  {
		     if (sp[i] == 'y' || sp[i] == 'Y')
			ycount++;
		  }
	       if (ycount > 3)
		  ClipMemoryStore->flags |= CENTURY_FLAG;
	       else
		  ClipMemoryStore->flags &= ~CENTURY_FLAG;
	    }
	 break;
      case _SET_EPOCH:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->epoch);
	 if (argc > 1)
	    {
	       lp = _clip_parni(ClipMemoryStore, 2);
	       ClipMemoryStore->epoch = lp;
	    }
	 break;
      case _SET_PATH:
	 _clip_retc(ClipMemoryStore, ClipMemoryStore->path);
	 if (argc > 1 && (sp = _clip_parc(ClipMemoryStore, 2)))
	    {
	       free(ClipMemoryStore->path);
	       ClipMemoryStore->path = strdup(sp);
	    }
	 break;
      case _SET_DEFAULT:
	 _clip_retc(ClipMemoryStore, ClipMemoryStore->defaul);
	 if (argc > 1 && (sp = _clip_parc(ClipMemoryStore, 2)))
	    {
	       if (ClipMemoryStore->defaul)
		  free(ClipMemoryStore->defaul);
	       if (sp[0])
		  ClipMemoryStore->defaul = strdup(sp);
	       else
		  ClipMemoryStore->defaul = strdup(".");
	    }
	 break;
      case _SET_EXCLUSIVE:
	 set_flag(ClipMemoryStore, EXCLUSIVE_FLAG);
	 break;
      case _SET_SOFTSEEK:
	 set_flag(ClipMemoryStore, SOFTSEEK_FLAG);
	 break;
      case _SET_UNIQUE:
	 set_flag(ClipMemoryStore, UNIQUE_FLAG);
	 break;
      case _SET_DELETED:
	 set_flag(ClipMemoryStore, DELETED_FLAG);
	 _clip_set_deleted(ClipMemoryStore, !(ClipMemoryStore->flags & DELETED_FLAG));
	 break;
      case _SET_CANCEL:
	 return clip_SETCANCEL(ClipMemoryStore);
	 break;
      case _SET_DEBUG:
	 set_flag(ClipMemoryStore, DEBUG_FLAG);
	 break;
      case _SET_TYPEAHEAD:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->typeahead);
	 if (argc > 1)
	    {
	       lp = _clip_parni(ClipMemoryStore, 2);
	       if (lp < 16)
		  lp = 16;
	       ClipMemoryStore->typeahead = lp;
	       ClipMemoryStore->kbdbuf = (int *) realloc(ClipMemoryStore->kbdbuf, lp * sizeof(int));

	       *ClipMemoryStore->kbdptr = ClipMemoryStore->kbdbuf;
	    }
	 break;

      case _SET_COLOR:
	 {
	    char buf[64];

	    get_color(ClipMemoryStore, buf, sizeof(buf), 0);
	    _clip_retc(ClipMemoryStore, buf);
	    if (argc > 1 && (sp = _clip_parc(ClipMemoryStore, 2)))
	       {
		  if (*sp)
		     set_color(ClipMemoryStore, sp);
		  else
		     set_color(ClipMemoryStore, DEFAULT_COLOR);
	       }
	 }
	 break;
      case _SET_CURSOR:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->cursor);
	 if (argc > 1)
	    {
	       lp = _clip_parni(ClipMemoryStore, 2);
	       ClipMemoryStore->cursor = lp;
	    }
	 break;
      case _SET_CONSOLE:
	 set_flag(ClipMemoryStore, CONSOLE_FLAG);
	 break;
      case _SET_ALTERNATE:
	 set_flag(ClipMemoryStore, ALTERNATE_FLAG);
	 break;
      case _SET_ALTFILE:
	 _clip_retc(ClipMemoryStore, ClipMemoryStore->altfile);
	 if (argc > 1 && (sp = _clip_parc(ClipMemoryStore, 2)))
	    {
	       FILE *alt = (FILE *) ClipMemoryStore->alt;
	       char *mode;
	       char buf[PATH_MAX];

	       if (_clip_parl(ClipMemoryStore, 3))
		  mode = "a";
	       else
		  mode = "w";
	       if (alt && alt != stdout)
		  fclose(alt);
	       if (sp && sp[0])
		  {
		     if (_clip_path(ClipMemoryStore, sp, buf, sizeof(buf), 1))
			{
			   _clip_trap_printf(ClipMemoryStore, __FILE__, __LINE__, "%s: '%s'", strerror(errno), buf);
			   return 1;
			}

		     _clip_addExtToFile(buf, PATH_MAX, "txt");
		     sp = strdup(buf);

		     if (access(sp, F_OK) != 0)
			close(creat(sp, ClipMemoryStore->fileCreateMode));
		     alt = fopen(sp, mode);
		     if (!alt)
			{
			   _clip_trap_printf(ClipMemoryStore, __FILE__, __LINE__, "%s: '%s'", strerror(errno), sp);
			   return 1;
			}
#ifdef _WIN32
		     setmode(fileno(alt), O_BINARY);
#endif
		     if (ClipMemoryStore->altfile != NULL)
			free(ClipMemoryStore->altfile);
		     ClipMemoryStore->altfile = sp;
		     ClipMemoryStore->alt = alt;
		  }
	       else
		  {
		     if (ClipMemoryStore->altfile != NULL)
			free(ClipMemoryStore->altfile);
		     ClipMemoryStore->alt = 0;
		     ClipMemoryStore->altfile = 0;
		  }
	    }
	 break;
      case _SET_DEVICE:
	 _clip_retc(ClipMemoryStore, (ClipMemoryStore->flags & DEVICE_FLAG) ? "PRINTER" : "SCREEN");
	 if (argc > 1 && (sp = _clip_parc(ClipMemoryStore, 2)))
	    {
	       if (!strcasecmp(sp, "PRINTER"))
		  ClipMemoryStore->flags |= DEVICE_FLAG;
	       else if (!strcasecmp(sp, "PRINT"))
		  ClipMemoryStore->flags |= DEVICE_FLAG;
	       else
		  ClipMemoryStore->flags &= ~DEVICE_FLAG;
	    }
	 break;
      case _SET_EXTRA:
	 set_flag(ClipMemoryStore, EXTRA_FLAG);
	 break;
      case _SET_EXTRAFILE:
	 _clip_retc(ClipMemoryStore, ClipMemoryStore->extrafile);
	 if (argc > 1 && (sp = _clip_parc(ClipMemoryStore, 2)))
	    {
	       FILE *extra = (FILE *) ClipMemoryStore->extra;
	       char *mode;
	       char buf[PATH_MAX];

	       if (_clip_parl(ClipMemoryStore, 3))
		  mode = "a";
	       else
		  mode = "w";
	       if (extra && extra != stdout)
		  fclose(extra);
	       if (sp && sp[0])
		  {
		     if (_clip_path(ClipMemoryStore, sp, buf, sizeof(buf), 1))
			{
			   _clip_trap_printf(ClipMemoryStore, __FILE__, __LINE__, "%s: '%s'", strerror(errno), buf);
			   return 1;
			}

		     _clip_addExtToFile(buf, PATH_MAX, "txt");
		     sp = strdup(buf);

		     if (access(sp, F_OK) != 0)
			close(creat(sp, ClipMemoryStore->fileCreateMode));
		     extra = fopen(sp, mode);
		     if (!extra)
			{
			   _clip_trap_printf(ClipMemoryStore, __FILE__, __LINE__, "%s: '%s'", strerror(errno), sp);
			   return 1;
			}
		     if (ClipMemoryStore->extrafile != NULL)
			free(ClipMemoryStore->extrafile);
#ifdef _WIN32
		     setmode(fileno(extra), O_BINARY);
#endif
		     ClipMemoryStore->extrafile = sp;
		     ClipMemoryStore->extra = extra;
		  }
	       else
		  {
		     if (ClipMemoryStore->extrafile != NULL)
			free(ClipMemoryStore->extrafile);
		     ClipMemoryStore->extra = 0;
		     ClipMemoryStore->extrafile = 0;
		  }
	    }
	 break;
      case _SET_PRINTER:
	 set_flag(ClipMemoryStore, PRINTER_FLAG);
	 if (!(ClipMemoryStore->flags & PRINTER_FLAG) && ClipMemoryStore->printer)
	    fflush((FILE *) ClipMemoryStore->printer);
	 break;
      case _SET_PRINTFILE:
	 _clip_retc(ClipMemoryStore, ClipMemoryStore->prfile);
	 if (argc <= 1)
	    break;

	 _clip_close_printer(ClipMemoryStore);

	 if (_clip_parinfo(ClipMemoryStore, 2) != CHARACTER_type_of_ClipVarType)
	    break;

	 sp = _clip_parc(ClipMemoryStore, 2);
	 if (!sp || !(*sp))
	    break;

	 if (ClipMemoryStore->pbuf)
	    {
	       destroy_Buf(ClipMemoryStore->pbuf);
	       free(ClipMemoryStore->pbuf);
	       ClipMemoryStore->pbuf = 0;
	    }
	 if (strncmp(sp, "MEMBUF", 6) == 0)
	    {
	       ClipMemoryStore->pbuf = calloc(1, sizeof(OutBuf));
	       init_Buf(ClipMemoryStore->pbuf);
	    }
	 else
	    {
	      /* create file for printer output */
	       FILE *printer;
	       char *mode;

	       if (_clip_parl(ClipMemoryStore, 3))
		  mode = "a";
	       else
		  mode = "w";

	       if (!strcasecmp(sp, "prn") || !strcasecmp(sp, "prn:")
#if 0
		   || !strcasecmp(sp, "lpt1") || !strcasecmp(sp, "lpt1:") || !strcasecmp(sp, "lpt2") || !strcasecmp(sp, "lpt2:") || !strcasecmp(sp, "lpt3") || !strcasecmp(sp, "lpt3:")
#else
		   || (!strncasecmp(sp, "lpt", 3) && strlen(sp) > 3 && sp[3] >= '1' && sp[3] <= '9')
#endif
		  )
		  {
		     char buf[256];

		     snprintf(buf, sizeof(buf), "prn-%s-%lx%lx", _clip_progname, (long) getpid(), (long) random());
		     ClipMemoryStore->real_prfile = strdup(buf);
		     ClipMemoryStore->prfile = strdup(sp);
		  }
	       else
		  {
		     char buf[PATH_MAX];

		     _clip_translate_path(ClipMemoryStore, sp, buf, sizeof(buf));
		     _clip_addExtToFile(buf, PATH_MAX, "prn");
		     ClipMemoryStore->real_prfile = ClipMemoryStore->prfile = strdup(buf);
		  }

	       if (access(ClipMemoryStore->real_prfile, F_OK) != 0)
		  close(creat(ClipMemoryStore->real_prfile, ClipMemoryStore->fileCreateMode));
	       printer = fopen(ClipMemoryStore->real_prfile, mode);
	       _clip_logg(2, "set print to:%s,%s", ClipMemoryStore->real_prfile, ClipMemoryStore->prfile);

	       if (!printer)
		  {
		     _clip_trap_printf(ClipMemoryStore, __FILE__, __LINE__, "cannot open printer file '%s': %s", sp, strerror(errno));
		     if (ClipMemoryStore->real_prfile != ClipMemoryStore->prfile)
			free(ClipMemoryStore->real_prfile);
		     free(ClipMemoryStore->prfile);
		     ClipMemoryStore->real_prfile = 0;
		     ClipMemoryStore->prfile = 0;
		     return 1;
		  }
#ifdef _WIN32
	       setmode(fileno(printer), O_BINARY);
#endif
	       ClipMemoryStore->printer = printer;
	       set_printer_charset(ClipMemoryStore);
	    }
	 break;
      case _SET_MARGIN:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->margin);
	 if (argc > 1)
	    {
	       lp = _clip_parni(ClipMemoryStore, 2);
	       if (lp < 0)
		  lp = 0;
	       ClipMemoryStore->margin = lp;
	    }
	 break;

      case _SET_BELL:
	 set_flag(ClipMemoryStore, BELL_FLAG);
	 break;
      case _SET_CONFIRM:
	 set_flag(ClipMemoryStore, CONFIRM_FLAG);
	 break;
      case _SET_ESCAPE:
	 set_flag(ClipMemoryStore, ESCAPE_FLAG);
	 break;
      case _SET_INSERT:
	 set_flag(ClipMemoryStore, INSERT_FLAG);
	 break;
      case _SET_EXIT:
	 set_flag(ClipMemoryStore, EXIT_FLAG);
	 break;
      case _SET_INTENSITY:
	 set_flag(ClipMemoryStore, INTENSITY_FLAG);
	 break;
      case _SET_SCOREBOARD:
	 set_flag(ClipMemoryStore, SCOREBOARD_FLAG);
	 break;
      case _SET_DELIMITERS:
	 set_flag(ClipMemoryStore, DELIMITERS_FLAG);
	 break;
      case _SET_DELIMCHARS:
	 _clip_retc(ClipMemoryStore, ClipMemoryStore->delimchars);
	 if (argc > 1 && (sp = _clip_parc(ClipMemoryStore, 2)))
	    {
	       free(ClipMemoryStore->delimchars);
	       ClipMemoryStore->delimchars = strdup(sp);
	    }
	 break;

      case _SET_WRAP:
	 set_flag(ClipMemoryStore, WRAP_FLAG);
	 break;
      case _SET_MESSAGE:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->msgline);
	 if (argc > 1)
	    {
	       lp = _clip_parni(ClipMemoryStore, 2);
	       if (lp < 0)
		  lp = 0;
	       ClipMemoryStore->msgline = lp;
	    }
	 break;
      case _SET_MCENTER:
	 set_flag(ClipMemoryStore, MCENTER_FLAG);
	 break;
      case _SET_SCROLLBREAK:
	 set_flag(ClipMemoryStore, SCROLLBREAK_FLAG);
	 break;
      case _SET_RATIONAL:
	 set_flag(ClipMemoryStore, RATIONAL_FLAG);
	 break;
      case _SET_TRANSLATE_PATH:
	 set_flag(ClipMemoryStore, TRANSLATE_FLAG);
	 break;
      case _SET_AUTOPEN:
	 set_flag1(ClipMemoryStore, AUTOPEN_FLAG, 1, 0);
	 break;
      case _SET_AUTORDER:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->autorder);
	 ClipMemoryStore->autorder = _clip_parni(ClipMemoryStore, 2);
	 break;
      case _SET_MBLOCKSIZE:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->mblocksize);
	 ClipMemoryStore->mblocksize = _clip_parni(ClipMemoryStore, 2);
	 break;
      case _SET_MAPERR:
	 set_flag1(ClipMemoryStore, MAPERR_FLAG, 1, 0);
	 break;
      case _SET_UTF8TERM:
	 set_flag1(ClipMemoryStore, UTF8TERM_FLAG, 1, 0);
	 break;
      case _SET_IDLE_INKEY:
	 set_flag1(ClipMemoryStore, IDLEINKEY_FLAG, 1, 0);
	 break;
      case _SET_DISPBOX:
	 set_flag1(ClipMemoryStore, DISPBOX_FLAG, 1, 0);
	 if (ClipMemoryStore->fullscreen)
	    {
	       _clip_fullscreen(ClipMemoryStore);
	       setPgMode_Screen(ClipMemoryStore->screen, !(ClipMemoryStore->flags1 & DISPBOX_FLAG));
	    }
	 break;
      case _SET_OPTIMIZE:
	 set_flag1(ClipMemoryStore, OPTIMIZE_FLAG, 1, 0);
	 break;
      case _SET_MACRO_IN_STRING:
	 set_flag1(ClipMemoryStore, NOEXPAND_MACRO_FLAG, 1, 1);
	 break;
      case _SET_FOPENMODE:
	 set_flag1(ClipMemoryStore, FOPENMODE_FLAG, 1, 0);
	 break;
      case _SET_BUFFERING:
	 set_flag1(ClipMemoryStore, BUFFERING_FLAG, 1, 0);
	 break;
      case _SET_MAP_FILE:
	 set_flag1(ClipMemoryStore, MAP_FILE_FLAG, 1, 0);
	 break;
      case _SET_MULTILOCKS:
	 set_flag1(ClipMemoryStore, MULTILOCKS_FLAG, 1, 0);
	 break;
      case _SET_FLUSHOUT:
	 set_flag1(ClipMemoryStore, FLUSHOUT_FLAG, 1, 0);
	 break;
      case _SET_OPTIMIZELEVEL:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->optimizelevel + 1);
	 if (argc > 1)
	    {
	       lp = _clip_parni(ClipMemoryStore, 2);
	       if (lp == 1 || lp == 2)
		  ClipMemoryStore->optimizelevel = lp - 1;
	    }
	 break;
      case _SET_LOCKSTYLE:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->lockstyle);
	 if (argc > 1)
	    {
	       lp = _clip_parni(ClipMemoryStore, 2);
	       ClipMemoryStore->lockstyle = lp;
	    }
	 break;
      case _SET_HOURS:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->hours);
	 if (argc > 1)
	    {
	       lp = _clip_parni(ClipMemoryStore, 2);
	       ClipMemoryStore->hours = lp;
	    }
	 break;
      case _SET_SECONDS:
	 _clip_retl(ClipMemoryStore, ClipMemoryStore->seconds);
	 if (argc > 1)
	    {
	       lp = 0;
	       if (_clip_parinfo(ClipMemoryStore, 2) == LOGICAL_type_of_ClipVarType)
		  {
		     lp = _clip_parl(ClipMemoryStore, 2);
		    //printf("\nset=logical,lp=%d\n",lp);
		  }
	       if (_clip_parinfo(ClipMemoryStore, 2) == CHARACTER_type_of_ClipVarType)
		  {
		     if (strncasecmp("ON", _clip_parc(ClipMemoryStore, 2), 2) == 0)
			lp = 1;
		    //printf("\nset=%s,lp=%d\n",_clip_parc(ClipMemoryStore,2),lp);
		  }
	       ClipMemoryStore->seconds = lp;
	    }
	 break;
      case _SET_EVENTMASK:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->eventmask);
	 if (argc > 1)
	    {
	       lp = _clip_parni(ClipMemoryStore, 2);
	       if (lp < 0)
		  lp = 0;
	       ClipMemoryStore->eventmask = lp;
	    }
	 break;
      case _SET_INDEX_BUFFER_LIMIT:
	 _clip_retni(ClipMemoryStore, ClipMemoryStore->index_buffer_limit);
	 if (argc > 1)
	    {
	       lp = _clip_parni(ClipMemoryStore, 2);
	       if (lp < 0)
		  lp = 0;
	       ClipMemoryStore->index_buffer_limit = lp;
	    }
	 break;
      case _SET_ESC_DELAY:
	 _clip_retni(ClipMemoryStore, esc_delay_Screen);
	 if (argc > 1)
	    {
	       lp = _clip_parni(ClipMemoryStore, 2);
	       if (lp > 0)
		  esc_delay_Screen = lp;
	    }
	 break;

      case _SET_ROOTPATH:
	 _clip_retc(ClipMemoryStore, ClipMemoryStore->rootpath);
	 if (ClipMemoryStore->rootpath == NULL && argc > 1 && (sp = _clip_parc(ClipMemoryStore, 2)))
	    {
	       int len;
	       len = strlen(sp);
	       ClipMemoryStore->rootpath = malloc(len + 2);
	       strcpy(ClipMemoryStore->rootpath, sp);
	       if (sp[len - 1] == '/' || sp[len - 1] == '\\')
		  ;
	       else
		  {
#ifdef _WIN32
		     ClipMemoryStore->rootpath[len] = '\\';
#else
		     ClipMemoryStore->rootpath[len] = '/';
#endif
		     ClipMemoryStore->rootpath[len + 1] = 0;
		  }
	    }
	 break;
      default:
	 {
	    char *name = _clip_parc(ClipMemoryStore, 1);

	    if (name)
	       {
		  char *val;
		  long hash;
		  hash = _clip_hashstr(name);
		  val = _clip_fetch_item(ClipMemoryStore, hash);
		  if (val)
		     _clip_retc(ClipMemoryStore, val);
		  else
		     _clip_retc(ClipMemoryStore, "");

		  if (argc > 1)
		     {
			int len;

			val = _clip_parcl(ClipMemoryStore, 2, &len);
			if (val)
			   _clip_store_item(ClipMemoryStore, hash, _clip_memdup(val, len));
		     }

		  if (strlen(name) == 2 && name[1] == ':' && name[0] >= 'A' && name[0] <= 'Z')
		     {
			int clip_INIT__CTOOLS_DISKFUNC(ClipMachine * ClipMemoryStore);
			clip_INIT__CTOOLS_DISKFUNC(ClipMemoryStore);
		     }
	       }
	 }
      }

   return 0;
}

int
clip_SETCOLOR(ClipMachine * ClipMemoryStore)
{
   char buf[64], *sp;
   int get_num = _clip_parl(ClipMemoryStore, 2);

   get_color(ClipMemoryStore, buf, sizeof(buf), get_num);
   _clip_retc(ClipMemoryStore, buf);
   if (ClipMemoryStore->argc > 0 && (sp = _clip_parc(ClipMemoryStore, 1)))
      {
	 if (*sp)
	    set_color(ClipMemoryStore, sp);
	 else
	    set_color(ClipMemoryStore, DEFAULT_COLOR);
      }
   return 0;
}

int
clip_COLORTON(ClipMachine * ClipMemoryStore)
{
   int l, attr = _clip_parni(ClipMemoryStore, 1);
   char *s = _clip_parcl(ClipMemoryStore, 1, &l);

   if (s != NULL)
      attr = _clip_str2attr(s, l);
   _clip_retni(ClipMemoryStore, attr);
   return 0;
}

int
clip_FT_COLOR2N(ClipMachine * ClipMemoryStore)
{
   return clip_COLORTON(ClipMemoryStore);
}

int
clip_NTOCOLOR(ClipMachine * ClipMemoryStore)
{
   int attr = _clip_parni(ClipMemoryStore, 1);
   int num_format = _clip_parl(ClipMemoryStore, 2);
   char *buf;
   if (attr < 0 || attr > 0xff)
      {
	 _clip_retc(ClipMemoryStore, "");
	 return 0;
      }
   buf = malloc(32);
   memset(buf, 0, 32);
   _clip_attr2str(attr, buf, 31, !num_format);
   _clip_retc(ClipMemoryStore, buf);
   free(buf);
   return 0;
}

int
clip_INVERTATTR(ClipMachine * ClipMemoryStore)
{
   int l, t, attr = _clip_parni(ClipMemoryStore, 1);
   char *s = _clip_parcl(ClipMemoryStore, 1, &l);

   _clip_retni(ClipMemoryStore, 0);
   t = _clip_parinfo(ClipMemoryStore, 1);

   if (t != NUMERIC_type_of_ClipVarType && t != CHARACTER_type_of_ClipVarType)
      return 0;
   if (t == CHARACTER_type_of_ClipVarType)
      attr = _clip_str2attr(s, l);
   attr = ((attr & 0x88) + ((attr & 0x07) << 4) + ((attr & 0x70) >> 4));
   _clip_retni(ClipMemoryStore, attr);
   return 0;
}

int
clip_COLORSELECT(ClipMachine * ClipMemoryStore)
{
   ClipMemoryStore->colorSelect = _clip_parni(ClipMemoryStore, 1);
   return 0;
}

int
clip_STANDARD(ClipMachine * ClipMemoryStore)
{
   ClipMemoryStore->colorSelect = 0;
   _clip_retc(ClipMemoryStore, "");
   return 0;
}

int
clip_UNSELECT(ClipMachine * ClipMemoryStore)
{
   ClipMemoryStore->colorSelect = 4;
   _clip_retc(ClipMemoryStore, "");
   return 0;
}

int
clip_ENHANCED(ClipMachine * ClipMemoryStore)
{
   ClipMemoryStore->colorSelect = 1;
   _clip_retc(ClipMemoryStore, "");
   return 0;
}

int
clip_SETCLEARB(ClipMachine * ClipMemoryStore)
{
   int ch = _clip_parni(ClipMemoryStore, 1);
   char *s = _clip_parc(ClipMemoryStore, 1);
   int *set = _clip_fetch_item(ClipMemoryStore, HASH_setclearb);

   if (s != NULL)
      ch = (*s);
   ch = (ch % 0xff);
   *((char *) set) = (char) ch;
   _clip_retc(ClipMemoryStore, "");

   return 0;
}

int
clip_GETCLEARB(ClipMachine * ClipMemoryStore)
{
   _clip_retni(ClipMemoryStore, *((int *) _clip_fetch_item(ClipMemoryStore, HASH_setclearb)));
   return 0;
}

int
clip_SETCLEARA(ClipMachine * ClipMemoryStore)
{
   int l, attr = _clip_parni(ClipMemoryStore, 1);
   char *s = _clip_parcl(ClipMemoryStore, 1, &l);
   int *set = _clip_fetch_item(ClipMemoryStore, HASH_setcleara);

   if (s != NULL)
      attr = _clip_str2attr(s, l);
   *set = attr;
   _clip_retc(ClipMemoryStore, "");
   return 0;
}

int
clip_GETCLEARA(ClipMachine * ClipMemoryStore)
{
   _clip_retni(ClipMemoryStore, *((int *) _clip_fetch_item(ClipMemoryStore, HASH_setcleara)));
   return 0;
}

static void
set_printer_charset(ClipMachine * ClipMemoryStore)
{
   char *p1, *p2;
   cons_CharsetEntry *cs1 = 0, *cs2 = 0;
   int len1 = 0, len2 = 0;

   p2 = (char *) _clip_fetch_item(ClipMemoryStore, _clip_hashstr("PRINTER_CHARSET"));
   p1 = _clip_getenv("CLIP_HOSTCS");
   if (!p2)
      p2 = _clip_getenv("CLIP_CLIENTCS");
   if (!p1 || !p2 || !*p1 || !*p2)
      goto norm;

   _clip_logg(2, "set printer translation  %s -> %s", p1, p2);

   if (load_charset_name(p1, &cs1, &len1))
      {
	 _clip_logg(0, "cannot load charset file '%s': %s", p1, strerror(errno));
	 return;
      }

   if (load_charset_name(p2, &cs2, &len2))
      {
	 free(cs1);
	 _clip_logg(0, "cannot load charset file '%s': %s", p2, strerror(errno));
	 return;
      }

   make_translation(cs1, len1, cs2, len2, ClipMemoryStore->prntbl);

   free(cs1);
   free(cs2);
 norm:
   ;
}
