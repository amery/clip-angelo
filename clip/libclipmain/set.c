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

extern char *_get_unix_name(ClipMachine * ClipMachineMemory, char *dname);

static void set_printer_charset(ClipMachine * ClipMachineMemory);

static void
set_flag0(ClipMachine * ClipMachineMemory, int flag, int beg)
{
   _clip_retl(ClipMachineMemory, ClipMachineMemory->flags & flag);
   if (ClipMachineMemory->argc > beg)
    {
       int       lp = -1;

       ClipVar  *vp;

       vp = _clip_par(ClipMachineMemory, beg + 1);
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
	      char     *s = _clip_parc(ClipMachineMemory, beg + 1);

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
	      ClipMachineMemory->flags |= flag;
	   else
	      ClipMachineMemory->flags &= ~flag;
	}
    }
}

static void
set_flag(ClipMachine * ClipMachineMemory, int flag)
{
   return set_flag0(ClipMachineMemory, flag, 1);
}

static void
set_flag1(ClipMachine * ClipMachineMemory, int flag, int beg, int inverse)
{
   _clip_retl(ClipMachineMemory, ClipMachineMemory->flags1 & flag);
   if (ClipMachineMemory->argc > beg)
    {
       int       lp = -1;

       ClipVar  *vp;

       vp = _clip_par(ClipMachineMemory, beg + 1);
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
	      char     *s = _clip_parc(ClipMachineMemory, beg + 1);

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
		  ClipMachineMemory->flags1 &= ~flag;
	       else
		  ClipMachineMemory->flags1 |= flag;
	    }
	   else
	    {
	       if (lp)
		  ClipMachineMemory->flags1 |= flag;
	       else
		  ClipMachineMemory->flags1 &= ~flag;
	    }
	}
    }
}

static int
_clip_addExtToFile(char *buf, int maxlen, const char *sExt)
{
   int       len = strlen(buf);

   int       len2 = strlen(sExt);

   int       i = len, j;

   int       extExist = 0;

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
set_color(ClipMachine * ClipMachineMemory, char *str)
{
   char     *s;

   int       no = 0, attr, ls, l;

   if (strchr(str, 'X') || strchr(str, 'x'))
    {
       ClipMachineMemory->attr.standard_of_ClipAttr = 0;
       ClipMachineMemory->attr.enhanced_of_ClipAttr = 0;
       ClipMachineMemory->attr.border_of_ClipAttr = 0;
       ClipMachineMemory->attr.background_of_ClipAttr = 0;
       ClipMachineMemory->attr.unselected_of_ClipAttr = 0;
       ClipMachineMemory->attr.u1_of_ClipAttr = 0;
       ClipMachineMemory->attr.u2_of_ClipAttr = 0;
       ClipMachineMemory->attr.u3_of_ClipAttr = 0;
       ClipMachineMemory->attr.u4_of_ClipAttr = 0;
       ClipMachineMemory->attr.u5_of_ClipAttr = 0;
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
	   ClipMachineMemory->attr.standard_of_ClipAttr = attr;
	   break;
	case 1:
	   ClipMachineMemory->attr.enhanced_of_ClipAttr = attr;
	   ClipMachineMemory->attr.unselected_of_ClipAttr = attr;
	   break;
	case 2:
	   ClipMachineMemory->attr.border_of_ClipAttr = attr;
	   break;
	case 3:
	   ClipMachineMemory->attr.background_of_ClipAttr = attr;
	   break;
	case 4:
	   ClipMachineMemory->attr.unselected_of_ClipAttr = attr;
	   break;
	case 5:
	   ClipMachineMemory->attr.u1_of_ClipAttr = attr;
	   break;
	case 6:
	   ClipMachineMemory->attr.u2_of_ClipAttr = attr;
	   break;
	case 7:
	   ClipMachineMemory->attr.u3_of_ClipAttr = attr;
	   break;
	case 8:
	   ClipMachineMemory->attr.u4_of_ClipAttr = attr;
	   break;
	case 9:
	   ClipMachineMemory->attr.u5_of_ClipAttr = attr;
	   break;
	}
    }
}

