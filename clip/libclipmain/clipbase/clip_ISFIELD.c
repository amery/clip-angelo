int
clip_ISFIELD(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int l, ret = 0;

   char *name = _clip_parcl(ClipMachineMemory, 1, &l);

   char *aname, *fname;

   int al, fl;

   if (name)
   {
      if (wa)
      {
	 if (_rdd_fieldno(wa->rd, _clip_casehashword(name, l)) != -1)
	    ret = 1;
      }

      if (!ret)
      {
	 if (_clip_parse_name(ClipMachineMemory, name, l, &aname, &al, &fname, &fl, 0, 0) == 2)
	 {
	    if (aname)
	       wa = get_area(ClipMachineMemory, _clip_casehashword(aname, al), 0, 0);
	    else
	       wa = cur_area(ClipMachineMemory);
	    if (wa && fname)
	    {
	       if (_rdd_fieldno(wa->rd, _clip_casehashword(fname, fl)) != -1)
		  ret = 1;
	    }
	 }
      }
   }

   _clip_retl(ClipMachineMemory, ret);
   return 0;
}
