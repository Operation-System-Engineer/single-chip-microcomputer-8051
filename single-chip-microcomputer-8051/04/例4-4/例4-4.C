/* ֱ����ѭ���ṹʾ�� */

#include <stdio.h>

void main()
{
	char ch;
	char buf[256];
	char *p;

	printf("Input a string.\n");    // ��ȡ�ַ���
	scanf("%s", buf);

	p = buf;
	do
	{
		ch = *p;    // ��ȡ�ַ�
		if((ch>'a') && (ch<'z'))    // �жϵ�ǰ�ַ��Ƿ�ΪСд
			ch -= 32;
		p++;
	}while((ch!=0) || ((p-buf)>255));    // �ַ��������򻺳����������ֹ

	printf("The result is:\n");
	printf("%s", buf);
}
