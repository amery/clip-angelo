void
destroy_Block(ClipMachine * ClipMachineMemory, ClipBlock * bp)
{
   delete_ClipFile(ClipMachineMemory, bp->file_of_ClipBlock);
}
