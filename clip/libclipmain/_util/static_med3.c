static ClipVar *
med3(ClipVar * a, ClipVar * b, ClipVar * c, q_cmp cmp, ClipMachine * ClipMachineMemory, void *par)
{

   return cmp(a, b, ClipMachineMemory, par) < 0 ?
      (cmp(b, c, ClipMachineMemory, par) < 0 ? b : (cmp(a, c, ClipMachineMemory, par) < 0 ? c : a)) : (cmp(b, c, ClipMachineMemory, par) > 0 ? b : (cmp(a, c, ClipMachineMemory, par) < 0 ? a : c));
}
