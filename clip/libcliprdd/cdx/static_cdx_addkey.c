static int
cdx_addkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__)
{
   int r = __cdx_addkey(ClipMachineMemory, rd->area, ro, v, rd->loc, rd->recno, __PROC__);

   int er;

   char cntcdx[4];

   ro->valid_stack = 0;
   ro->curoffs = 0;
   if (!r)
   {
      if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
	 return er;
      ro->cntcdx = _rdd_backuint((unsigned char *) cntcdx) + 1;
      _rdd_put_backuint((unsigned char *) cntcdx, ro->cntcdx);
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
	 return er;
   }
   return r;
}
