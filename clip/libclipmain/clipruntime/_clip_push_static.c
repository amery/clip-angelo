CLIP_DLLEXPORT int
_clip_push_static(ClipMachine * ClipMachineMemory, void *vpp)
{
   int r;

   r = _clip_push(ClipMachineMemory, vpp);
   if (!r)
   {
      ClipVar *vp;

      vp = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1);
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CCODE_type_of_ClipVarType || vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType)
      {
	 if (!vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar && ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame)
	 {
	    ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame->refcount_of_ClipVarFrame++;
	    vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar = ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame;
	 }
      }
   }
   return r;
}