static void
get_color(ClipMachine * ClipMachineMemory, char *buf, int buflen, int get_num)
{
   int       no;

   char     *s, *e;

   int       l;

   for (no = 0, s = buf, e = buf + buflen - 1; s < e && no < 10; ++no)
    {
       int       attr = 0;

       if (no)
	  *s++ = ',';

       switch (no)
	{
	case 0:
	   attr = ClipMachineMemory->attr.standard_of_ClipAttr;
	   break;
	case 1:
	   attr = ClipMachineMemory->attr.enhanced_of_ClipAttr;
	   break;
	case 2:
	   attr = ClipMachineMemory->attr.border_of_ClipAttr;
	   break;
	case 3:
	   attr = ClipMachineMemory->attr.background_of_ClipAttr;
	   break;
	case 4:
	   attr = ClipMachineMemory->attr.unselected_of_ClipAttr;
	   break;
	case 5:
	   attr = ClipMachineMemory->attr.u1_of_ClipAttr;
	   break;
	case 6:
	   attr = ClipMachineMemory->attr.u2_of_ClipAttr;
	   break;
	case 7:
	   attr = ClipMachineMemory->attr.u3_of_ClipAttr;
	   break;
	case 8:
	   attr = ClipMachineMemory->attr.u4_of_ClipAttr;
	   break;
	case 9:
	   attr = ClipMachineMemory->attr.u5_of_ClipAttr;
	   break;
	}

       l = _clip_attr2str(attr, s, e - s, get_num);

       s += l;
    }
   *s = 0;
}

int
_clip_close_printer(ClipMachine * ClipMachineMemory)
{
   FILE     *printer = (FILE *) ClipMachineMemory->printer;

   if (!printer)
      return 0;

   fclose(printer);
   ClipMachineMemory->printer = 0;
   if (ClipMachineMemory->real_prfile != ClipMachineMemory->prfile)
    {
       char      buf[256], *sp = ClipMachineMemory->prfile;

       int       prno = 0;

       char     *p_prog;

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
	  snprintf(buf, sizeof(buf), "%s -P lp%d %s 2>&1 >/dev/null", p_prog, prno, ClipMachineMemory->real_prfile);
       else
	  snprintf(buf, sizeof(buf), "%s %s 2>&1 >/dev/null", p_prog, ClipMachineMemory->real_prfile);
#else
       snprintf(buf, sizeof(buf), "copy /b %s %s >null", ClipMachineMemory->real_prfile, ClipMachineMemory->prfile);
#endif
       system(buf);
       _clip_logg(2, "close printer:%s", buf);
       remove(ClipMachineMemory->real_prfile);
       free(ClipMachineMemory->real_prfile);
    }
   free(ClipMachineMemory->prfile);

   ClipMachineMemory->prfile = 0;
   ClipMachineMemory->real_prfile = 0;
   return 0;
}

int
clip___SETCENTURY(ClipMachine * ClipMachineMemory)
{
   int       c_flag = (ClipMachineMemory->flags & CENTURY_FLAG);

   char     *s, *e, *new_format, ch = 'y';

   int       i, j, pos[10];

   set_flag0(ClipMachineMemory, CENTURY_FLAG, 0);

  /* remake data of SET DATE FORMAT */
   new_format = calloc(1, 11);
   e = ClipMachineMemory->date_format + strlen(ClipMachineMemory->date_format);
   for (i = 0, s = ClipMachineMemory->date_format; s < e; s++)
    {
       if (*s != 'y' && *s != 'Y')
	  continue;
       if (*s == 'Y')
	  ch = 'Y';
       pos[i] = s - ClipMachineMemory->date_format;
       i++;
    }
  /* new CENTURY setting -> ON */
   if (ClipMachineMemory->flags & CENTURY_FLAG)
    {
       if (i != 2)
	{
	   _clip_retl(ClipMachineMemory, c_flag);
	   return 0;
	}
       strcpy(new_format, ClipMachineMemory->date_format);
       new_format[pos[1] + 1] = ch;
       new_format[pos[1] + 2] = ch;
       for (i = pos[1] + 3, s = ClipMachineMemory->date_format + pos[1] + 1; s < e; s++, i++)
	  new_format[i] = *s;
       new_format[i] = 0;
    }
  /* new CENTURY setting -> OFF */
   else
    {
       if (i < 4)
	{
	   _clip_retl(ClipMachineMemory, c_flag);
	   return 0;
	}
       for (i = 0, j = 0, s = ClipMachineMemory->date_format; s < e; s++, i++)
	{
	   if (i == pos[2] || i == pos[3])
	      continue;

	   new_format[j] = *s;
	   j++;
	}
       new_format[j] = 0;
    }

   if (ClipMachineMemory->date_format != NULL)
      free(ClipMachineMemory->date_format);

   ClipMachineMemory->date_format = new_format;
   _clip_retl(ClipMachineMemory, c_flag);
   return 0;
}

