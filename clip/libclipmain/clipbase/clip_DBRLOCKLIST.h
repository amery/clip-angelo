int
clip_DBRLOCKLIST(ClipMachine * ClipMachineMemory)
{
#ifndef CLIPPER50_COMPATIBLE
   const char *__PROC__ = "DBRLOCKLIST";
#endif
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar  *rp = RETPTR(ClipMachineMemory);

   long      vect[1];

   int       i;


   CHECKWA(wa);

   vect[0] = wa->rd->nlocks;
   _clip_array(ClipMachineMemory, rp, 1, vect);

   for (i = 0; i < wa->rd->nlocks; i++)
    {
       ClipVar   var;

       var.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
       var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
       var.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
       var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = wa->rd->locks[i];
       var.ClipType_t_of_ClipVar.dec_of_ClipType = 0;
       var.ClipType_t_of_ClipVar.len_of_ClipType = 7;
       vect[0] = i;
       _clip_aset(ClipMachineMemory, rp, &var, 1, vect);
    }
   return 0;
}
