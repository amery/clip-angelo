/* cFilename, bAdditive */
int
clip___MRESTORE(ClipMachine * ClipMachineMemory)
{
   char *path = _clip_parc(ClipMachineMemory, 1);

   /*
      int l_add  = _clip_parl(ClipMachineMemory, 2);
    */

   /*int isAdditive = _clip_parl(ClipMachineMemory, 1); */
   char buf[256], errbuf[256], p[256];

   int r, i;

   DbfData *vars = 0;

   char **names = 0;

   int nvars = 0;

   char *e, *s;

   strncpy(p, path, sizeof(p));
   s = strrchr(p, '/');
   e = strrchr(p, '\\');
   s = (s > e) ? s : e;
   e = strrchr(p, '.');
   if (!e || (s && e < s))
      strcat(p, ".mem");
   _clip_path(ClipMachineMemory, p, buf, sizeof(buf), 0);

   r = read_MemFile(buf, dbf_get_locale(ClipMachineMemory), &vars, &names, &nvars, errbuf, sizeof(errbuf));

   if (!r)
   {
      for (i = 0; i < nvars; i++)
      {
	 ClipVar *vp, tmp;

	 char *s;

	 long hash = _clip_casehashstr(names[i]);

	 s = strdup(names[i]);
	 HashTable_store(ClipMachineMemory->hashnames, s, hash);

	 vp = _clip_ref_public_noadd(ClipMachineMemory, hash);
	 if (!vp)
	    vp = _clip_ref_memvar(ClipMachineMemory, hash);

	 if (!vp)
	    continue;
#if 0				/* 2006/11/21 restore in getsys bug fixed */
	 _clip_destroy(ClipMachineMemory, vp);
	 dbf2clip(vars + i, vp);
#else
	 dbf2clip(vars + i, &tmp);
	 if (_clip_push(ClipMachineMemory, &tmp))
	    continue;
	 _clip_assign(ClipMachineMemory, vp);
#endif
      }
   }

   for (i = 0; i < nvars; i++)
   {
      destroy_DbfData(vars + i);
      free(names[i]);
   }

   free(names);
   free(vars);

   if (r)
   {
      _clip_trap_str(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame, ClipMachineMemory->fp->line_of_ClipFrame, errbuf);
      return EG_ARG;
   }

   return 0;
}
