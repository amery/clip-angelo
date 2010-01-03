CLIP_DLLEXPORT int
_clip_fm_iassign(ClipMachine * ClipMachineMemory, long hash)
{
   const char nm[] = "_clip_fm_iassign";

   int       r = 0;

   ClipVar  *sp = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1);

   DBWorkArea *wa;

   int       fno;

   if ((wa = cur_area(ClipMachineMemory)) && (fno = _rdd_fieldno(wa->rd, hash)) != -1)
    {
       r = rdd_setvalue(ClipMachineMemory, wa->rd, fno, sp, nm);
    }
   else
    {
       r = _clip_iassign(ClipMachineMemory, _clip_ref_memvar(ClipMachineMemory, hash));
    }
   return r;
}
