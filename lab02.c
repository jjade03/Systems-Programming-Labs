#include <stdio.h>

int main() {
    char inString[100];
    int i = 0; // Current position in array
    int word = 0; // Starting position of the word

    printf("Enter a string: ");
    scanf("%[^\n]s", inString); // Allows for whitespace

    for(int k = 0; k <= sizeof(inString) && inString[i] != 0; k++) {
        if((inString[i] >= 'a' && inString[i] <= 'z') || (inString[i] >= 'A' && inString[i] <= 'Z')) {
            for(int z = word; z <= k; z++) {
                printf("%c", inString[z]);
            } // Outputs the word, adding one letter at a time per line
            printf("\n");
        } else {
            i++;
            word = i; // Sets the beginning point at the start of the new word
            continue;
        }
        i++;
    } // Loops until end of array is reached

    return 0;
}

