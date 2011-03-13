#ifndef DHT_STORE_MODULE_H
#define DHT_STORE_MODULE_H

#include "libbenc/benc.h"

/**
 * Each type of storage must register one of these with the DHTStorageModule.
 * This is a storage provider but not a storage client. The storage itself must
 * be used for the client.
 */
struct DHTStoreModule
{
    /** The query type which is used to get data in this storage type. */
    const benc_bstr_t getQuery;

    /** The query type which is used to put data in this storage type. */
    const benc_bstr_t putQuery;

    /** The length of the key for looking up the entry. */
    const uint16_t keySize;

    /** The length of a signature if mutable data. If static data then 0. */
    const uint16_t signatureSize;

    /**
     * A function which validates an entry based on a key, signature and content.
     * This function will return 0 (false) unless the key and signature are
     * the lengths given by keySize and signatureSize respectively,
     *
     * @param key the lookup key. This must be the entire lookup key (it must be keySize length)
     * @param bencodedPacket the entire message dictionary as a benc_dict_entry_t wrapped in a bobj_t.
     * @return 0 unless the message is in the format given by this storage type and is
     *         successfully verified using the given public key, otherwise 1
     */
    int32_t (* const isValid)(const benc_bstr_t* key,
                              const bobj_t* bencodedMessage);

    /**
     * Return the number of minutes between "now" and the time when this entry was signed.
     * This is a signed integer because clock skew may cause negative results.
     *
     * @param bencodedMessageArguments the arguments section of the message as a
     *                                 benc_dict_entry_t wrapped in a bobj_t.
     * @return the number of minutes in the past when this message was signed. 0 for static
     *         messages which contain no datestamp.
     */
    int64_t (* const getAge)(const bobj_t* bencodedMessageArguments);

    /**
     * This is set when loading the module so that it can be referenced by a unique number.
     * It should not be set by the module itself and will be cleared if it is.
     */
    uint8_t number;
};

#endif /* DHT_STORE_MODULE_H */
