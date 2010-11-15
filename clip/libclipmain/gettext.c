#include <sys/types.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <locale.h>
#include <dirent.h>
#include <ctype.h>

#include "ci_clip.h"
#include "ci_coll.h"
#include "ci_gettext.h"
#include "ci_plural.h"
#include "ci_clipcfg.h"

#ifdef OS_CYGWIN
#undef NO_MMAP
#define NO_MMAP
#endif

#ifndef NO_MMAP
#include <sys/mman.h>
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

typedef struct
{
	char *name;
	char *module;
	int ok;

	char *data;
	int use_mmap;
	int fd;
	size_t mmap_size;
	int must_swap;
	nls_uint32 nstrings;
	struct string_desc *orig_tab;
	struct string_desc *trans_tab;
	nls_uint32 hash_size;
	nls_uint32 *hash_tab;
	char *charset;

	char *nullentry;
	int nplural;
	PluralData *pd;
}
Locale;

static nls_uint32 SWAP(nls_uint32 i);
static void delete_Locale(void *item);
static char *find_msg(Locale * lp, const char *msg, int *lenp);
static unsigned long hash_string(const char *str);
static int cmp_Locale(void *p1, void *p2);
static Locale *find_locale(char *module);
static void add_locale(char *module, char *filename);

static Coll locales = { 0, 0, 0, 0, delete_Locale, cmp_Locale };

static Coll locale_names = { 0, 0, 0, 0, free, 0 };

/* [ publics */

void
_clip_add_locale(char *locale)
{
	if (strcasecmp(locale, "C") && strcasecmp(locale, "POSIX"))
		setlocale(LC_ALL, locale);
	_clip_logg(3, "add locale '%s'", locale);
	append_Coll(&locale_names, strdup(locale));
}

char *
_clip_gettext(const char *msg)
{
	Locale *locale;

	locale = find_locale("cliprt");

	if (locale)
	{
		char *s;

		s = find_msg(locale, msg, 0);
		if (s)
		{
			if (locale->charset && strcasecmp(locale->charset, _clip_hostcs))
			{
				static char buf[1024];

				int l = sizeof(buf) - 1;
				buf[l] = 0;
				_clip_translate_charset(locale->charset, _clip_hostcs, (unsigned char *)s, (unsigned char *)buf, l);
				_clip_logg(3, "localed msg: %s -> %s: %.*s -> %.*s",
					   locale->charset, _clip_hostcs, l, s, l, buf);
				return buf;
			}
			else
				return s;
		}
		else
			return (char*)msg;
	}
	else
		return (char *)msg;
}


void
_clip_locale_msg(char *module, char *msg, char **dst)
{
	Locale *locale;

	locale = find_locale(module);

	if (locale)
	{
		char *s;

		s = find_msg(locale, msg, 0);
		if (s)
		{
			_clip_logg(4, "locale msg: %s -> %s: %s", locale->charset, _clip_hostcs, s);
			if (locale->charset && strcasecmp(locale->charset, _clip_hostcs))
			{
				int l = strlen(s);

				*dst = (char *) malloc(l + 1);
				(*dst)[l] = 0;
				_clip_translate_charset(locale->charset, _clip_hostcs, (unsigned char *)s, (unsigned char *)(*dst), l);
				_clip_logg(3, "localed msg: %s -> %s: %.*s -> %.*s",
					   locale->charset, _clip_hostcs, l, s, l, *dst);
				return;
			}
			else
				msg = s;
		}
	}
	*dst = strdup(msg);
}

