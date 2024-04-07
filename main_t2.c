/* Ionita Alexandra-Mihaela - 314CB */

#include "t2.h"

int main(int argc, char const *argv[]) {
    Image image;
    if (!strcmp(argv[1], "-c1") || !strcmp(argv[1], "-c2")) {
        req_1_2(argv[3], argv[2], argv[1], argv[4]);
    } else {
        req_3(argv[2], argv[3]);
    }

    return 0;
}
