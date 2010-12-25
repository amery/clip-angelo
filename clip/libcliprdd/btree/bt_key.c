void *
bt_key(BTREE * bt)
{
   return bt->cur ? (void *) ((uchar *) NODE(bt->cur) + sizeof(BT_NODE)) : NULL;
}
