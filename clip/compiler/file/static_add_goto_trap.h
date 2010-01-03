static int
add_goto_trap(Node * np, void *par)
{
   Function *fp = (Function *) par;

   if (!np->isConst || np->isArray)
		fp->goto_trap_of_Function++;

   return 0;
}
