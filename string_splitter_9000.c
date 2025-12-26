#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

#define MAX_BUFFER_SIZE (10 * 1024 * 1024)  // 10MB buffer for very large inputs

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
        
        if (j + 4 >= output_size) return -1;
        
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

// Language syntax formatting functions (now accept FILE*)
void print_chunk_bash(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "str=\"${str}");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "\"\n");
}

void print_chunk_python(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "str += \"");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "\"\n");
}

void print_chunk_ruby(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "str += \"");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "\"\n");
}

void print_chunk_c(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "strcat(str, \"");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "\");\n");
}

void print_chunk_cpp(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "str += \"");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "\";\n");
}

void print_chunk_csharp(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "str += \"");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "\";\n");
}

void print_chunk_perl(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "$str .= \"");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "\";\n");
}

void print_chunk_java(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "str += \"");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "\";\n");
}

void print_chunk_javascript(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "str += \"");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "\";\n");
}

void print_chunk_vba(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "str = str & \"");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "\"\n");
}

void print_chunk_sql(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "SET str = str + '");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "'\n");
}

void print_chunk_r(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "str <- paste0(str, \"");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "\")\n");
}

void print_chunk_php(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "$str .= \"");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "\";\n");
}

void print_chunk_rust(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "str.push_str(\"");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "\");\n");
}

void print_chunk_go(FILE *out, const char *chunk, int chunk_len) {
    fprintf(out, "str += \"");
    for (int i = 0; i < chunk_len; i++) {
        fprintf(out, "%c", chunk[i]);
    }
    fprintf(out, "\"\n");
}

// Function pointer type for language formatting
typedef void (*print_chunk_func)(FILE *, const char *, int);

// Read input from file or stdin
char* read_input(const char *filename, size_t *len) {
    FILE *fp;
    char *buffer;
    size_t buffer_size = 1024 * 1024;  // Start with 1MB
    size_t total_read = 0;
    size_t n;
    
    if (filename && strcmp(filename, "-") != 0) {
        fp = fopen(filename, "rb");
        if (!fp) {
            fprintf(stderr, "Error: Cannot open file '%s': %s\n", filename, strerror(errno));
            return NULL;
        }
    } else {
        fp = stdin;
    }
    
    buffer = malloc(buffer_size);
    if (!buffer) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        if (fp != stdin) fclose(fp);
        return NULL;
    }
    
    while ((n = fread(buffer + total_read, 1, buffer_size - total_read, fp)) > 0) {
        total_read += n;
        if (total_read >= buffer_size - 1) {
            if (buffer_size >= MAX_BUFFER_SIZE) {
                fprintf(stderr, "Error: Input too large (max %d bytes)\n", MAX_BUFFER_SIZE);
                free(buffer);
                if (fp != stdin) fclose(fp);
                return NULL;
            }
            buffer_size *= 2;
            if (buffer_size > MAX_BUFFER_SIZE) buffer_size = MAX_BUFFER_SIZE;
            char *new_buffer = realloc(buffer, buffer_size);
            if (!new_buffer) {
                fprintf(stderr, "Error: Memory reallocation failed\n");
                free(buffer);
                if (fp != stdin) fclose(fp);
                return NULL;
            }
            buffer = new_buffer;
        }
    }
    
    if (fp != stdin) fclose(fp);
    
    // Null terminate
    buffer[total_read] = '\0';
    *len = total_read;
    return buffer;
}

void print_usage(const char *prog_name) {
    printf("Usage: %s [OPTIONS]\n", prog_name);
    printf("\nOptions:\n");
    printf("  -f, --fixed SIZE          Fixed length splitting (size in characters)\n");
    printf("  -v, --variable MIN MAX    Variable size splitting (min and max bounds)\n");
    printf("  -l, --lang LANGUAGE       Language syntax (bash, python, ruby, c, cpp, csharp,\n");
    printf("                            perl, java, javascript, vba, sql, r, php, rust, go)\n");
    printf("                            Default: python\n");
    printf("  -i, --input FILE          Input file (use '-' for stdin, default: stdin)\n");
    printf("  -o, --output FILE         Output file (default: stdout)\n");
    printf("  -e, --encode               Encode plaintext to base64 before splitting\n");
    printf("  -h, --help                Show this help message\n");
    printf("\nExamples:\n");
    printf("  %s -f 50 -l python < input.txt\n", prog_name);
    printf("  echo 'hello world' | %s -f 5 -l bash\n", prog_name);
    printf("  %s -f 50 -i file.txt -o output.txt -l python\n", prog_name);
    printf("  %s -v 30 70 -i - -o result.txt -l javascript\n", prog_name);
    printf("\nIf no options are provided, interactive mode will be used.\n");
}

