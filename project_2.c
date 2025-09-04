#include <stdio.h>
#include <stdlib.h>

int euclid(int a, int b) {

    int r;
    r = abs(a) % abs(b);

    if (r == 0) {
        return abs(b);
    }
    return euclid(abs(b), r);
}

int main() {

    int a, b;

    int res = scanf("%d %d", &a, &b);
    if (res != 2 || b == 0) {
        printf("Неверный формат ввода\n");
        abort();
    }
    int gcd = euclid(a, b);
    printf("%d\n", gcd);
}