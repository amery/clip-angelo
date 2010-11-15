/***
*
*  Locks87.ch
*
*  Summer '87 Locks.prg compatibility header file
*
*  Copyright (c) 1993, Computer Associates International Inc.
*  All rights reserved.
*
*/


/***
*  Add_Rec( <nWaitSeconds> ) --> lSuccess
*  Attempt to APPEND BLANK with optional retry.
*/
#translate Add_rec(<nWaitSeconds>) => AddRec(<nWaitSeconds>)



/***
*  Fil_Lock( <nWaitSeconds> ) --> lSuccess
*  Attempt to FLOCK() with optional retry.
*/
#translate Fil_lock(<nSeconds>) => FilLock(<nSeconds>)



/***
*  Net_Use( <cDatabase>, <lOpenMode>, <nWaitSeconds> ) --> lSuccess
*  Attempt to USE a database file with optional retry
*/
#translate Net_use(<cDatabase>, <lOpenMode>, <nSeconds>) => ;
   NetUse(<cDatabase>, <lOpenMode>, <nSeconds>)



/***
*  Rec_Lock( <nWaitSeconds> ) --> lSuccess
*  Attempt to RLOCK() with optional retry.
*/
#translate Rec_lock(<nSeconds>) => RecLock(<nSeconds>)

