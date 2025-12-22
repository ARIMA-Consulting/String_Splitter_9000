#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main() {
    char str[500000];
    char choice[10];
    int n;
    int min_size, max_size;
    int len;
    
    printf("Do you want fixed length or variable string sizes? (fixed/variable): ");
    fgets(choice, sizeof(choice), stdin);
    
    // Remove newline from choice
    int choice_len = strlen(choice);
    if (choice_len > 0 && choice[choice_len - 1] == '\n') {
        choice[choice_len - 1] = '\0';
    }
    
    if (strcmp(choice, "fixed") == 0) {
        printf("What size would you like? ");
        scanf("%d", &n);
        getchar(); // consume the newline after scanf
    } else if (strcmp(choice, "variable") == 0) {
        printf("Enter minimum bound (number of characters): ");
        scanf("%d", &min_size);
        printf("Enter maximum bound (number of characters): ");
        scanf("%d", &max_size);
        getchar(); // consume the newline after scanf
    } else {
        printf("Invalid choice. Exiting.\n");
        return 1;
    }
    
    printf("Enter the string to be split: ");
    fflush(stdout);
    
    // Read the string
    if (fgets(str, sizeof(str), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    // Check if string was truncated (no newline and buffer is full)
    len = strlen(str);
    int was_truncated = (len == sizeof(str) - 1 && str[len - 1] != '\n');
    
    // Remove the newline character if present
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
        len--;
    }
    
    // Only clear leftover input if the buffer was truncated
    // This means there's more input (including the newline from Enter) waiting
    if (was_truncated) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            // Discard leftover characters until we find the newline
        }
    }
    
    if (strcmp(choice, "fixed") == 0) {
        // Fixed length splitting
        for (int i = 0; i < len; i += n) {
            printf("Str = Str + \"");
            for (int j = i; j < i + n && j < len; j++) {
                printf("%c", str[j]);
            }
            printf("\"\n");
        }
    } else {
        // Variable size splitting
        srand(time(NULL));
        int i = 0;
        int chunk_num = 0;
        while (i < len) {
            // Determine chunk size (between min and max)
            int chunk_size = min_size + (rand() % (max_size - min_size + 1));
            if (i + chunk_size > len) {
                chunk_size = len - i;
            }
            
            printf("Str = Str + \"");
            for (int j = i; j < i + chunk_size && j < len; j++) {
                printf("%c", str[j]);
            }
            printf("\"\n");
            
            i += chunk_size;
            chunk_num++;
        }
    }
    
    return 0;
}

