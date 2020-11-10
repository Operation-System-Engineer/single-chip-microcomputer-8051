/* 选择结构示例 */

#include <stdio.h>

void main()
{
	char week;
	printf("Choose a number:");    // 输入数据
	scanf("%d", &week);
	
	if((week<0) ||(week>7))    // 判断输入是否有效
	{
		printf("Error number.\n");
	}
	else
	{
		printf("Your choose is .");
		switch(week)    // 根据输入选择要输出的语句
		{
			case 1: printf("Monday.\n");
					break;
			case 2: printf("Tuseday.\n");
					break;
			case 3: printf("Wednesday.\n");
					break;
			case 4: printf("Thursday\n");
					break;
			case 5: printf("Friday.\n");
					break;
			case 6: printf("Saturday.\n");
					break;
			case 7: printf("Sunday.\n");
					break;
			default:
				printf("\n***error***\n");
		}
	}
}
