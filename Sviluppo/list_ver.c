#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct station
{
    int id;
    int *cars;
    int color;
    int *right_stn;
    int *left_stn
} station;

typedef struct q_str {
    int id;
    int father;
} q_str;

int main (int argc, char *argv[])
{
    char *line = NULL;
    size_t line_len = 0;
    ssize_t line_read;
    FILE *file_input = NULL;
    FILE *file_output = NULL;

    file_input = fopen(argv[1], "r");
    if (file_input == NULL) {
        perror("Error opening input file");
        return(0);
    }
    file_output = fopen("Sviluppo/output.txt", "w");
    if (file_output == NULL) {
        perror("Error opening output file");
        return(0);
    }

    char *command = NULL;
    int distance = 0;

    while ((line_read = getline(&line, &line_len, file_input)) != -1) {
        command = strtok(line, " ");
        if (strcmp(command, "aggiungi-stazione") == 0) {
                return 0;
        }
        else if (strcmp(command, "demolisci-stazione") == 0) {
            return 0;
        }
        else if (strcmp(command, "aggiungi-auto") == 0) {
            return 0;
        }
        else if (strcmp(command, "rottama-auto") == 0) {
            return 0;
        }
        else if (strcmp(command, "pianifica-percorso") == 0) {
            return 0;
        }
        else {
            return 0;
        }
    }
    fclose(file_input);
    fclose(file_output);
    if (line)
        free(line);
    return 0;
}

// ./test_gen_2023_macos -s 3000 -c 20 -C 100 -r 30
// Test/test_gen_2023/test_gen_2023_macos -s 5000 -c 20 -C 200 -r 30 > Test/archivio_test_aperti/open_200.txt

// open_104 / open_105 ha stesso tempo di parsing del verificatore --> open_104->612s / open_105->296s --> 104 sono meno istruzioni e ci impiega il triplo del tempo
