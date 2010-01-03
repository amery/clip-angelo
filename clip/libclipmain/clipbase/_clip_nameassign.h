CLIP_DLLEXPORT int
_clip_nameassign(ClipMachine * ClipMachineMemory, char *name, int l, ClipVar * vp, int fieldprec)
{
   int       r;

   char     *aname, *fname;

   int       al, fl;

   DBWorkArea *wa;

   char      nm[] = "_clip_nameassign";

   long     *dim = 0;

   int       ndim = 0;


   if (fieldprec)
    {
       wa = cur_area(ClipMachineMemory);
       if (wa)
	{
	   r = rdd_setvaluebn(ClipMachineMemory, wa->rd, name, vp, nm);
	   if (!r)
	      return 0;
	}
       else
	  return rdd_err(ClipMachineMemory, EG_NOALIAS, 0, __FILE__, __LINE__, nm, er_noalias);
    }

   r = _clip_parse_name(ClipMachineMemory, name, l, &aname, &al, &fname, &fl, &dim, &ndim);
   if (r == 2)
    {
       if (aname)
	  wa = get_area(ClipMachineMemory, _clip_casehashword(aname, al), 0, 0);
       else
	  wa = cur_area(ClipMachineMemory);
       if (wa && fname)
	{
	   r = rdd_setvaluebn(ClipMachineMemory, wa->rd, fname, vp, nm);
	   if (r)
	      return r;
	}
       else
	  return EG_NOALIAS;
    }
   else if (r == 3)
    {
       int       r = EG_NOVAR;

       ClipVar  *rp;

       rp = _clip_ref_memvar(ClipMachineMemory, _clip_casehashword(fname, fl));
       if (rp)
	  r = _clip_aset(ClipMachineMemory, rp, vp, ndim, dim);
       free(dim);

       return r;
    }
   else
    {
       ClipVar  *rp;

       rp = _clip_ref_memvar(ClipMachineMemory, _clip_casehashword(fname, fl));
       if (rp)
	{
	   if (_clip_mclone(ClipMachineMemory, rp, vp))
	      return EG_NOVAR;
	}
       else
	  return EG_NOVAR;

    }

   return 0;
}
