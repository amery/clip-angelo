typedef struct
{
   ClipMachine *ClipMachineMemory;
   char *name;
   ClipVar block;
   ClipVar *stack;
   int argc;
}
start_data;