int
clip_SETCANCEL(ClipMachine * ClipMachineMemory)
{
   int       l = -1;

   char     *str = NULL;

   _clip_retl(ClipMachineMemory, !ClipMachineMemory->notcancel);

   if (_clip_parinfo(ClipMachineMemory, 1) == LOGICAL_type_of_ClipVarType)
      l = _clip_parl(ClipMachineMemory, 1);
   if (_clip_parinfo(ClipMachineMemory, 1) == CHARACTER_type_of_ClipVarType)
      str = _clip_parc(ClipMachineMemory, 1);

   if (_clip_parinfo(ClipMachineMemory, 2) == LOGICAL_type_of_ClipVarType)
      l = _clip_parl(ClipMachineMemory, 2);
   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
      str = _clip_parc(ClipMachineMemory, 2);

   if (str != NULL)
      l = !strcasecmp(str, "on");

   if (l >= 0)
    {
       if (ClipMachineMemory->fullscreen)
	  setCtrlBreak_Screen(ClipMachineMemory->screen, l);
       ClipMachineMemory->notcancel = !l;
    }

   return 0;
}

int
clip_SET(ClipMachine * ClipMachineMemory)
{
   int       opt = _clip_parni(ClipMachineMemory, 1);

   int       lp;

   int       argc = ClipMachineMemory->argc;

   char     *sp;

   switch (opt)
    {
    case _SET_LOGLEVEL:
       if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
	  _clip_retni(ClipMachineMemory, _clip_loglevel(_clip_parni(ClipMachineMemory, 2)));
       else
	  _clip_retni(ClipMachineMemory, _clip_loglevel(-1));
       break;
    case _SET_LOGFILE:
       if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
	{
	   sp = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 2));
	   _clip_log_init(sp);
	   free(sp);
	   _clip_retl(ClipMachineMemory, 1);
	}
       else
	  _clip_retl(ClipMachineMemory, 0);

       break;
    case _SET_EXACT:
       set_flag(ClipMachineMemory, EXACT_FLAG);
       break;
    case _SET_FIXED:
       set_flag(ClipMachineMemory, FIXED_FLAG);
       break;
    case _SET_DECIMALS:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->decimals);
       if (argc > 1)
	{
	   lp = _clip_parni(ClipMachineMemory, 2);
	   ClipMachineMemory->decimals = lp;
	}
       break;
    case _SET_FILECREATEMODE:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->fileCreateMode);
       lp = -1;
       if (argc > 1)
	{
	   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
	      lp = _clip_fileStrModeToNumMode(_clip_parc(ClipMachineMemory, 2));
	   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
	      lp = _clip_parni(ClipMachineMemory, 2);
	   if (lp > 0)
	      ClipMachineMemory->fileCreateMode = lp;
	}
       break;
    case _SET_DIRCREATEMODE:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->dirCreateMode);
       lp = -1;
       if (argc > 1)
	{
	   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
	      lp = _clip_fileStrModeToNumMode(_clip_parc(ClipMachineMemory, 2));
	   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
	      lp = _clip_parni(ClipMachineMemory, 2);
	   if (lp > 0)
	      ClipMachineMemory->dirCreateMode = lp;
	}
       break;
    case _SET_DATEFORMAT:
       _clip_retc(ClipMachineMemory, ClipMachineMemory->date_format);
       if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
	{
	   int       i, ycount = 0;

	   free(ClipMachineMemory->date_format);
	   ClipMachineMemory->date_format = strdup(sp);
	   for (i = 0; sp[i] != 0; i++)
	    {
	       if (sp[i] == 'y' || sp[i] == 'Y')
		  ycount++;
	    }
	   if (ycount > 3)
	      ClipMachineMemory->flags |= CENTURY_FLAG;
	   else
	      ClipMachineMemory->flags &= ~CENTURY_FLAG;
	}
       break;
    case _SET_EPOCH:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->epoch);
       if (argc > 1)
	{
	   lp = _clip_parni(ClipMachineMemory, 2);
	   ClipMachineMemory->epoch = lp;
	}
       break;
    case _SET_PATH:
       _clip_retc(ClipMachineMemory, ClipMachineMemory->path);
       if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
	{
	   free(ClipMachineMemory->path);
	   ClipMachineMemory->path = strdup(sp);
	}
       break;
    case _SET_DEFAULT:
       _clip_retc(ClipMachineMemory, ClipMachineMemory->defaul);
       if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
	{
	   if (ClipMachineMemory->defaul)
	      free(ClipMachineMemory->defaul);
	   if (sp[0])
	      ClipMachineMemory->defaul = strdup(sp);
	   else
	      ClipMachineMemory->defaul = strdup(".");
	}
       break;
    case _SET_EXCLUSIVE:
       set_flag(ClipMachineMemory, EXCLUSIVE_FLAG);
       break;
    case _SET_SOFTSEEK:
       set_flag(ClipMachineMemory, SOFTSEEK_FLAG);
       break;
    case _SET_UNIQUE:
       set_flag(ClipMachineMemory, UNIQUE_FLAG);
       break;
    case _SET_DELETED:
       set_flag(ClipMachineMemory, DELETED_FLAG);
       _clip_set_deleted(ClipMachineMemory, !(ClipMachineMemory->flags & DELETED_FLAG));
       break;
    case _SET_CANCEL:
       return clip_SETCANCEL(ClipMachineMemory);
       break;
    case _SET_DEBUG:
       set_flag(ClipMachineMemory, DEBUG_FLAG);
       break;
    case _SET_TYPEAHEAD:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->typeahead);
       if (argc > 1)
	{
	   lp = _clip_parni(ClipMachineMemory, 2);
	   if (lp < 16)
	      lp = 16;
	   ClipMachineMemory->typeahead = lp;
	   ClipMachineMemory->kbdbuf = (int *) realloc(ClipMachineMemory->kbdbuf, lp * sizeof(int));

	   *ClipMachineMemory->kbdptr = ClipMachineMemory->kbdbuf;
	}
       break;

    case _SET_COLOR:
       {
	  char      buf[64];

	  get_color(ClipMachineMemory, buf, sizeof(buf), 0);
	  _clip_retc(ClipMachineMemory, buf);
	  if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
	   {
	      if (*sp)
		 set_color(ClipMachineMemory, sp);
	      else
		 set_color(ClipMachineMemory, DEFAULT_COLOR);
	   }
       }
       break;
    case _SET_CURSOR:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->cursor);
       if (argc > 1)
	{
	   lp = _clip_parni(ClipMachineMemory, 2);
	   ClipMachineMemory->cursor = lp;
	}
       break;
    case _SET_CONSOLE:
       set_flag(ClipMachineMemory, CONSOLE_FLAG);
       break;
    case _SET_ALTERNATE:
       set_flag(ClipMachineMemory, ALTERNATE_FLAG);
       break;
    case _SET_ALTFILE:
       _clip_retc(ClipMachineMemory, ClipMachineMemory->altfile);
       if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
	{
	   FILE     *alt = (FILE *) ClipMachineMemory->alt;

	   char     *mode;

	   char      buf[PATH_MAX];

	   if (_clip_parl(ClipMachineMemory, 3))
	      mode = "a";
	   else
	      mode = "w";
	   if (alt && alt != stdout)
	      fclose(alt);
	   if (sp && sp[0])
	    {
	       if (_clip_path(ClipMachineMemory, sp, buf, sizeof(buf), 1))
		{
		   _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "%s: '%s'", strerror(errno), buf);
		   return 1;
		}

	       _clip_addExtToFile(buf, PATH_MAX, "txt");
	       sp = strdup(buf);

	       if (access(sp, F_OK) != 0)
		  close(creat(sp, ClipMachineMemory->fileCreateMode));
	       alt = fopen(sp, mode);
	       if (!alt)
		{
		   _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "%s: '%s'", strerror(errno), sp);
		   return 1;
		}
