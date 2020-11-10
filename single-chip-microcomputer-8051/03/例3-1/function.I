
#line 1 ".\function.c" /0
  
#line 1 "C:\KEIL\C51\INC\STDIO.H" /0






 
 
 
 
 
 
 
 
 
 
 
 
 typedef unsigned int size_t;
 
 
 #pragma SAVE
 #pragma REGPARMS
 extern char _getkey (void);
 extern char getchar (void);
 extern char ungetchar (char);
 extern char putchar (char);
 extern int printf   (const char *, ...);
 extern int sprintf  (char *, const char *, ...);
 extern int vprintf  (const char *, char *);
 extern int vsprintf (char *, const char *, char *);
 extern char *gets (char *, int n);
 extern int scanf (const char *, ...);
 extern int sscanf (char *, const char *, ...);
 extern int puts (const char *);
 
 #pragma RESTORE
#line 1 ".\function.c" /0
 
  
#line 1 "def.h" /0
 extern int getline (char line[ ], int max);
 extern int strindex(char source[ ], char searchfor[ ]);
 
 
#line 2 ".\function.c" /0
 
 
 
 int getline(char s[ ], int lim)
 {
 int c, i;
 i = 0;
 while ( -- lim > 0&& ( c = getchar() ) != -1&&c!='\n')
 s[i++] = c;
 if (c == '\n' )
 s[i++] = c;
 s[i] = '\0';
 return i;
 }
 
 int strindex(char s[], char t[] )
 {
 int i, j, k;
 for ( i = 0; s[i] != '\0'; i++ ) {
 for ( j =i, k = 0; t[k] != '\0' && s[j] ==t[k]; j++, k++ );
 if ( k > 0 && t[k] == '\0' )
 return i;
 }
 return -1;
 }
