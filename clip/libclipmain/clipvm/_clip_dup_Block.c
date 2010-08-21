void
_clip_dup_Block(ClipMachine * ClipMachineMemory, ClipBlock * src, ClipBlock * dest)
{
   *dest = *src;
   ++src->file_of_ClipBlock->refCount_of_ClipFile;
}
