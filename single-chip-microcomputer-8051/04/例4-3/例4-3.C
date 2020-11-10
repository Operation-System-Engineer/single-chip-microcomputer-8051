/* 当型循环结构示例 */

#include <stdio.h>

void main()
{
	long sum = 0;
	long mul = 1;
	char i;
	float result;

	for(i=1; i<=10; i++)    // 循环体
	{
		mul = mul*i;    // 计算阶乘
		sum += mul;    // 求和
	}

	result = sum/10;    // 求均值

	printf("The result is %f", result);
}
