static int
rm_checkscope(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int *map, int size, int oper, char *fname, ClipVar * val, int ic, const char *__PROC__)
{
   ClipVar key;

   unsigned int bytes = ((size + 1) >> 5) + 1;

   int i, b, bb, t, tt, fno;

   unsigned int oldrecno = rd->recno;

   int oldeof = rd->eof;

   int er;

   rd->eof = 0;
   memset(&key, 0, sizeof(ClipVar));
   fno = _rdd_fieldno(rd, _clip_casehashword(fname, strlen(fname)));
   for (i = 0; i < bytes; i++)
   {
      if (map[i])
      {
	 for (b = (i << 2), bb = 0; bb < 4; b++, bb++)
	 {
	    if (((char *) map)[b])
	    {
	       for (t = (b << 3) + 1, tt = 0; tt < 8; t++, tt++)
	       {
		  if (_rm_getbit(map, size, t))
		  {
		     if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, t, 0, __PROC__)))
			goto err;
		     if (rdd_takevalue(ClipMachineMemory, rd, fno, &key, __PROC__))
			goto err;
/*
							if(_clip_eval(ClipMachineMemory,block,0,NULL,&key)) goto err;
*/
		     if (rm_cmp(ClipMachineMemory, oper, _clip_vptr(&key), val, ic))
		     {
			_rm_clrbit(map, size, t);
		     }
		     _clip_destroy(ClipMachineMemory, &key);
		  }
	       }
	    }
	 }
      }
   }
   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, oldrecno, 1, __PROC__)))
      goto err;
   rd->eof = oldeof;
   return 0;
 err:
   return 1;
}
