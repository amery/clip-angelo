static int
ctx_create(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri,
	   RDD_ORDER ** rop, const char *tag, const char *expr, ClipVar * block, int unique, int first, unsigned int h, const char *__PROC__)
{
   CTX_HEADER chdr;

   int i, er;

   unsigned int header = 0;

   if (first)
   {
      memset(&chdr, 0, sizeof(CTX_HEADER));
      _rdd_put_ushort(chdr.sig, (unsigned short int) 0x9591);
      if ((er = rdd_write(ClipMachineMemory, &ri->file, 0, sizeof(CTX_HEADER), &chdr, __PROC__)))
	 return er;
      if (strcmp(rd->name, ri->name) == 0)
	 ri->structural = 1;
   }
   else
   {
      if ((er = rdd_read(ClipMachineMemory, &ri->file, 0, sizeof(CTX_HEADER), &chdr, __PROC__)))
	 return er;
   }
   for (i = 0; i < chdr.ntags; i++)
   {
      if (strcasecmp((const char *) chdr.tags[i].tagname, tag) == 0)
      {
	 if (ri->file.md == (caddr_t) - 1)
	 {
	    struct stat st;

	    fstat(ri->file.fd, &st);
	    _rdd_put_uint(chdr.tags[i].header, st.st_size);
	 }
	 else
	 {
	    _rdd_put_uint(chdr.tags[i].header, ri->file.mapsize);
	    header = _rdd_uint(chdr.tags[i].header);
	 }
	 break;
      }
   }
   if (i == chdr.ntags)
   {
      if (i == 62)
	 return rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, "Too many tags");
      strncpy((char *) chdr.tags[i].tagname, tag, MAX_TAG_LEN);
      chdr.tags[i].tagname[MAX_TAG_LEN] = 0;
      if (ri->file.md == (caddr_t) - 1)
      {
	 struct stat st;

	 fstat(ri->file.fd, &st);
	 _rdd_put_uint(chdr.tags[i].header, st.st_size);
      }
      else
      {
	 _rdd_put_uint(chdr.tags[i].header, ri->file.mapsize);
      }
      header = _rdd_uint(chdr.tags[i].header);
      chdr.ntags = i + 1;
   }
   if ((er = rdd_write(ClipMachineMemory, &ri->file, 0, sizeof(CTX_HEADER), &chdr, __PROC__)))
      return er;
   if ((er = ntx_create(ClipMachineMemory, rd, ri, rop, tag, expr, block, unique, 0, header, __PROC__)))
      return er;
   return 0;
}
