#include <stdio.h>

#include "jjson.h"

int main (int argc, char **argv) {

    jj_context doc = {0};


    jj_root(&doc) {
        jj_number(&doc, "id", 9000237);
        jj_object(&doc, "buyer") {
            jj_number(&doc, "id", 23255);
            jj_string(&doc, "name", "Mikromjuk AB");
            jj_keyword(&doc, "active", jj_true);

            jj_object(&doc, "address")
                jj_number(&doc, "zip_code", 58732);
        };

        jj_object(&doc, "product") {
            jj_number(&doc, "id", 9839820);
            jj_object(&doc, "status") {
                jj_keyword(&doc, "sent", jj_true);
                jj_keyword(&doc, "delivered", jj_false);
            };

            jj_object(&doc, "name") {
                jj_string(&doc, "en", "SaaS Software stuff");
                jj_string(&doc, "lt", "Lorem Ipsum");
                jj_string(&doc, "sv", "SaaS-mjukvara-grejer");
            };
        };

    };


    return 0;
}
