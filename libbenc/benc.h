#ifndef BENC_H
#define BENC_H

#include <stdbool.h>
/*#include <stdint.h>*/

/* Needed for size_t */
/*#include <stddef.h>*/

#include <memory/MemAllocator.h>
#include <io/Reader.h>
#include <io/Writer.h>

typedef struct bbuf_s               bbuf_t;
typedef struct bobj_s               bobj_t;
typedef int64_t                     benc_int_t;
typedef struct benc_bstr_s          benc_bstr_t;
typedef struct benc_list_entry_s    benc_list_entry_t;
typedef struct benc_dict_entry_s    benc_dict_entry_t;

enum benc_data_type { BENC_INT, BENC_BSTR, BENC_LIST, BENC_DICT };

/*
bool            benc_file(bobj_t *o, char *file_path);
bobj_t *        bdec_file(char *file_path);
 */

bbuf_t *        bbuf_new(size_t len, char *base);
void            bbuf_free(bbuf_t *b);
/*bbuf_t *        benc_mem(bobj_t *o);*/
bobj_t *        bdec_mem(bbuf_t *b);

bobj_t *        bobj_int_new(benc_int_t i);

bobj_t *        bobj_bstr_new(size_t len, char *bytes);

bobj_t *        bobj_list_new();
void            bobj_list_push(bobj_t *obj, bobj_t *elem);
bobj_t *        bobj_list_pop(bobj_t *obj);

bobj_t *        bobj_dict_new();
bobj_t *        bobj_dict_lookup(bobj_t *obj, benc_bstr_t *key);
bool            bobj_dict_insert(bobj_t *obj, benc_bstr_t *key, bobj_t *val);
bobj_t *        bobj_dict_remove(bobj_t *obj, benc_bstr_t *key);

void            bobj_free(bobj_t *o);

/**
 * Print a bobject in human readable format.
 *
 * @param writer the Writer to write to.
 * @param obj the object to print.
 * @return whatever the Writer returns when writing or -2
 *         if the type of object cannot be determined.
 */
int bobj_print(struct Writer* writer,
               bobj_t* obj);

/**
 * Serialize a benc object into a bencoded string.
 *
 * @param writer a Writer which to write the output to.
 * @param obj the object to serialize.
 * @return -2 if the type of object cannot be determined, otherwise
 *            whatever is returned by the Writer.
 */
int bobj_serialize(struct Writer* writer,
                   bobj_t* obj);

/**
 * Parse some kind of bencoded object.
 *
 * @param reader a Reader which will provide the content to parse. This is expected to
 *               have it's pointer aligned on the first character of the object and the
 *               type of object will be detected from the first character. The pointer
 *               will be left on the first char AFTER the end of the object.
 * @param allocator a memory allocator which will be used to get the memory needed to store
 *                  the parsed data.
 * @param objPointer a pointer which will be set to the object which is parsed.
 * @return 0 if everything goes well, -1 if NULL returned by write() indicating an array
 *           overflow, -2 if -1 returned by read indicating an array underflow,
 *           -3 if content unparsable.
 */
int bobj_parse(struct Reader* reader,
               struct MemAllocator* allocator,
               bobj_t** objPointer);

/**
 * Create a new MemAllocator which writes to memory allocated on the heap.
 * Since writer->write() returns a pointer to the location in memory, when this
 * writer runs out of memory it will allocate another block and continue writing there.
 * It is critical that writer->free() is called when it is finished since it will have
 * multiple pointers to separate memory blocks.
 *
 * @param blockSize the number of bytes which will be allocated in each allocation.
 *                  for optimum performance make this larger than you expect the content to be.
 */
/*struct MemAllocator MemAllocator_allocating(const size_t blockSize);*/


struct bbuf_s {
    char                         *base;
    size_t                       len;
    char                         *ptr;
};

struct bobj_s {
    enum benc_data_type          type;
    union {
        benc_int_t               int_;
        benc_bstr_t              *bstr;
        benc_list_entry_t        *list;
        benc_dict_entry_t        *dict;
    } as;
};

struct benc_bstr_s {
    size_t                       len;
    char                      *bytes;
};

struct benc_list_entry_s {
    benc_list_entry_t            *next;
    bobj_t                       *elem;
};

struct benc_dict_entry_s {
    benc_dict_entry_t            *next;
    benc_bstr_t                  *key;
    bobj_t                       *val;
};

#endif        /* #ifndef BENC_H */
