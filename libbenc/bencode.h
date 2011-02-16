#ifndef BENCODE_H
#define BENCODE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#include "benc.h"

void benc_log_exception(const char *file, int line, const char *func, const char *msg, ...);
void benc_log_syscall(const char *file, int line, const char *func, const char *syscall_name, const char *msg, ...);

#define BENC_LOG_EXCEPTION(...) (benc_log_exception(__FILE__, __LINE__, __func__, __VA_ARGS__))
#define BENC_LOG_SYSCALL_ERROR(...) (benc_log_exception(__FILE__, __LINE__, __func__, __VA_ARGS__))

bobj_t *        bobj_new(enum benc_data_type type);
size_t          bobj_repsize(bobj_t *o);
void            bobj_encode(bbuf_t *b, bobj_t *o);

bool bbuf_inc_ptr(bbuf_t *b);

size_t          benc_int_repsize(benc_int_t i);
void            benc_int_encode(bbuf_t *b, benc_int_t i);
bool            benc_int_decode(bbuf_t *b, benc_int_t *i_p);

benc_bstr_t *   benc_bstr_new(size_t len, char *bytes);
void            benc_bstr_free(benc_bstr_t *s);
size_t          benc_bstr_repsize(benc_bstr_t *s);
void            benc_bstr_encode(bbuf_t *b, benc_bstr_t *s);
bool            benc_bstr_decode(bbuf_t *b, benc_bstr_t **s_p);
int             benc_bstr_compare(benc_bstr_t *a, benc_bstr_t *b);

/**
 * Print a bobject of unknown type in human readable format.
 *
 * @param writer the Writer to write to.
 * @param padSpaceCount the number of spaces to pad if multiple lines are needed.
 * @param obj the object to print.
 * @return whatever the Writer returns when writing or -2
 *         if the type of object cannot be determined.
 */
int benc_bobj_print(struct Writer* writer,
                    size_t padSpaceCount,
                    bobj_t* obj);

/**
 * Print a string in human readable format.
 * Unprintable characters are escaped.
 *
 * @param writer the Writer to write to.
 * @param string the string to write.
 * @return whatever the Writer returns when writing.
 */
int benc_bstr_print(struct Writer* writer,
                    benc_bstr_t* string);

/**
 * Serialize a string and write to a writer.
 *
 * @param writer the Writer to write to.
 * @param string the string to write.
 * @return whatever the Writer returns when writing.
 */
int benc_bstr_serialize(struct Writer* writer,
                        benc_bstr_t* string);

/**
 * Parse a string, reading in with the first callback and writing to the second.
 *
 * @param reader a Reader which will be asked for the data to parse. This implementation
 *               assumes the reader's pointer is alligned on the first digit of the length
 *               of the string and will leave the read pointer on the first character AFTER
 *               the end of the string.
 * @param writer a MemAllocator which will be used to store data.
 * @param stringPointer a pointer which will be set to the benc_bstr_t struct.
 * @return 0 if everything goes well, -1 if NULL returned by the writer indicating an array
 *           overflow, -2 if -1 returned by the reader indicating an array underflow,
 *           -3 if content unparsable.
 */
int benc_bstr_parse(struct Reader* reader,
                    struct MemAllocator* allocator,
                    benc_bstr_t** stringPointer);

/**
 * Write an integer as decimal in human readable format.
 *
 * @param writer the Writer to write to.
 * @param integer the number to write.
 * @return whatever the Writer returns when writing.
 */
int benc_int_print(struct Writer* writer,
                           benc_int_t integer);

/**
 * Write an integer as decimal in bencoded format.
 * the integer 10 would be written as "i10e"
 *
 * @param writer the Writer to write to.
 * @param integer the number to write.
 * @return whatever the Writer returns when writing.
 */
int benc_int_serialize(struct Writer* writer,
                       benc_int_t integer);

