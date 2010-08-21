/* ALIAS.CH - Database Commands with aliases

   Written by: Mike Schinkel

   Copyright (c) 1993, Expert Education, Inc.
   All Rights Reserved.

*/

// Compile with "/dALIAS_REQUIRED" to flag unaliased commands
#ifdef ALIAS_REQUIRED   
   #command APPEND BLANK         => #error ALIAS REQUIRED: APPEND BLANK
   #command DELETE ALL           => #error ALIAS REQUIRED: DELETE ALL
   #command DELETE               => #error ALIAS REQUIRED: DELETE
   #command GO BOTTOM            => #error ALIAS REQUIRED: GO BOTTOM
   #command GO TOP               => #error ALIAS REQUIRED: GO TOP
   #command GOTO                 => #error ALIAS REQUIRED: GOTO
   #command PACK                 => #error ALIAS REQUIRED: PACK
   #command RECALL               => #error ALIAS REQUIRED: RECALL
   #command SEEK <value>         => #error ALIAS REQUIRED: SEEK
   #command SET ORDER TO <n>     => #error ALIAS REQUIRED: SET ORDER TO
   #command SET ORDER TO         => #error ALIAS REQUIRED: SET ORDER TO
   #command SKIP                 => #error ALIAS REQUIRED: SKIP
   #command SKIP <n>             => #error ALIAS REQUIRED: SKIP
   #command UNLOCK               => #error ALIAS REQUIRED: UNLOCK
   #command ZAP                  => #error ALIAS REQUIRED: ZAP
#endif

#command APPEND BLANK ALIAS <a>;
=>;
         <a>->( dbAppend() )

#command DELETE                                                         ;
         [FOR <for>]                                                    ;
         [WHILE <while>]                                                ;
         [NEXT <next>]                                                  ;
         [RECORD <rec>]                                                 ;
         [<rest:REST>]                                                  ;
         [ALL]                                                          ;
			Alias <a>                                                            ;
      => DBEval(                                                        ;
                 {|| <a>-> ( dbDelete() )},                                       ;
                 <{for}>, <{while}>, <next>, <rec>, <.rest.>            ;
               )


#command DELETE ALIAS <a>;
=>;
         <a>->( dbDelete() )

#command GO BOTTOM ALIAS <a>;
=>;
         <a>->( dbGoBottom() )

#command GO TOP ALIAS <a>;
=>;
         <a>->( dbGoTop() )

#command GOTO <n> ALIAS <a>;
=>;
         <a>->( dbGoTo( <n> ) )

#command PACK ALIAS <a>;
=>;
         <a>->( __dbPack() )

#command RECALL ALIAS <a>;
=>;
         <a>->( dbRecall() )

#command SEEK <xpr> [ALIAS <a>] [<soft: SOFTSEEK>];
=>;
         <a>->( dbSeek( <xpr>, if( <.soft.>, .T., NIL ) ) )

#command SET ORDER TO <n> ALIAS <a>;
=>;
         <a>->( dbSetOrder( <n> ) )

#command UNLOCK ALIAS <a>;
=>;
         <a>->( dbUnLock() )

#command ZAP ALIAS <a>;
=>;
         <a>->( __dbZap() )

// EOF
