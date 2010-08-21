CLIP_DLLEXPORT int
_clip_errorblock_res(ClipMachine * ClipMachineMemory, ClipVar * vp, int genCode, ClipVar * res)
{
   int r;

   if (ClipMachineMemory->noerrblock)
      return 0;

   /* default err handler */
   if (!ClipMachineMemory->errorblock || (_clip_type(ClipMachineMemory->errorblock) != CCODE_type_of_ClipVarType && _clip_type(ClipMachineMemory->errorblock) != PCODE_type_of_ClipVarType))
   {
      char *msg = 0;

      int i;

      descr_str(ClipMachineMemory, vp, &msg);
      if (!msg)
	 msg = "unknown runtime error";
      printf("\n%s: system errorblock:\n", _clip_progname);
      printf("%s: runtime error: %s\n", _clip_progname, msg);
      _clip_logg(0, "%s: systerm errorblock: runtime error: %s", _clip_progname, msg);
      for (i = 0; i < ClipMachineMemory->trapTrace.count_of_ClipVect; ++i)
      {
	 printf("trace: %s\n", (char *) ClipMachineMemory->trapTrace.items_of_ClipVect[i]);
	 _clip_logg(0, "trace: %s", (char *) ClipMachineMemory->trapTrace.items_of_ClipVect[i]);
      }
      clip___QUIT(ClipMachineMemory);
   }

   ClipMachineMemory->errorlevel++;
   if (ClipMachineMemory->errorlevel > CLIP_MAX_ERRORBLOCK_DEEP)
   {
      int i;

      printf("%s: errorblock nested too deep (%d):\n", _clip_progname, CLIP_MAX_ERRORBLOCK_DEEP);
      _clip_logg(0, "%s: errorblock nested too deep (%d): ", _clip_progname, CLIP_MAX_ERRORBLOCK_DEEP);
      for (i = 0; i < ClipMachineMemory->trapTrace.count_of_ClipVect; ++i)
      {
	 printf("trace: %s\n", (char *) ClipMachineMemory->trapTrace.items_of_ClipVect[i]);
	 _clip_logg(0, "trace: %s", (char *) ClipMachineMemory->trapTrace.items_of_ClipVect[i]);
      }

      clip___QUIT(ClipMachineMemory);
   }

   if (_clip_type(vp) == MAP_type_of_ClipVarType)
   {
      double d = 0;

      _clip_mgetn(ClipMachineMemory, vp, HASH_gencode, &d);
      if (d == 0.0)
      {
	 _clip_var_num(genCode, res);
	 _clip_madd(ClipMachineMemory, vp, HASH_gencode, res);
	 _clip_destroy(ClipMachineMemory, res);
      }
   }

   r = _clip_eval(ClipMachineMemory, ClipMachineMemory->errorblock, 1, vp, res);
   ClipMachineMemory->errorlevel--;

   return r;
}
