static void
eject(ClipMachine * ClipMachineMemory)
{
   FILE *printer = (FILE *) ClipMachineMemory->printer;

   int i;

   ClipMachineMemory->prow = 0;
   ClipMachineMemory->pcol = 0;

   if (!printer)
      return;

   fputc('\f', printer);
   fputc('\r', printer);

   for (i = 0; i < ClipMachineMemory->margin; ++i)
      fputc(' ', printer);
}
