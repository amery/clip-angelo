static void
q_sort(ClipVar * a, int n, q_cmp cmp, ClipMachine * ClipMachineMemory, void *par)
{
   ClipVar *pa, *pb, *pc, *pd, *pl, *pm, *pn;

   int d, r, swap_cnt;

 loop:
   swap_cnt = 0;
   if (n < 7)
   {
      for (pm = a + 1; pm < a + n; pm++)
	 for (pl = pm; pl > a && cmp((pl - 1), pl, ClipMachineMemory, par) > 0; pl--)
	    q_swap(pl, pl - 1);
      return;
   }
   pm = a + (n / 2);
   if (n > 7)
   {
      pl = a;
      pn = a + (n - 1);
      if (n > 40)
      {
	 d = (n / 8);
	 pl = med3(pl, pl + d, pl + 2 * d, cmp, ClipMachineMemory, par);
	 pm = med3(pm - d, pm, pm + d, cmp, ClipMachineMemory, par);
	 pn = med3(pn - 2 * d, pn - d, pn, cmp, ClipMachineMemory, par);
      }
      pm = med3(pl, pm, pn, cmp, ClipMachineMemory, par);
   }
   q_swap(a, pm);
   pa = pb = a + 1;

   pc = pd = a + (n - 1);
   for (;;)
   {
      while (pb <= pc && (r = cmp(pb, a, ClipMachineMemory, par)) <= 0)
      {
	 if (r == 0)
	 {
	    swap_cnt = 1;
	    q_swap(pa, pb);
	    pa += 1;
	 }
	 pb += 1;
      }
      while (pb <= pc && (r = cmp(pc, a, ClipMachineMemory, par)) >= 0)
      {
	 if (r == 0)
	 {
	    swap_cnt = 1;
	    q_swap(pc, pd);
	    pd -= 1;
	 }
	 pc -= 1;
      }
      if (pb > pc)
	 break;
      q_swap(pb, pc);
      swap_cnt = 1;
      pb += 1;
      pc -= 1;
   }
   if (swap_cnt == 0)
   {				/* Switch to insertion sort */
      for (pm = a + 1; pm < a + n; pm += 1)
	 for (pl = pm; pl > a && cmp((pl - 1), pl, ClipMachineMemory, par) > 0; pl -= 1)
	    q_swap(pl, pl - 1);
      return;
   }

   pn = a + n;
   r = q_min(pa - a, pb - pa);
   q_vecswap(a, pb - r, r);
   r = q_min(pd - pc, pn - pd - 1);
   q_vecswap(pb, pn - r, r);
   if ((r = pb - pa) > 1)
      q_sort(a, r, cmp, ClipMachineMemory, par);
   if ((r = pd - pc) > 1)
   {
      /* Iterate rather than recurse to save stack space */
      a = pn - r;
      n = r;
      goto loop;
   }
}
