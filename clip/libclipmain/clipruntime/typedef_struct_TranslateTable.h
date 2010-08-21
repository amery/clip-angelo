typedef struct
{
   char *src;
   char *dst;
   unsigned char tbl[256];
   unsigned char *pg;		/*[128] */
}
TranslateTable;
