int
bt1_destroy(BTREE1 * bt)
{
   _bt1_destroy(bt, bt->root);
   free(bt);
   return 0;
}
