static void
delete_Locale(void *item)
{
   Locale *lp;

   if (!item)
      return;

   lp = (Locale *) item;

#ifndef NO_MMAP
   if (lp->use_mmap)
      munmap((caddr_t) lp->data, lp->mmap_size);
   else
#endif
      free(lp->data);
   if (lp->fd != -1)
      close(lp->fd);
   free(lp->name);
   free(lp->module);
   free(lp->charset);
   if (lp->pd)
      plural_delete(lp->pd);

   free(lp);
}
