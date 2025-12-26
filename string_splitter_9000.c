#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Base64 encoding table
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Base64 encode function
int base64_encode(const unsigned char *input, int input_len, char *output, int output_size) {
    int i, j = 0;
    unsigned char a, b, c;
    
    for (i = 0; i < input_len; i += 3) {
        a = input[i];
        b = (i + 1 < input_len) ? input[i + 1] : 0;
        c = (i + 2 < input_len) ? input[i + 2] : 0;
        
        if (j + 4 >= output_size) return -1; // Buffer overflow protection
        
        output[j++] = base64_table[(a >> 2) & 0x3F];
        output[j++] = base64_table[((a << 4) | (b >> 4)) & 0x3F];
        
        if (i + 1 < input_len) {
            output[j++] = base64_table[((b << 2) | (c >> 6)) & 0x3F];
        } else {
            output[j++] = '=';
        }
        
        if (i + 2 < input_len) {
            output[j++] = base64_table[c & 0x3F];
        } else {
            output[j++] = '=';
        }
    }
    output[j] = '\0';
    return j;
}

// Language syntax formatting functions
void print_chunk_bash(const char *chunk, int chunk_len) {
    printf("str=\"${str}");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("\"\n");
}

void print_chunk_python(const char *chunk, int chunk_len) {
    printf("str += \"");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("\"\n");
}

void print_chunk_ruby(const char *chunk, int chunk_len) {
    printf("str += \"");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("\"\n");
}

void print_chunk_c(const char *chunk, int chunk_len) {
    printf("strcat(str, \"");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("\");\n");
}

void print_chunk_cpp(const char *chunk, int chunk_len) {
    printf("str += \"");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("\";\n");
}

void print_chunk_csharp(const char *chunk, int chunk_len) {
    printf("str += \"");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("\";\n");
}

void print_chunk_perl(const char *chunk, int chunk_len) {
    printf("$str .= \"");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("\";\n");
}

void print_chunk_java(const char *chunk, int chunk_len) {
    printf("str += \"");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("\";\n");
}

void print_chunk_javascript(const char *chunk, int chunk_len) {
    printf("str += \"");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("\";\n");
}

void print_chunk_vba(const char *chunk, int chunk_len) {
    printf("str = str & \"");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("\"\n");
}

void print_chunk_sql(const char *chunk, int chunk_len) {
    printf("SET str = str + '");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("'\n");
}

void print_chunk_r(const char *chunk, int chunk_len) {
    printf("str <- paste0(str, \"");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("\")\n");
}

void print_chunk_php(const char *chunk, int chunk_len) {
    printf("$str .= \"");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("\";\n");
}

void print_chunk_rust(const char *chunk, int chunk_len) {
    printf("str.push_str(\"");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("\");\n");
}

void print_chunk_go(const char *chunk, int chunk_len) {
    printf("str += \"");
    for (int i = 0; i < chunk_len; i++) {
        printf("%c", chunk[i]);
    }
    printf("\"\n");
}

// Function pointer type for language formatting
typedef void (*print_chunk_func)(const char *, int);

int main() {
    char str[500000];
    char encoded[700000]; // Base64 output is ~33% larger
    char choice[20];
    char encode_choice[10];
    char lang_choice[20];
    int n;
    int min_size, max_size;
    int len;
    int encode_plaintext = 0;
    
    // Ask about base64 encoding
    printf("Do you have plaintext you would like to encode? (yes/no): ");
    fgets(encode_choice, sizeof(encode_choice), stdin);
    int encode_len = strlen(encode_choice);
    if (encode_len > 0 && encode_choice[encode_len - 1] == '\n') {
        encode_choice[encode_len - 1] = '\0';
    }
    
    if (strcmp(encode_choice, "yes") == 0 || strcmp(encode_choice, "y") == 0) {
        encode_plaintext = 1;
        printf("Enter the plaintext to encode: ");
        fflush(stdout);
        
        if (fgets(str, sizeof(str), stdin) == NULL) {
            printf("Error reading input.\n");
            return 1;
        }
        
        len = strlen(str);
        if (len > 0 && str[len - 1] == '\n') {
            str[len - 1] = '\0';
            len--;
        }
        
        // Encode to base64
        int encoded_len = base64_encode((unsigned char *)str, len, encoded, sizeof(encoded));
        if (encoded_len < 0) {
            printf("Error: Base64 encoding failed (buffer too small).\n");
            return 1;
        }
        
        // Copy encoded string back to str for processing
        strncpy(str, encoded, sizeof(str) - 1);
        str[sizeof(str) - 1] = '\0';
        len = strlen(str);
        printf("\nBase64 encoded string: %s\n\n", str);
    }
    
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
    
    // If not encoding, get the string now
    if (!encode_plaintext) {
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
        if (was_truncated) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
                // Discard leftover characters
            }
        }
    }
    
    // Language selection
    printf("\nSelect language syntax:\n");
    printf("1. Bash\n2. Python\n3. Ruby\n4. C\n5. C++\n6. C#\n7. Perl\n8. Java\n9. JavaScript\n10. VBA\n11. SQL\n12. R\n13. PHP\n14. Rust\n15. Go\n");
    printf("Enter choice (1-15): ");
    fgets(lang_choice, sizeof(lang_choice), stdin);
    int lang_num = atoi(lang_choice);
    
    // Set up function pointer
    print_chunk_func print_func = NULL;
    const char *lang_name = "";
    
    switch (lang_num) {
        case 1: print_func = print_chunk_bash; lang_name = "Bash"; break;
        case 2: print_func = print_chunk_python; lang_name = "Python"; break;
        case 3: print_func = print_chunk_ruby; lang_name = "Ruby"; break;
        case 4: print_func = print_chunk_c; lang_name = "C"; break;
        case 5: print_func = print_chunk_cpp; lang_name = "C++"; break;
        case 6: print_func = print_chunk_csharp; lang_name = "C#"; break;
        case 7: print_func = print_chunk_perl; lang_name = "Perl"; break;
        case 8: print_func = print_chunk_java; lang_name = "Java"; break;
        case 9: print_func = print_chunk_javascript; lang_name = "JavaScript"; break;
        case 10: print_func = print_chunk_vba; lang_name = "VBA"; break;
        case 11: print_func = print_chunk_sql; lang_name = "SQL"; break;
        case 12: print_func = print_chunk_r; lang_name = "R"; break;
        case 13: print_func = print_chunk_php; lang_name = "PHP"; break;
        case 14: print_func = print_chunk_rust; lang_name = "Rust"; break;
        case 15: print_func = print_chunk_go; lang_name = "Go"; break;
        default:
            printf("Invalid choice. Using default format.\n");
            print_func = print_chunk_python; // Default
            lang_name = "Python";
    }
    
    printf("\nOutput format: %s\n\n", lang_name);
    
    if (strcmp(choice, "fixed") == 0) {
        // Fixed length splitting
        for (int i = 0; i < len; i += n) {
            int chunk_len = (i + n < len) ? n : len - i;
            print_func(&str[i], chunk_len);
        }
    } else {
        // Variable size splitting
        srand(time(NULL));
        int i = 0;
        while (i < len) {
            int chunk_size = min_size + (rand() % (max_size - min_size + 1));
            if (i + chunk_size > len) {
                chunk_size = len - i;
            }
            print_func(&str[i], chunk_size);
            i += chunk_size;
        }
    }
    
    return 0;
}
