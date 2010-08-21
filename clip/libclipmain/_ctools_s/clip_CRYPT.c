int
clip_CRYPT(ClipMachine * ClipMachineMemory)
{
   /*
      ULONG ulCryptLen;
      ULONG ulStringLen;
    */
   int ulCryptLen, ulStringLen;

   ULONG ulCryptPos = 0;

   ULONG ulStringPos;

   BYTE *pbyCrypt = (BYTE *) _clip_parcl(ClipMachineMemory, 2, &ulCryptLen);

   BYTE *pbyString = (BYTE *) _clip_parcl(ClipMachineMemory, 1, &ulStringLen);

   if (ulCryptLen > 1)
   {

      BYTE *pbyResult = (BYTE *) malloc(ulStringLen + 1);

      USHORT uiCount2 = (((USHORT) (pbyCrypt[ulCryptPos] + (USHORT) (pbyCrypt[ulCryptPos + 1] * 256))) & 0xFFFF) ^ ((USHORT) ulCryptLen & 0xFFFF);

      USHORT uiCount1 = 0xAAAA;

      for (ulStringPos = 0; ulStringPos < ulStringLen;)
      {
	 USHORT uiTmpCount1 = uiCount1;

	 USHORT uiTmpCount2 = uiCount2;

	 BYTE byte = pbyString[ulStringPos] ^ pbyCrypt[ulCryptPos++];

	 USHORT tmp;

	 uiTmpCount2 = HB_MKUSHORT((HB_LOBYTE(uiTmpCount2) ^ HB_HIBYTE(uiTmpCount2)), HB_HIBYTE(uiTmpCount2));

	 for (tmp = HB_LOBYTE(uiTmpCount2); tmp; tmp--)
	    uiTmpCount2 = (uiTmpCount2 >> 1) | ((uiTmpCount2 & 1) << 15);

	 uiTmpCount2 ^= uiTmpCount1;
	 uiTmpCount2 += 16;

	 uiCount2 = uiTmpCount2;

	 uiTmpCount2 &= 0x1E;
	 uiTmpCount2 += 2;

	 do
	 {
	    BYTE byTmp;

	    uiTmpCount2--;

	    for (tmp = HB_LOBYTE(uiTmpCount2); tmp; tmp--)
	       uiTmpCount1 = (uiTmpCount1 >> 1) | ((uiTmpCount1 & 1) << 15);

	    uiTmpCount1 = HB_MKUSHORT(HB_HIBYTE(uiTmpCount1), HB_LOBYTE(uiTmpCount1));
	    uiTmpCount1 = HB_MKUSHORT((HB_LOBYTE(uiTmpCount1) ^ 0xFF), HB_HIBYTE(uiTmpCount1));
	    uiTmpCount1 = (uiTmpCount1 << 1) | ((uiTmpCount1 & 0x8000) >> 15);
	    uiTmpCount1 ^= 0xAAAA;

	    byTmp = HB_LOBYTE(uiTmpCount1);
	    byTmp = (byTmp << 1) | ((byTmp & 0x80) >> 7);

	    uiTmpCount1 = HB_MKUSHORT(byTmp, HB_HIBYTE(uiTmpCount1));

	 }
	 while (--uiTmpCount2);

	 uiCount1 = uiTmpCount1;

	 pbyResult[ulStringPos++] = byte ^ HB_LOBYTE(uiTmpCount1);

	 if (ulCryptPos == ulCryptLen)
	    ulCryptPos = 0;
      }

      _clip_retcn(ClipMachineMemory, (char *) pbyResult, ulStringLen);
      free(pbyResult);
   }
   else
   {
      _clip_retcn(ClipMachineMemory, (char *) pbyString, ulStringLen);
   }
   return 0;
}
