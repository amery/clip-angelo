typedef struct
{
	char *filename_of_PoFile;
	char *charset_of_PoFile;
	iconv_t iconv_t_cd_of_PoFile;
	Coll Coll_ids_of_PoFile;
}
PoFile;
