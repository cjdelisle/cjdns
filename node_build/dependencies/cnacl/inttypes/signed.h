#define ASSERT(exp) if (!(exp)) return 100
#define DOIT(bits,target) \
int main() \
{ \
  target x; \
  target y; \
  int i; \
 \
  x = 1; \
  for (i = 0;i < bits-1;++i) { \
    y = x; \
    if (i >= (bits-2)) x += (x - 1); \
      else x += x; \
    ASSERT(y > 0); \
    ASSERT(x > y); \
  } \
  y = 0; \
  x = y - 1; \
  ASSERT(y > x); \
  for (i = 0;i < bits-1;++i) { \
    y = x; \
    if (i >= (bits-2)) x += (x + 1); \
      else x += x; \
    ASSERT(y < 0); \
    ASSERT(x < y); \
  } \
 \
  return 0; \
}
