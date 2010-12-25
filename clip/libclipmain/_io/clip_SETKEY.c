int
clip_SETKEY(ClipMachine * ClipMachineMemory)
{
   int key = _clip_parni(ClipMachineMemory, 1);

   ClipVar *bp = _clip_spar(ClipMachineMemory, 2);

   char buf[16];

   long hash;

   void *p;

   snprintf(buf, sizeof(buf), "setkey_%x", key);
   hash = _clip_hashstr(buf);

   p = _clip_fetch_item(ClipMachineMemory, hash);

   if (bp)
   {
      ClipVar *rp;

      if (p)
      {
	 rp = (ClipVar *) p;
	 _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), rp);
	 _clip_destroy(ClipMachineMemory, rp);
	 _clip_free_item(ClipMachineMemory, hash);
      }
      rp = (ClipVar *) calloc(sizeof(ClipVar), 1);

      _clip_clone(ClipMachineMemory, rp, bp);
      _clip_store_item(ClipMachineMemory, hash, rp);
   }
   else
   {
      if (p)
      {
	 ClipVar *rp = (ClipVar *) p;

	 _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), rp);
      }
   }

   return 0;
}