#ifdef _WIN32
	       setmode(fileno(alt), O_BINARY);
#endif
	       if (ClipMachineMemory->altfile != NULL)
		  free(ClipMachineMemory->altfile);
	       ClipMachineMemory->altfile = sp;
	       ClipMachineMemory->alt = alt;
	    }
	   else
	    {
	       if (ClipMachineMemory->altfile != NULL)
		  free(ClipMachineMemory->altfile);
	       ClipMachineMemory->alt = 0;
	       ClipMachineMemory->altfile = 0;
	    }
	}
       break;
    case _SET_DEVICE:
       _clip_retc(ClipMachineMemory, (ClipMachineMemory->flags & DEVICE_FLAG) ? "PRINTER" : "SCREEN");
       if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
	{
	   if (!strcasecmp(sp, "PRINTER"))
	      ClipMachineMemory->flags |= DEVICE_FLAG;
	   else if (!strcasecmp(sp, "PRINT"))
	      ClipMachineMemory->flags |= DEVICE_FLAG;
	   else
	      ClipMachineMemory->flags &= ~DEVICE_FLAG;
	}
       break;
    case _SET_EXTRA:
       set_flag(ClipMachineMemory, EXTRA_FLAG);
       break;
    case _SET_EXTRAFILE:
       _clip_retc(ClipMachineMemory, ClipMachineMemory->extrafile);
       if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
	{
	   FILE     *extra = (FILE *) ClipMachineMemory->extra;

	   char     *mode;

	   char      buf[PATH_MAX];

	   if (_clip_parl(ClipMachineMemory, 3))
	      mode = "a";
	   else
	      mode = "w";
	   if (extra && extra != stdout)
	      fclose(extra);
	   if (sp && sp[0])
	    {
	       if (_clip_path(ClipMachineMemory, sp, buf, sizeof(buf), 1))
		{
		   _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "%s: '%s'", strerror(errno), buf);
		   return 1;
		}

	       _clip_addExtToFile(buf, PATH_MAX, "txt");
	       sp = strdup(buf);

	       if (access(sp, F_OK) != 0)
		  close(creat(sp, ClipMachineMemory->fileCreateMode));
	       extra = fopen(sp, mode);
	       if (!extra)
		{
		   _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "%s: '%s'", strerror(errno), sp);
		   return 1;
		}
	       if (ClipMachineMemory->extrafile != NULL)
		  free(ClipMachineMemory->extrafile);