void
_clip_locale_msg_plural(char *module, char *msgid, char *msgid_plural, long n, char **dst)
{
	Locale *lp;

	lp = find_locale(module);

	if (lp)
	{
		int i, l;
		unsigned long int nn = 0;
		char *ep;
		char *sp = 0;

#ifdef PO_COMPAT
		{
			int l1, l2;
			char *buf;

			l1 = strlen(msgid);
			l2 = strlen(msgid_plural);
			buf = alloca(l1+l2+2);

			memcpy(buf, msgid, l1);
			memcpy(buf+l1+1, msgid_plural, l2);
			buf[l1] = PO_COMPAT_CHAR;
			buf[l1+l2+1] = 0;

			sp = find_msg(lp, buf, &l);
		}
#endif
		if (!sp)
			sp = find_msg(lp, msgid, &l);
		if (!sp)
			goto ret;

		if (!lp->pd)
		{
			if (n == 1)
				goto retok;
			sp = find_msg(lp, msgid_plural, &l);
			if (sp)
				goto retok;
			else
				goto ret;
		}

		nn = plural_eval(lp->pd, n);
		ep = sp + l;
		for (i = 0; i < lp->nplural && sp < ep; i++)
		{
			char *p;

#ifdef PO_COMPAT
			p = strchr(sp, PO_COMPAT_CHAR);
			if (!p)
			{
				l = strlen(sp);
				break;
			}
			else
			{
				l = p - sp;
				p++;
			}
#else
			p = sp + strlen(sp) + 1;
#endif
			if (p >= ep || i >= nn)
				break;
			sp = p;
		}

	retok:
		if (sp)
		{
#ifndef PO_COMPAT
			l = strlen(sp);
#endif
			_clip_logg(4, "locale msg plural(%ld:%lu): %s -> %s: %s", n, nn, lp->charset, _clip_hostcs, sp);
			if (lp->charset && strcasecmp(lp->charset, _clip_hostcs))
			{
				*dst = (char *) malloc(l + 1);
				(*dst)[l] = 0;
				_clip_translate_charset(lp->charset, _clip_hostcs, (unsigned char *)sp, (unsigned char *)(*dst), l);
				_clip_logg(3, "localed msg: %s -> %s: %.*s -> %.*s",
					   lp->charset, _clip_hostcs, l, sp, l, *dst);
			}
			else
			{
				*dst = (char *) malloc(l + 1);
				(*dst)[l] = 0;
				memcpy(*dst, sp, l);
			}
			return;
		}
	}

ret:
	if (n == 1)
		*dst = strdup(msgid);
	else
		*dst = strdup(msgid_plural);
}

int
_clip_module_locale(char *module, char *filename)
{
	add_locale(module, filename);
	return 0;
}

/* publics ][ statics */

static Locale *
find_locale(char *module)
{
	int ind, i;
	Locale l, *lp;

	l.module = module;

	for (i = 0; i < 2; i++)
	{
		if (search_Coll(&locales, &l, &ind))
		{
			lp = (Locale *) locales.items[ind];
			if (lp->ok)
				return lp;
			else
				return 0;
		}
		add_locale(module, 0);
	}

	return 0;
}

static int
cmp_Locale(void *p1, void *p2)
{
	return strcmp(((Locale *) p1)->module, ((Locale *) p2)->module);
}

static int
m_access(const char *path, int mode)
{
	int r;
	struct stat st;

	if ((r = stat(path, &st)))
		return r;

	if (!S_ISREG(st.st_mode))
	{
		_clip_logg(2, "no access to file '%s': not a regular file", path);
		return 1;
	}

	r = access(path, mode);
	if (r)
		_clip_logg(2, "no access to file '%s': %s", path, strerror(errno));

	return r;
}

