CLIP_DLLEXPORT void
_clip_pop_fieldhash(ClipMachine * ClipMachineMemory, long *area, long *field)
{
   ClipVar *vp, *vpp;

   vpp = --(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);
   *area = -1;
   *field = -1;

   vp = _clip_vptr(vpp);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case CHARACTER_type_of_ClipVarType:
      {
	 int al = 0, fl = 0;

	 char *aname = 0, *fname = 0;

	 _clip_parse_name(ClipMachineMemory, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
			  vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, &aname, &al, &fname, &fl, 0, 0);
	 if (aname)
	    *area = _clip_casehashbytes(0, aname, al);
	 if (fname)
	    *field = _clip_casehashbytes(0, fname, fl);
      }
      break;
   case NUMERIC_type_of_ClipVarType:
      *field = _clip_double(vp);
      break;
   case OBJECT_type_of_ClipVarType:
      *field = vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->hash_of_ClipObjRtti(ClipMachineMemory, vp->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar, vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar);
      break;
   default:
      break;
   }

   _clip_destroy(ClipMachineMemory, vpp);
}
