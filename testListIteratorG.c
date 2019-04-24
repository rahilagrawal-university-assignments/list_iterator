/*
  testListIteratorG.c ... client to test Generic List Iterator 
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "listIteratorG.h"
#include "positiveIntType.h"
#include "stringType.h" 
#include <ctype.h>
#include <unistd.h>

#define MAXARRAY 5
#define  MAX_LINE      500
void trim(char *str);

int main(int argc, char *argv[])
{
  
  /* The following code, inside the comments, may or may not be correct!
     
     It's provided in case you find it difficult to understand 
     how to create a generic list iterator, and use it's functions.
     
     Please study SetG example we discussed in the lecture in detail.

     The code below only tests basic operations, however 
     make sure that you do test all the required functions properly in
     your test file. 

  */
  
  /* =======================================
     --------- positiveIntType List Iterator ----------
  */
  

//  IteratorG it1 = IteratorGNew(positiveIntCompare, positiveIntNew, positiveIntFree);



  /* =======================================
     --------- stringType List Iterator ----------
  */
 
 
  IteratorG it2 = IteratorGNew(stringCompare, stringNew, stringFree);
  char line[MAX_LINE];

  while (fgets(line,MAX_LINE,stdin) != NULL) {
      // if running from a script, show the command
      if (!isatty(0)) fputs(line,stdout);
      // trim spurious spaces
      trim(line);
		char b2[MAX_LINE];
      if (sscanf(line, "add %s ", b2) == 1) {
         add(it2,b2);
         print_list(it2);
      }
		else if (sscanf(line, "findprevious %s", b2) == 1) {
         findPrevious(it2,b2);
         print_list(it2);
      }
		else if (sscanf(line, "findnext %s", b2) == 1) {
         findNext(it2,b2);
         print_list(it2);
      }
		else if (sscanf(line, "set %s",b2) ==1)
		{
			set(it2, b2);
			print_list(it2);
		} 
		else if (sscanf(line, "%s", b2) == 1) {
         if(strcmp(b2,"delete")==0)
			delete(it2);
			else if(strcmp(b2,"next")==0)
			next(it2);
			else if(strcmp(b2,"previous")==0)
			previous(it2);
			else if(strcmp(b2,"reset")==0)
			reset(it2);
			else if(strcmp(b2,"freeIt")==0)
			{
				freeIt(it2);
				return 0;
			}
			else if(strcmp(b2,"exit")==0)
			return 0;
         print_list(it2);
      }
		
	}

  return EXIT_SUCCESS;
  
}

void trim(char *str)
{
   char *s, *t, *w;
   if (str == NULL) return;
   // find first non-space
   s = str;
   while (isspace(*s) && *s != '\0') s++;
   // find last non-space
   t = &s[strlen(s)-1];
   // copy trimmed data into position
   while (isspace(*t) && t > s) t--;
   w = str;
   while (s <= t) { *w = *s; w++; s++; }
   *w = '\0';
}
