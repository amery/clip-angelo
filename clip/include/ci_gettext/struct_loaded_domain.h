struct loaded_domain
{
   const char *data;
   int       use_mmap;
   size_t    mmap_size;
   int       must_swap;
   nls_uint32 nstrings;
   struct string_desc *orig_tab;
   struct string_desc *trans_tab;
   nls_uint32 hash_size;
   nls_uint32 *hash_tab;
};
