CLIP_DLLEXPORT int
_clip_load(ClipMachine * ClipMachineMemory, const char *name, struct Coll *names, ClipVar * resp)
{
   ClipFile *file = 0;

   int r = 0;

   ClipBlock b;

   ClipCodeVar c;

   long hash;

   char *e = strrchr(name, '.');

   char *s = strrchr(name, '/');

   c.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar = &b;
   if (e && (!s || s < e))
   {
      if (!strcmp(e, ".po") || !strcmp(e, ".pa"))
      {
	 file = NEW(ClipFile);
	 r = load_pobj(ClipMachineMemory, file, name, 0);
      }
      else if (!strcasecmp(e, DLLREALSUFF))
      {
	 r = load_dll(ClipMachineMemory, name, names, resp);
	 return r;
      }
      else
      {
	 _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "unknown file type: '%s'", name);
	 return _clip_call_errblock(ClipMachineMemory, 1);
      }
   }
   else
   {
      char path[256];

      snprintf(path, sizeof(path), "%s.po", name);
      if (!access(path, R_OK))
      {
	 file = NEW(ClipFile);
	 r = load_pobj(ClipMachineMemory, file, path, 0);
      }
      else
      {
	 snprintf(path, sizeof(path), "%s.pa", name);
	 if (!access(path, R_OK))
	 {
	    file = NEW(ClipFile);
	    r = load_pobj(ClipMachineMemory, file, path, 0);
	 }
	 else
	 {
	    snprintf(path, sizeof(path), "%s%s", name, DLLREALSUFF);
	    if (!access(path, R_OK))
	    {
	       r = load_dll(ClipMachineMemory, path, names, resp);
	       return r;
	    }
	    else
	    {
	       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "cannot load file: '%s'", name);
	       return _clip_call_errblock(ClipMachineMemory, 1);
	    }
	 }
      }
   }

   if (r)
      return r;

   ++file->refCount_of_ClipFile;

   for (r = _clip_first_File(file, &c, &hash); r; r = _clip_next_File(file, &c, &hash))
   {
      if (c.ClipType_type_of_ClipCodeVar.ClipVartype_type_of_ClipType == CCODE_type_of_ClipVarType)
      {
	 if (resp)
	 {
	    ClipVar *vp;

	    vp = NEW(ClipVar);

	    CLEAR_CLIPVAR(resp);
	    resp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	    resp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CCODE_type_of_ClipVarType;
	    resp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;

	    vp->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	    vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CCODE_type_of_ClipVarType;
	    vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar = c.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar;
	    ++file->refCount_of_ClipFile;
	    break;
	 }
	 else
	 {
	    if (_clip_register_hash(ClipMachineMemory, c.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar, hash))
	       ++file->refCount_of_ClipFile;
	 }
      }
      else if (c.ClipType_type_of_ClipCodeVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType)
      {
	 if (resp)
	 {
	    ClipVar *vp;

	    ClipBlock *bp;

	    vp = NEW(ClipVar);
	    bp = NEW(ClipBlock);

	    CLEAR_CLIPVAR(resp);
	    resp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	    resp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
	    resp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;

	    vp->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	    vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
	    vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    *bp = *c.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar;
	    vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar = bp;
	    ++file->refCount_of_ClipFile;
	    break;
	 }
	 else
	 {
	    if (_clip_register_block(ClipMachineMemory, *c.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar, hash))
	    {
	       char *name;

	       if (names)
	       {
		  name = get_proc_name(&c);
		  append_Coll(names, strdup(name));
	       }

	       ++file->refCount_of_ClipFile;
	    }
	 }
      }
   }

   _clip_load_inits(ClipMachineMemory, file);

   delete_ClipFile(ClipMachineMemory, file);

   return 0;
}
