static ClipVar *
fetch_arr(ClipMachine * ClipMachineMemory, ClipVar * ap, int dim, long *vect, int Dim, long *Vect, int store, ClipVar ** mapp, long *hashp)
{
   long no;

   ClipVar *vp = NULL;

   ClipVarEl *vlp = 0;

   int isMap, type;

   int pdim;

   long *pvect;

   ClipVar *pap;

   if (!dim)
      return ap;

   type = _clip_type(ap);

   isMap = (type == MAP_type_of_ClipVarType ? 1 : 0);

   if (type == CHARACTER_type_of_ClipVarType)
   {
      long pos, len = 1, slen;

      char *buf;

      ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame, var;

      pos = _clip_hash(ClipMachineMemory, sp - dim);
      pos--;
      if (dim > 1)
	 len = _clip_hash(ClipMachineMemory, sp - dim + 1);
      if (len < 1)
	 len = 1;

      vp = _clip_vptr(ap);
      slen = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
      buf = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

      CLEAR_CLIPVAR(&var);
      if (pos < 0 || pos >= slen)
      {
	 pos = 0;
	 len = 0;
      }
      if (pos + len > slen)
	 len = slen - pos;
      _clip_var_str(buf + pos, len, &var);
      vp = _clip_add_tempvar(ClipMachineMemory, &var);
      _clip_destroy(ClipMachineMemory, &var);
      return vp;
   }

   if (type != ARRAY_type_of_ClipVarType && !isMap)
   {
      char buf[64];

      print_dim(ClipMachineMemory, buf, sizeof(buf), dim - 1, Dim, Vect);
      _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			ClipMachineMemory->fp->line_of_ClipFrame, "try fetch element from non-array object%s%s", buf[0] ? " " : "", buf);
      _clip_call_errblock(ClipMachineMemory, EG_BOUND);
      return NULL;
   }

   pap = ap;
   pvect = vect;
   pdim = dim;

   ap = _clip_vptr(ap);
   no = GETLONG(vect);

   ++vect;
   --dim;

   if (isMap)
   {
      int ind;

      int c = ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;

      if (search_map(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, c, no, &ind))
	 vlp = ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind;
      else if (store)
      {
	 /*addvar: */
	 ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar = (ClipVarEl *) realloc(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, (c + 1) * sizeof(ClipVarEl));
	 if (ind < c)
	    memmove(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind + 1, ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind, (c - ind /*-1*/ ) * sizeof(ClipVarEl));
	 ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar++;
	 vlp = ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind;
	 memset(vlp, 0, sizeof(ClipVarEl));
	 vlp->no_of_ClipVarEl = no;
      }
      else
      {
	 if ((ClipMachineMemory->flags1 & MAPERR_FLAG))
	 {
	    /*goto addvar; */
	    vp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;
	    memset(vp, 0, sizeof(ClipVar));
	    vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;
	    return vp;
	 }
	 else
	 {
	    char buf[64];

	    _clip_hash_name(ClipMachineMemory, no, buf, sizeof(buf));
	    if (test_maponerror(ClipMachineMemory, ap, buf))
	    {
	       _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame, ClipMachineMemory->fp->line_of_ClipFrame, "map fetch error: index %s", buf);
	       _clip_call_errblock(ClipMachineMemory, EG_BOUND);
	       return 0;
	    }
	    else
	    {
	       vp = fetch_arr(ClipMachineMemory, pap, pdim, pvect, Dim, Vect, store, mapp, hashp);
	       return vp;
	    }
	 }
      }

      if (store && !dim && ap->ClipType_t_of_ClipVar.memo_of_ClipType && mapp && hashp)
      {
	 *mapp = ap;
	 *hashp = no;
      }
   }
   else if (no < 1 || no > ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
   {
      char buf[64];

      print_dim(ClipMachineMemory, buf, sizeof(buf), dim, Dim, Vect);
      _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			ClipMachineMemory->fp->line_of_ClipFrame, "out-of-bounds array %s, index=%d, count=%d", buf, no, ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar);
      _clip_call_errblock(ClipMachineMemory, EG_BOUND);
      return NULL;
   }
   if (isMap)
   {
      vp = fetch_arr(ClipMachineMemory, &vlp->ClipVar_var_of_ClipVarEl, dim, vect, Dim, Vect, store, mapp, hashp);
   }
   else
      vp = fetch_arr(ClipMachineMemory, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + no - 1, dim, vect, Dim, Vect, store, mapp, hashp);

   return vp;
}
