int __cdecl _inp(unsigned);
int __cdecl _outp(unsigned, int);

#pragma	intrinsic(_inp, _outp)

#define READ_PORT_UCHAR(a)  	_inp((unsigned)a)
#define WRITE_PORT_UCHAR(a, b)  _outp((unsigned)a, (int)b)
