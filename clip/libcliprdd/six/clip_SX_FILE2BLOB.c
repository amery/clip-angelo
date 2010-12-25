int
clip_SX_FILE2BLOB(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_FILE2BLOB";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   const char *filename = _clip_parc(ClipMachineMemory, 1);

   const char *fldname = _clip_parc(ClipMachineMemory, 2);

   /*int action = _clip_parni(ClipMachineMemory,3); */
   char file[PATH_MAX];

   void *buf;

   int fd, fno, er;

   struct stat st;

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

   _clip_translate_path(ClipMachineMemory, filename, file, sizeof(file));
   if ((er = rdd_open(ClipMachineMemory, file, 1, 1, &fd, __PROC__)))
      goto err;

   if (fstat(fd, &st) == -1)
      goto err_open;
   buf = malloc(st.st_size);
   if (read(fd, buf, st.st_size) == -1)
      goto err_open;
   if (_clip_close(ClipMachineMemory, _clip_hashstr(file), fd) == -1)
      goto err_open;

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   v.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   v.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = buf;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = st.st_size;
   WRITELOCK;
   if ((er = wa->rd->vtbl->setvalue(ClipMachineMemory, wa->rd, fno, &v, 1, __PROC__)))
      goto err_unlock;
   UNLOCK;

   _clip_destroy(ClipMachineMemory, &v);
   _clip_retl(ClipMachineMemory, 1);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, file);
 err:
   _clip_destroy(ClipMachineMemory, &v);
   return er;
}
