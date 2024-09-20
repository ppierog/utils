#include<stdio.h>
#include <time.h>
int main() {

  int i=0;
  long long sum = 0;
  int n = 100000000;
  struct timespec res1,res2; 

  clock_gettime(CLOCK_MONOTONIC,&res1);
  
  for (int i=0;i<=n;++i)
    sum+=i;

  clock_gettime(CLOCK_MONOTONIC,&res2);

  printf("%lld\n",sum);
  printf("Difference: %ld\n", res2.tv_nsec - res1.tv_nsec);

} 
