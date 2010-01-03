#command MENU TO <v>		;
									=> <v> := __MenuTo( {|_1| iif(PCount() == 0, <v>, <v> := _1)}, #<v> )

