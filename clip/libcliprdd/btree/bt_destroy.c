void
bt_destroy(BTREE * bt)
{
   if (bt)
   {
      if (bt->data)
	 free(bt->data);
      free(bt);
   }
}
