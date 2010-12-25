static int
_comp_uint(void *op, void *key1, void *key2, int *uniqfound)
{
   return *(u4 *) key1 - *(u4 *) key2;
}
