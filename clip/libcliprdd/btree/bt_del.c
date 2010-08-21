int
bt_del(BTREE * bt)
{
   int l = 0, r = 0;

   u4 cur = bt->cur;

   int isroot = (cur == bt->root);

   u4 newroot, pn;

   int left = 0;

   if (!cur)
      return 1;

   pn = NODE(cur)->parent;
   if (!NODE(cur)->left && !NODE(cur)->right)
   {
      if (NODE(cur)->parent)
      {
	 if (NODE(NODE(cur)->parent)->left == cur)
	    NODE(NODE(cur)->parent)->left = 0;
	 else
	    NODE(NODE(cur)->parent)->right = 0;
	 _bt_adjust(bt, NODE(cur)->parent);
      }
      newroot = 0;
   }
   else if (NODE(cur)->left && !NODE(cur)->right)
   {
      unsigned int t2 = cur;

      unsigned int t1 = NODE(t2)->left;

      unsigned int t3 = NODE(t2)->parent;

      NODE(t1)->parent = t3;
      if (t3)
	 NODE(t3)->left = t1;
      newroot = t1;
   }
   else if (NODE(cur)->right && !NODE(cur)->left)
   {
      unsigned int t2 = cur;

      unsigned int t3 = NODE(t2)->right;

      unsigned int t1 = NODE(t2)->parent;

      NODE(t3)->parent = t1;
      if (t1)
	 NODE(t1)->right = t3;
      newroot = t3;
   }
   else
   {
      if (NODE(bt->cur)->left)
	 l = _bt_right_size(bt, NODE(cur)->left) + 1;
      if (NODE(bt->cur)->right)
	 r = _bt_left_size(bt, NODE(cur)->right) + 1;

      if (l >= r)
      {
	 if (l == 1)
	 {
	    unsigned int t2 = cur;

	    unsigned int t1 = NODE(cur)->left;

	    unsigned int t3 = NODE(cur)->right;

	    unsigned int t = NODE(cur)->parent;

	    if (t)
	       left = (NODE(t)->left == t2);
	    NODE(t1)->parent = t;
	    NODE(t1)->right = t3;
	    if (t3)
	       NODE(t3)->parent = t1;
	    if (t)
	    {
	       if (left)
		  NODE(t)->left = t1;
	       else
		  NODE(t)->right = t1;
	    }
	    _bt_adjust(bt, t1);
	    newroot = t1;
	 }
	 else
	 {
	    unsigned int t8 = cur, t7, t4, t6, t12, t;

	    _bt_lastnode(bt, NODE(cur)->left);
	    t7 = bt->cur;
	    t4 = NODE(t8)->left;
	    t12 = NODE(t8)->right;
	    t6 = NODE(t7)->parent;
	    t = NODE(cur)->parent;

	    if (t)
	       left = (NODE(t)->left == t8);
	    NODE(t7)->left = t4;
	    NODE(t7)->right = t12;
	    NODE(t7)->parent = NODE(t8)->parent;
	    NODE(t6)->right = 0;
	    NODE(t4)->parent = t7;
	    NODE(t12)->parent = t7;
	    if (t)
	    {
	       if (left)
		  NODE(t)->left = t7;
	       else
		  NODE(t)->right = t7;
	    }
	    _bt_adjust(bt, t6);
	    newroot = t7;
	 }
      }
      else
      {
	 if (r == 1)
	 {
	    unsigned int t2 = cur;

	    unsigned int t1 = NODE(cur)->left;

	    unsigned int t3 = NODE(cur)->right;

	    unsigned int t = NODE(cur)->parent;

	    if (t)
	       left = (NODE(t)->left == t2);
	    NODE(t3)->parent = NODE(t2)->parent;
	    NODE(t3)->left = t1;
	    if (t1)
	       NODE(t1)->parent = t3;
	    if (t)
	    {
	       if (left)
		  NODE(t)->left = t1;
	       else
		  NODE(t)->right = t1;
	    }
	    _bt_adjust(bt, t1);
	    newroot = t3;
	 }
	 else
	 {
	    unsigned int t8 = cur, t9, t4, t10, t12, t;

	    _bt_firstnode(bt, NODE(cur)->right);
	    t9 = bt->cur;
	    t12 = NODE(t8)->right;
	    t4 = NODE(t8)->left;
	    t10 = NODE(t9)->parent;
	    t = NODE(cur)->parent;

	    if (t)
	       left = (NODE(t)->left == t8);

	    NODE(t9)->left = t4;
	    NODE(t9)->right = t12;
	    NODE(t9)->parent = NODE(t8)->parent;
	    NODE(t10)->left = 0;
	    NODE(t12)->parent = t9;
	    NODE(t4)->parent = t9;
	    if (t)
	    {
	       if (left)
		  NODE(t)->left = t9;
	       else
		  NODE(t)->right = t9;
	    }
	    _bt_adjust(bt, t10);
	    newroot = t9;
	 }
      }
   }
   if (isroot)
      bt->root = newroot;
   _bt_free(bt, cur);
   while ((pn = _bt_balance(bt, pn)));
   bt->in--;
   return 0;
}
