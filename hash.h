#pragma once

unsigned long hash(const unsigned char* string) {

    unsigned long hash = 5381;

    unsigned char c;

    while ((c = *string++)) {
        hash = ((hash << 5) + hash) + c;  /* hash * 33 + c */
    }

    return hash;
}