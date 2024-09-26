#include <stdio.h>
:
void __itoa(char *s, int number, int base)
{
  if (number < 0)
    *s++ = '-';

  char *p = s;

  for (; number; number /= base)
    *s++ = number % base + '0';

  unsigned int length = s - 1 - p;

  for (int i = 0; i < length / 2; i++)
    p[i] ^= p[length - i] ^= p[i] ^= p[length - i];

  *s = 0x0;
}

int main()
{
  char s[32];
  __itoa(s, 123456789, 10);
  printf("%s", s);
}
