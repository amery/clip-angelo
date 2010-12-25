CLIP_DLLEXPORT int
_clip_catstr(ClipMachine * ClipMachineMemory, int num)
{
   int i;

   char *s = 0, buf[128], *s1;

   int l = 0, l1, len, dec;

   ClipVar *rp;

   for (i = 0; i < num; i++)
   {
      ClipVar *vp = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - num + i);

      switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      default:
	 snprintf(buf, sizeof(buf), "NIL");
	 s1 = buf;
	 l1 = 3;
	 break;
      case CHARACTER_type_of_ClipVarType:
	 l1 = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 s1 = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 break;
      case NUMERIC_type_of_ClipVarType:
	 len = vp->ClipType_t_of_ClipVar.len_of_ClipType;
	 dec = vp->ClipType_t_of_ClipVar.dec_of_ClipType;
	 if (!len)
	 {
	    len = 10;
	    dec = 2;
	 }
	 if (!vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	 {
	    int c;

	    snprintf(buf, sizeof(buf), "%*.*f", len, dec, vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	    for (s1 = buf + strlen(buf); s1 > buf; --s1)
	       if ((*s1) == ',')
		  (*s1) = '.';
	    for (s1 = buf; (c = *s1) && c == ' '; s1++)
	       ;
	    l1 = strlen(s1);
	 }
	 else
	 {
	    if (len)
	       s1 = rational_toString(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, dec, 0);
	    else
	    {
	       s1 = rational_toString(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, len, dec, 0);
	       snprintf(buf, sizeof(buf), "%s", s1);
	       free(s1);
	    }
	    l1 = strlen(buf);
	 }
	 break;
      }

      {
	 s = (char *) realloc(s, l + l1 + 1);
	 memcpy(s + l, s1, l1);
	 s[l + l1] = 0;
	 l += l1;
      }

      _clip_destroy(ClipMachineMemory, ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - num + i);
   }

   ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame -= num;
   rp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;

   rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = s;
   rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = l;
   rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   rp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   rp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;

   ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame++;
   CLIP_CHECK_STACK;

   return 0;
}
