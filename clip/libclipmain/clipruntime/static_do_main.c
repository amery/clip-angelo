static int
do_main(ClipMachine * ClipMachineMemory, long hash, ClipFunction * func, ClipBlock * block, int argc, char **argv, char **envp)
{
   int i, ret;

#ifdef OS_MINGW
   ClipVar *_stack = (ClipVar *) malloc(sizeof(ClipVar) * (argc + 1));
#else
   ClipVar *_stack = (ClipVar *) alloca(sizeof(ClipVar) * (argc + 1));
#endif
   ClipFrame _frame = { _stack, _stack, "" /*__FILE__*//* ": do_main()" */ , /*__LINE__*/ 0, 0,
      0, 0, 0, 0, 0, 0, argc + 1, 0
   };

   _frame.ClipFrame_up_of_ClipFrame = ClipMachineMemory->fp;
   ClipMachineMemory->fp = &_frame;
   if (_clip_init(ClipMachineMemory, argc, argv, envp))
      goto _trap;
   argc = _clip_argc;
   argv = _clip_argv;

   if (!argc)
      argc = 1;
   ClipMachineMemory->argc = argc - 1;
   _frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
   _frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   _frame.ClipVar_sp_of_ClipFrame++;
   CLIP_CHECK_STACK;
   for (i = 1; i < argc; ++i, ++_frame.ClipVar_sp_of_ClipFrame)
      _clip_var_str(argv[i], strlen(argv[i]), _frame.ClipVar_sp_of_ClipFrame);

   init_machine(ClipMachineMemory);
   if (func)
      ret = do_call(ClipMachineMemory, func, argc - 1, 0, 0, 0);
   else if (block)
      ret = _clip_code_func(ClipMachineMemory, block, argc - 1, 0, 0);
   else
      ret = _clip_func_hash(ClipMachineMemory, hash, argc - 1, 0, 0);

   if (ret)
   {
      _clip_trap(ClipMachineMemory, __file__, __LINE__);
      goto _trap;
   }

   ret = _clip_long(_stack);
   goto _return;
 _trap:
   {
      char *msg = 0;

      descr_str(ClipMachineMemory, ClipMachineMemory->trapVar, &msg);
      if (!msg)
	 msg = "unknown runtime error";
      printf("\n%s: runtime error: %s\n", _clip_progname, msg);
      _clip_logg(1, "%s: runtime error: %s", _clip_progname, msg);
      for (i = 0; i < ClipMachineMemory->trapTrace.count_of_ClipVect; ++i)
      {
	 printf("trace: %s\n", (char *) ClipMachineMemory->trapTrace.items_of_ClipVect[i]);
	 _clip_logg(1, "trace: %s", (char *) ClipMachineMemory->trapTrace.items_of_ClipVect[i]);
      }
      ret = 1;
   }
 _return:
   _clip_resume(ClipMachineMemory, 0, 0);
   if (ret == 0)
      ret = errorlevel;
#ifdef OS_MINGW
   free(_stack);
#endif
   return ret;
}
