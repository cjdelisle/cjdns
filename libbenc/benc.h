#ifndef BENC_H
#define BENC_H

#include <memory/MemAllocator.h>
#include <io/Reader.h>
#include <io/Writer.h>

typedef struct bobj_s               bobj_t;
typedef int64_t                     benc_int_t;
typedef struct benc_bstr_s          benc_bstr_t;
typedef struct benc_list_entry_s    benc_list_entry_t;
typedef struct benc_dict_entry_s    benc_dict_entry_t;
#define String benc_bstr_t
/* Dictionaries and lists are pointers to the head entry so that the head can change. */
/*#define Dict benc_dict_entry_t* */
typedef benc_dict_entry_t* Dict;
#define List benc_list_entry_t*
#define Object bobj_t

enum benc_data_type { BENC_INT, BENC_BSTR, BENC_LIST, BENC_DICT };

bobj_t *        bobj_dict_lookup(bobj_t* obj, const benc_bstr_t* key);

/**
 * Remove an entry from the dictionary.
 *
 * @param dictionary the dictionary to remove the entry from.
 * @param key the key which the entry is entered under.
 * @return the value of the entry if it exists, otherwise NULL.
 */
Object* benc_removeEntry(Dict* dictionary, const String* key);

/**
 * Add an item to a list, if the list does not exist then it is allocated.
 * NOTE: This will not copy the given object, only add a pointer to it in the list.
 *
 * @param list the list to add an item to, if NULL then it is allocated.
 * @param item the item to add to the list.
 * @param allocator the means to get memory to store list item and possibly the newly allocated list.
 * @return the list after adding the item.
 */
List* benc_addObject(List* list, Object* item, const struct MemAllocator* allocator);

/**
 * Add a String to a list, if the list does not exist then it is allocated.
 * NOTE: This will not copy the given string, only add a pointer to it in the list.
 *
 * @param list the list to add the string item to, if NULL then it is allocated.
 * @param toAdd the string to add to the list.
 * @param allocator the means of getting memory space for storing the list entry.
 * @return the list after adding the string.
 */
List* benc_addString(List* list, String* toAdd, const struct MemAllocator* allocator);

/**
 * Put a key:value pair into a dictionary.
 * NOTE: This will not copy the given object,
 *       only add a pointer to it in the dictionary.
 * If dictionary is NULL then a new dictionary will be created and returned.
 *
 * @param dictionary this must be a bencoded dictionary or NULL, if NULL then a new dictionary is made.
 * @param key the reference key to use for putting the entry in the dictionary.
 * @param value the value to insert with the key.
 * @param allocator the means to get memory for storing the dictionary entry wrapper.
 * @return if the dictionary parameter is NULL then this will be the newly created dictionary.
 *         Otherwise: if the key already exists in the dictionary then the value which was
 *         displaced by the put, if not then NULL.
 */
Object* benc_putObject(Dict* dictionary,
                       const String* key,
                       Object* value,
                       const struct MemAllocator* allocator);

/**
 * Utility function which wraps value as a bencoded generic object and calls benc_putEntry()
 *
 * @param putIntoThis the dictionary to insert an entry into.
 * @param key the reference key to use for putting the entry in the dictionary.
 * @param value the string to insert.
 * @param allocator the means to get memory for storing the dictionary entry wrapper.
 * @return if the key already exists in the dictionary then the value which was
 *         displaced by the put, otherwise NULL.
 */
Object* benc_putString(Dict* putIntoThis,
                       const String* key,
                       String* value,
                       const struct MemAllocator* allocator);

/**
 * Insert a Dictionary object into another dictionary.
 * NOTE: This will not copy the given object,
 *       only add a pointer to it in the dictionary.
 *
 * @param putIntoThis the dictionary to insert an entry into.
 * @param key the reference key to use for putting the entry in the dictionary.
 * @param value the value to insert.
 * @param allocator the memory allocator to use for getting memory for the entry.
 * @return if the key already exists in the dictionary then the value which was
 *         displaced by the put, otherwise NULL.
 */
Object* benc_putDictionary(Dict* putIntoThis,
                           const String* key,
                           Dict* value,
                           const struct MemAllocator* allocator);

