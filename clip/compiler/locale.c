/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
 */
#include "ci_clip.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "ci_clic.h"
#include "ci_clipcfg.h"

static FILE *out = 0;

static char *fname = 0, *mname = 0;

static char *file_name = 0;

static int
make_dir(char *path)
{
   char dir[256];

   int r;

   snprintf(dir, sizeof(dir), path);
#ifdef OS_MINGW
   r = mkdir(dir);
#else
   r = mkdir(dir, 0775);
#endif
   if (r && errno == EEXIST)
      return 0;

   if (r)
      {
	 char *s = strrchr(dir, '/');

	 if (s)
	    {
	       *s = 0;
	       r = make_dir(dir);
	       if (r)
		  yywarning("cannot create dir '%s': %s", dir, strerror(errno));
	       else
		  {
		     snprintf(dir, sizeof(dir), path);
#ifdef OS_MINGW
		     if ((r = mkdir(dir)))
#else
		     if ((r = mkdir(dir, 0775)))
#endif
			yywarning("cannot create dir '%s': %s", dir, strerror(errno));
		  }
	       return r;
	    }
	 else
	    {
	       return -1;
	    }
      }
   return 0;
}

static int
set_locale(void)
{
   char path[256], *s, *e;

   char *filename = file_name;

   if (out)
      {
	 fclose(out);
	 out = 0;
      }

   snprintf(path, sizeof(path), "%s/locale.pot/%s", CLIP_LOCALE_ROOT, CLIP_MODULE);

   if (make_dir(path))
      {
	 file_name = 0;
	 return -1;
      }

   snprintf(path, sizeof(path), "%s", filename);
   s = strrchr(path, '/');
   e = strrchr(path, '.');
   if (!s)
      s = path;
   if (e && e > path)
      *e = 0;

   free(fname);
   fname = strdup(s);

   snprintf(path, sizeof(path), "%s/locale.pot/%s/%s.pot", CLIP_LOCALE_ROOT, CLIP_MODULE, fname);
   mname = strdup(CLIP_MODULE);
   for (s = mname; *s; s++)
      *s = toupper(*s);

   out = fopen(path, "w");
   if (!out)
      {
	 yywarning("cannot create file '%s': %s", path, strerror(errno));
	 file_name = 0;
	 return -1;
      }
   v_printf(1, "\nopen locale file '%s'\n", path);

   return 0;
}

int
set_locale_name(char *name)
{
   if (out)
      {
	 fclose(out);
	 out = 0;
      }
   file_name = strdup(name);
   return 0;
}

int
resume_locale(void)
{
   if (out)
      {
	 fclose(out);
	 out = 0;
      }
   return 0;
}

static int
put_char(FILE * out, int ch)
{
   switch (ch)
      {
      case '"':
	 return fputs("\\\"", out);
      case '\n':
	 return fputs("\\n\"\n\"", out);
      case '\r':
	 return fputs("\\r", out);
      case '\v':
	 return fputs("\\v", out);
      case '\t':
	 return fputs("\\t", out);
      default:
	 if (ch >= 0 && ch < 32)
	    {
	       char oct[5];

	       sprintf(oct, "\\03%o", ch);
	       return fputs(oct, out);
	    }
	 else
	    {
	       if (fputc(ch, out) == EOF)
		  return EOF;
	    }
      }
   return 0;
}

static int
put_str(FILE * out, char *str)
{
   for (; *str; ++str)
      if (put_char(out, *str) == EOF)
	 return -1;
   return 0;
}

int
put_locale_string(char *name)
{
   if (!out && file_name)
      set_locale();
   if (!out)
      return 0;

   fprintf(out, "#: %s:%ld\n", file_name, clic_line);
   fprintf(out, "msgid \"");
   put_str(out, name);
   fprintf(out, "\"\n");
   fprintf(out, "msgstr \"\"\n\n");

   v_printf(2, "wrote locale message entry '%s'\n", name);
   return 0;
}

int
put_locale_string_plural(char *singular, char *plural)
{
   if (!out && file_name)
      set_locale();
   if (!out)
      return 0;

   fprintf(out, "#: %s:%ld\n", file_name, clic_line);
   fprintf(out, "msgid \"");
   put_str(out, singular);
   fprintf(out, "\"\n");
   fprintf(out, "msgid_plural \"");
   put_str(out, plural);
   fprintf(out, "\"\n");
   fprintf(out, "msgstr[0] \"\"\n");
   fprintf(out, "msgstr[1] \"\"\n\n");

   v_printf(2, "wrote plural locale message '%s' '%s'\n", singular, plural);

  /*put_locale_string(singular); */
  /*put_locale_string(plural); */

   return 0;
}
