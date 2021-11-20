
#include <stdio.h>
#include "jjson.h"
int main() {
    jj_ctx doc = {0};

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


    printf( jj_serialize(doc) );
    printf( "\n------------\n");
    printf( jj_serialize_minified(doc) );
}

