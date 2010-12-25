static u4
_bt_balance(BTREE * bt, u4 bn)
{
   int l, r;

   l = _bt_left_size(bt, bn);
   r = _bt_right_size(bt, bn);
   if (l - r > 1)
      _bt_left_balance(bt, bn);
   else if (r - l > 1)
      _bt_right_balance(bt, bn);

   return NODE(bn)->parent;
}
