CLIP_DLLEXPORT int
_clip_errorblock(ClipMachine * ClipMachineMemory, ClipVar * vp, int genCode)
{
   int r;

   ClipVar res;

   memset(&res, 0, sizeof(res));

   r = _clip_errorblock_res(ClipMachineMemory, vp, genCode, &res);

   _clip_destroy(ClipMachineMemory, &res);

   if (!r)
      r = _clip_log(&res);
   else
      r = 0;

   return 0;
}