/**
 * Insert a List object into a dictionary.
 * NOTE: This will not copy the given object,
 *       only add a pointer to it in the dictionary.
 *
 * @param putIntoThis the dictionary to insert an entry into.
 * @param key the reference key to use for putting the entry in the dictionary.
 * @param value the list to insert.
 * @param allocator the memory allocator to use for getting memory for the entry.
 * @return if the key already exists in the dictionary then the value which was
 *         displaced by the put, otherwise NULL.
 */
Object* benc_putList(Dict* putIntoThis,
                     const String* key,
                     List* value,
                     const struct MemAllocator* allocator);

/**
 * Lookup a value from a dictionary type if the value is not present or is not a bencoded
 * string type then NULL is returned.
 *
 * @param dictionary the dictionary to look the entry up in.
 * @param key the key to look the entry up with.
 */
String* benc_lookupString(const Dict* dictionary, const String* key);

/**
 * Lookup a value from a dictionary type if the value is not present or is not a bencoded
 * dictionary type then NULL is returned.
 *
 * @param dictionary the dictionary to look the entry up in.
 * @param key the key to look the entry up with.
 */
Dict* benc_lookupDictionary(const Dict* dictionary, const String* key);

/**
 * Create a new bencoded dictionary type.
 *
 * @param allocator the place to allocate the memory for storing the dictionary.
 */
Dict* benc_newDictionary(const struct MemAllocator* allocator);

/**
 * Create a new bencoded integer from an integer. This will copy the integer into
 * the allocator.
 *
 * @param number the number to create a bencoded integer from.
 * @param allocator a means of getting the memory to store the bencoded number.
 * @return a bencoded integer object.
 */
bobj_t* benc_newInteger(int64_t number, const struct MemAllocator* allocator);

/**
 * Create a new bencoded string from a C null terminated string.
 * This implementation will make a copy of the string into the memory provided by the allocator.
 *
 * @param bytes the beginning of a memory location containing the string to use.
 * @param allocator a means of getting the memory to store the string object.
 * @return a bencoded string.
 */
String* benc_newString(const char* bytes, const struct MemAllocator* allocator);

/**
 * Create a new bencoded string from a set of bytes.
 * This implementation will make a copy of the string into the memory provided by the allocator.
 *
 * @param bytes the beginning of a memory location containing thre string to use.
 * @param length the number of bytes to use from the location.
 * @param allocator a means of getting the memory to store the string object.
 * @return a bencoded string.
 */
String* benc_newBinaryString(const char* bytes, size_t length, const struct MemAllocator* allocator);

/**
 * Compare 2 bencoded strings.
 * If the first differing character is numerically smaller for input a then
 * a negative number is returned, if the first differing character is numerically
 * smaller for input b then a positive number. If all characters in a and b are
 * the same then the difference in length (a->len - b->len) is returned.
 * If a is NULL and b is not NULL then a negative is returned, if b is NULL and a
 * not NULL then a positive is returned, if both NULL then 0.
 *
 * @param a the first string to compare.
 * @param b the second string to compare.
 * @return the output from comparison, 0 if and only if they are equal.
 */
int benc_bstr_compare(const benc_bstr_t* a, const benc_bstr_t* b);

/**
 * Will return 1 if and only if the benc_bstr_compare() would return 0.
 *
 * @param a the first string to compare.
 * @param b the second string to compare.
 * @return !(benc_bstr_compare(a, b))
 */
int32_t benc_stringEquals(const benc_bstr_t* a, const benc_bstr_t* b);

/**
 * Print a bobject in human readable format.
 *
 * @param writer the Writer to write to.
 * @param obj the object to print.
 * @return whatever the Writer returns when writing or -2
 *         if the type of object cannot be determined.
 */
int bobj_print(const struct Writer* writer,
               bobj_t* obj);

/**
 * Serialize a benc object into a bencoded string.
 *
 * @param writer a Writer which to write the output to.
 * @param obj the object to serialize.
 * @return -2 if the type of object cannot be determined, otherwise
 *            whatever is returned by the Writer.
 */
int bobj_serialize(const struct Writer* writer,
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
int bobj_parse(const struct Reader* reader,
               const struct MemAllocator* allocator,
               bobj_t** objPointer);

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
    const benc_bstr_t* key;
    bobj_t                       *val;
};

#endif        /* #ifndef BENC_H */
