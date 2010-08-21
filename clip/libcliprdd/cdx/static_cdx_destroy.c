static int
cdx_destroy(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_HEADER chdr;

   RDD_ORDER *tagbag = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));

   char tn[11];

   int found, i, er;

   tagbag->level = 0;
   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 0, sizeof(CDX_HEADER), &chdr, __PROC__)))
      goto err;
   if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ro->index, 0, tagbag, __PROC__)))
      goto err;
   strcpy(tn, ro->name);
   memset(tn + strlen(tn), 0x20, 10 - strlen(tn));
   tn[10] = 0;
   for (i = 0; i < 10; i++)
      tn[i] = toupper(tn[i]);
   tagbag->stack[0].page = _rdd_uint(chdr.root);
   if ((er = _cdx_search_tree(ClipMachineMemory, tagbag, tn, 10, 0, &found, NULL, __PROC__)))
      goto err;
   if (found)
   {
      if ((er = _cdx_delkey(ClipMachineMemory, tagbag, __PROC__)))
	 goto err;
   }
   free(tagbag);
   return 0;
 err:
   free(tagbag);
   return er;
}
