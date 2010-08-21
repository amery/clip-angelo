static void
_bt_free(BTREE * bt, u4 node)
{
   NODE(node)->parent = -(int) bt->fuu;
   bt->fuu = -(int) node;
}