#ifdef _WIN32
	       setmode(fileno(extra), O_BINARY);
#endif
	       ClipMachineMemory->extrafile = sp;
	       ClipMachineMemory->extra = extra;
	    }
	   else
	    {
	       if (ClipMachineMemory->extrafile != NULL)
		  free(ClipMachineMemory->extrafile);
	       ClipMachineMemory->extra = 0;
	       ClipMachineMemory->extrafile = 0;
	    }
	}
       break;
    case _SET_PRINTER:
       set_flag(ClipMachineMemory, PRINTER_FLAG);
       if (!(ClipMachineMemory->flags & PRINTER_FLAG) && ClipMachineMemory->printer)
	  fflush((FILE *) ClipMachineMemory->printer);
       break;
    case _SET_PRINTFILE:
       _clip_retc(ClipMachineMemory, ClipMachineMemory->prfile);
       if (argc <= 1)
	  break;

       _clip_close_printer(ClipMachineMemory);

       if (_clip_parinfo(ClipMachineMemory, 2) != CHARACTER_type_of_ClipVarType)
	  break;

       sp = _clip_parc(ClipMachineMemory, 2);
       if (!sp || !(*sp))
	  break;

       if (ClipMachineMemory->pbuf)
	{
	   destroy_Buf(ClipMachineMemory->pbuf);
	   free(ClipMachineMemory->pbuf);
	   ClipMachineMemory->pbuf = 0;
	}
       if (strncmp(sp, "MEMBUF", 6) == 0)
	{
	   ClipMachineMemory->pbuf = calloc(1, sizeof(OutBuf));
	   init_Buf(ClipMachineMemory->pbuf);
	}
       else
	{
	  /* create file for printer output */
	   FILE     *printer;

	   char     *mode;

	   if (_clip_parl(ClipMachineMemory, 3))
	      mode = "a";
	   else
	      mode = "w";

	   if (!strcasecmp(sp, "prn") || !strcasecmp(sp, "prn:")
#if 0
	       || !strcasecmp(sp, "lpt1") || !strcasecmp(sp, "lpt1:")
	       || !strcasecmp(sp, "lpt2") || !strcasecmp(sp, "lpt2:") || !strcasecmp(sp, "lpt3") || !strcasecmp(sp, "lpt3:")
#else
	       || (!strncasecmp(sp, "lpt", 3) && strlen(sp) > 3 && sp[3] >= '1' && sp[3] <= '9')
#endif
	    )
	    {
	       char      buf[256];

	       snprintf(buf, sizeof(buf), "prn-%s-%lx%lx", _clip_progname, (long) getpid(), (long) random());
	       ClipMachineMemory->real_prfile = strdup(buf);
	       ClipMachineMemory->prfile = strdup(sp);
	    }
	   else
	    {
	       char      buf[PATH_MAX];

	       _clip_translate_path(ClipMachineMemory, sp, buf, sizeof(buf));
	       _clip_addExtToFile(buf, PATH_MAX, "prn");
	       ClipMachineMemory->real_prfile = ClipMachineMemory->prfile = strdup(buf);
	    }

	   if (access(ClipMachineMemory->real_prfile, F_OK) != 0)
	      close(creat(ClipMachineMemory->real_prfile, ClipMachineMemory->fileCreateMode));
	   printer = fopen(ClipMachineMemory->real_prfile, mode);
	   _clip_logg(2, "set print to:%s,%s", ClipMachineMemory->real_prfile, ClipMachineMemory->prfile);

	   if (!printer)
	    {
	       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__,
				 "cannot open printer file '%s': %s", sp, strerror(errno));
	       if (ClipMachineMemory->real_prfile != ClipMachineMemory->prfile)
		  free(ClipMachineMemory->real_prfile);
	       free(ClipMachineMemory->prfile);
	       ClipMachineMemory->real_prfile = 0;
	       ClipMachineMemory->prfile = 0;
	       return 1;
	    }
