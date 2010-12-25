int
_clip_register_file(ClipMachine * ClipMachineMemory, ClipFile * file)
{
   char *modp = file->body_of_ClipFile;

   char *modbeg = M_OFFS(modp, 2, 0);

   long funcOffs = GETLONG(M_OFFS(modp, 7, 4));

   int funcnum = GETLONG(M_OFFS(modp, 4, 2));

   char *bp = (modbeg + funcOffs);

   int i;

   _clip_hash_buckets(ClipMachineMemory, file);

   for (i = 0; i < funcnum; ++i, bp += 2 * sizeof(long))
   {
      long hash = GETLONG(bp);

      char *func = modbeg + GETLONG(bp + sizeof(long));

      ClipBlock block;

      block.file_of_ClipBlock = file;
      block.function_of_ClipBlock = func;
      if (_clip_register_block(ClipMachineMemory, block, hash))
	 ++file->refCount_of_ClipFile;
   }

   return 0;
}