static void
add_locale(char *module, char *filename)
{
	Locale *lp;
	int fd, j, ind;
	int use_mmap = 0;
	size_t size;
	struct stat st;
	struct mo_file_header *data = (struct mo_file_header *) -1;
	nls_uint32 revision;
	char path[256];
	char loc[32];
	char *locale = "";
	char *charset = 0;

	lp = (Locale *) calloc(1, sizeof(Locale));
	lp->module = strdup(module);

	if (search_Coll(&locales, lp, &ind))
	{
		delete_Locale((Locale *) locales.items[ind]);
		locales.items[ind] = lp;
	}
	else if (!insert_Coll(&locales, lp))
	{
		delete_Locale(lp);
		return;
	}
	path[0] = 0;
	if (filename)
	{
		snprintf(path, sizeof(path), "%s", filename);
	}
	else
	{
		for (j = 0; j < locale_names.count; j++)
		{
			char *e;
			DIR *dir;

			if (charset)
			{
				free(charset);
				charset = 0;
			}
			locale = locale_names.items[j];
			snprintf(loc, sizeof(loc), "%s", locale);
			snprintf(path, sizeof(path), "%s/locale.mo/%s/%s.mo", CLIP_LOCALE_ROOT, loc, module);

			if (!m_access(path, R_OK))
			{
				e = strrchr(loc, '.');
				if (e)
					charset = strdup(e + 1);
				break;
			}

			e = strrchr(loc, '.');
			if (e)
				*e = 0;
			snprintf(path, sizeof(path), "%s/locale.mo/%s/%s.mo", CLIP_LOCALE_ROOT, loc, module);

			if (!m_access(path, R_OK))
			{
				if (e)
					charset = strdup(e + 1);
				break;
			}

			e = strrchr(loc, '_');
			if (e)
				*e = 0;

			snprintf(path, sizeof(path), "%s/locale.mo", CLIP_LOCALE_ROOT);
			dir = opendir(path);
			if (dir)
			{
				struct dirent *dp;
				int br = 0;

				while ((dp = readdir(dir)))
				{
					if (strncmp(dp->d_name, loc, strlen(loc)))
						continue;

					snprintf(path, sizeof(path), "%s/locale.mo/%s/%s.mo", CLIP_LOCALE_ROOT, dp->d_name, module);

					if (!m_access(path, R_OK))
					{
						br = 1;
						e = strrchr(dp->d_name, '.');
						if (e)
							charset = strdup(e + 1);
						break;
					}
				}
				closedir(dir);
				if (br)
					break;
				else
					path[0] = 0;
			}
		}
	}

	lp->name = strdup(locale);
	if (*path == 0)
	{
		_clip_logg(2, "locale settings not found for module '%s'", module);
		return;
	}
	if (m_access(path, R_OK))
	{
		_clip_logg(2, "no access to locale file '%s': %s", path, strerror(errno));
		return;
	}

	fd = open(path, O_RDONLY | O_BINARY);
	if (fd < 0)
	{
		_clip_logg(1, "cannot open locale file '%s': %s", path, strerror(errno));
		return;
	}
	else
		_clip_logg(3, "open locale file '%s', charset '%s'", path, charset);

	if (fstat(fd, &st) != 0 || ((size = st.st_size) < sizeof(struct mo_file_header)))
	{
		close(fd);
		_clip_logg(1, "cannot stat locale file '%s': %s", path, strerror(errno));
		return;
	}

#ifndef NO_MMAP
	data = (struct mo_file_header *) mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (data != (struct mo_file_header *) -1)
	{
		/* mmap() call was successful.  */
#ifndef OS_CYGWIN
/* this f...ed win32 require(undocumented, of couse!) that fd must be keeped OPEN while use mmaped memory...*/
		close(fd);
		fd = -1;
#endif
		use_mmap = 1;
	}
#endif
	/* If the data is not yet available (i.e. mmap'ed) we try to load
	   it manually.  */

	if (data == (struct mo_file_header *) -1)
	{
		size_t to_read;
		char *read_ptr;

		data = (struct mo_file_header *) malloc(size);

		to_read = size;
		read_ptr = (char *) data;
		do
		{
			long int nb = (long int) read(fd, read_ptr, to_read);

			if (nb == -1)
			{
				close(fd);
				_clip_logg(1, "cannot read locale file '%s': %s", path, strerror(errno));
				return;
			}

			read_ptr += nb;
			to_read -= nb;
		}
		while (to_read > 0);

		close(fd);
		fd = -1;
	}

	/* Using the magic number we can test whether it really is a message
	   catalog file.  */
	if (data->magic != _MAGIC && data->magic != _MAGIC_SWAPPED)
	{
		/* The magic number is wrong: not a message catalog file.  */
#ifndef NO_MMAP
		if (use_mmap)
			munmap((caddr_t) data, size);
		else
#endif
			free(data);
		if (fd != -1)
			close(fd);
		_clip_logg(1, "wrong magic number of locale file '%s'", path);
		return;
	}

	lp->use_mmap = use_mmap;
	lp->mmap_size = size;
	lp->must_swap = data->magic != _MAGIC;
	lp->data = (char *) data;
	lp->fd = fd;
	lp->charset = charset;

	/* Fill in the information about the available tables.  */
	revision = W(lp->must_swap, data->revision);
	switch (revision)
	{
	case 0:
		lp->nstrings = W(lp->must_swap, data->nstrings);
		lp->orig_tab = (struct string_desc *) ((char *) data + W(lp->must_swap, data->orig_tab_offset));
		lp->trans_tab = (struct string_desc *) ((char *) data + W(lp->must_swap, data->trans_tab_offset));
		lp->hash_size = W(lp->must_swap, data->hash_tab_size);
		lp->hash_tab = (nls_uint32 *) ((char *) data + W(lp->must_swap, data->hash_tab_offset));
		break;
	default:
		_clip_logg(1, "illegal revision of locale file '%s': %d", path, revision);
#ifndef NO_MMAP
		if (use_mmap)
			munmap((caddr_t) data, size);
		else
#endif
			free(data);
		if (fd != -1)
			close(fd);
		free(lp);
		return;
	}

	lp->ok = 1;
	lp->nullentry = find_msg(lp, "", 0);

	if (lp->nullentry)
	{
		char *plural;
		char *nplurals;

		plural = strstr(lp->nullentry, "plural=");
		nplurals = strstr(lp->nullentry, "nplurals=");
		if (plural && nplurals)
		{
			char *endp;
			unsigned long int n;
			int l;

			/* First get the number.  */
			nplurals += 9;
			while (*nplurals != '\0' && isspace((unsigned char) *nplurals))
				++nplurals;
			if (!(*nplurals >= '0' && *nplurals <= '9'))
				goto no_plural;
			for (endp = nplurals, n = 0; *endp >= '0' && *endp <= '9'; endp++)
				n = n * 10 + (*endp - '0');
			if (nplurals == endp)
				goto no_plural;
			lp->nplural = n;

			plural += 7;
			l = strcspn(plural, ";\n\r");
			lp->pd = plural_new(plural, l);
		}
	      no_plural:
		charset = strstr(lp->nullentry, "charset=");
		if (charset)
			{
			int l;
			charset += 8;
			l = strcspn(charset, ";\n\r \t");
			if (lp->charset)
				free(lp->charset);
			lp->charset = (char*) malloc(l+1);
			memcpy(lp->charset, charset, l);
			lp->charset[l] = 0;
		}
	}

	if (lp->charset)
	{
		char *s;

		for (s = lp->charset; *s; s++)
			*s = tolower(*s);
	}
}

