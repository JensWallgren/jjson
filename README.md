# jjson
## Proof-of-concept library for Json serialization in C

This small library illustrates how Json serializer interfaces could be built in a readable and close-to-output way. Writing json in C normally means calling ```x_begin(context)``` and ```x_end(context)```-style functions. This isn't ***hard*** or ***tricky*** to do - but the C normally looks looking nothing like the json output. In contrast to this - Json code in Python or Javascript actually *looks* like the expected output, which is a nice feature for ease of use and *particularly* for clarity of the code.

However, with a simple macro trick we can make this happen in C. This library is a proof of concept of this - at the moment not intendet for production use. *I originally came across this macro trick in a snippet of GUI code (credit to Ryan of https://dion.systems/)*.

## Example of usage
#### Expected output
```json
{
  "id": 9000237,
  "address": {
    "street": "Creek Street",
    "street_number": 10
  },
  "an_array": [
    "value",
    100
  ]
}
```
#### jjson code
```c
    // Initialize a json context/handle to your json document
    jj_ctx doc = {0};

    // Populate the document with data
    jj_root(&doc) {
        jj_num("id", 9000237);
        jj_obj("address") {
            jj_str("street", "Creek Street");
            jj_num("street_number", 10);
        };
        jj_arr("an_array") {
            jj_ustr("value"); // unnamed string
            jj_unum(100);
        };
    };
    
    // Serialize and print
    printf( jj_serialize(doc) );
```

#### Minified vs. Pretty json
All output is "pretty" by default. I.e. with spacing, newlines and indents. To output minified json - use the ```jj_serialize_minified``` function.
```c
// Output pretty json
char *pretty = jj_serialize(doc);

// Output minified json
char *minified = jj_serialize_minified(doc);
```

