include ../common.mak

libbenc_obj=benc_bstr.o benc_dict.o benc_int.o benc_list.o bencode.o

all: libbenc.a

libbenc.a: $(libbenc_obj)
	$(AR) ru $@ $+

clean:
	-rm -f libbenc.a
	-rm -f $(libbenc_obj)