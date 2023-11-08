
检查MAC地址是否全0，是否相等：
```
#define MAC_ADDR_IS_ZERO(mac) ((*(ui32_t*) mac == 0) && (*(ui16_t*) (&mac[4]) == 0))
#define MAC_ADDR_IS_EQUAL(X, Y) ((*(ui32_t*) X == *(ui32_t*) Y) && (*(ui16_t*) (&X[4]) == *(ui16_t*) (&Y[4])))
#define MAC_ADDR_IS_MULTICAST(mac) (mac[0] & 0x01 == 0x01)
#define MAC_ADDR_IS_BROADCAST(mac) (mac[0] == 0xff)
```


