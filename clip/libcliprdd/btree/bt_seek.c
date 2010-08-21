int
bt_seek(BTREE * bt, void *op, void *key)
{
   u4 *node = &bt->root;

   int r = 0;

   while (*node)
   {
      r = bt->bt_compare(op, key, (void *) ((uchar *) NODE(*node) + sizeof(BT_NODE)), NULL);
      bt->cur = *node;
      if (r == 0)
      {
	 if (!bt->unique)
	 {
	    while (!bt_prev(bt))
	    {
	       r = bt->bt_compare(op, key, bt_key(bt), NULL);
	       if (r)
	       {
		  bt_next(bt);
		  break;
	       }
	    }
	 }
	 return 1;
      }
      if (r < 0)
	 node = &(NODE(*node)->left);
      else
	 node = &(NODE(*node)->right);
   }
   if (r > 0)
      bt_next(bt);
   return 0;
}
