static int
_flex_removestick(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * child, int size, const char *__PROC__)
{
   FLEX_TREE *parent = child->parent;

   int er;

   if (parent)
   {
      if (parent->count == 1)
      {
	 rm->flex_removed = realloc(rm->flex_removed, (rm->flex_nremoved + 1) * sizeof(unsigned int));
	 rm->flex_removed[rm->flex_nremoved] = parent->offs;
	 rm->flex_nremoved++;

	 if ((er = _flex_removestick(ClipMachineMemory, rm, parent, size, __PROC__)))
	    return er;
      }
      else
      {
	 memmove(parent->items + parent->pos, parent->items + parent->pos + 1, sizeof(FLEX_ITEM) * (parent->count - parent->pos - 1));
	 parent->count--;
	 parent->items = (FLEX_ITEM *) realloc(parent->items, sizeof(FLEX_ITEM) * parent->count);
	 if ((er = _flex_savebranch(ClipMachineMemory, rm, parent, size, __PROC__)))
	    return er;
	 if (parent->pos == parent->count)
	 {
	    if ((er = _flex_setlastitem(ClipMachineMemory, rm, parent->parent, parent->items + (parent->count - 1), size, __PROC__)))
	       return er;
	 }
      }
   }
/*
	else {
		char buf[4];
		int pos = (size?(char*)&hdr->flexsize:(char*)&hdr->flexoffs)-(char*)hdr;

		_rdd_put_uint(buf,0);
		if((er = rdd_write(ClipMachineMemory,&rm->file,pos,4,buf,__PROC__)))
			return er;
	}
*/
   return 0;
}
