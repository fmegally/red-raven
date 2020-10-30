#define BITSET(b,n)  ((b) |=  (1<<(n)))
#define BITCLR(b,n)  ((b) &= ~(1<<(n)))
#define BITFLIP(b,n) ((b) ^=  (1<<(n)))
#define BITGET(b,n)  ((b) &   (1<<(n)))
