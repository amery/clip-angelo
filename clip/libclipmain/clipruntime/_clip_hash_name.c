CLIP_DLLEXPORT void
_clip_hash_name(ClipMachine * ClipMachineMemory, long hash, char *buf, int buflen)
{
   ClipFrame *fp;

   char *s = 0;

   if (!buflen)
      return;
   else
      buf[0] = 0;

   switch (hash)
   {
   case HASH_main:
      s = "MAIN";
      break;
   case HASH_name:
      s = "NAME";
      break;
   case HASH_filename:
      s = "filename";
      break;
   case HASH_operation:
      s = "operation";
      break;
   case HASH_tries:
      s = "tries";
      break;
   case HASH_canSubstitute:
      s = "canSubstitute";
      break;
   case HASH_canRetry:
      s = "canRetry";
      break;
   case HASH_canDefault:
      s = "canDefault";
      break;
   case HASH_osCode:
      s = "osCode";
      break;
   case HASH_genCode:
      s = "genCode";
      break;
   case HASH_severity:
      s = "severity";
      break;
   case HASH_description:
      s = "description";
      break;
   case HASH_args:
      s = "args";
      break;
   case HASH_cargo:
      s = "cargo";
      break;
   case HASH_subCode:
      s = "subCode";
      break;
   case HASH_subSystem:
      s = "subSystem";
      break;
   case HASH_syserr:
      s = "syserr";
      break;
   }

   if (s)
   {
      strncpy(buf, s, buflen);
      return;
   }

   s = (char *) HashTable_fetch(ClipMachineMemory->hashnames, hash);
   if (s)
   {
      strncpy(buf, s, buflen);
      return;
   }

   for (fp = ClipMachineMemory->fp; fp; fp = fp->ClipFrame_up_of_ClipFrame)
   {
      if (fp->names_of_ClipFrame)
      {
	 ClipHashBucket *bp;

	 bp = (ClipHashBucket *) bsearch(&hash, fp->names_of_ClipFrame->buckets_of_ClipHashNames, fp->names_of_ClipFrame->num_of_ClipHashNames, sizeof(ClipHashBucket), cmp_hash_bucket);
	 if (bp)
	 {
	    strncpy(buf, fp->names_of_ClipFrame->mem_of_ClipHashNames + bp->offs_of_ClipHashBucket, buflen);
	    return;
	 }
      }
   }

   snprintf(buf, buflen, "0x%08lx", hash);
}
