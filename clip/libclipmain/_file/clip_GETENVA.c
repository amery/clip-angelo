int
clip_GETENVA(ClipMachine * ClipMachineMemory)
{
   ClipVar *rp = RETPTR(ClipMachineMemory);

   int i = 0;
   long dims[1] = { 0 };

   rp = RETPTR(ClipMachineMemory);
   _clip_array(ClipMachineMemory, rp, 1, dims);
   while (_clip_envp[i])
   {
      int l;

      char *s = _clip_envp[i];

      ClipVar ap, s1, s2;
      long adims[1] = { 0 };

      _clip_array(ClipMachineMemory, &ap, 1, adims);
      l = strcspn(s, "=");
      if (l)
      {
	 _clip_var_str(s, l, &s1);
	 _clip_var_str(s + l + 1, strlen(s + l + 1), &s2);
	 _clip_aadd(ClipMachineMemory, &ap, &s1);
	 _clip_aadd(ClipMachineMemory, &ap, &s2);
	 _clip_aadd(ClipMachineMemory, rp, &ap);

	 _clip_destroy(ClipMachineMemory, &s1);
	 _clip_destroy(ClipMachineMemory, &s2);
	 _clip_destroy(ClipMachineMemory, &ap);
      }
      i++;
   }
   return 0;
}
