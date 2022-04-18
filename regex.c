#include <stdio.h>
#include <string.h>

#define BUFFERSIZE 40
#define DEBUG 1

int matchstar(int c, char* seq, char* text, int length) {
  if (DEBUG) {
    printf("matchstar\n");
  }
  if (match(seq, text)) {
    return matchstar(c+1, seq, text+length, length);
  }
  else {
    if (DEBUG) {
      printf("leaving matchstar\n");
      printf("%i\n", c*length);
    }
    return c*length;
  }
}

int matchplus(int c, char* seq, char* text, int length) {
  if (DEBUG) {
    printf("matchplus\n");
  }
  int starmatch = matchstar(c, seq, text+length, length);
  if (starmatch) {
    return starmatch;
  }
  
  return 0;
}

int findexprlength(char* regexp, int c) {
  if (regexp[0] == ')') {
    return c;
  }
  else {
    return findexprlength(regexp+1, c+1);
  }
}
   

int matchhere(char* regexp, char* text) {
  
  int brackets = 0;
  int escaped = 0;
  int length = 1;

  if (DEBUG) {
    printf("Start of match here\n");
    printf("REGEX: %s\n", regexp);
    printf("STRING: %s\n", text);
  }

  if (regexp[0] == '\\') {
    if (DEBUG) {
      printf("Escape character found\n");
    }
    escaped = 1;
    regexp++;
  }
    
  if (regexp[0] == '(' && !escaped) {
    if (DEBUG) {
      printf("Bracket found\n");
    }
    brackets = 1;
    regexp++;
    length = findexprlength(regexp, 0);
  }

  if (regexp[brackets+length] == '*' && !escaped) {
    char* copied;
    memcpy(copied, &regexp[0], length);
    copied[length] = '\0';
    text = text + matchstar(0, copied, text, length);
    regexp = regexp + brackets+length+1;
  };

  if (regexp[brackets+length] == '+' && !escaped) {
    char* copied;
    memcpy(copied, &regexp[0], length);
    copied[length] = '\0';
    int pluscount = matchplus(0, copied, text, length);
    if (!pluscount) {
      return 0;
    }
    text = text + pluscount;
    regexp = regexp + brackets+length+1;
  };

  if (DEBUG) {
    printf("Before final checks\n");
    printf("REGEX: %s\n", regexp);
    printf("STRING: %s\n", text);
  }

  if (regexp[0] == '\0') {
    return 1;
  };

  if (regexp[0] == *text) {
    return matchhere(regexp+1, text+1);
  }
  return 0;
}

int match(char* regexp, char* text) {
  if (DEBUG) {
    printf("Begin\n");
  }
  
  if (regexp[0] == '^') {
    return matchhere(regexp+1, text);
  }

  do {
    if (DEBUG) {
      printf("Match iteration\n");
    }
   if (matchhere(regexp, text)) {
      return 1;
    }
  } while (*text++ != '\0');
  
  return 0;
}

int main(int argc, char* argv[]) {
  if (argv[1] == NULL || argv[2] == NULL) {
    printf("Requires 2 CLI arguments! in the form <pattern> <string>\n");
    return 0;
  }
  printf("%i\n", match(argv[1], argv[2]));
  return 0;
}


/* todo: match except, optional, list selection (treat these like a loop on escapes?) */
