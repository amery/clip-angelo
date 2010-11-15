/***
*
*  Getexit.ch
*
*  Clipper GET system exitState constants
*
*  Copyright (c) 1990-1993, Computer Associates International, Inc.
*  All rights reserved.
*
*/
#IfNdef _GETEXIT_CH
 
/* get:exitState values */
#define GE_NOEXIT       0      // no exit attempted (blank)
#define GE_UP           1
#define GE_DOWN         2
#define GE_TOP          3
#define GE_BOTTOM       4
#define GE_ENTER        5
#define GE_WRITE        6
#define GE_ESCAPE       7
#define GE_WHEN         8      // when clause unsatisfied
#define GE_SHORTCUT     9      // introduced in x5.3
#define GE_MOUSEHIT     10     // introduced in x5.3


#define _GETEXIT_CH
#Endif 
