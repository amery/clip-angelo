
int
bt_prev(BTREE * bt)
{
   u4 cur = bt->cur;

   if (!bt->cur)
      return 1;
   if (NODE(bt->cur)->left)
   {
      _bt_lastnode(bt, NODE(bt->cur)->left);
   }
   else
   {
      while (NODE(bt->cur)->parent && NODE(NODE(bt->cur)->parent)->left == bt->cur)

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
