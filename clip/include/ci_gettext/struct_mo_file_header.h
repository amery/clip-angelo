struct mo_file_header
{
  /* The magic number.  */
   nls_uint32 magic;
  /* The revision number of the file format.  */
   nls_uint32 revision;
  /* The number of strings pairs.  */
   nls_uint32 nstrings;
  /* Offset of table with start offsets of original strings.  */
   nls_uint32 orig_tab_offset;
  /* Offset of table with start offsets of translation strings.  */
   nls_uint32 trans_tab_offset;
  /* Size of hashing table.  */
   nls_uint32 hash_tab_size;
  /* Offset of first hashing entry.  */
   nls_uint32 hash_tab_offset;
};
