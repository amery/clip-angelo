/*
   1) returns number of characters matched, -1 if failed.
   (Can return 0 on patterns like "" or "$")
   2) does not require pattern to match to end of string
   3) case-independent
 */

CLIP_DLLEXPORT int
_clip_glob_match(const char *string, const char *pattern, int caseflag)
{
   char c2;

   int match = 0;		/* # of chars matched */

   while (1)
   {
      /* If at end of pattern, success! */
      if (*pattern == 0)
      {
#if 0
	 return match;
#else
	 /* uri added: bug with pattern length < string length */
	 if (*string == 0)
	    return match;
	 else
	    return (-1);
#endif
      }

      /* If last pattern character is '$', verify that entire
       * string has been matched.
       */
      if ((*pattern == '$') && (pattern[1] == 0))
      {
	 if (*string == 0)
	    return (match);
	 else
	    return (-1);
      }

      /* Check for a "*" as the next pattern character.  It matches
       * any substring.  We handle this by calling ourselves
       * recursively for each postfix of string, until either we
       * match or we reach the end of the string.
       */

      if (*pattern == '*')
      {

	 int head_len;

	 const char *tail;

	 pattern += 1;
	 if (*pattern == 0)
	 {
	    return (strlen(string) + match);	/* DEL */
	 }

	 /* find longest match - switched to this on 12/31/93 */
	 head_len = strlen(string);	/* length before tail */
	 tail = string + head_len;
	 /*printf("pattern='%s' tail='%s'\n", pattern, tail); */
	 while (head_len >= 0)
	 {
	    int rc;

	    if (-1 != (rc = _clip_glob_match(tail, pattern, caseflag)))
	    {
	       /*printf("rc=%d match=%d head_len=%d tail='%s' str='%s' patt='%s'\n", rc, match, head_len, tail,
	          sstr, patt); */
	       return rc + match + head_len;	/* DEL */
	    }
	    tail--;
	    head_len--;
	 }

	 return -1;		/* DEL */
      }

#if 1
      if (*pattern == '?')
      {
	 if (!*string)
	 {
	    for (; *pattern; pattern++)
	       if (*pattern != '?' && *pattern != '*')
		  return -1;
	    return match;
	 }
      }
#endif
      /*
       * after this point, all patterns must match at least one
       * character, so check this
       */

      if (*string == 0)
	 return -1;

#if 1
      /* Check for a "?" as the next pattern character.  It matches
       * any single character.
       */

      if (*pattern == '?')
      {
	 goto thisCharOK;
      }
#endif
      /* Check for a "[" as the next pattern character.  It is followed
       * by a list of characters that are acceptable, or by a range
       * (two characters separated by "-").
       */

      if (*pattern == '[')
      {
	 pattern += 1;
	 while (1)
	 {
	    if ((*pattern == ']') || (*pattern == 0))
	    {
	       return -1;	/* was 0; DEL */
	    }
	    if (*pattern == *string)
	    {
	       break;
	    }
	    if (pattern[1] == '-')
	    {
	       c2 = pattern[2];
	       if (c2 == 0)
	       {
		  return -1;	/* DEL */
	       }
	       if ((*pattern <= *string) && (c2 >= *string))
	       {
		  break;
	       }
	       if ((*pattern >= *string) && (c2 <= *string))
	       {
		  break;
	       }
	       pattern += 2;
	    }
	    pattern += 1;
	 }

/* OOPS! Found a bug in vanilla Tcl - have sent back to Ousterhout */
/* but he hasn't integrated it yet. - DEL */

#if 0
	 while ((*pattern != ']') && (*pattern != 0))
#else
	 while (*pattern != ']')
	 {
	    if (*pattern == 0)
	    {
	       pattern--;
	       break;
	    }
#endif
	    pattern += 1;
	 }
	 goto thisCharOK;
      }

      /* If the next pattern character is backslash, strip it off
       * so we do exact matching on the character that follows.
       */

      if (*pattern == '%')
      {
	 pattern += 1;
	 if (*pattern == 0)
	 {
	    return -1;
	 }
	 switch (*pattern)
	 {
	 case 'r':
	    if (*string != '\r')
	       return -1;
	    break;
	 case 'n':
	    if (*string != '\n')
	       return -1;
	    break;
	 case 's':
	    if (*string != ' ')
	       return -1;
	    break;
	 case 'e':
	    if (*string != '\033')
	       return -1;
	    break;
	 default:
	    if (*string != *pattern)
	       return -1;
	    break;
	 }
      }
      else if ((caseflag && _clip_toupper(*pattern) != _clip_toupper(*string)) || (!caseflag && *pattern != *string))
      {
	 return -1;
      }

    thisCharOK:
      pattern += 1;
      string += 1;
      match++;
   }
}
