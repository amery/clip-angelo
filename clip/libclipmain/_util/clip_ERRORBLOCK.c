int
clip_ERRORBLOCK(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_spar(ClipMachineMemory, 1);

   if (ClipMachineMemory->errorblock)
      _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), ClipMachineMemory->errorblock);
   if (vp)
   {
      _clip_delete(ClipMachineMemory, ClipMachineMemory->errorblock);
      ClipMachineMemory->errorblock = (ClipVar *) calloc(1, sizeof(ClipVar));
      _clip_clone(ClipMachineMemory, ClipMachineMemory->errorblock, vp);
   }
   return 0;
}
