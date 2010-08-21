int
clip_BASE64ENCODE(ClipMachine * ClipMachineMemory)
{
   int inlen;

   char *str = _clip_parcl(ClipMachineMemory, 1, &inlen);

   const char base64char[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

   char *inp = str;

   int outlen = 0;

   int l = ((inlen + 2) / 3) * 4;

   char *outp, *out;

   l += (inlen + 53) / 54;
   out = outp = calloc(1, l + 1);

   while (inlen >= 3)
   {
      *outp++ = base64char[(inp[0] >> 2) & 0x3f];
      *outp++ = base64char[((inp[0] & 0x03) << 4) | ((inp[1] >> 4) & 0x0f)];
      *outp++ = base64char[((inp[1] & 0x0f) << 2) | ((inp[2] >> 6) & 0x03)];
      *outp++ = base64char[inp[2] & 0x3f];

      inp += 3;
      inlen -= 3;
      if (++outlen == 18)
      {
	 *outp++ = 10;
	 outlen = 0;
      }
   }

   if (inlen > 0)
   {
      *outp++ = base64char[(inp[0] >> 2) & 0x3f];
      if (inlen == 1)
      {
	 *outp++ = base64char[(inp[0] & 0x03) << 4];
	 *outp++ = '=';
      }
      else
      {
	 *outp++ = base64char[((inp[0] & 0x03) << 4) | ((inp[1] >> 4) & 0x0f)];
	 *outp++ = base64char[((inp[1] & 0x0f) << 2)];
      }
      *outp++ = '=';
   }

   _clip_retcn_m(ClipMachineMemory, out, l);
   return 0;
}
