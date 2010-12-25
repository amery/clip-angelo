int
destroy_ClipFile(ClipMachine * ClipMachineMemory, ClipFile * fp)
{
   int i;

   if (!fp)
      return 0;
   --fp->refCount_of_ClipFile;
   if (fp->refCount_of_ClipFile > 0)
      return 0;

   switch (fp->bodyMem_of_ClipFile)
   {
#ifndef OS_MINGW
#ifdef HAVE_MMAN_H
   case 0:
      munmap(fp->mem_of_ClipFile, fp->bodySize_of_ClipFile);
      break;
#endif
#endif
   case 1:
      free(fp->mem_of_ClipFile);
      break;
   }
   if (fp->fd_of_ClipFile > 2)
      close(fp->fd_of_ClipFile);

   for (i = 0; i < fp->nstatics_of_ClipFile; ++i)
      _clip_destroy(ClipMachineMemory, fp->ClipVar_statics_of_ClipFile + i);

   switch (fp->staticsMem_of_ClipFile)
   {
#ifndef OS_MINGW
#ifdef HAVE_MMAN_H
   case 0:
      munmap((void *) fp->ClipVar_statics_of_ClipFile, sizeof(ClipVar) * fp->nstatics_of_ClipFile);
      break;
#endif
#endif
   case 1:
      free(fp->ClipVar_statics_of_ClipFile);
      break;
   }

   free(fp->name_of_ClipFile);
   if (fp->hash_names_of_ClipFile)
   {
      free(fp->hash_names_of_ClipFile->buckets_of_ClipHashNames);
      /*free(fp->hash_names->mem); */
      free(fp->hash_names_of_ClipFile);
   }
   if (fp->ClipVarDef_staticDefs_of_ClipFile)
      free(fp->ClipVarDef_staticDefs_of_ClipFile);

   return 1;
}
