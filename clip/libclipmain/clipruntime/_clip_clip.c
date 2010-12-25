CLIP_DLLEXPORT int
_clip_clip(ClipMachine * ClipMachineMemory, const char *funcname, int argc, ClipVar * stackp, ClipVar * retp)
{
	static int clip_clip_clip = 0;
   ClipVar *stack;
   ClipFrame frame = { 0, 0, __file__, __LINE__, 0, 0, 0, 0, 0, 0, "clip", 0, 0 };
   int i, j, r;

   ClipFunction *fp;

   ClipBlock *bp;

   long hash;
	++ clip_clip_clip ;
	printf ( "clip_clip_clip : %s", clip_clip_clip);
   hash = _clip_casehashbytes(0, funcname, strlen(funcname));
   if (!_clip_get_function(ClipMachineMemory, hash, &fp, &bp))
   {
      return 1;
   }

   stack = (ClipVar *) malloc(sizeof(ClipVar) * (argc + 1));
   frame.ClipVar_stack_of_ClipFrame = stack;
   frame.ClipVar_sp_of_ClipFrame = stack + 1 + argc;
   memset(stack, 0, sizeof(ClipVar) * (argc + 1));

   for (i = 1, j = 0; j < argc; ++j, ++i)
      _clip_clone(ClipMachineMemory, stack + i, stackp + j);

   frame.ClipFrame_up_of_ClipFrame = ClipMachineMemory->fp;
   ClipMachineMemory->fp = &frame;

   if (fp)
      r = _clip_func(ClipMachineMemory, fp, argc, 0, 0);
   else
      r = _clip_code_func(ClipMachineMemory, bp, argc, 0, 0);

   ClipMachineMemory->fp = frame.ClipFrame_up_of_ClipFrame;
   if (retp)
      *retp = stack[0];
   else
      _clip_destroy(ClipMachineMemory, stack + 0);

   free(stack);
   return r;
}
