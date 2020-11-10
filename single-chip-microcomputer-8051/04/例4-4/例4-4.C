/* 直到型循环结构示例 */

#include <stdio.h>

void main()
{
	char ch;
	char buf[256];
	char *p;

	printf("Input a string.\n");    // 读取字符串
	scanf("%s", buf);

	p = buf;
	do
	{
		ch = *p;    // 读取字符
		if((ch>'a') && (ch<'z'))    // 判断当前字符是否为小写
			ch -= 32;
		p++;
	}while((ch!=0) || ((p-buf)>255));    // 字符串结束或缓冲区溢出则终止

	printf("The result is:\n");
	printf("%s", buf);
}
