****
*
*    CTPRINT.CH
*
*    Symbolic constants for the printer functions of
*    CA-Clipper Tools / 5.2
*    Lothar Bongartz
*    Copyright (c) 1991-93 Computer Associates International.
*    All rights reserved.
*
*    Version 5.2
*

********* PRINTSTAT(), PRINTINIT(), PRINTERROR() ************

#define PST_TIME_OUT		1      // timeout error
#define PST_TRANSFER		8      // transmission error
#define PST_ONLINE	       16      // printer online
#define PST_PAPER	       32      // paper out
#define PST_ACKNOWLEDGE        64      // acknowledged
#define PST_NOT_BUSY	      128      // NOT busy


******************* PRINTERROR() *******************

#define P_ERR_ACCESS_DENIED	5      // access denied
#define P_ERR_UNKOWN_HANDLE	6      // unknown handle
#define P_ERR_USER_BREAK      127      // Ctrl-Break (by user)

