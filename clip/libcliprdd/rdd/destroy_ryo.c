void
destroy_ryo(void *data)
{
   RDD_FILTER *fp = (RDD_FILTER *) data;

   if (fp)
   {
      if (fp->sfilter)
	 free(fp->sfilter);
      if (fp->rmap)
	 free(fp->rmap);
      if (fp->fps)
	 free(fp->fps);
      if (fp->list)
	 free(fp->list);
      free(fp);
   }
}
