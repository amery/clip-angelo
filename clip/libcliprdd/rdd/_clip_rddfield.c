int
_clip_rddfield(ClipMachine * ClipMachineMemory, int h, int no)
{
   const char *__PROC__ = "_clip_rddfield";

   RDD_DATA *rd = (RDD_DATA *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RDD);

   ClipVar vv;

   int er;

   CLEAR_CLIPVAR(&vv);

   if (!rd)
   {
      rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_badinstance);
      return -1;
   }
   if ((er = rdd_takevalue(ClipMachineMemory, rd, no, &vv, __PROC__)))
      return er;
   _clip_push(ClipMachineMemory, &vv);
   _clip_destroy(ClipMachineMemory, &vv);
   return 0;
}
