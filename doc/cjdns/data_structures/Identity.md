### Identity

We don't know how its mistress works maybe we don't want to know

```javascript
#if defined(Identity_CHECK)

    /** This goes in each structure which will be checked. */
    #define Identity \
        unsigned long Identity_verifier;

    #define Identity_set(pointer) \
        (pointer)->Identity_verifier = Identity_MAGIC

    #define Identity_check(pointer) \
        (__extension__ ({                                                      \
            __typeof__(pointer) Identity_ptr = (pointer);                      \
            Assert_true(Identity_ptr->Identity_verifier == Identity_MAGIC);  \
            Identity_ptr;                                                      \
        }))

    #define Identity_ncheck(pointer) \
        (__extension__ ({                                                                       \
            __typeof__(pointer) Identity_ptr = (pointer);                                       \
            Assert_true(!Identity_ptr || Identity_ptr->Identity_verifier == Identity_MAGIC);  \
            Identity_ptr;                                                                       \
        }))

#else
    #define Identity
    #define Identity_set(pointer)

    #define Identity_check(pointer) \
        (__extension__ ({                                                      \
            (pointer);                                                         \
        }))

    #define Identity_ncheck(pointer) Identity_check(pointer)
#endif
```
