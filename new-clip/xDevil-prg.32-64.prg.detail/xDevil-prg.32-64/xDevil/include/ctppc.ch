****
*
*    CTPPC.CH
*
*    Symbolic constants for Point To Point Communication
*    functions of CA-Clipper Tools / 5.2.
*
*    Ralf Saborowski
*
*    Copyright (c) 1992-93 Computer Associates International.
*    All rights reserved.
*
*    Version 5.2
*


#ifndef _NTPCC

// Event codes
#define PPC_RECSUC 		1
#define PPC_RECFAIL	      	2
#define PPC_RECBUFF75FULL     	4
#define PPC_RECPACKDISCARD    	8
#define PPC_SNDFAIL	      	16
#define PPC_CONESTAB	      	32
#define PPC_CONTERM	      	64

// Buffer types for PPCBUFTYP()
#define PPC_IPX		    	1
#define	PPC_SPX			17
#define	PPC_NBD			2
#define	PPC_NBS			18

#define _NTPCC

#endif

