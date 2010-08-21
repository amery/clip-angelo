****
*
*    NT2PRINT.CH
*
*    Symbolic constants for thze printer functions of
*    Clipper Tools II / 5.0
*    Lothar Bongartz
*    Copyright (c) 1991 Nantucket Corp.  All rights reserved.
*
*    Version 5.01
*

********* PRINTSTAT(), PRINTINIT(), PRINTERROR() ************

#define PST_TIME_OUT            1      // timeout error
#define PST_TRANSFER            8      // transmission error
#define PST_ONLINE             16      // printer online
#define PST_PAPER              32      // paper out
#define PST_ACKNOWLEDGE        64      // acknowledged
#define PST_NOT_BUSY          128      // NOT busy


******************* PRINTERROR() *******************

#define P_ERR_ACCESS_DENIED     5      // access denied
#define P_ERR_UNKOWN_HANDLE     6      // unknown handle
#define P_ERR_USER_BREAK      127      // Ctrl-Break (by user)

