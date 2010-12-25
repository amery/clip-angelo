CLIP_DLLEXPORT int
_clip_trap_err(ClipMachine * ClipMachineMemory, int genCode, int canDefault, int canRetry, const char *subSystem, int subCode, const char *operation)
{
   ClipVar *vp;

   vp = _clip_new_error(ClipMachineMemory, _clip_errname(genCode), 2, genCode, 0, canDefault, canRetry, 0, 1, subSystem, subCode, operation);

   _clip_trap_var(ClipMachineMemory, ClipMachineMemory->fp ? ClipMachineMemory->fp->filename_of_ClipFrame : "", ClipMachineMemory->fp ? ClipMachineMemory->fp->line_of_ClipFrame : 0, vp);

   return genCode;
}
