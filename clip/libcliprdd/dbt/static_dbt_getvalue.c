static int
dbt_getvalue(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int id, ClipVar * vp, const char *__PROC__)
{
   char buf[DBT_PAGE_SIZE + 1];

   char *pos;

   int len = 0;

   int count = 0;

   int fsize, er;

   vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(1);
   vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[0] = 0;
   vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 0;

   if (!id)
      return 0;

   if (rm->file.md == (caddr_t) - 1)
   {
      struct stat st;

      fstat(rm->file.fd, &st);
      fsize = st.st_size;
   }
   else
      fsize = rm->file.mapsize;

   while (1)
   {
      if ((id + count) * DBT_PAGE_SIZE + DBT_PAGE_SIZE > fsize)
      {
	 int toread = fsize - (id + count) * DBT_PAGE_SIZE;

	 if ((er = rdd_read(ClipMachineMemory, &rm->file, (id + count++) * DBT_PAGE_SIZE, toread, buf, __PROC__)))
	    return er;
	 buf[toread] = 0x1A;
	 buf[toread + 1] = 0x1A;
      }
      else
      {
	 if ((er = rdd_read(ClipMachineMemory, &rm->file, (id + count++) * DBT_PAGE_SIZE, DBT_PAGE_SIZE + 1, buf, __PROC__)))
	    return er;
      }
      if ((pos = memchr(buf, 0x1a, DBT_PAGE_SIZE)))
      {
/*			if(*(pos+1)==0x1a){*/
	 vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = realloc(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len + (pos - buf) + 1);
	 memcpy(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + len, buf, pos - buf);
	 vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len + (pos - buf);
	 loc_read(rm->loc, (unsigned char *) vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	 return 0;
/*			}*/
      }
      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = realloc(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len + DBT_PAGE_SIZE + 1);
      memcpy(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + len, buf, DBT_PAGE_SIZE);
      len += DBT_PAGE_SIZE;
   }
   return 0;
}
