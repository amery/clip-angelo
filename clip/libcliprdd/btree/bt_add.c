int
bt_add(BTREE * bt, void *op, void *key)
{
   u4 bn;

   u4 *node = &bt->root;

   u4 parent = 0;

   int r, uniqfound;

   bn = _bt_getfuu(bt);
   bt->in++;
   if ((u4) bt->in == bt->count)
   {
      bt->count += bt->count >> 2;
      bt->data = realloc(bt->data, (bt->count + 1) * (bt->size + sizeof(BT_NODE)));
   }
   if (!bn)
      bn = bt->in * (bt->size + sizeof(BT_NODE));
   memset((char *) NODE(bn), 0, sizeof(BT_NODE));
   memcpy((char *) NODE(bn) + sizeof(BT_NODE), key, bt->size);
   while (*node)
   {
      parent = *node;
      r = bt->bt_compare(op, key, (void *) ((uchar *) NODE(*node) + sizeof(BT_NODE)), &uniqfound);
      if (bt->unique && !uniqfound)
	 return 0;
      if (r < 0)
	 node = &(NODE(*node)->left);
      else
	 node = &(NODE(*node)->right);
   }
   NODE(bn)->parent = parent;
   *node = bn;
   if (parent)
      _bt_adjust(bt, parent);
   while ((bn = _bt_balance(bt, bn)));
   return 0;

}
