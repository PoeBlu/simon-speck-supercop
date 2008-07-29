#include <stdlib.h>
#include "randombytes.h"
#include "cpucycles.h"
#include "crypto_hash.h"

extern void printentry(long long,const char *,long long *,long long);
extern unsigned char *alignedcalloc(unsigned long long);
extern const char *primitiveimplementation;
extern const char *implementationversion;
extern const char *sizenames[];
extern const long long sizes[];
extern void allocate(void);
extern void measure(void);

const char *primitiveimplementation = crypto_hash_IMPLEMENTATION;
const char *implementationversion = crypto_hash_VERSION;
const char *sizenames[] = { "bytes", 0 };
const long long sizes[] = { crypto_hash_BYTES };

#define MAXTEST_BYTES 4096

static unsigned char *h;
static unsigned char *m;

void allocate(void)
{
  h = alignedcalloc(crypto_hash_BYTES);
  m = alignedcalloc(MAXTEST_BYTES);
}

#define TIMINGS 15
static long long cycles[TIMINGS + 1];

static void printcycles(long long mlen)
{
  int i;
  for (i = 0;i < TIMINGS;++i) cycles[i] = cycles[i + 1] - cycles[i];
  printentry(mlen,"cycles",cycles,TIMINGS);
}

void measure(void)
{
  int i;
  int loop;
  int mlen;

  for (loop = 0;loop < 3;++loop) {
    for (mlen = 0;mlen <= MAXTEST_BYTES;mlen += 1 + mlen / 8192) {
      randombytes(m,mlen);
      for (i = 0;i <= TIMINGS;++i) {
        cycles[i] = cpucycles();
	crypto_hash(h,m,mlen);
      }
      printcycles(mlen);
    }
  }
}
