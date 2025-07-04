
```
acl add group 0 entry 0 forward-ext-entry field-mode
acl entry 0 add key-field field-port gport 8

stats create statsid 1000 acl priority 0 ingress 
acl entry 0 add action-field stats 1000

acl entry 0 add key-field discard 1 1
acl install entry 0

```