#ifdef _WIN32
	   setmode(fileno(printer), O_BINARY);
#endif
	   ClipMachineMemory->printer = printer;
	   set_printer_charset(ClipMachineMemory);
	}
       break;
    case _SET_MARGIN:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->margin);
       if (argc > 1)
	{
	   lp = _clip_parni(ClipMachineMemory, 2);
	   if (lp < 0)
	      lp = 0;
	   ClipMachineMemory->margin = lp;
	}
       break;

    case _SET_BELL:
       set_flag(ClipMachineMemory, BELL_FLAG);
       break;
    case _SET_CONFIRM:
       set_flag(ClipMachineMemory, CONFIRM_FLAG);
       break;
    case _SET_ESCAPE:
       set_flag(ClipMachineMemory, ESCAPE_FLAG);
       break;
    case _SET_INSERT:
       set_flag(ClipMachineMemory, INSERT_FLAG);
       break;
    case _SET_EXIT:
       set_flag(ClipMachineMemory, EXIT_FLAG);
       break;
    case _SET_INTENSITY:
       set_flag(ClipMachineMemory, INTENSITY_FLAG);
       break;
    case _SET_SCOREBOARD:
       set_flag(ClipMachineMemory, SCOREBOARD_FLAG);
       break;
    case _SET_DELIMITERS:
       set_flag(ClipMachineMemory, DELIMITERS_FLAG);
       break;
    case _SET_DELIMCHARS:
       _clip_retc(ClipMachineMemory, ClipMachineMemory->delimchars);
       if (argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
	{
	   free(ClipMachineMemory->delimchars);
	   ClipMachineMemory->delimchars = strdup(sp);
	}
       break;

    case _SET_WRAP:
       set_flag(ClipMachineMemory, WRAP_FLAG);
       break;
    case _SET_MESSAGE:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->msgline);
       if (argc > 1)
	{
	   lp = _clip_parni(ClipMachineMemory, 2);
	   if (lp < 0)
	      lp = 0;
	   ClipMachineMemory->msgline = lp;
	}
       break;
    case _SET_MCENTER:
       set_flag(ClipMachineMemory, MCENTER_FLAG);
       break;
    case _SET_SCROLLBREAK:
       set_flag(ClipMachineMemory, SCROLLBREAK_FLAG);
       break;
    case _SET_RATIONAL:
       set_flag(ClipMachineMemory, RATIONAL_FLAG);
       break;
    case _SET_TRANSLATE_PATH:
       set_flag(ClipMachineMemory, TRANSLATE_FLAG);
       break;
    case _SET_AUTOPEN:
       set_flag1(ClipMachineMemory, AUTOPEN_FLAG, 1, 0);
       break;
    case _SET_AUTORDER:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->autorder);
       ClipMachineMemory->autorder = _clip_parni(ClipMachineMemory, 2);
       break;
    case _SET_MBLOCKSIZE:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->mblocksize);
       ClipMachineMemory->mblocksize = _clip_parni(ClipMachineMemory, 2);
       break;
    case _SET_MAPERR:
       set_flag1(ClipMachineMemory, MAPERR_FLAG, 1, 0);
       break;
    case _SET_UTF8TERM:
       set_flag1(ClipMachineMemory, UTF8TERM_FLAG, 1, 0);
       break;
    case _SET_IDLE_INKEY:
       set_flag1(ClipMachineMemory, IDLEINKEY_FLAG, 1, 0);
       break;
    case _SET_DISPBOX:
       set_flag1(ClipMachineMemory, DISPBOX_FLAG, 1, 0);
       if (ClipMachineMemory->fullscreen)
	{
	   _clip_fullscreen(ClipMachineMemory);
	   setPgMode_Screen(ClipMachineMemory->screen, !(ClipMachineMemory->flags1 & DISPBOX_FLAG));
	}
       break;
    case _SET_OPTIMIZE:
       set_flag1(ClipMachineMemory, OPTIMIZE_FLAG, 1, 0);
       break;
    case _SET_MACRO_IN_STRING:
       set_flag1(ClipMachineMemory, NOEXPAND_MACRO_FLAG, 1, 1);
       break;
    case _SET_FOPENMODE:
       set_flag1(ClipMachineMemory, FOPENMODE_FLAG, 1, 0);
       break;
    case _SET_BUFFERING:
       set_flag1(ClipMachineMemory, BUFFERING_FLAG, 1, 0);
       break;
    case _SET_MAP_FILE:
       set_flag1(ClipMachineMemory, MAP_FILE_FLAG, 1, 0);
       break;
    case _SET_MULTILOCKS:
       set_flag1(ClipMachineMemory, MULTILOCKS_FLAG, 1, 0);
       break;
    case _SET_FLUSHOUT:
       set_flag1(ClipMachineMemory, FLUSHOUT_FLAG, 1, 0);
       break;
    case _SET_OPTIMIZELEVEL:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->optimizelevel + 1);
       if (argc > 1)
	{
	   lp = _clip_parni(ClipMachineMemory, 2);
	   if (lp == 1 || lp == 2)
	      ClipMachineMemory->optimizelevel = lp - 1;
	}
       break;
    case _SET_LOCKSTYLE:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->lockstyle);
       if (argc > 1)
	{
	   lp = _clip_parni(ClipMachineMemory, 2);
	   ClipMachineMemory->lockstyle = lp;
	}
       break;
    case _SET_HOURS:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->hours);
       if (argc > 1)
	{
	   lp = _clip_parni(ClipMachineMemory, 2);
	   ClipMachineMemory->hours = lp;
	}
       break;
    case _SET_SECONDS:
       _clip_retl(ClipMachineMemory, ClipMachineMemory->seconds);
       if (argc > 1)
	{
	   lp = 0;
	   if (_clip_parinfo(ClipMachineMemory, 2) == LOGICAL_type_of_ClipVarType)
	    {
	       lp = _clip_parl(ClipMachineMemory, 2);
	      //printf("\nset=logical,lp=%d\n",lp);
	    }
	   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
	    {
	       if (strncasecmp("ON", _clip_parc(ClipMachineMemory, 2), 2) == 0)
		  lp = 1;
	      //printf("\nset=%s,lp=%d\n",_clip_parc(ClipMachineMemory,2),lp);
	    }
	   ClipMachineMemory->seconds = lp;
	}
       break;
    case _SET_EVENTMASK:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->eventmask);
       if (argc > 1)
	{
	   lp = _clip_parni(ClipMachineMemory, 2);
	   if (lp < 0)
	      lp = 0;
	   ClipMachineMemory->eventmask = lp;
	}
       break;
    case _SET_INDEX_BUFFER_LIMIT:
       _clip_retni(ClipMachineMemory, ClipMachineMemory->index_buffer_limit);
       if (argc > 1)
	{
	   lp = _clip_parni(ClipMachineMemory, 2);
	   if (lp < 0)
	      lp = 0;
	   ClipMachineMemory->index_buffer_limit = lp;
	}
       break;
    case _SET_ESC_DELAY:
       _clip_retni(ClipMachineMemory, esc_delay_Screen);
       if (argc > 1)
	{
	   lp = _clip_parni(ClipMachineMemory, 2);
	   if (lp > 0)
	      esc_delay_Screen = lp;
	}
       break;

    case _SET_ROOTPATH:
       _clip_retc(ClipMachineMemory, ClipMachineMemory->rootpath);
       if (ClipMachineMemory->rootpath == NULL && argc > 1 && (sp = _clip_parc(ClipMachineMemory, 2)))
	{
	   int       len;

	   len = strlen(sp);
	   ClipMachineMemory->rootpath = malloc(len + 2);
	   strcpy(ClipMachineMemory->rootpath, sp);
	   if (sp[len - 1] == '/' || sp[len - 1] == '\\')
	      ;
	   else
	    {
#ifdef _WIN32
	       ClipMachineMemory->rootpath[len] = '\\';
#else
	       ClipMachineMemory->rootpath[len] = '/';
#endif
	       ClipMachineMemory->rootpath[len + 1] = 0;
	    }
	}
       break;
    default:
       {
	  char     *name = _clip_parc(ClipMachineMemory, 1);

	  if (name)
	   {
	      char     *val;

	      long      hash;

	      hash = _clip_hashstr(name);
	      val = _clip_fetch_item(ClipMachineMemory, hash);
	      if (val)
		 _clip_retc(ClipMachineMemory, val);
	      else
		 _clip_retc(ClipMachineMemory, "");

	      if (argc > 1)
	       {
		  int       len;

		  val = _clip_parcl(ClipMachineMemory, 2, &len);
		  if (val)
		     _clip_store_item(ClipMachineMemory, hash, _clip_memdup(val, len));
	       }

	      if (strlen(name) == 2 && name[1] == ':' && name[0] >= 'A' && name[0] <= 'Z')
	       {
		  int       clip_INIT__CTOOLS_DISKFUNC(ClipMachine * ClipMachineMemory);

		  clip_INIT__CTOOLS_DISKFUNC(ClipMachineMemory);
	       }
	   }
       }
    }

   return 0;
}

