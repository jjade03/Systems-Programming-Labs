#include <stdio.h>

int positive(int);
int negative(int);

int main() {
    int intIn = 0;

    printf("Please enter an integer: ");
    scanf("%d", &intIn);

    if(intIn >= 0) {
        positive(intIn);
    } else {
        negative(intIn);
    }

    return 0;
}

int positive(int intIn) {
    for(int i = 0; i <= intIn; i++) {
       if(i % 5 == 0 && i % 3 == 0) {
            printf("Fizzbuzz\n");
       }  else if(i % 3 == 0) {
            printf("Fizz\n");
       } else if(i % 5 == 0) {
            printf("Buzz\n");
       } else {
            printf("%d\n", i);
       }
    }
    return 0;
}

int negative(int intIn) {
    for(int i = 0; i > intIn; i--) {
       if(i % 5 == 0 && i % 3 == 0) {
            printf("Fizzbuzz\n");
       }  else if(i % 3 == 0) {
            printf("Fizz\n");
       } else if(i % 5 == 0) {
            printf("Buzz\n");
       } else {
            printf("%d\n", i);
       }
    }
    return 0;
}
