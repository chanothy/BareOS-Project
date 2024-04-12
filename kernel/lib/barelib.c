
void* memset(void* s, int c, int n) {
  while (n-- >= 0) ((char*)s)[n] = (char)c;
  return s;
}


void* memcpy(void* dst, const void* src, int n) {
  while (--n >= 0) ((char*)dst)[n] = ((char*)src)[n];
  return dst;
}

int strcomp(const char *c1, const char *c2) {
  while(*c1 && (*c1 == *c2)) {
    c1++;
    c2++;
  }
  return *(const unsigned char *)c1 - *(const unsigned char *)c2;
}
