# jjson
## Proof-of-concept library for Json serialization in C

This small library illustrates how Json serializer interfaces could be built in a readable and close-to-output way.
At the core of it is a macro trick that I came across in a GUI code example (credit to Ryan of https://dion.systems/)

## Example of usage
#### Expected output
```json
{
  "id": 5000,
  "wow": {
    "foo": "bar",
    "yes": 20
  }
}
```
#### jjson code
```c
jj_root() {
  jj_num("id", 5000);
  jj_obj("wow") {
    jj_str("foo", "bar");
    jj_num("yes", 20);
  };
}
```
