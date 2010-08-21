static int
obj_call(struct ClipMachine *ClipMachineMemory, int argc, long hash, int call)
{
   ClipVar *op = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - argc);

   int ret = 0;

   int _argc = ClipMachineMemory->argc;

   ClipFrame *fp = ClipMachineMemory->fp;

   ClipVar *bp = ClipMachineMemory->bp;

   ClipFrame *inMacro = ClipMachineMemory->inMacro;

   ClipMachineMemory->inMacro = 0;
   switch (op->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case OBJECT_type_of_ClipVarType:
      ClipMachineMemory->argc = argc;
      ClipMachineMemory->bp = fp->ClipVar_sp_of_ClipFrame;

      switch (call)
      {
      case CALL_CALL:
	 ret =
	    op->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->call_of_ClipObjRtti(ClipMachineMemory, op->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar, op->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar, hash);
	 break;
      case CALL_GET:
	 ret =
	    op->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->get_of_ClipObjRtti(ClipMachineMemory, op->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar, op->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar, hash);
	 break;
      case CALL_SET:
	 ret =
	    op->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->set_of_ClipObjRtti(ClipMachineMemory, op->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar, op->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar, hash);
	 break;
      }

      ClipMachineMemory->argc = _argc;
      ClipMachineMemory->fp = fp;
      ClipMachineMemory->bp = bp;
      for (; argc; --argc)
      {
	 --(fp->ClipVar_sp_of_ClipFrame);
	 _clip_destroy(ClipMachineMemory, fp->ClipVar_sp_of_ClipFrame);
      }

      break;
   case MAP_type_of_ClipVarType:
      {
	 ClipVarEl *vlp;

	 ClipVar *vpp;

	 enum ClipVarType type;

	 vlp = get_map_el(&op->ClipMapVar_m_of_ClipVar, hash);
	 if (!vlp)
	 {
	    char buf[32];

	    _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
	    _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "no method %s", buf);
	    ret = EG_NOMETHOD;
	    break;
	 }
	 vpp = &vlp->ClipVar_var_of_ClipVarEl;
	 type = _clip_type(vpp);

	 if (type != CCODE_type_of_ClipVarType && type != PCODE_type_of_ClipVarType)
	 {
	    if (argc == 1)
	    {
	       ClipVar *rp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - argc - 1;

	       _clip_clone(ClipMachineMemory, rp, vpp);
	       --(fp->ClipVar_sp_of_ClipFrame);
	       _clip_destroy(ClipMachineMemory, fp->ClipVar_sp_of_ClipFrame);
	       ret = 0;
	    }
	    else
	    {
	       char buf[32];

	       _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
	       _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "method %s is not code", buf);
	       ret = EG_NOMETHOD;
	    }
	    break;
	 }

	 vpp = _clip_vptr(vpp);

	 if (type == CCODE_type_of_ClipVarType)
	    ret =
	       _clip_func(ClipMachineMemory, vpp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar, argc, 0,
			  vpp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar);
	 else
	    ret =
	       _clip_code_func(ClipMachineMemory, vpp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar, argc, 0,
			       vpp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar);

      }
      break;
   default:
      ret = _clip_func_hash(ClipMachineMemory, hash, argc, 0, 0);
      break;
   }

   ClipMachineMemory->inMacro = inMacro;
   if (ret)
      return _clip_call_errblock(ClipMachineMemory, ret);

   return ret;
}
