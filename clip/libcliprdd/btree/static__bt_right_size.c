static int
_bt_right_size(BTREE * bt, u4 bn)
{
   int s = 0;

   while (NODE(bn)->right)
   {
      bn = NODE(bn)->right;
      s++;
   }
   return s;
}
