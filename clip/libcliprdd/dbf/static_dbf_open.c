static int
dbf_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   DBF_HEADER hdr;

   DBF_FIELD field;

   int i;

   int offs = 1;

   char *s;

   int er;

   if ((er = rdd_read(ClipMachineMemory, &rd->file, 0, sizeof(DBF_HEADER), (char *) &hdr, __PROC__)))
      return er;
   rd->hdrsize = _rdd_ushort(hdr.hdrsize);
   rd->recsize = _rdd_ushort(hdr.recsize);
   rd->record = calloc(1, rd->recsize + 1);
   rd->recno = 0;
   rd->bof = rd->v_bof = 0;
   rd->eof = 0;

   if ((hdr.version != 0x03) && (hdr.version != 0x83) && (hdr.version != 0xF5) && (hdr.version != 0x30))
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badheader);

   rd->sig = hdr.version;

   if (rd->sig == 0x30)		/* VFP */
      rd->nfields = (rd->hdrsize - 263 - sizeof(DBF_HEADER)) / sizeof(DBF_FIELD);
   else
      rd->nfields = (rd->hdrsize - sizeof(DBF_HEADER)) / sizeof(DBF_FIELD);
   rd->fields = (RDD_FIELD *) calloc(rd->nfields, sizeof(RDD_FIELD));
   for (i = 0; i < rd->nfields; i++)
   {
      if ((er = rdd_read(ClipMachineMemory, &rd->file, sizeof(DBF_HEADER) + sizeof(DBF_FIELD) * i, sizeof(DBF_FIELD), (char *) &field, __PROC__)))
	 return er;
      if (field.type == '0')
      {
	 rd->nulloffs = offs;
	 rd->nfields--;
	 rd->fields = (RDD_FIELD *) realloc(rd->fields, sizeof(RDD_FIELD) * rd->nfields);
	 break;
      }
      rd->fields[i].type = field.type;
      strcpy(rd->fields[i].name, field.name);
      s = rd->fields[i].name + strlen(rd->fields[i].name);
      while (((--s) >= rd->fields[i].name) && *s == ' ')
	 *s = 0;
      loc_read(rd->loc, (unsigned char *) (rd->fields[i].name), strlen(rd->fields[i].name));
      if (strchr("NBFYX", field.type))
      {
	 if (field.type == 'X')
	    rd->x = 1;
	 rd->fields[i].len = field.len.num.len;
	 rd->fields[i].dec = field.len.num.dec;
      }
      else if (field.type == 'C')
      {
	 rd->fields[i].len = _rdd_ushort((unsigned char *) (field.len.len));
	 rd->fields[i].dec = 0;
      }
      else
      {
	 rd->fields[i].len = field.len.num.len;
	 rd->fields[i].dec = 0;
      }
      if (rd->sig == 0x30)
      {				/* VFP */
	 rd->fields[i].binary = (field.flags & 0x04) != 0;
	 if (rd->fields[i].type == 'P')
	    rd->fields[i].binary = 1;
	 rd->fields[i].nullable = (field.flags & 0x02) != 0;
      }
      if (strchr("PG", rd->fields[i].type))
	 rd->fields[i].binary = 1;
      rd->fields[i].offs = offs;
      offs += rd->fields[i].len;
      if (!HashTable_insert(rd->hashes, &rd->fields[i], _clip_casehashword(rd->fields[i].name, strlen(rd->fields[i].name))))
      {
	 return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badheader);
      }
   }
   return 0;
}
