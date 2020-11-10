/* 顺序结构示例 */

#include <math.h>
#include <stdio.h>

void main()
{
	float x, y;
	float tmp;
	float result;
	
	x = 3;    
	y = 4;
	tmp = x*x+y*y;    // 计算表达式的值
	result = pow(tmp, 3);

	printf("The result is %d\n", result);    // 输出结果
