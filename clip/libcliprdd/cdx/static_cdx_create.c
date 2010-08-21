static int
cdx_create(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri,
	   RDD_ORDER ** rop, const char *tag, const char *expr, ClipVar * block, int unique, int first, unsigned int header, const char *__PROC__)
{
   CDX_HEADER chdr;

   RDD_ORDER *tagbag = malloc(sizeof(RDD_ORDER));

   unsigned int root;

   ClipVar v;

   unsigned int newheader, dummy;

   char tagname[11];

   char tn[11];

   unsigned int oldrn;

   int found, er;

   char *e;

   tagbag->level = 0;
   memset(&v, 0, sizeof(ClipVar));
   ri->loc = rd->loc;
   if (first)
   {
      root = sizeof(CDX_HEADER);
      memset(&chdr, 0, sizeof(CDX_HEADER));
      _rdd_put_uint(chdr.root, sizeof(CDX_HEADER));
      _rdd_put_ushort(chdr.keysize, 10);
      chdr.options = 0xE0;
      chdr.sign = 1;
      _rdd_put_ushort(chdr.totlen, 1);
      _rdd_put_ushort(chdr.forlen, 1);
      _rdd_put_ushort(chdr.explen, 1);
      if ((er = rdd_write(ClipMachineMemory, &ri->file, 0, sizeof(CDX_HEADER), &chdr, __PROC__)))
	 goto err;
      if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &root, __PROC__)))
	 goto err;
      if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, 0, tagbag, __PROC__)))
	 goto err;
      if ((er = _cdx_init_rootleaf(ClipMachineMemory, tagbag, root, __PROC__)))
	 goto err;
      if (strcmp(rd->name, ri->name) == 0)
	 ri->structural = 1;
   }
   else
   {
      if ((er = rdd_read(ClipMachineMemory, &ri->file, 0, sizeof(CDX_HEADER), &chdr, __PROC__)))
	 goto err;
      if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, 0, tagbag, __PROC__)))
	 goto err;
   }

   tagbag->unique = 1;
   if (tag)
      strncpy(tagname, tag, 10);
   else
      strncpy(tagname, ri->name, 10);
   tagname[10] = 0;
   _clip_upstr(tagname, strlen(tagname));

   e = tagname + strlen(tagname);
   while (*(--e) == ' ');
   *(e + 1) = 0;
   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   v.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = tagname;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(tagname);
   if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &newheader, __PROC__)))
      goto err;
   if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &dummy, __PROC__)))
      goto err;

   strcpy(tn, tagname);
   memset(tn + strlen(tn), 0x20, 10 - strlen(tn));
   tn[10] = 0;
   tagbag->stack[0].page = _rdd_uint(chdr.root);
   if ((er = _cdx_search_tree(ClipMachineMemory, tagbag, tn, 10, 0, &found, NULL, __PROC__)))
      goto err;
   if (found)
   {
      if ((er = _cdx_delkey(ClipMachineMemory, tagbag, __PROC__)))
	 goto err;
   }
   oldrn = rd->recno;
   rd->recno = newheader;
   if ((er = cdx_addkey(ClipMachineMemory, rd, tagbag, &v, __PROC__)))
      goto err;
   rd->recno = oldrn;
   if ((er = _cdx_create(ClipMachineMemory, rd, ri, rop, tagname, expr, block, unique, newheader, __PROC__)))
      goto err;
   rd->recno = oldrn;

   destroy_rdd_order(ClipMachineMemory, tagbag);
   return 0;
 err:
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return er;
}
