static int
_cdx_addroot(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int brpage,
	     void *key1, unsigned int rec1, unsigned int page1, void *key2, unsigned int rec2, unsigned int page2, const char *__PROC__)
{
   CDX_BRANCH root;

   char b[4];

   int er;

   memset(&root, 0, sizeof(CDX_BRANCH));

   _rdd_put_ushort(root.attrs, 1);
   _rdd_put_ushort(root.nkeys, 2);
   _rdd_put_uint(root.left, 0xffffffff);
   _rdd_put_uint(root.right, 0xffffffff);

   memcpy(root.keys, key1, ro->bufsize);
   _rdd_put_backuint(root.keys + ro->bufsize, rec1);
   _rdd_put_backuint(root.keys + ro->bufsize + 4, page1);

   memcpy(root.keys + ro->bufsize + 8, key2, ro->bufsize);
   _rdd_put_backuint(root.keys + (ro->bufsize + 8) + ro->bufsize, rec2);
   _rdd_put_backuint(root.keys + (ro->bufsize + 8) + ro->bufsize + 4, page2);

   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, brpage, sizeof(CDX_BRANCH), &root, __PROC__)))
      return er;

   _rdd_put_uint((unsigned char *) b, brpage);
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, ro->header, sizeof(unsigned int), b, __PROC__)))
      return er;
   return 0;
}
