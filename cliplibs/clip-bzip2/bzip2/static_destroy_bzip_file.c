static void
destroy_bzip_file(void *file)
{
   if (file != NULL)
      BZPREF(bzclose) ((BZFILE *) file);
}

