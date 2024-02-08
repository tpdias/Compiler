#include <stdlib.h>
#include <stdio.h>
int a = 5, b = 0, c = 5;
int j(int param1, int param2, int param3, int param4) {
    return param1;
}
int x(int param1) {
    a = c;
    return b;
}

int main() {
    j(a, b, c, a);
    x(a);
    return 0;
}
