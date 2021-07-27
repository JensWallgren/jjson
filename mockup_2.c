#include <stdio.h>

#include "jjson.h"

int main (int argc, char **argv) {

    jj_context doc = {0};

    jj_root(&doc) {
        jj_number(&doc, "id", 9000237);
        jj_string(&doc, "name", "yes");
        jj_object(&doc, "address") {
            jj_string(&doc, "street", "Creek Street");
            jj_number(&doc, "street_number", 10);
            jj_number(&doc, "zip_code", 58710);
        };
        jj_number(&doc, "items_bought", 3);
    };

    return 0;
}
