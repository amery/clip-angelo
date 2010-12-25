static int
cdx_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   RDD_ORDER *tagbag = malloc(sizeof(RDD_ORDER));

   int out, i;

   CDX_HEADER *hdrs = NULL;

   char **tags = NULL;

   int *ros = NULL;

   unsigned int recno;

   int ntags = 0, er;

   if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, 0, tagbag, __PROC__)))
      goto err;

   if ((er = _cdx_first(ClipMachineMemory, tagbag, &out, __PROC__)))
      goto err;
   while (!out)
   {
      ntags++;
      ros = realloc(ros, sizeof(int) * ntags);
      hdrs = realloc(hdrs, sizeof(CDX_HEADER) * ntags);
      tags = realloc(tags, sizeof(char *) * ntags);
      tags[ntags - 1] = malloc(11);
      ros[ntags - 1] = -1;
      if ((er = _cdx_recno_keyval(ClipMachineMemory, tagbag, tagbag->level, &recno, tags[ntags - 1], __PROC__)))
	 goto err;
      for (i = 0; i < ri->norders; i++)
      {
	 if (ri->orders[i]->header == recno)
	 {
	    ros[ntags - 1] = i;
	    break;
	 }
      }
      if ((er = rdd_read(ClipMachineMemory, &ri->file, recno, sizeof(CDX_HEADER), &hdrs[ntags - 1], __PROC__)))
	 goto err;
      if ((er = _cdx_next(ClipMachineMemory, rd, tagbag, &out, NULL, __PROC__)))
	 goto err;
   }

   if ((er = rdd_trunc(ClipMachineMemory, &ri->file, sizeof(CDX_HEADER), __PROC__)))
      goto err;
   if ((er = _cdx_init_rootleaf(ClipMachineMemory, tagbag, sizeof(CDX_HEADER), __PROC__)))
      goto err;
   recno = 0;
   if ((er = rdd_write(ClipMachineMemory, &ri->file, 4, 4, &recno, __PROC__)))
      goto err;

   for (i = 0; i < ntags; i++)
   {
      ClipVar v;

      RDD_ORDER *ro = malloc(sizeof(RDD_ORDER));

      int j = 9;

      int oldrecno;

      unsigned int newroot;

      unsigned int dummy;

      v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
      v.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = tags[ros[i]];
      v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 10;
      while (tags[ros[i]][j] == ' ')
      {
	 v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf--;
	 tags[ros[i]][j--] = 0;
      }
      if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &recno, __PROC__)))
	 goto err;
      if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &dummy, __PROC__)))
	 goto err;
      oldrecno = rd->recno;
      rd->recno = recno;
      if ((er = cdx_addkey(ClipMachineMemory, rd, tagbag, &v, __PROC__)))
	 goto err;
      rd->recno = oldrecno;
      if ((er = rdd_write(ClipMachineMemory, &ri->file, recno, sizeof(CDX_HEADER), &hdrs[ros[i]], __PROC__)))
	 goto err;
      if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, recno, ro, __PROC__)))
	 goto err;
      if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &newroot, __PROC__)))
	 goto err;
      if ((er = _cdx_init_rootleaf(ClipMachineMemory, ro, newroot, __PROC__)))
	 goto err;
//              if(ros[i]!=-1)
      ri->orders[i]->header = recno;
      destroy_rdd_order(ClipMachineMemory, ro);
   }
   if ((er = rdd_childs(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (tags)
   {
      for (i = 0; i < ntags; i++)
	 if (tags[i])
	    free(tags[i]);
      free(tags);
   }
   if (hdrs)
      free(hdrs);
   if (ros)
      free(ros);
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return 0;
 err:
   if (tags)
   {
      for (i = 0; i < ntags; i++)
	 if (tags[i])
	    free(tags[i]);
      free(tags);
   }
   if (hdrs)
      free(hdrs);
   if (ros)
      free(ros);
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return er;
}
