CLIP_DLLEXPORT int
_clip_load_inits(ClipMachine * ClipMachineMemory, ClipFile * file)
{
   long funcOffs = GETLONG(M_OFFS(file->body_of_ClipFile, 7, 4));

   char *modbeg = M_OFFS(file->body_of_ClipFile, 2, 0);

   int pubnum = GETLONG(M_OFFS(file->body_of_ClipFile, 4, 2));

   int initnum = GETSHORT(M_OFFS(file->body_of_ClipFile, 8, 4));

   int exitnum = GETSHORT(M_OFFS(file->body_of_ClipFile, 8, 5));

   ClipBlock b;

   ClipCodeVar c;

   int i;

   long hash;

   c.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar = &b;

   file->pos_of_ClipFile = (long *) (modbeg + funcOffs);
   file->pos_of_ClipFile += 2 * pubnum;
   /*file->end = file->pos + 2 * num; */
   for (i = 0; i < initnum; ++i)
   {
      make_func(file, &c, &hash);
      file->pos_of_ClipFile += 2;
      _clip_main_code(ClipMachineMemory, c.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar, _clip_argc, _clip_argv, _clip_envp);
   }
   for (i = 0; i < exitnum; ++i)
   {
      make_func(file, &c, &hash);
      file->pos_of_ClipFile += 2;
      ClipMachineMemory->exits = (ClipBlock *) realloc(ClipMachineMemory->exits, (ClipMachineMemory->nexits + 1) * sizeof(ClipBlock));
      ClipMachineMemory->exits[ClipMachineMemory->nexits] = *c.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar;
      ++file->refCount_of_ClipFile;
      ++ClipMachineMemory->nexits;
   }
   return 0;
}
