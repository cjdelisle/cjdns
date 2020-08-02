#ifndef SC_H
#define SC_H

/*
The set of scalars is \Z/l
where l = 2^252 + 27742317777372353535851937790883648493.
*/

#define sc_reduce cjdns_sc_reduce
#define sc_muladd cjdns_sc_muladd

extern void sc_reduce(unsigned char *);
extern void sc_muladd(unsigned char *,const unsigned char *,const unsigned char *,const unsigned char *);

#endif
