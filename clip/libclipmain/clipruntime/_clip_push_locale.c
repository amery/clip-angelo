CLIP_DLLEXPORT void
_clip_push_locale(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1);

   char *s = 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
   {
      char *e, *p, *m;

      int i;

      p = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
      for (i = 0, e = p; *e && *e != 1; e++, i++)
	 ;
      e++;
      m = (char *) malloc(i + 1);
      memcpy(m, p, i);
      m[i] = 0;

      _clip_locale_msg(m, e, &s);
#if 0
      for (e = s; *e; e++)
	 *e = _clip_intbl[*(unsigned char *) e];
#endif
      free(m);
   }
   else
      s = strdup("");

   vp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;
   _clip_destroy(ClipMachineMemory, vp);
   _clip_var_str(s, strlen(s), vp);
   free(s);
}
