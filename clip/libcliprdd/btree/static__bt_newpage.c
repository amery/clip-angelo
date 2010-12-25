/* --------------------------------------------------------- */
static void *
_bt_newpage(BTREE1 * bt)
{
   char *page = calloc(1, bt->pagesize);

   short offs = 2 * sizeof(short) + sizeof(void *) + bt->keysonpage * sizeof(short);

   int i;

   bt->memused++;
   for (i = 0; i < bt->keysonpage; i++)
   {
      *(short *) (page + 2 * sizeof(short) + sizeof(void *) + i * sizeof(short)) = offs;
      offs += bt->recsize;
   }
   return (void *) page;
}
