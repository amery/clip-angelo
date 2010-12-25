static int
ctx_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__)
{
   CTX_HEADER hdr;

   int i, er;

   if ((er = rdd_read(ClipMachineMemory, &ri->file, 0, sizeof(CTX_HEADER), (char *) &hdr, __PROC__)))
      return er;
   if (_rdd_ushort(hdr.sig) != (unsigned short int) 0x9591)
   {
      char err[1024];

      snprintf(err, sizeof(err), _clip_gettext("Not CTX file (bad signature): %s"), ri->path);
      return rdd_err(ClipMachineMemory, EG_OPEN, 0, __FILE__, __LINE__, __PROC__, err);
   }
   if (!tag)
   {
      ri->orders = (RDD_ORDER **) malloc(sizeof(RDD_ORDER *) * hdr.ntags);
      for (i = 0; i < hdr.ntags; i++)
	 if ((er = _ctx_openorder(ClipMachineMemory, rd, ri, &hdr.tags[i], &ri->orders[i], __PROC__)))
	    return er;
   }
   else
   {
      ri->orders = (RDD_ORDER **) malloc(sizeof(RDD_ORDER *));
      for (i = 0; i < hdr.ntags; i++)
	 if (strcasecmp(tag, (const char *) hdr.tags[i].tagname) == 0)
	 {
	    if ((er = _ctx_openorder(ClipMachineMemory, rd, ri, &hdr.tags[i], &ri->orders[i], __PROC__)))
	       return er;
	    break;
	 }
   }
   return 0;
}
