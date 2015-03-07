#### static int compareNodes(const struct [Node_Two][]* na, const struct [Node_Two][]* nb)
call [Address_xorcmp][] over 4 hunks of the ipv6 address 
in the address struct the data is stored like this:
uint32_t three_be;  
uint32_t four_be;  
uint32_t one_be;  
uint32_t two_be;  

compareNodes accesses them in counting order

ret = Address_xorcmp(0, na->address.ip6.ints.one_be, nb->address.ip6.ints.one_be);  
if (ret) { return ret; }
<BR>
ret = Address_xorcmp(0, na->address.ip6.ints.two_be, nb->address.ip6.ints.two_be);  
if (ret) { return ret; }
<BR>
ret = Address_xorcmp(0, na->address.ip6.ints.three_be, nb->address.ip6.ints.three_be);  
if (ret) { return ret; }
<BR>
ret = Address_xorcmp(0, na->address.ip6.ints.four_be, nb->address.ip6.ints.four_be);  

if all of these values from node a are equal to node b return 0
if something other than 0 from Address_xorcmp return that.
