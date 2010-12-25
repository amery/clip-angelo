static void
_bt_left_balance(BTREE * bt, u4 pn)
{
   u4 t2, t3, t4, t;

   int left = 0;

   u4 bn = NODE(pn)->left;

   t2 = pn;
   t4 = bn;
   t = NODE(t2)->parent;

   _bt_lastnode(bt, t4);
   t3 = bt->cur;

   if (t)
      left = (NODE(t)->left == t2);
   NODE(t2)->parent = t3;
   NODE(t2)->left = 0;
   NODE(t3)->right = t2;
   NODE(t4)->parent = t;
   if (t)
   {
      if (left)
	 NODE(t)->left = t4;
      else
	 NODE(t)->right = t4;
   }
   if (t2 == bt->root)
      bt->root = t4;
   return;
}
