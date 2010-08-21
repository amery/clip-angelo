int
clip_SX_BLOB2FILE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_FILE2BLOB";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   const char *filename = _clip_parc(ClipMachineMemory, 1);

   const char *fldname = _clip_parc(ClipMachineMemory, 2);

   char file[PATH_MAX];

   int fd, fno, er;

   ClipVar v;

   ClipMachineMemory->m6_error = 0;
   memset(&v, 0, sizeof(ClipVar));
   CHECKWA(wa);

   if (!wa->rd->memo)
      return rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_nomemo);

   for (fno = 0; fno < wa->rd->nfields; fno++)
   {
      if (strcasecmp(wa->rd->fields[fno].name, fldname) == 0)
	 break;
   }
   if (fno >= wa->rd->nfields)
      return rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_nofield);

   if (wa->rd->fields[fno].type != 'M')
      return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_notmemo);

   if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = wa->rd->vtbl->getvalue(ClipMachineMemory, wa->rd, fno, &v, __PROC__)))
      goto err_unlock;
   UNLOCK;

   _clip_translate_path(ClipMachineMemory, filename, file, sizeof(file));
   fd = _clip_creat(ClipMachineMemory, file, O_RDWR, ClipMachineMemory->fileCreateMode, 1);
   if (fd == -1)
      goto err_create;
   if (write(fd, v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf) == -1)
      goto err_create;
   if (_clip_close(ClipMachineMemory, _clip_hashstr(file), fd) == -1)
      goto err_create;

   _clip_destroy(ClipMachineMemory, &v);
   _clip_retl(ClipMachineMemory, 1);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
   goto err;
 err_create:
   er = rdd_err(ClipMachineMemory, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, file);
 err:
   _clip_destroy(ClipMachineMemory, &v);
   return er;
}
