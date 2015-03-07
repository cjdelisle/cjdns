### Address

- uint32_t protocolVersion;
  - The protocol version of the node.

- uint32_t padding;
  - unused
```javascript
union {
    struct {
        // tricksy: this is effectively a 64 bit rotate of the following bytes array
        uint32_t three_be;
        uint32_t four_be;
        uint32_t one_be;
        uint32_t two_be;
    } ints;

    struct {
        uint64_t two_be;
        uint64_t one_be;
    } longs;
xxxxi  - $diwx
    uint8_t bytes[Address_SEARCH_TARGET_SIZE];
} ip6;
```
- uint8_t key[Address_KEY_SIZE];

- uint64_t path;  
TODO: This struct is in dire need of some annotation
