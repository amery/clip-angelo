/*
 * NUMDISKL() --> nNumber
 *
 * NUMDISKL() returns the number of available drives or the value specified
 * by LASTDRIVE = in CONFIG.SYS.  In a NOVELL network, you can determine
 * which is the first network drive (LASTDRIVE +1).
 * Normally, the function return value is 5, if there is no
 * deviating input in CONFIG.SYS for LASTDRIVE =.
 */
int
clip_NUMDISKL(ClipMachine * ClipMachineMemory)	/* Determines the number of available logical drives */
{
   /*
    * ����� �������, ��� ���� C: -> ��� �������
    */
   _clip_retni(ClipMachineMemory, 0);
   return 0;
}
