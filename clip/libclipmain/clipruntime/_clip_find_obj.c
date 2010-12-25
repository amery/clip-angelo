CLIP_DLLEXPORT int
_clip_find_obj(ClipMachine * ClipMachineMemory, long otype, ClipObjRtti ** rtti)
{
   ClipObjRtti **rp;

   for (rp = init_struct->_objrtti_of_ClipInitStruct; *rp; ++rp)
      if ((*rp)->hashId_of_ClipObjRtti == otype)
      {
	 *rtti = *rp;
	 return 0;
      }

   return 1;
}
