****
*
*    CTCOM.CH
*
*    Symbolic constants for the interface functions of
*    CA-Clipper Tools II / 5.2
*    Lothar Bongartz
*    Copyright (c) 1991-93 Computer Associates International.
*    All rights reserved.
*
*    Version 5.2
*

******************* COM_MSR() *******************

#define MSR_ERROR	       0       // parameter error
#define MSR_DELTA_CTS	       1       // Delta Clear to Send
#define MSR_DELTA_DSR	       2       // Delta Data Set Ready
#define MSR_TERI	       4       // Trailing Edge Ring Indicator
#define MSR_DELTA_DCD	       8       // Delta Data Carrier Detect
#define MSR_CTS 	      16       // Clear to Send
#define MSR_DSR 	      32       // Data Set Ready
#define MSR_RI		      64       // Ring Indicator
#define MSR_DCD 	     128       // Data Carrier Detect


******************* COM_LSR() *******************

#define LSR_ERROR		0      // Parameter Fehler
#define LSR_DATA_READY		1      // Data	  Ready
#define LSR_OVERRUN_ERR 	2      // Overrun Error
#define LSR_PARITY_ERR		4      // Parity  Error
#define LSR_FRAMING_ERR 	8      // Framing Error
#define LSR_BREAK	       16      // Break   Interrupt
#define LSR_TRANS_HOLD_EMPTY   32      // Transmitter Holding Empty
#define LSR_TRANS_EMPTY        64      // Transmitter Empty


******************* COM_MCR() *******************

#define MCR_ERROR		 0     // parameter error
#define MCR_DTR 		 1     // Data Terminal Ready
#define MCR_RTS 		 2     // Request To Send
#define MCR_OUT_1		 4     //
#define MCR_OUT_2		 8     //
#define MCR_LOOP		16     //


******************* COM_SMODE() *******************

#define SMODE_EMPTY	      1     // send buffer empty
#define SMODE_SOFT	      2     // wait for release software handshake (X-OFF)
#define SMODE_HARD	      4     // wait for release hardware handshake (CTS)
#define SMODE_RFLUSH	      8     // buffer deleted by remote station


********************* COM_CRC *********************

#define Parity		      3     // 2^^1 + 1^u
#define LCR_8		    257     // 2^^8 + 1^u
#define CRC_12		   5011     // 2^^12 + 2^^11 + 2^^3 + 2^^2 + 2^^1 + 1^u
#define CRC_16_X25	  69665     // 2^^16 + 2^^12 + 2^^5 + 1^u
#define CRC_16		  98309     // 2^^16 + 2^^15 + 2^^2 + 1^u
