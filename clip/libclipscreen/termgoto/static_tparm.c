/*
   // Routine to perform parameter substitution.
   // instring is a string containing printf type escapes.
   // The whole thing uses a stack, much like an HP 35.
   // The following escapes are defined for substituting row/column:
   //
   //      %d      print pop() as in printf
   //      %[0]2d  print pop() like %2d
   //      %[0]3d  print pop() like %3d
   //      %c      print pop() like %c
   //
   //      %p[1-0] push ith parm
   //      %P[a-z] set variable
   //      %g[a-z] get variable
   //      %'c'    char constant c
   //      %{nn}   integer constant nn
   //
   //      %+ %- %* %/ %m         arithmetic (%m is mod): push(pop() op pop())
   //      %& %| %^             bit operations:  push(pop() op pop())
   //      %= %> %<             logical operations:     push(pop() op pop())
   //      %! %~                   unary operations     push(op pop())
   //      %b                 unary BCD conversion
   //      %d                 unary Delta Data conversion
   //
   //      %? expr %t thenpart %e elsepart %;
   //                         if-then-else, %e elsepart is optional.
   //                         else-if's are possible ala Algol 68:
   //                         %? c1 %t %e c2 %t %e c3 %t %e c4 %t %e %;
   //
   // all other characters are ``self-inserting''.  %% gets % output.
 */

static void
tparm(char *outp, char *cp, int p1, int p2)
{
   int c, op, vars[26], stack[10], top, sign;

#define PUSH(i) (stack [++top] = (i))
#define POP()   (stack [top--])

   if (!cp)
      TPARMERR("null arg");
   top = 0;
   while ((c = *cp++))
   {
      if (c != '%')
      {
	 *outp++ = c;
	 continue;
      }
      op = stack[top];
      if (*cp == '0')
	 ++cp;
      switch (c = *cp++)
      {
      case 'd':		/* PRINTING CASES */
	 if (op < 10)
	    goto one;
	 if (op < 100)
	    goto two;
	 /* fall into... */
      case '3':
       three:
	 if (c == '3' && *cp++ != 'd')
	    TPARMERR("bad char after %3");
	 *outp++ = (op / 100) | '0';
	 op %= 100;
	 /* fall into... */
      case '2':
	 if (op >= 100)
	    goto three;
	 if (c == '2' && *cp++ != 'd')
	    TPARMERR("bad char after %2");
       two:
	 *outp++ = op / 10 | '0';
       one:
	 *outp++ = op % 10 | '0';
	 (void) POP();
	 continue;
      case 'c':
	 *outp++ = op;
	 (void) POP();
	 break;
      case '%':
	 *outp++ = c;
	 break;

	 /*
	    // %i: shorthand for increment first two parms.
	    // Useful for terminals that start numbering from
	    // one instead of zero (like ANSI terminals).
	  */
      case 'i':
	 p1++;
	 p2++;
	 break;
      case 'p':		/* %pi: push the ith parameter */
	 switch (c = *cp++)
	 {
	 case '1':
	    PUSH(p1);
	    break;
	 case '2':
	    PUSH(p2);
	    break;
	 default:
	    TPARMERR("bad parm number");
	 }
	 break;
      case 'P':		/* %Pi: pop from stack into variable i (a-z) */
	 vars[*cp++ - 'a'] = POP();
	 break;
      case 'g':		/* %gi: push variable i (a-z) */
	 PUSH(vars[*cp++ - 'a']);
	 break;
      case '\'':		/* %'c' : character constant */
	 PUSH(*cp++);
	 if (*cp++ != '\'')
	    TPARMERR("missing closing quote");
	 break;
      case '{':		/* %{nn} : integer constant. */
	 op = 0;
	 sign = 1;
	 if (*cp == '-')
	 {
	    sign = -1;
	    cp++;
	 }
	 else if (*cp == '+')
	    cp++;
	 while ((c = *cp++) >= '0' && c <= '9')
	 {
	    op = 10 * op + c - '0';
	 }
	 if (c != '}')
	    TPARMERR("missing closing brace");
	 PUSH(sign * op);
	 break;
	 /* binary operators */
      case '+':
	 c = POP();
	 op = POP();
	 PUSH(op + c);
	 break;
      case '-':
	 c = POP();
	 op = POP();
	 PUSH(op - c);
	 break;
      case '*':
	 c = POP();
	 op = POP();
	 PUSH(op * c);
	 break;
      case '/':
	 c = POP();
	 op = POP();
	 PUSH(op / c);
	 break;
      case 'm':
	 c = POP();
	 op = POP();
	 PUSH(op % c);
	 break;
      case '&':
	 c = POP();
	 op = POP();
	 PUSH(op & c);
	 break;
      case '|':
	 c = POP();
	 op = POP();
	 PUSH(op | c);
	 break;
      case '^':
	 c = POP();
	 op = POP();
	 PUSH(op ^ c);
	 break;
      case '=':
	 c = POP();
	 op = POP();
	 PUSH(op = c);
	 break;
      case '>':
	 c = POP();
	 op = POP();
	 PUSH(op > c);
	 break;
      case '<':
	 c = POP();
	 op = POP();
	 PUSH(op < c);
	 break;
	 /* Unary operators. */
      case '!':
	 stack[top] = !stack[top];
	 break;
      case '~':
	 stack[top] = ~stack[top];
	 break;
	 /* Sorry, no unary minus, because minus is binary. */

	 /*
	    If-then-else.  Implemented by a low level hack of
	    skipping forward until the match is found, counting
	    nested if-then-elses.
	  */
      case '?':		/* IF - just a marker */
	 break;
      case 't':		/* THEN - branch if false */
	 if (!POP())
	    cp = branchto(cp, 'e');
	 break;
      case 'e':		/* ELSE - branch to ENDIF */
	 cp = branchto(cp, ';');
	 break;
      case ';':		/* ENDIF - just a marker */
	 break;
      default:
	 TPARMERR("bad % sequence");
      }
   }
   *outp = 0;
}
