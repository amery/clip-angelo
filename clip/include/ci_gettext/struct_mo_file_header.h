struct mo_file_header
{
   /* The magic number.  */
	nls_uint32 magic_of_mo_file_header;
   /* The revision number of the file format.  */
	nls_uint32 revision_of_mo_file_header;
   /* The number of strings pairs.  */
	nls_uint32 nstrings_of_mo_file_header;
   /* Offset of table with start offsets of original strings.  */
	nls_uint32 orig_tab_offset_of_mo_file_header;
   /* Offset of table with start offsets of translation strings.  */
	nls_uint32 trans_tab_offset_of_mo_file_header;
   /* Size of hashing table.  */
	nls_uint32 hash_tab_size_of_mo_file_header;
   /* Offset of first hashing entry.  */
	nls_uint32 hash_tab_offset_of_mo_file_header;
};
