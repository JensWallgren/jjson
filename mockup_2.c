#include <stdio.h>

#include "jjson.h"

int main (int argc, char **argv) {

    jj_context doc = {0};
    jj_root(&doc) {
        jj_num("id", 9000237);
        jj_obj("address") {
            jj_str("street", "Creek Street");
            jj_num("street_number", 10);
        };
        jj_arr("tags") {
            jj_ustr("intro deal");
            jj_ustr("other deal");
            jj_unum(100);
        };
    };

    printf( jj_serialize(&doc) );
}



