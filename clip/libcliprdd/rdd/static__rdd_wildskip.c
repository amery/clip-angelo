static int
_rdd_wildskip(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *pattern, int regular, const char *s, int *found, const char *__PROC__)
{
   RDD_ORDER *ro = rd->orders[rd->curord];

   int er;

   if (ro->vtbl->wildskip)
   {
      if ((er = ro->vtbl->wildskip(ClipMachineMemory, rd, ro, pattern, regular, s, NULL, found, __PROC__)))
	 return er;
   }
   else
   {
      ClipVar v, *vp;

      char *e;

      *found = 1;
      memset(&v, 0, sizeof(ClipVar));
      if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
	 return er;
      vp = _clip_vptr(&v);
      e = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
      while ((--e >= vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf) && *e == ' ');
      *(e + 1) = 0;
      while (!rd->eof
	     && (memcmp(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, pattern, s - pattern) == 0)
	     && (_clip_glob_match(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, pattern, 1) == -1))
      {

	 _clip_destroy(ClipMachineMemory, &v);
	 if ((er = rdd_skip(ClipMachineMemory, rd, 1, __PROC__)))
	    return er;
	 if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
	    return er;
	 vp = _clip_vptr(&v);
	 e = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 while ((--e >= vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf) && *e == ' ');
	 *(e + 1) = 0;
      }
      if (rd->eof || (memcmp(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, pattern, s - pattern) != 0))
	 *found = 0;

      _clip_destroy(ClipMachineMemory, &v);
   }
   return 0;
}
