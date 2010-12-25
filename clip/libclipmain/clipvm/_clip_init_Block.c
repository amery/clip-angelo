void
_clip_init_Block(ClipMachine * ClipMachineMemory, ClipBlock * dest, struct ClipFile *file, char *func)
{
   dest->file_of_ClipBlock = file;
   dest->function_of_ClipBlock = func;
   ++file->refCount_of_ClipFile;
}
