static int
_bt_left_size(BTREE * bt, u4 bn)
{
   int s = 0;

   while (NODE(bn)->left)
   {
      bn = NODE(bn)->left;
      s++;
   }
   return s;
}
