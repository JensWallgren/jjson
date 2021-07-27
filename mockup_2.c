#include <stdio.h>

#include "jjson.h"

int main (int argc, char **argv) {

    jj_context doc = {0};

    jj_root_begin(&doc);
    jj_number(&doc, "id", 9000237);
    jj_root_end(&doc);
    /*
    jj_root(&doc) {
        jj_number(&doc, "id", 9000237);
    };
    */

    return 0;
}
