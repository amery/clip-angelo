// translate all variations of FUNCTION and PROCEDURE to
// simply PROC <name>

#command FUNCTION <name> [( <params,...> )]                            ;
       => PROC <name>

*#command FUNCTIO  <name> [( <params,...> )]                            ;
*       => PROC <name>

*#command FUNCTI   <name> [( <params,...> )]                            ;
*       => PROC <name>

*#command FUNCT    <name> [( <params,...> )]                            ;
*       => PROC <name>

*#command FUNC     <name> [( <params,...> )]                            ;
*       => PROC <name>


#command PROCEDURE <name> [( <params,...> )]                           ;
       => PROC <name>

*#command PROCEDUR  <name> [( <params,...> )]                           ;
*       => PROC <name>

*#command PROCEDU   <name> [( <params,...> )]                           ;
*       => PROC <name>

*#command PROCED    <name> [( <params,...> )]                           ;
*       => PROC <name>

*#command PROCE     <name> [( <params,...> )]                           ;
*       => PROC <name>

#command PROC     <name> ( <params,...> )                           ;
       => PROC <name>


