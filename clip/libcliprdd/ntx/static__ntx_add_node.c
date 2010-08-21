static int
_ntx_add_node(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, char *key, int recno, int child, const char *__PROC__)
{
   NTX_PAGE page;

   int no, count, er;

   unsigned int offs = ro->stack[ro->level].page;

   _ntx_page(ro, offs, &page);
   count = _rdd_ushort(page.nkeys);
   no = ro->stack[ro->level].pos;

   if (count < ro->fullpage)
   {
      return _ntx_put_node(ClipMachineMemory, rd, ro, &page, offs, key, recno, child, no, 0, 1, __PROC__);
   }
   else
   {
      return _ntx_split_node(ClipMachineMemory, rd, ro, key, recno, child, __PROC__);
   }
}
