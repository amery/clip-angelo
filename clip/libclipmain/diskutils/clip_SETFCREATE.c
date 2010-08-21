/*
 * Default attribute for creating with CA-Clipper Tools functions
 */
int
clip_SETFCREATE(ClipMachine * ClipMachineMemory)
{
/*
	   0       FA_NORMAL
	   1       FA_READONLY         READ ONLY (Read-only)
	   2       FA_HIDDEN           HIDDEN (Hidden files)
	   4       FA_SYSTEM           SYSTEM (System files)
	   32      FA_ARCHIVE          ARCHIVE (Changes since last backup)
*/
   int oldattr = -1, *ptrattr = NULL;

   int newattr = _clip_parni(ClipMachineMemory, 1);

   if (newattr < 0)
   {
      _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
      return 1;
   }
   ptrattr = _clip_fetch_item(ClipMachineMemory, CLIP_CA_FCREATE_ATTR);
   oldattr = *ptrattr;

   ptrattr = (int *) malloc(sizeof(int));

   *ptrattr = newattr;
   _clip_store_item(ClipMachineMemory, CLIP_CA_FCREATE_ATTR, ptrattr);

   _clip_retni(ClipMachineMemory, oldattr);

   return 0;
}
