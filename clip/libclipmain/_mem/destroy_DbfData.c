void
destroy_DbfData(DbfData * vp)
{
   if (!vp)
      return;
   switch (vp->type)
   {
   case 'C':
   case 'M':
      if (!(vp->flags & DBFDATA_NONFREE))
	 free(vp->u.c.str);
      break;
   }
   vp->type = 'U';
   vp->flags = 0;
}