int main(int argc, char *argv[]) {
    int fixed_size = 0;
    int variable_min = 0, variable_max = 0;
    int use_fixed = 0, use_variable = 0;
    int encode_plaintext = 0;
    int lang_num = 2;  // Default to Python
    const char *input_file = NULL;
    const char *output_file = NULL;
    char *input_data = NULL;
    size_t input_len = 0;
    FILE *output_fp = stdout;
    
    // Command-line argument parsing
    static struct option long_options[] = {
        {"fixed", required_argument, 0, 'f'},
        {"variable", required_argument, 0, 'v'},
        {"lang", required_argument, 0, 'l'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {"encode", no_argument, 0, 'e'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    int opt;
    int option_index = 0;
    
    while ((opt = getopt_long(argc, argv, "f:v:l:i:o:eh", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'f':
                fixed_size = atoi(optarg);
                use_fixed = 1;
                break;
            case 'v':
                // Parse MIN MAX from optarg (format: MIN,MAX or MIN MAX)
                if (sscanf(optarg, "%d,%d", &variable_min, &variable_max) != 2) {
                    // Try space-separated
                    if (sscanf(optarg, "%d %d", &variable_min, &variable_max) != 2) {
                        // Try to get second argument from next optarg
                        if (optind < argc && sscanf(argv[optind], "%d", &variable_max) == 1) {
                            variable_min = atoi(optarg);
                            optind++;
                        } else {
                            fprintf(stderr, "Error: Variable mode requires MIN,MAX or MIN MAX or -v MIN -v MAX\n");
                            fprintf(stderr, "Example: -v 30,70 or -v 30 70\n");
                            return 1;
                        }
                    }
                }
                use_variable = 1;
                break;
            case 'l':
                {
                    const char *lang = optarg;
                    if (strcmp(lang, "bash") == 0) lang_num = 1;
                    else if (strcmp(lang, "python") == 0) lang_num = 2;
                    else if (strcmp(lang, "ruby") == 0) lang_num = 3;
                    else if (strcmp(lang, "c") == 0) lang_num = 4;
                    else if (strcmp(lang, "cpp") == 0 || strcmp(lang, "c++") == 0) lang_num = 5;
                    else if (strcmp(lang, "csharp") == 0 || strcmp(lang, "c#") == 0) lang_num = 6;
                    else if (strcmp(lang, "perl") == 0) lang_num = 7;
                    else if (strcmp(lang, "java") == 0) lang_num = 8;
                    else if (strcmp(lang, "javascript") == 0 || strcmp(lang, "js") == 0) lang_num = 9;
                    else if (strcmp(lang, "vba") == 0) lang_num = 10;
                    else if (strcmp(lang, "sql") == 0) lang_num = 11;
                    else if (strcmp(lang, "r") == 0) lang_num = 12;
                    else if (strcmp(lang, "php") == 0) lang_num = 13;
                    else if (strcmp(lang, "rust") == 0) lang_num = 14;
                    else if (strcmp(lang, "go") == 0) lang_num = 15;
                    else {
                        fprintf(stderr, "Error: Unknown language '%s'\n", lang);
                        return 1;
                    }
                }
                break;
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'e':
                encode_plaintext = 1;
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }
    
    // If no command-line arguments, use interactive mode
    if (argc == 1 || (!use_fixed && !use_variable)) {
        // Interactive mode (original behavior)
        char str[500000];
        char encoded[700000];
        char choice[20];
        char encode_choice[10];
        char lang_choice[20];
        int n;
        int min_size, max_size;
        int len;
        int encode = 0;
        
        printf("Do you have plaintext you would like to encode? (yes/no): ");
        fgets(encode_choice, sizeof(encode_choice), stdin);
        int encode_len = strlen(encode_choice);
        if (encode_len > 0 && encode_choice[encode_len - 1] == '\n') {
            encode_choice[encode_len - 1] = '\0';
        }
        
        if (strcmp(encode_choice, "yes") == 0 || strcmp(encode_choice, "y") == 0) {
            encode = 1;
            printf("Enter the plaintext to encode: ");
            fflush(stdout);
            
            if (fgets(str, sizeof(str), stdin) == NULL) {
                fprintf(stderr, "Error reading input.\n");
                return 1;
            }
            
            len = strlen(str);
            if (len > 0 && str[len - 1] == '\n') {
                str[len - 1] = '\0';
                len--;
            }
            
            int encoded_len = base64_encode((unsigned char *)str, len, encoded, sizeof(encoded));
            if (encoded_len < 0) {
                fprintf(stderr, "Error: Base64 encoding failed.\n");
                return 1;
            }
            
            strncpy(str, encoded, sizeof(str) - 1);
            str[sizeof(str) - 1] = '\0';
            len = strlen(str);
            printf("\nBase64 encoded string: %s\n\n", str);
        }
        
        printf("Do you want fixed length or variable string sizes? (fixed/variable): ");
        fgets(choice, sizeof(choice), stdin);
        int choice_len = strlen(choice);
        if (choice_len > 0 && choice[choice_len - 1] == '\n') {
            choice[choice_len - 1] = '\0';
        }
        
        if (strcmp(choice, "fixed") == 0) {
            printf("What size would you like? ");
            scanf("%d", &n);
            getchar();
            use_fixed = 1;
            fixed_size = n;
        } else if (strcmp(choice, "variable") == 0) {
            printf("Enter minimum bound: ");
            scanf("%d", &min_size);
            printf("Enter maximum bound: ");
            scanf("%d", &max_size);
            getchar();
            use_variable = 1;
            variable_min = min_size;
            variable_max = max_size;
        } else {
            fprintf(stderr, "Invalid choice. Exiting.\n");
            return 1;
        }
        
        if (!encode) {
            printf("Enter the string to be split: ");
            fflush(stdout);
            
            if (fgets(str, sizeof(str), stdin) == NULL) {
                fprintf(stderr, "Error reading input.\n");
                return 1;
            }
            
            len = strlen(str);
            if (len > 0 && str[len - 1] == '\n') {
                str[len - 1] = '\0';
                len--;
            }
            
            input_data = malloc(len + 1);
            if (!input_data) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                return 1;
            }
            strcpy(input_data, str);
            input_len = len;
        } else {
            input_data = malloc(len + 1);
            if (!input_data) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                return 1;
            }
            strcpy(input_data, str);
            input_len = len;
        }
        
        printf("\nSelect language syntax:\n");
        printf("1. Bash 2. Python 3. Ruby 4. C 5. C++ 6. C# 7. Perl 8. Java\n");
        printf("9. JavaScript 10. VBA 11. SQL 12. R 13. PHP 14. Rust 15. Go\n");
        printf("Enter choice (1-15): ");
        fgets(lang_choice, sizeof(lang_choice), stdin);
        lang_num = atoi(lang_choice);
        if (lang_num < 1 || lang_num > 15) lang_num = 2;
    } else {
        // Command-line mode: read input
        input_data = read_input(input_file, &input_len);
        if (!input_data) {
            return 1;
        }
        
        // Remove trailing newline if present
        if (input_len > 0 && input_data[input_len - 1] == '\n') {
            input_data[input_len - 1] = '\0';
            input_len--;
        }
    }
    
    // Validate arguments
    if (!use_fixed && !use_variable) {
        fprintf(stderr, "Error: Must specify either -f/--fixed or -v/--variable\n");
        free(input_data);
        return 1;
    }
    
    if (use_fixed && fixed_size <= 0) {
        fprintf(stderr, "Error: Fixed size must be greater than 0\n");
        free(input_data);
        return 1;
    }
    
    if (use_variable && (variable_min <= 0 || variable_max <= 0 || variable_min > variable_max)) {
        fprintf(stderr, "Error: Invalid variable bounds (min: %d, max: %d)\n", variable_min, variable_max);
        free(input_data);
        return 1;
    }
    
    // Handle base64 encoding
    if (encode_plaintext) {
        char *encoded = malloc((input_len * 4 / 3) + 100);
        if (!encoded) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            free(input_data);
            return 1;
        }
        int encoded_len = base64_encode((unsigned char *)input_data, input_len, encoded, (input_len * 4 / 3) + 100);
        if (encoded_len < 0) {
            fprintf(stderr, "Error: Base64 encoding failed\n");
            free(encoded);
            free(input_data);
            return 1;
        }
        free(input_data);
        input_data = encoded;
        input_len = encoded_len;
    }
    
    // Open output file if specified
    if (output_file) {
        output_fp = fopen(output_file, "w");
        if (!output_fp) {
            fprintf(stderr, "Error: Cannot open output file '%s': %s\n", output_file, strerror(errno));
            free(input_data);
            return 1;
        }
    }
    
    // Set up language function
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
        default: print_func = print_chunk_python; lang_name = "Python"; break;
    }
    
    // Process and output
    if (use_fixed) {
        for (size_t i = 0; i < input_len; i += fixed_size) {
            size_t chunk_len = (i + fixed_size < input_len) ? fixed_size : input_len - i;
            print_func(output_fp, &input_data[i], chunk_len);
        }
    } else {
        srand(time(NULL));
        size_t i = 0;
        while (i < input_len) {
            int chunk_size = variable_min + (rand() % (variable_max - variable_min + 1));
            if (i + chunk_size > input_len) {
                chunk_size = input_len - i;
            }
            print_func(output_fp, &input_data[i], chunk_size);
            i += chunk_size;
        }
    }
    
    // Cleanup
    if (output_fp != stdout) {
        fclose(output_fp);
    }
    free(input_data);
    
    return 0;
}
