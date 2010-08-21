static int
_bt_lastnode(BTREE * bt, u4 root)
{
   bt->cur = root;
   if (!bt->cur)
      return 1;
   while (NODE(bt->cur)->right)
      bt->cur = NODE(bt->cur)->right;
   return 0;
}
