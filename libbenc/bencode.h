#ifndef BENCODE_H
#define BENCODE_H

#include "benc.h"

/**
 * Print a bobject of unknown type in human readable format.
 *
 * @param writer the Writer to write to.
 * @param padSpaceCount the number of spaces to pad if multiple lines are needed.
 * @param obj the object to print.
 * @return whatever the Writer returns when writing or -2
 *         if the type of object cannot be determined.
 */
int benc_bobj_print(const struct Writer* writer,
                    size_t padSpaceCount,
                    const bobj_t* obj);

/**
 * Print a string in human readable format.
 * Unprintable characters are escaped.
 *
 * @param writer the Writer to write to.
 * @param string the string to write.
 * @return whatever the Writer returns when writing.
 */
int benc_bstr_print(const struct Writer* writer,
                    const benc_bstr_t* string);

/**
 * Serialize a string and write to a writer.
 *
 * @param writer the Writer to write to.
 * @param string the string to write.
 * @return whatever the Writer returns when writing.
 */
int benc_bstr_serialize(const struct Writer* writer,
                        const benc_bstr_t* string);

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
int benc_bstr_parse(const struct Reader* reader,
                    const struct MemAllocator* allocator,
                    benc_bstr_t** stringPointer);

/**
 * Write an integer as decimal in human readable format.
 *
 * @param writer the Writer to write to.
 * @param integer the number to write.
 * @return whatever the Writer returns when writing.
 */
int benc_int_print(const struct Writer* writer,
                   const benc_int_t integer);

/**
 * Write an integer as decimal in bencoded format.
 * the integer 10 would be written as "i10e"
 *
 * @param writer the Writer to write to.
 * @param integer the number to write.
 * @return whatever the Writer returns when writing.
 */
int benc_int_serialize(const struct Writer* writer,
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
int benc_int_parse(const struct Reader* reader,
                   benc_int_t* intPointer);

/**
 * Write a list in human readable format.
 *
 * @param writer the Writer to write to.
 * @param padSpaceCount the number of spaces to put in front of the list.
 * @param head the top entry of the list.
 * @return whatever the Writer returns when writing.
 */
int benc_list_print(const struct Writer* writer,
                    size_t padSpaceCount,
                    const benc_list_entry_t* head);

/**
 * Serialize a list.
 *
 * @param writer the Writer to write to.
 * @param head the top entry of the list.
 * @return whatever the Writer returns when writing.
 */
int benc_list_serialize(const struct Writer* writer,
                        const benc_list_entry_t* head);

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
int benc_list_parse(const struct Reader* reader,
                    const struct MemAllocator* writer,
                    benc_list_entry_t** listPointer);

/**
 * Write a dictionary in human readable format.
 *
 * @param writer the Writer to write to.
 * @param padSpaceCount the number of spaces to put in front of the dictionary.
 * @param head the top entry of the dictionary.
 * @return whatever the Writer returns when writing.
 */
int benc_dict_print(const struct Writer* writer,
                    size_t padSpaceCount,
                    const benc_dict_entry_t* head);

/**
 * Serialize a dictionary.
 *
 * @param writer the Writer to write to.
 * @param integer the number to write.
 * @return whatever the Writer returns when writing.
 */
int benc_dict_serialize(const struct Writer* writer,
                        const benc_dict_entry_t* head);

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
int benc_dict_parse(const struct Reader* reader,
                    const struct MemAllocator* allocator,
                    benc_dict_entry_t** headPointer);

#endif        /* #ifndef BENCODE_H */
