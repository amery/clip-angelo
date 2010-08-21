static int
_bt_firstnode(BTREE * bt, u4 root)
{
   bt->cur = root;
   if (!bt->cur)
      return 1;
   while (NODE(bt->cur)->left)
      bt->cur = NODE(bt->cur)->left;
   return 0;
}
