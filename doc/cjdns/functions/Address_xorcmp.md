##### int Address_xorcmp(uint32_t target, uint32_t negativeIfCloser, uint32_t positiveIfCloser)
```javascript
if (negativeIfCloser == positiveIfCloser) {
    return 0;
}
```


```javascript
uint32_t ref = Endian_bigEndianToHost32(target);
return ((Endian_bigEndianToHost32(negativeIfCloser) ^ ref)
           < (Endian_bigEndianToHost32(positiveIfCloser) ^ ref)) ? -1 : 1;
```
Endian_bigEndianToHost32 is a macro with a large branching trail what you need to understand about it is
if machine endian ness is big return what was passed 

without the macros it looks like:

```javascript
uint32_t ref = target;
return ((negativeIfCloser ^ ref) < (positiveIfCloser ^ ref)) ? -1 : 1;
```
simple stuff right?
TODO: explain how the build config files tie in to this Endian business.  
