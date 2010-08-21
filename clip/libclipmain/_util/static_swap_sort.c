static void
swap_sort(ClipVar * a, int n, q_cmp cmp, ClipMachine * ClipMachineMemory, void *par)
{
   ClipVar *pl, *pm;

   for (pm = a + 1; pm < a + n; pm++)
      for (pl = pm; pl > a && cmp((pl - 1), pl, ClipMachineMemory, par) > 0; pl--)
	 q_swap(pl, pl - 1);
}
