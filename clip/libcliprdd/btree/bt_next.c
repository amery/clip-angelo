int
bt_next(BTREE * bt)
{
   u4 cur = bt->cur;

   if (!bt->cur)
      return 1;
   if (NODE(bt->cur)->right)
   {
      _bt_firstnode(bt, NODE(bt->cur)->right);
   }
   else
   {
      while (NODE(bt->cur)->parent && NODE(NODE(bt->cur)->parent)->right == bt->cur)

	 bt->cur = NODE(bt->cur)->parent;
      if (!NODE(bt->cur)->parent)
      {
	 bt->cur = cur;
	 return 1;
      }
      bt->cur = NODE(bt->cur)->parent;
   }
   return 0;
}