int
clip_SETCOLOR(ClipMachine * ClipMachineMemory)
{
   char      buf[64], *sp;

   int       get_num = _clip_parl(ClipMachineMemory, 2);

   get_color(ClipMachineMemory, buf, sizeof(buf), get_num);
   _clip_retc(ClipMachineMemory, buf);
   if (ClipMachineMemory->argc > 0 && (sp = _clip_parc(ClipMachineMemory, 1)))
    {
       if (*sp)
	  set_color(ClipMachineMemory, sp);
       else
	  set_color(ClipMachineMemory, DEFAULT_COLOR);
    }
   return 0;
}

int
clip_COLORTON(ClipMachine * ClipMachineMemory)
{
   int       l, attr = _clip_parni(ClipMachineMemory, 1);

   char     *s = _clip_parcl(ClipMachineMemory, 1, &l);

   if (s != NULL)
      attr = _clip_str2attr(s, l);
   _clip_retni(ClipMachineMemory, attr);
   return 0;
}

int
clip_FT_COLOR2N(ClipMachine * ClipMachineMemory)
{
   return clip_COLORTON(ClipMachineMemory);
}

int
clip_NTOCOLOR(ClipMachine * ClipMachineMemory)
{
   int       attr = _clip_parni(ClipMachineMemory, 1);

   int       num_format = _clip_parl(ClipMachineMemory, 2);

   char     *buf;

   if (attr < 0 || attr > 0xff)
    {
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }
   buf = malloc(32);
   memset(buf, 0, 32);
   _clip_attr2str(attr, buf, 31, !num_format);
   _clip_retc(ClipMachineMemory, buf);
   free(buf);
   return 0;
}

