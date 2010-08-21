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

   int tr_inited;
#ifdef HAVE_ICONV
   iconv_t it;
#else
   unsigned char trans_tbl[256];
#endif
}
Locale;
