/*
 * Sets default opening mode for CA-Clipper Tools file functions
 * ----------------------------------------------------------------------
 * Code    Share Mode
 * ----------------------------------------------------------------------
 * 0       Compatibility mode.  Here DOS or the network software itself
 *         determines the share mode.  In Novell networks, the SHAREABLE
 *         attribute plays an important role in this relationship.
 * 1       Both read and write by other programs are locked out.
 * 2       Write by other programs are locked out.
 * 3       Read by other programs are locked out.
 * 4       Not locked.  Read and write by other programs is allowed
 * ----------------------------------------------------------------------
 */
int
clip_SETSHARE(ClipMachine * ClipMachineMemory)
{
   int *smode = NULL, n = 0;

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
   {
      _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
      return 1;
   }

   switch ((n = _clip_parni(ClipMachineMemory, 1)))
   {
   case LOCK_OS:
   case LOCK_RDWR:
   case LOCK_WRONLY:
   case LOCK_RDONLY:
   case LOCK_NONE:
      break;
   default:
      _clip_retl(ClipMachineMemory, 0);
      return 0;
   }

   smode = (int *) calloc(1, sizeof(int));

   *smode = n;
   _clip_store_item(ClipMachineMemory, CLIP_CA_SHARE_MODE, smode);
   _clip_retl(ClipMachineMemory, 1);
   return 0;
}
