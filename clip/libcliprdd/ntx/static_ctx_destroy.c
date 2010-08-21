static int
ctx_destroy(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   CTX_HEADER chdr;

   int i, er;

   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 0, sizeof(CTX_HEADER), &chdr, __PROC__)))
      return er;
   for (i = 0; i < chdr.ntags; i++)
   {
      if (strcasecmp((const char *) chdr.tags[i].tagname, ro->name) == 0)
      {
	 for (i++; i < chdr.ntags; i++)
	 {
	    memcpy(&chdr.tags[i - 1], &chdr.tags[i], sizeof(CTX_TAG));
	 }
	 memset(&chdr.tags[i - 1], 0, sizeof(CTX_TAG));
	 chdr.ntags--;
	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, 0, sizeof(CTX_HEADER), &chdr, __PROC__)))
	    return er;
	 break;
      }
   }
   return 0;
}