/**
 * Parse an integer, read in with the reader and set the intPointer to the value of the int.
 *
 * @param reader a Reader which will be asked for the data to parse. This implementation
 *               assumes the reader's pointer is alligned on the 'i' which begins the integer
 *               and will leave the read pointer on the first character AFTER the 'e' which
 *               ends the integer.
 * @param intPointer a pointer to a memory location which will be set to the valie of the int.
 * @return 0 if everything goes well, -2 if -1 returned by the reader indicating an
 *           array underflow, -3 if content unparsable.
 */
int benc_int_parse(struct Reader* reader,
                   benc_int_t* intPointer);

/**
 * Write a list in human readable format.
 *
 * @param writer the Writer to write to.
 * @param padSpaceCount the number of spaces to put in front of the list.
 * @param head the top entry of the list.
 * @return whatever the Writer returns when writing.
 */
int benc_list_print(struct Writer* writer,
                    size_t padSpaceCount,
                    benc_list_entry_t* head);

/**
 * Serialize a list.
 *
 * @param writer the Writer to write to.
 * @param head the top entry of the list.
 * @return whatever the Writer returns when writing.
 */
int benc_list_serialize(struct Writer* writer,
                        benc_list_entry_t* head);

/**
 * Parse a list.
 *
 * @param reader a Reader which will be asked for the data to parse. This implementation
 *               assumes the reader's pointer is alligned on the 'l' which signifies the
 *               beginning of the list. This will leave the pointer on the first character
 *               AFTER the 'e' which signifies the end of the list.
 * @param writer a MemAllocator which will be used to store data.
 * @param listPointer a pointer which will be set to the location of the benc_list_t.
 * @return 0 if everything goes well, -1 if NULL returned by the writer indicating an array
 *           overflow, -2 if -1 returned by the reader indicating an array underflow,
 *           -3 if content unparsable.
 */
int benc_list_parse(struct Reader* reader,
                    struct MemAllocator* writer,
                    benc_list_entry_t** listPointer);

/**
 * Write a dictionary in human readable format.
 *
 * @param writer the Writer to write to.
 * @param padSpaceCount the number of spaces to put in front of the dictionary.
 * @param head the top entry of the dictionary.
 * @return whatever the Writer returns when writing.
 */
int benc_dict_print(struct Writer* writer,
                    size_t padSpaceCount,
                    benc_dict_entry_t* head);

/**
 * Serialize a dictionary.
 *
 * @param writer the Writer to write to.
 * @param integer the number to write.
 * @return whatever the Writer returns when writing.
 */
int benc_dict_serialize(struct Writer* writer,
                        benc_dict_entry_t* head);

/**
 * Parse a dictionary, reading in with the first callback and writing to the second.
 *
 * @param reader a Reader which will be asked for the data to parse. This implementation
 *               assumes the reader's pointer is alligned on the 'd' which indicates
 *               dictionary and will leave the read pointer on the first character AFTER
 *               the 'e' which ends the dictionary.
 * @param allocator a MemAllocator which will be used to store data.
 * @param headPointer a pointer which will be set to the head of the dictionary.
 * @return 0 if everything goes well, -1 if NULL returned by write() indicating an array
 *           overflow, -2 if -1 returned by read indicating an array underflow,
 *           -3 if content unparsable.
 */
int benc_dict_parse(struct Reader* reader,
                    struct MemAllocator* allocator,
                    benc_dict_entry_t** headPointer);

void            benc_list_free(benc_list_entry_t *head);
size_t          benc_list_repsize(benc_list_entry_t *head);
void            benc_list_encode(bbuf_t *b, benc_list_entry_t *head);
bool            benc_list_decode(bbuf_t *b, benc_list_entry_t **head_p);

void            benc_dict_free(benc_dict_entry_t *head);
size_t          benc_dict_repsize(benc_dict_entry_t *head);
void            benc_dict_encode(bbuf_t *b, benc_dict_entry_t *head);
bool            benc_dict_decode(bbuf_t *b, benc_dict_entry_t **head_p);

#endif        /* #ifndef BENCODE_H */
