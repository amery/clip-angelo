static int
_bt1_destroy(BTREE1 * bt, void *root)
{
   int i;

   if (LEFT(root, 0))
   {
      for (i = 0; i < NKEYS(root); i++)
      {
	 _bt1_destroy(bt, LEFT(root, i));
      }
   }
   free(root);
   return 0;
}