int
clip_INVERTATTR(ClipMachine * ClipMachineMemory)
{
   int       l, t, attr = _clip_parni(ClipMachineMemory, 1);

   char     *s = _clip_parcl(ClipMachineMemory, 1, &l);

   _clip_retni(ClipMachineMemory, 0);
   t = _clip_parinfo(ClipMachineMemory, 1);

   if (t != NUMERIC_type_of_ClipVarType && t != CHARACTER_type_of_ClipVarType)
      return 0;
   if (t == CHARACTER_type_of_ClipVarType)
      attr = _clip_str2attr(s, l);
   attr = ((attr & 0x88) + ((attr & 0x07) << 4) + ((attr & 0x70) >> 4));
   _clip_retni(ClipMachineMemory, attr);
   return 0;
}

int
clip_COLORSELECT(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->colorSelect = _clip_parni(ClipMachineMemory, 1);
   return 0;
}

int
clip_STANDARD(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->colorSelect = 0;
   _clip_retc(ClipMachineMemory, "");
   return 0;
}

int
clip_UNSELECT(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->colorSelect = 4;
   _clip_retc(ClipMachineMemory, "");
   return 0;
}

int
clip_ENHANCED(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->colorSelect = 1;
   _clip_retc(ClipMachineMemory, "");
   return 0;
}

int
clip_SETCLEARB(ClipMachine * ClipMachineMemory)
{
   int       ch = _clip_parni(ClipMachineMemory, 1);

   char     *s = _clip_parc(ClipMachineMemory, 1);

   int      *set = _clip_fetch_item(ClipMachineMemory, HASH_setclearb);

   if (s != NULL)
      ch = (*s);
   ch = (ch % 0xff);
   *((char *) set) = (char) ch;
   _clip_retc(ClipMachineMemory, "");

   return 0;
}

int
clip_GETCLEARB(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, *((int *) _clip_fetch_item(ClipMachineMemory, HASH_setclearb)));
   return 0;
}

int
clip_SETCLEARA(ClipMachine * ClipMachineMemory)
{
   int       l, attr = _clip_parni(ClipMachineMemory, 1);

   char     *s = _clip_parcl(ClipMachineMemory, 1, &l);

   int      *set = _clip_fetch_item(ClipMachineMemory, HASH_setcleara);

   if (s != NULL)
      attr = _clip_str2attr(s, l);
   *set = attr;
   _clip_retc(ClipMachineMemory, "");
   return 0;
}

int
clip_GETCLEARA(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, *((int *) _clip_fetch_item(ClipMachineMemory, HASH_setcleara)));
   return 0;
}

static void
set_printer_charset(ClipMachine * ClipMachineMemory)
{
   char     *p1, *p2;

   cons_CharsetEntry *cs1 = 0, *cs2 = 0;

   int       len1 = 0, len2 = 0;

   p2 = (char *) _clip_fetch_item(ClipMachineMemory, _clip_hashstr("PRINTER_CHARSET"));
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

   make_translation(cs1, len1, cs2, len2, ClipMachineMemory->prntbl);

   free(cs1);
   free(cs2);
 norm:
   ;
}
