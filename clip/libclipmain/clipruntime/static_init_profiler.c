static void
init_profiler(ClipMachine * ClipMachineMemory)
{
   if (ClipMachineMemory->profiler)
      return;

   ClipMachineMemory->profiler = new_HashTable();
}
