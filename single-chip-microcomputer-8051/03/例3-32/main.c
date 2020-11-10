#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdlib.h>
//#include <alloc.h>
#include <string.h>
#include <stdio.h>

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {

	char *binary(char *ptr[],char *str,int n);/* ��������*/
	char *ptr1[5],*temp;
	int i;
/*------------------------------------------------
Setup the serial port for 1200 baud at 16MHz.
------------------------------------------------*/
#ifndef MONITOR51
    SCON  = 0x50;		        /* SCON: mode 1, 8-bit UART, enable rcvr      */
    TMOD |= 0x20;               /* TMOD: timer 1, mode 2, 8-bit reload        */
    TH1   = 221;                /* TH1:  reload value for 1200 baud @ 16MHz   */
    TR1   = 1;                  /* TR1:  timer 1 run                          */
    TI    = 1;                  /* TI:   set TI to send first char of UART    */
#endif
/*------------------------------------------------
Note that an embedded program never exits (because
there is no operating system to return to).  It
must loop and execute forever.
------------------------------------------------*/
	init_mempool (0x1000,0x500);					/*��ʼ���ڴ��*/	
	for (i=0;i<5;i++)
	{
		ptr1[i]=malloc(20);/*���ֵ�˳�������ַ���*/
		gets(ptr1[i],20);
	}
	printf("\n");
	printf("original string:\n");
	for(i= 0 ; i < 5 ; i++)
		printf("%s\n",ptr1[i]) ;
	printf("input search string:\n");
	temp=malloc(20) ;
	gets(temp,20);/*���뱻�����ַ���*/
	i=5 ;
	temp=binary(ptr1,temp,i );/*���ò��Һ���*/
	if (temp) printf("succesful-----%s\n" ,temp);
	else printf("no succesful!\n");
	return;
    while (1) {};
}

char *binary(char *ptr[],char *str,int n)/*���巵���ַ�ָ��ĺ���*/
{/*�۰����*/
	int hig,low,mid;
	low=0;
	hig=n-1;
	while(low<=hig)
	{
		mid=(low+hig)/2 ;
		if (strcmp(str,ptr[mid])<0)
			hig=mid-1 ;
		else if(strcmp(str,ptr[mid])>0)
			low=mid+1 ;
		else return(str);/*���ʳɹ������ر����ַ���*/
	}
	return NULL;/*����ʧ�ܣ����ؿ�ָ��*/
}



