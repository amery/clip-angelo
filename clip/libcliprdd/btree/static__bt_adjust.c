static void
_bt_adjust(BTREE * bt, u4 node)
{
   if (NODE(node)->parent && NODE(NODE(node)->parent)->left == node && NODE(node)->right && !NODE(node)->left)
   {
      unsigned int t1 = node;

      unsigned int t5 = NODE(t1)->parent;

      unsigned int t3 = NODE(t1)->right;

      unsigned int t2;

      _bt_firstnode(bt, t3);
      t2 = bt->cur;

      NODE(t1)->parent = t2;
      NODE(t1)->right = 0;
      NODE(t3)->parent = t5;
      NODE(t5)->left = t3;
      NODE(t2)->left = t1;
   }
   else if (NODE(node)->parent && NODE(NODE(node)->parent)->right == node && NODE(node)->left && !NODE(node)->right)
   {
      unsigned int t9 = node;

      unsigned int t5 = NODE(t9)->parent;

      unsigned int t7 = NODE(t9)->left;

      unsigned int t8;

      _bt_lastnode(bt, t7);
      t8 = bt->cur;

      NODE(t9)->parent = t8;
      NODE(t9)->left = 0;
      NODE(t7)->parent = t5;
      NODE(t5)->right = t7;
      NODE(t8)->right = t9;
   }
}
