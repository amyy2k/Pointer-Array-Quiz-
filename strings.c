//
//  strings.c
//  strings
//
//  Created by William McCarthy on 052//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define MAXLINES 5000

char dest2[BUFSIZ];
char dest[BUFSIZ];
char src[BUFSIZ] = "Ask not what you can do for your country";
char *lineptr[MAXLINES];
char *alloc(int);

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
void qsort(char *lineptr[], int left, int right);

//-------------------------------------------------------------------
// string functions
//-------------------------------------------------------------------
char* strcpy_(char* s, const char* t) {
  char* p = s;
  while ((*s++ = *t++) != '\0') { }

  return p;
}

char* strncpy_(char* s, const char* t, size_t n) {
  char* p = s;

  while (n-- > 0 && *t != '\0') {
    *s++ = *t++;
  }

  while ((int)n-- > 0) {
    *s++ = '\0';
  }

  *s = '\0';
  return p;
}

char* strcat_(char* s, const char* t) {
  char* p = s;

  while (*s != '\0') { ++s; }
  while ((*s++ = *t++) != '\0') { }

  return p;
}

char* strncat_(char* s, const char* t, size_t n) {
  char* p = s;

  while (*s != '\0') { ++s; }
  while (n-- > 0 && *t != '\0') {
    *s++ = *t++;
  }

  *s = '\0';  // in case n goes to 0 while still characters in t
  return p;
}

int strcmp_(const char* s, const char* t) {
  while (*s != '\0' && *t != '\0' && *s == *t) {
    ++s;
    ++t;
  }
  return *s - *t;
}

int strncmp_(const char* s, const char* t, size_t n) {
  while (n-- > 1 && *s != '\0' && *t != '\0' && *s == *t) {
    ++s;
    ++t;
  }
  return *s - *t;
}

const char* strchr_(const char* s, char c) {
  while (*s != '\0' && *s != c) {
    ++s;
  }
  return (*s == c ? s : NULL);
}
const char* strrchr_(const char* s, char c) {
  const char* t = s + strlen(s) - 1;

  while (t >= s && *t != c) {
    --t;
  }
  return ((t >= s && *t == c) ? t : NULL);
}

size_t strspn_(const char* s, const char* t) {
  const char* p = s;
  while (*s != '\0' && strchr(t, *s) != NULL) { ++s; }
  return s - p;
}

size_t strcspn_(const char* s, const char* t) {
  const char* p = s;
  while (*s != '\0' && strchr(t, *s) == NULL) { ++s; }
  return s - p;
}

char* strpbrk_(const char* s, const char* t) {
  while (*s != '\0' && strchr(t, *s) == NULL) {
    ++s;
  }
  return strchr_(t, *s) != NULL ? (char*)s : NULL;
}

char* strstr_(const char* s, const char* t) {
  bool found = false;
  while (*s != '\0' && !found) {
    const char* p = s;
    const char* q = t;
    while (*p != '\0' && *q != '\0' && *p == *q) {
      ++p;
      ++q;
    }
    found = (*p == *q);
    if (!found) { ++s; }
  }
  return *s != '\0' ? (char*)s : NULL;
}

char* ptr = NULL;
char* strtok_(char* s, const char* t) {
  if (s != NULL) { ptr = s; }
  if (strlen(ptr) == 0) { return NULL; }
  size_t prefix = strcspn(ptr, t);  // size of characters before delim
  size_t suffix = strspn(ptr + prefix, t);  // number of delim
  char* token = ptr;
  char* q = ptr + prefix;
  *q = '\0';
  ptr = q + suffix;
  return token;
}

size_t strlen_(const char* s) {
  const char* p = s;
  while (*s != '\0') { ++s; }
  return s - p;
}

void setup_buffers() {
  memset(dest2, 0, sizeof(dest2));
  memset(dest, 0, sizeof(dest));
}

bool open_io_files(int argc, const char* argv[], FILE** fin, FILE** fout,
                   int min_expected_argc, int max_expected_argc,
                   const char* usage) {
  /* open an input file, and optionally an output file */
  if (argc < min_expected_argc || argc > max_expected_argc) {
    fprintf(stderr, "%s\n", usage);
    return false;
  }
  *fin = fopen(argv[1], "r");
  if (*fin == NULL) {
    fprintf(stderr, "failed to open input file: '%s'\n", argv[1]);
    return false;
  }
            // In this case, we don't want to open output file
  if (fout == NULL) {  return true; }  // everything cool

  *fout = fopen(argv[2], "w");
  if (*fout == NULL) {  // output file failed to open
    fprintf(stderr, "failed to open output file: '%s'\n", argv[2]);
    fprintf(stderr, "closing already open input file: '%s'\n", argv[1]);
    fclose(*fin);
    return false;
  }

  return true;

}
void closefiles(int n, ...) {   // uses varargs (variable # of args)
  va_list pargs;
  va_start (pargs, n);    // initialize the list ptr

  for (int i = 0; i < n; i++)
    fclose(va_arg (pargs, FILE*));  // get next argument

  va_end (pargs);   // clean up
}

int gline(char line[],int lim);

int gline(char s[],int lim) {
  int i,c;
  for(i=0;i<lim-1 &&(c=getchar())!=EOF && c != '\n';++i)
    s[i] = c;
  if( c == '\n') {
    s[i] = c;
    ++i;
  }
  else { s[i] = '\0'; }
  return i;
}

 int main(int argc, const char * argv[]) {
 FILE* fin;
 FILE* fout;
 int nlines;

 if ((nlines = readlines(lineptr, MAXLINES)) >= 0 ){
  qsort(lineptr, 0, nlines-1);
  writelines(lineptr, nlines);
  return 0;
  } else {
  printf("error: input too big to sort\n");
  return 1;
  }
  closefiles(2, fin, fout);

 }

int readlines(char *lineptr[], int maxline) {
int len, nlines;
char *p, line[MAXLINES];
nlines = 0;
while ((len = gline(line, MAXLINES)) > 0)
if (nlines >= maxline)
return -1;
else {
line[len-1] = '\0'; /* delete newline */
strcpy(p, line);
printf("%s\n", p);
lineptr[nlines++] = p;
}
return nlines;
}


/* writelines: write output lines */
 void writelines(char *lineptr[], int nlines) {
 int i;
 for (i = 0; i < nlines; i++)
 fprintf(stderr, "%s\n", lineptr[i]);
 }

void qsort(char *v[], int left, int right) {
int i, last;
void swap(char *v[], int i, int j);
if (left >= right) /* do nothing if array contains */
return; /* fewer than two elements */
swap(v, left, (left + right)/2);
last = left;
for (i = left+1; i <= right; i++)
 if (strcmp(v[i], v[left]) < 0)
 swap(v, ++last, i);
 swap(v, left, last);
 qsort(v, left, last-1);
 qsort(v, last+1, right);
}

void swap(char *v[], int i, int j) {
char *temp;
temp = v[i];
v[i] = v[j];
v[j] = temp;
}
