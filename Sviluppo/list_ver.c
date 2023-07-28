#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct stn
{
    int id;
    int *cars;
    int len_cars;
    int color;
    int *right_stn;
    int len_right_stn;
    int *left_stn;
    int len_left_stn;
    struct stn *nxt;
    struct stn *prv;
} stn;

typedef struct q_str {
    int id;
    int father;
} q_str;

stn *add_station (stn *first_stn, stn *new_stn, int station, int *cars, int len_cars);
int del_station (stn *first_stn, int station);
int add_car (stn *first_stn, int station, int car);
int del_car (stn *first_stn, int station, int car);

int main (int argc, char *argv[])
{
    char *line = NULL;
    size_t line_len = 0;
    ssize_t line_read;
    FILE *file_input = NULL;
    FILE *file_output = NULL;

    file_input = fopen("/Users/luca/Documents/Progetto-API/Test/archivio_test_aperti/open_100.txt", "r");
    if (file_input == NULL) {
        perror("Error opening input file");
        return(0);
    }
    file_output = fopen("/Users/luca/Documents/Progetto-API/Sviluppo/output.txt", "w");
    if (file_output == NULL) {
        perror("Error opening output file");
        return(0);
    }

    char *command = NULL;
    int station = 0;
    int car = 0;
    int *cars = NULL;
    int len_cars = 0;
    stn *first_stn = NULL;
    stn *new_stn = NULL;

    while ((line_read = getline(&line, &line_len, file_input)) != -1) {
        command = strtok(line, " ");
        if (strcmp(command, "aggiungi-stazione") == 0) {
                station = (int)strtol(strtok(NULL, " "), NULL, 10);
                len_cars = (int)strtol(strtok(NULL, " "), NULL, 10);
                cars = (int *) calloc(len_cars, sizeof(int));
                for (int i = 0; i < len_cars; i++) {
                    cars[i] = (int)strtol(strtok(NULL, " "), NULL, 10); 
                }
                new_stn = NULL;
                new_stn = (stn *) calloc(1, sizeof(stn));
                new_stn = add_station(first_stn, new_stn, station, cars, len_cars);
                if (first_stn == NULL) {
                    first_stn = new_stn;
                }
                if (new_stn != NULL) {
                    fprintf(file_output, "aggiunta\n");
                }
                else {
                    fprintf(file_output, "non aggiunta\n");
                }
        }
        else if (strcmp(command, "demolisci-stazione") == 0) {
            station = (int)strtol(strtok(NULL, " "), NULL, 10);
            if (del_station(first_stn, station) == 1) {
                fprintf(file_output, "demolita\n");
            }
            else {
                fprintf(file_output, "non demolita\n");
            }
        }
        else if (strcmp(command, "aggiungi-auto") == 0) {
            station = (int)strtol(strtok(NULL, " "), NULL, 10);
            car = (int)strtol(strtok(NULL, " "), NULL, 10);
            if (add_car(first_stn, station, car) == 1) {
                fprintf(file_output, "aggiunta\n");
            }
            else {
                fprintf(file_output, "non aggiunta\n");
            }
        }
        else if (strcmp(command, "rottama-auto") == 0) {
            station = (int)strtol(strtok(NULL, " "), NULL, 10);
            car = (int)strtol(strtok(NULL, " "), NULL, 10);
            if (del_car(first_stn, station, car) == 1) {
                fprintf(file_output, "rottamata\n");
            }
            else {
                fprintf(file_output, "non rottamata\n");
            }
        }
        else if (strcmp(command, "pianifica-percorso") == 0) {
            fprintf(file_output, "nessun percorso\n");
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

stn *add_station (stn *first_stn, stn *new_stn, int station, int *cars, int len_cars) {
    stn *curr_stn = first_stn;
    if (curr_stn == NULL) {
        first_stn = new_stn;
        first_stn->id = station;
        first_stn->cars = cars;
        first_stn->len_cars = len_cars;
        return new_stn;
    }
    else {
        while (curr_stn != NULL) {
            if (curr_stn->id < station) {
                curr_stn = curr_stn->nxt;
            }
            else if (curr_stn->id == station) {
                return NULL;
            }
            else {
                new_stn = (stn *) calloc(1, sizeof(stn));
                curr_stn = curr_stn->prv;
                curr_stn->nxt->prv = new_stn;
                new_stn->nxt = curr_stn->nxt;
                curr_stn->nxt = new_stn;
                new_stn->prv = curr_stn;
                new_stn->id = station;
                new_stn->cars = cars;
                new_stn->len_cars = len_cars;
                // da sistemare va messa prima, non dopo
                // usa ipad perchè se no è un casino
                return new_stn;
            }
        }
        new_stn = (stn *) calloc(1, sizeof(stn));
        curr_stn->nxt = new_stn;
        new_stn->prv = curr_stn;
        new_stn->id = station;
        new_stn->cars = cars;
        new_stn->len_cars = len_cars;
        return new_stn;
    }
}

int del_station (stn *first_stn, int station) {
    stn *curr_stn = first_stn;
    while (curr_stn != NULL) {
        if (curr_stn->id == station) {
            if (curr_stn->prv != NULL) {
                curr_stn->prv->nxt = curr_stn->nxt;
            }
            else {
                curr_stn->nxt->prv = NULL;
            }
            if (curr_stn->nxt != NULL) {
                curr_stn->nxt->prv = curr_stn->prv;
            }
            else {
                curr_stn->prv->nxt = NULL;
            }
            // free(curr_stn);
            return 1;
        }
        curr_stn = curr_stn->nxt;
    }
    return 0;
}

int add_car (stn *first_stn, int station, int car) {
    stn *curr_stn = first_stn;
    while (curr_stn != NULL) {
        if (curr_stn->id == station) {
            curr_stn->cars = (int *) realloc(curr_stn->cars, (curr_stn->len_cars + 1) * sizeof(int));
            curr_stn->cars[curr_stn->len_cars] = car;
            curr_stn->len_cars ++;
            return 1;
        }
        curr_stn = curr_stn->nxt;
    }
    return 0;
}

int del_car (stn *first_stn, int station, int car) {
    stn *curr_stn = first_stn;
    int *new_cars = NULL;
    int len_cars = 0;
    int k = 0;
    while (curr_stn != NULL) {
        if (curr_stn->id == station) {
            for (int i = 0; i < curr_stn->len_cars; i++) {
                if (curr_stn->cars[i] == car){
                    curr_stn->cars[i] = -1;
                    len_cars = curr_stn->len_cars - 1;
                    new_cars = (int *) malloc(len_cars * sizeof(int));
                    for (int j = 0; j < curr_stn->len_cars; j++) {
                        if (curr_stn->cars[j] != -1) {
                            new_cars[k] = curr_stn->cars[j];
                        }
                        k ++;
                    }
                    free(curr_stn->cars);
                    curr_stn->cars = new_cars;
                    curr_stn->len_cars = len_cars;
                    return 1;
                }
            }
            return 0;
        }
        curr_stn = curr_stn->nxt;
    }
    return 0;
}


// ./test_gen_2023_macos -s 3000 -c 20 -C 100 -r 30
// Test/test_gen_2023/test_gen_2023_macos -s 5000 -c 20 -C 200 -r 30 > Test/archivio_test_aperti/open_200.txt

// open_104 / open_105 ha stesso tempo di parsing del verificatore --> open_104->612s / open_105->296s --> 104 sono meno istruzioni e ci impiega il triplo del tempo

// miglioramenti -> implementare cars come una lista