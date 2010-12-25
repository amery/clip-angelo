static void
_cdx_init_branch(RDD_ORDER * ro, CDX_BRANCH * branch, unsigned int left)
{
   memset(branch, 0, sizeof(CDX_BRANCH));
   _rdd_put_ushort(branch->attrs, (left == 0xffffffff) ? 1 : 0);
   _rdd_put_uint(branch->left, left);
   _rdd_put_uint(branch->right, 0xffffffff);
}
