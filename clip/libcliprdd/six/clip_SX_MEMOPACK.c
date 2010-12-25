int
clip_SX_MEMOPACK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_MEMOPACK";

   int bsize = _clip_parni(ClipMachineMemory, 1);

   ClipVar *block = _clip_spar(ClipMachineMemory, 2);

   int step = _clip_parni(ClipMachineMemory, 3);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   char tmp[PATH_MAX];

   char *s;

   int tmpfd;

   struct stat st;

   int r, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(3, NUMERIC_type_of_ClipVarType);

   if ((er = rdd_event(ClipMachineMemory, EVENT_PREMEMOPACK, wa->rd->area, 0, NULL, &r, __PROC__)))
      return er;
   if (!r)
      return 0;

   if (!wa->rd->memo)
      return 0;
   if (!wa->rd->memo->vtbl->pack)
      return 0;

   if (wa->rd->shared)
   {
      er = rdd_err(ClipMachineMemory, EG_SHARED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
      goto err;
   }
   if (wa->rd->readonly)
   {
      er = rdd_err(ClipMachineMemory, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
      goto err;
   }

   if ((er = rdd_checkifnew(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;

#ifdef HAVE_MMAN_H
   if (wa->rd->memo->file.md != (caddr_t) - 1)
   {
      if (munmap(wa->rd->memo->file.md, wa->rd->memo->file.mapsize) == -1)
	 goto err_write;
   }
#endif

   if (_clip_close(ClipMachineMemory, wa->rd->memo->file.filehash, wa->rd->memo->file.fd) == -1)
      goto err_write;

   strcpy(tmp, wa->rd->memo->path);
   s = strrchr(tmp, '/');
   sprintf(s + 1, "memo%lx", (long) getpid());
   sprintf(s + 9, ".%lx", (long) random());
   s[13] = 0;

   if (rename(wa->rd->memo->path, tmp) == -1)
      goto err_write;

   wa->rd->memo->file.fd = _clip_creat(ClipMachineMemory, wa->rd->memo->path, O_RDWR, ClipMachineMemory->fileCreateMode, !wa->rd->shared);
   if (wa->rd->memo->file.fd == -1)
      goto err_open1;

   if ((er = rdd_open(ClipMachineMemory, tmp, 1, 1, &tmpfd, __PROC__)))
      goto err;

   if (wa->rd->memo->vtbl->pack)
   {
      WRITELOCK;
      if ((er = wa->rd->memo->vtbl->pack(ClipMachineMemory, wa->rd, wa->rd->memo, tmpfd, bsize, block, step, __PROC__)))
	 goto err_unlock;
      UNLOCK;
   }
   if (_clip_close(ClipMachineMemory, _clip_hashstr(tmp), tmpfd) == -1)
      goto err_write;

   if (fstat(wa->rd->memo->file.fd, &st) == -1)
      goto err_write;
#ifdef HAVE_MMAN_H
   wa->rd->memo->file.mapsize = st.st_size;
   wa->rd->memo->file.md = (caddr_t) mmap(0, wa->rd->memo->file.mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, wa->rd->memo->file.fd, 0);
#endif
   remove(tmp);

   if ((er = rdd_event(ClipMachineMemory, EVENT_POSTMEMOPACK, wa->rd->area, 0, NULL, NULL, __PROC__)))
      goto err;
   return 0;
 err_write:
   er = rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, wa->rd->memo->path);
   goto err;
 err_open1:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, wa->rd->memo->path);
   goto err;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
