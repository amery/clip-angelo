int
clip_DBRSELECT(ClipMachine * ClipMachineMemory)
{
   int       rno = _clip_parni(ClipMachineMemory, 1) - 1;

   DBWorkArea *wa = cur_area(ClipMachineMemory);


   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   if (rno < 0 || rno >= wa->rd->rels_opened)
      return 0;
   _clip_retni(ClipMachineMemory, wa->rd->relations[rno]->child->area + 1);
   return 0;
}
