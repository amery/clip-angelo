static int
fpt_getvalue(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int id, ClipVar * vp, const char *__PROC__)
{
   int len, er;

   char buf[8];

   char *str, *s;

   if (!id)
   {
      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(1);
      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[0] = 0;
      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 0;
      return 0;
   }
   if ((er = rdd_read(ClipMachineMemory, &rm->file, id * rm->blocksize, 8, buf, __PROC__)))
      return er;
   len = _rdd_backuint((unsigned char *) (buf + 4));
   str = malloc(len + 1);
   if ((er = rdd_read(ClipMachineMemory, &rm->file, id * rm->blocksize + 8, len, str, __PROC__)))
      return er;
   str[len] = 0;
   if (_rdd_backuint((unsigned char *) buf) == 3)
   {
      _clip_str2var(ClipMachineMemory, vp, str, len, 0);
      free(str);
   }
   else if (_rdd_backuint((unsigned char *) buf) == 0x8000)
   {
      s = str;
      _read_six_array(ClipMachineMemory, vp, rm->loc, &str, len);
      free(s);
   }
   else
   {
      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = str;
      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len;
      if (_rdd_backuint((unsigned char *) buf) == 1)
	 loc_read(rm->loc, (unsigned char *) vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
   }
   return 0;
}
