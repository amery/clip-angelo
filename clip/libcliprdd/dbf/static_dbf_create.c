static int
dbf_create(ClipMachine * ClipMachineMemory, RDD_DATA_VTBL * vtbl, char *name, RDD_STRUCT * stru, int nfields, const char *__PROC__)
{
   RDD_FILE file;

   DBF_HEADER *hdr = malloc(sizeof(DBF_HEADER));

   char *chdr, *s;

   DBF_FIELD *fld;

   int i, offs;

   time_t ltm;

   struct tm *date;

   HashTable *hashes;

   int er;

   unsigned int recsize;

   memset(hdr, 0, sizeof(DBF_HEADER));
   hdr->version = vtbl->dbfsig;
   for (i = 0; i < nfields; i++)
      if (strchr("MPG", stru[i].type) || (stru[i].type == 'V' && stru[i].len != 3 && stru[i].len != 4))
      {
	 hdr->version = vtbl->dbfwithmemosig;
	 break;
      }
   ltm = time(NULL);
   date = gmtime(&ltm);
   hdr->yy = date->tm_year;
   hdr->mm = date->tm_mon + 1;
   hdr->dd = date->tm_mday;
   _rdd_put_uint(hdr->recs, 0);
   _rdd_put_ushort(hdr->hdrsize, sizeof(DBF_HEADER) + nfields * sizeof(DBF_FIELD) + 2);
   recsize = 1;
   hdr = realloc(hdr, _rdd_ushort(hdr->hdrsize) + 1);
   hashes = new_HashTable();
   offs = 1;
   for (i = 0; i < nfields; i++)
   {
      recsize += stru[i].len;
      fld = (DBF_FIELD *) ((char *) hdr + sizeof(DBF_HEADER) + i * sizeof(DBF_FIELD));
      memset(fld, 0, sizeof(DBF_FIELD));
      strncpy(fld->name, stru[i].name, 10);
      for (s = fld->name; *s && *s != ' '; s++);
      *s = 0;
      loc_write(dbf_get_locale(ClipMachineMemory), (unsigned char *) (fld->name), strlen(fld->name));
      fld->type = stru[i].type;
      if (stru[i].type == 'V' && stru[i].len != 3 && stru[i].len != 4 && stru[i].len < 6)
      {
	 er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badstructure);
	 goto err;
      }
      _rdd_put_uint(fld->offs, offs);
      offs += stru[i].len;
      if (fld->type == 'N')
      {
	 if (stru[i].dec >= stru[i].len - (stru[i].dec != 0))
	 {
	    er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badstructure);
	    goto err;
	 }
	 fld->len.num.len = stru[i].len;
	 fld->len.num.dec = stru[i].dec;
      }
      else if (fld->type == 'X')
      {
	 if (stru[i].len < 10 || stru[i].len > 127)
	 {
	    er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badstructure);
	    goto err;
	 }
	 fld->len.num.len = stru[i].len;
	 fld->len.num.dec = stru[i].dec;
      }
      else
      {
	 _rdd_put_ushort((unsigned char *) (fld->len.len), stru[i].len);
      }
      if (!HashTable_insert(hashes, fld, _clip_casehashword(fld->name, strlen(fld->name))))
      {
	 er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_fielduplicate);
	 goto err;
      }
   }
   if (recsize > 0xffff)
   {
      er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badstructure);
      goto err;
   }
   _rdd_put_ushort(hdr->recsize, recsize);
   delete_HashTable(hashes);

   chdr = (char *) hdr;
   chdr[_rdd_ushort(hdr->hdrsize) - 2] = 0x0D;
   chdr[_rdd_ushort(hdr->hdrsize) - 1] = 0x00;
   chdr[_rdd_ushort(hdr->hdrsize)] = 0x1A;

   memset(&file, 0, sizeof(RDD_FILE));
   file.md = (char *) -1;
   file.fd = _clip_creat(ClipMachineMemory, name, O_RDWR, ClipMachineMemory->fileCreateMode, 1);
   if (file.fd == -1)
      goto err_create;
   if ((er = rdd_write(ClipMachineMemory, &file, 0, _rdd_ushort(hdr->hdrsize) + 1, (char *) hdr, __PROC__)))
      goto err;
   if (_clip_close(ClipMachineMemory, _clip_hashstr(name), file.fd) == -1)
      goto err_create;

   free(hdr);
   return 0;
 err_create:
   er = rdd_err(ClipMachineMemory, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, name);
 err:
   free(hdr);
   return er;
}