#if 0
static void
close_locales()
{
	destroy_Coll(&locales);
}
#endif

static void
delete_Locale(void *item)
{
	Locale *lp;

	if (!item)
		return;

	lp = (Locale *) item;

#ifndef NO_MMAP
	if (lp->use_mmap)
		munmap((caddr_t) lp->data, lp->mmap_size);
	else
#endif
		free(lp->data);
	if (lp->fd != -1)
		close(lp->fd);
	free(lp->name);
	free(lp->module);
	free(lp->charset);
	if (lp->pd)
		plural_delete(lp->pd);

	free(lp);
}

static nls_uint32
SWAP(nls_uint32 i)
{
	return (i << 24) | ((i & 0xff00) << 8) | ((i >> 8) & 0xff00) | (i >> 24);
}

static char *
find_msg(Locale * lp, const char *msgid, int *lenp)
{
	size_t top, act = 0, bottom;

	/* Locate the MSGID and its translation.  */
	if (lp->hash_size > 2 && lp->hash_tab != NULL)
	{
		/* Use the hashing table.  */
		nls_uint32 len = strlen(msgid);
		nls_uint32 hash_val = hash_string(msgid);
		nls_uint32 idx = hash_val % lp->hash_size;
		nls_uint32 incr = 1 + (hash_val % (lp->hash_size - 2));
		nls_uint32 nstr = W(lp->must_swap, lp->hash_tab[idx]);

		if (nstr == 0)
			/* Hash table entry is empty.  */
			return NULL;

		if (W(lp->must_swap, lp->orig_tab[nstr - 1].length) >= len
		    && strcmp(msgid, lp->data + W(lp->must_swap, lp->orig_tab[nstr - 1].offset)) == 0)
		{
			if (lenp)
				*lenp = W(lp->must_swap, lp->trans_tab[nstr - 1].length);
			return (char *) lp->data + W(lp->must_swap, lp->trans_tab[nstr - 1].offset);
		}

		while (1)
		{
			if (idx >= lp->hash_size - incr)
				idx -= lp->hash_size - incr;
			else
				idx += incr;

			nstr = W(lp->must_swap, lp->hash_tab[idx]);
			if (nstr == 0)
				/* Hash table entry is empty.  */
				return NULL;

			if (W(lp->must_swap, lp->orig_tab[nstr - 1].length) >= len
			    && strcmp(msgid, lp->data + W(lp->must_swap, lp->orig_tab[nstr - 1].offset)) == 0)
			{
				if (lenp)
					*lenp = W(lp->must_swap, lp->trans_tab[nstr - 1].length);
				return (char *) lp->data + W(lp->must_swap, lp->trans_tab[nstr - 1].offset);
			}
		}
		/* NOTREACHED */
	}

	/* Now we try the default method:  binary search in the sorted
	   array of messages.  */
	bottom = 0;
	top = lp->nstrings;
	while (bottom < top)
	{
		int cmp_val;

		act = (bottom + top) / 2;
		cmp_val = strcmp(msgid, lp->data + W(lp->must_swap, lp->orig_tab[act].offset));
		if (cmp_val < 0)
			top = act;
		else if (cmp_val > 0)
			bottom = act + 1;
		else
			break;
	}

	if (lenp && bottom < top)
		*lenp = W(lp->must_swap, lp->trans_tab[act].length);
	/* If an translation is found return this.  */
	return bottom >= top ? NULL : (char *) lp->data + W(lp->must_swap, lp->trans_tab[act].offset);
}

/* We assume to have `unsigned long int' value with at least 32 bits.  */
#define HASHWORDBITS 32

/* Defines the so called `hashpjw' function by P.J. Weinberger
   [see Aho/Sethi/Ullman, COMPILERS: Principles, Techniques and Tools,
   1986, 1987 Bell Telephone Laboratories, Inc.]  */

static unsigned long
hash_string(const char *str)
{
	unsigned long int hval, g;

	/* Compute the hash value for the given string.  */
	hval = 0;
	while (*str != '\0')
	{
		hval <<= 4;
		hval += (unsigned long) *str++;
		g = hval & ((unsigned long) 0xf << (HASHWORDBITS - 4));
		if (g != 0)
		{
			hval ^= g >> (HASHWORDBITS - 8);
			hval ^= g;
		}
	}
	return hval;
}

/* ][ */
