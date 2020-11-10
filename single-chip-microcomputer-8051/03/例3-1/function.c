#include <stdio.h>
#include "def.h"
/* getline ��ȡһ�зŵ�s �У������ظ��еĳ���*/

int getline(char s[ ], int lim)
{
	int c, i;
	i = 0;
	while ( -- lim > 0&& ( c = getchar() ) != EOF&&c!='\n')
		s[i++] = c;
	if (c == '\n' )
		s[i++] = c;
		s[i] = '\0';
	return i;
}
/* strindex ������t ��s �е�λ�ã���δ�ҵ��򷵻�-1 */
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
