struct loaded_domain
{
	const char *data_of_loaded_domain;
	int use_mmap_of_loaded_domain;
	size_t mmap_size_of_loaded_domain;
	int must_swap_of_loaded_domain;
	nls_uint32 nstrings_of_loaded_domain;
	struct string_desc *orig_tab_of_loaded_domain;
	struct string_desc *trans_tab_of_loaded_domain;
	nls_uint32 hash_size_of_loaded_domain;
	nls_uint32 *hash_tab_of_loaded_domain;
};
