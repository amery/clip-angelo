static void
addDeep(Function * fp, int no)
{
   fp->deep_of_Function += no;
   if (fp->deep_of_Function > fp->maxdeep_of_Function)
      fp->maxdeep_of_Function = fp->deep_of_Function;
}
