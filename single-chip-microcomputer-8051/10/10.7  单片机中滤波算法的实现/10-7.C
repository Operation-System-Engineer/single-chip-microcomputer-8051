//在单片机系统中常用的滤波算法

//（1）程序判断法

#define A
char data;											// 上一次的数据
char filter_1()
{
char  datanew;									// 新数据变量
datanew = get_data();								// 获得新数据
 if ( (datanew – data > A ) || ( data – datanew > A )			// 滤波算法
 return data;
 return datanew;
}
//（2）中值滤波法

#define N  11
char filter_2()
{
char value_buf[N];
char count,i,j,temp;
for ( count=0;count<N;count++)					// 获取数据
{
value_buf[count] = get_data();
delay();
}
for (j=0;j<N-1;j++)							// 对数据进行排序
{
 for (i=0;i<N-j;i++)
{
if ( value_buf[i]>value_buf[i+1] )
{
temp = value_buf[i];
value_buf[i] = value_buf[i+1]; 
value_buf[i+1] = temp;
}
}
}
return value_buf[(N-1)/2];
}
//（3）算术平均滤波法

char filter_3()
{
int  sum = 0;
for ( count=0;count<N;count++)
{
 sum + = get_ad();
 delay();
}
return (char)(sum/N);
}

//（4）加权平均滤波法

char code jq[N] = {1,2,3,4,5,6,7,8,9,10,11,12};//coe数组为加权系数表，存在程序存储区。
char code sum_jq = 1+2+3+4+5+6+7+8+9+10+11+12;

char filter_4()
{
char count;
char value_buf[N];
int  sum=0;
for (count=0,count<N;count++)
{
 value_buf[count] = get_data();
 delay();
}
for (count=0,count<N;count++)
sum += value_buf[count]*jq[count];
return (char)(sum/sum_jq);
}
//（5）滑动平均滤波法

char value_buf[N];
char i=0;

char filter_5()
{
char count;
int  sum=0;
value_buf[i++] = get_ad();
if ( i == N )   i = 0;
for ( count=0;count<N,count++)
sum = value_buf[count];
return (char)(sum/N);
}
//（6）低通滤波法

char filter_6()
{
   char  new_value;
   new_value = get_data();
   return (100-a)*value + a*new_value; 
}
//（7）中位值平均滤波

char filter_7()
{
char count,i,j;
char value_buf[N];
int  sum=0;
for  (count=0;count<N;count++)
{
value_buf[count] = get_data();
delay();
}
for (j=0;j<N-1;j++)
{
for (i=0;i<N-j;i++)
{
if ( value_buf[i]>value_buf[i+1] )
{
temp = value_buf[i];
value_buf[i] = value_buf[i+1]; 
value_buf[i+1] = temp;
}
}
}
for(count=1;count<N-1;count++)
sum += value[count];
return (char)(sum/(N-2));
}
