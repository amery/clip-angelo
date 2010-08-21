int
clip_DBINFO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBINFO";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int cmd = _clip_parni(ClipMachineMemory, 1);

   int er;

   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   switch (cmd)
   {
   case DBI_ALIAS:
      {
	 _clip_retc(ClipMachineMemory, wa->alias);
	 break;
      }
   case DBI_GETDELIMITER:
   case DBI_SETDELIMITER:
   case DBI_ISDBF:
   case DBI_VALIDBUFFER:
   case DBI_LOCKOFFSET:
      {
	 if ((er = wa->rd->vtbl->info(ClipMachineMemory, wa->rd, cmd, __PROC__)))
	    goto err;
	 break;
      }
   case DBI_BLOB_DIRECT_LEN:
   case DBI_BLOB_DIRECT_TYPE:
   case DBI_BLOB_INTEGRITY:
   case DBI_BLOB_OFFSET:
   case DBI_BLOB_RECOVER:
   case DBI_MEMOBLOCKSIZE:
      {
	 if (wa->rd->memo)
	    if ((er = wa->rd->memo->vtbl->info(ClipMachineMemory, wa->rd, wa->rd->memo, cmd, __PROC__)))
	       goto err;
	 break;
      }
   case DBI_BOF:
      {
	 int bof;

	 if ((er = rdd_bof(ClipMachineMemory, wa->rd, &bof, __PROC__)))
	    goto err;
	 _clip_retl(ClipMachineMemory, bof);
	 break;
      }
   case DBI_CANPUTREC:
      {
	 _clip_retl(ClipMachineMemory, !wa->rd->readonly);
	 break;
      }
   case DBI_CHILDCOUNT:
      {
	 _clip_retni(ClipMachineMemory, wa->rd->rels_opened);
	 break;
      }
   case DBI_DB_VERSION:
   case DBI_RDD_VERSION:
      {
	 char ver[101];

	 int opt = _clip_parni(ClipMachineMemory, 2);

	 if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType && opt == 1)
	 {
	    snprintf(ver, sizeof(ver), "%d.%d (%s)", HIWORD(wa->rd->vtbl->version), LOWORD(wa->rd->vtbl->version), wa->rd->vtbl->desc);
	 }
	 else
	 {
	    snprintf(ver, sizeof(ver), "%d.%d", HIWORD(wa->rd->vtbl->version), LOWORD(wa->rd->vtbl->version));
	 }
	 _clip_retc(ClipMachineMemory, ver);
	 break;
      }
   case DBI_DBFILTER:
      {
	 _clip_retc(ClipMachineMemory, "");
	 if (wa->rd->filter && wa->rd->filter->sfilter)
	    _clip_retc(ClipMachineMemory, wa->rd->filter->sfilter);
	 break;
      }
   case DBI_DBFILTER_BLOCK:
      {
	 ClipVar *ret = RETPTR(ClipMachineMemory);

	 _clip_dup(ClipMachineMemory, ret, &wa->rd->filter->fps->bfilter);
	 break;
      }
   case DBI_EOF:
      {
	 int eof;

	 if ((er = rdd_eof(ClipMachineMemory, wa->rd, &eof, __PROC__)))
	    goto err;
	 _clip_retl(ClipMachineMemory, eof);
	 break;
      }
   case DBI_FCOUNT:
      {
	 _clip_retni(ClipMachineMemory, wa->rd->nfields);
	 break;
      }
   case DBI_FILEHANDLE:
      {
	 _clip_retni(ClipMachineMemory, -1);
	 break;
      }
   case DBI_FOUND:
      {
	 _clip_retl(ClipMachineMemory, wa->found);
	 break;
      }
   case DBI_FULLPATH:
      {
	 _clip_retc(ClipMachineMemory, wa->rd->path);
	 break;
      }
   case DBI_GETHEADERSIZE:
      {
	 _clip_retni(ClipMachineMemory, wa->rd->hdrsize);
	 break;
      }
   case DBI_GETLOCKARRAY:
      {
	 return clip_DBRLOCKLIST(ClipMachineMemory);
      }
   case DBI_GETRECSIZE:
      {
	 _clip_retni(ClipMachineMemory, wa->rd->recsize);
	 break;
      }
   case DBI_ISFLOCK:
      {
	 _clip_retl(ClipMachineMemory, wa->rd->flocked);
	 break;
      }
   case DBI_LASTUPDATE:
      {
	 return clip_LUPDATE(ClipMachineMemory);
      }
   case DBI_LOCKCOUNT:
      {
	 _clip_retni(ClipMachineMemory, wa->rd->nlocks);
	 break;
      }
   case DBI_MEMOEXT:
      {
	 return clip_MEMOEXT(ClipMachineMemory);
      }
   case DBI_MEMOHANDLE:
      {
	 _clip_retni(ClipMachineMemory, -1);
	 break;
      }
   case DBI_SHARED:
      {
	 _clip_retl(ClipMachineMemory, wa->rd->shared);
	 break;
      }
   case DBI_TABLEEXT:
      {
	 return clip_TABLEEXT(ClipMachineMemory);
      }
   }
   return 0;
 err:
   return er;
}
