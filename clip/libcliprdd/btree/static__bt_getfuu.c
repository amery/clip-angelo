static u4
_bt_getfuu(BTREE * bt)
{
   u4 r = -(int) bt->fuu;

   if (!r)
      return 0;
   bt->fuu = -(int) (NODE(r)->parent);
   return r;
}
