#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct stn
{
    int id;
    int *cars;
    int len_cars;
    int color;
    struct stn *nxt;
    struct stn *prv;
} stn;

typedef struct q_str {
    struct stn *id;
    struct stn *father;
} q_str;

typedef struct iq_str {
    int id;
    int father;
} iq_str;

stn* add_station (stn *first_stn, stn *new_stn, int station, int *cars, int len_cars);
stn* del_station (stn *first_stn, int station, int *f);
int add_car (stn *first_stn, int station, int car);
int del_car (stn *first_stn, int *new_cars, int station, int car);
int search_path (stn *first_stn, int stn_start, int stn_end, int *path, int n_stations);
int bfs (stn *end_stn, q_str *queue, int direction);
int max_car (int *cars, int len_cars);

int main (int argc, char *argv[])
{
    char *line = NULL;
    size_t line_len = 0;
    ssize_t line_read;
    FILE *file_input = NULL;
    FILE *file_output = NULL;

    // #if local
        #if file
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
        #else
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
        #endif
    // #else
    //     file_input = stdin;
    //     file_output = stdout;
    // #endif

    char *command = NULL;
    int station = 0;
    int destination = 0;
    int car = 0;
    int *cars = NULL;
    int len_cars = 0;
    stn *first_stn = NULL;
    stn *new_stn = NULL;
    stn *result = NULL;
    int *new_cars = NULL;
    int *path = NULL;
    int len_path = 0;
    int f = 0;
    int n_stations = 0;

    while ((line_read = getline(&line, &line_len, file_input)) != -1) {
        command = strtok(line, " ");
        if (strcmp(command, "aggiungi-stazione") == 0) {
                station = (int)strtol(strtok(NULL, " "), NULL, 10);
                len_cars = (int)strtol(strtok(NULL, " "), NULL, 10);
                cars = (int *) calloc(len_cars, sizeof(int));
                for (int i = 0; i < len_cars; i++) {
                    cars[i] = (int)strtol(strtok(NULL, " "), NULL, 10); 
                }
                new_stn = (stn *) calloc(1, sizeof(stn));
                result = add_station(first_stn, new_stn, station, cars, len_cars);
                if (result != NULL) {
                    first_stn = result;
                    fprintf(file_output, "aggiunta\n");
                    n_stations++;
                }
                else {
                    fprintf(file_output, "non aggiunta\n");
                    free(new_stn);
                    free(cars);
                }
        }
        else if (strcmp(command, "demolisci-stazione") == 0) {
            station = (int)strtol(strtok(NULL, " "), NULL, 10);
            f = 0;
            result = del_station(first_stn, station, &f);
            if (f == 1) {
                first_stn = result;
                fprintf(file_output, "demolita\n"); 
                n_stations--;
            }
            else if (result != NULL) {
                first_stn = result;
                fprintf(file_output, "demolita\n");
                n_stations--;
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
            if (del_car(first_stn, new_cars, station, car) == 1) {
                fprintf(file_output, "rottamata\n");
            }
            else {
                fprintf(file_output, "non rottamata\n");
            }
        }
        else if (strcmp(command, "pianifica-percorso") == 0) {
            station = (int)strtol(strtok(NULL, " "), NULL, 10);
            destination = (int)strtol(strtok(NULL, " "), NULL, 10);

            if (station == destination) {
                fprintf(file_output, "%d\n", station);
            }
            else {
                path = (int *) calloc(100, sizeof(int));
                // fix calloc of 100
                len_path = search_path(first_stn, station, destination, path, n_stations);
                if (len_path != 0) {
                    if (station > destination) {
                        for (int i = 0; i < len_path - 1; i++) {
                            fprintf(file_output, "%d ", path[i]);
                        }
                        fprintf(file_output, "%d\n", path[len_path - 1]);
                    }
                    else {
                        for (int i = len_path - 1; i > 0; i--) {
                            fprintf(file_output, "%d ", path[i]);
                        }
                        fprintf(file_output, "%d\n", path[0]);     
                    }
                }
                else {
                    fprintf(file_output, "nessun percorso\n");
                }
            }
        }
        else {
            return 0;
        }
    }

    stn *curr_stn = first_stn;
    while (curr_stn != NULL) {
        free(curr_stn->cars);
        free(curr_stn->prv);
        curr_stn = curr_stn->nxt;
    }

    fclose(file_input);
    fclose(file_output);
    if (line)
        free(line);
    return 0;
}

stn* add_station (stn *first_stn, stn *new_stn, int station, int *cars, int len_cars) {
    stn *curr_stn = first_stn;

    new_stn->id = station;
    new_stn->cars = cars;
    new_stn->len_cars = len_cars;

    if (first_stn == NULL) {
        first_stn = new_stn;
        return first_stn;
    }
    else if (first_stn->nxt == NULL && first_stn->prv == NULL) {
        if (new_stn->id > first_stn->id) {
            new_stn->prv = first_stn;
            first_stn->nxt = new_stn;
        }
        else {
            new_stn->nxt = first_stn;
            first_stn->prv = new_stn;
            first_stn = new_stn;
        }
        return first_stn;
    }
    else if (first_stn->id > new_stn->id) {
        new_stn->nxt = first_stn;
        first_stn->prv = new_stn;
        first_stn = new_stn;
        return first_stn;
    }
    else {
        while (curr_stn->id < new_stn->id && curr_stn->nxt != NULL) {
            if (curr_stn->id == new_stn->id) {
                return NULL;
            }
            curr_stn = curr_stn->nxt;
        }
        if (curr_stn->id == new_stn->id || curr_stn->prv->id == new_stn->id) {
            return NULL;
        }
        if (curr_stn->nxt == NULL) {
            if (curr_stn->id > new_stn->id) {
                curr_stn->prv->nxt = new_stn;
                new_stn->prv = curr_stn->prv;
                curr_stn->prv = new_stn;
                new_stn->nxt = curr_stn;
            }
            else {
                curr_stn->nxt = new_stn;
                new_stn->prv = curr_stn;
            }
            return first_stn;
        }
        else {
            curr_stn->prv->nxt = new_stn;
            new_stn->prv = curr_stn->prv;
            curr_stn->prv = new_stn;
            new_stn->nxt = curr_stn;
            return first_stn;
        }
    }
}

stn* del_station (stn *first_stn, int station, int *f) {
    stn *curr_stn = first_stn;
    while (curr_stn != NULL) {
        if (curr_stn->id == station) {
            if (curr_stn->nxt == NULL && curr_stn->prv == NULL) {
                free(curr_stn);
                *f = 1;
                first_stn = NULL;
            }
            else if (curr_stn->nxt == NULL) {
                curr_stn->prv->nxt = NULL;
                free(curr_stn);
            }
            else if (curr_stn->prv == NULL) {
                curr_stn->nxt->prv = NULL;
                first_stn = curr_stn->nxt;
                free(curr_stn);
            }
            else {
                curr_stn->nxt->prv = curr_stn->prv;
                curr_stn->prv->nxt = curr_stn->nxt;
                free(curr_stn);
            }
            return first_stn;
        }
        curr_stn = curr_stn->nxt;
    }
    return NULL;
}

int add_car (stn *first_stn, int station, int car) {
    stn *curr_stn = first_stn;
    int len_cars = 0;
    while (curr_stn != NULL) {
        if (curr_stn->id == station) {
            len_cars = curr_stn->len_cars;
            curr_stn->cars = (int *) realloc(curr_stn->cars, (curr_stn->len_cars + 1) * sizeof(int));
            curr_stn->cars[len_cars] = car;
            curr_stn->len_cars = len_cars + 1;
            return 1;
        }
        curr_stn = curr_stn->nxt;
    }
    return 0;
}

int del_car (stn *first_stn, int *new_cars, int station, int car) {
    stn *curr_stn = first_stn;
    int len_cars = 0;
    int k = 0;
    while (curr_stn != NULL) {
        if (curr_stn->id == station) {
            for (int i = 0; i < curr_stn->len_cars; i++) {
                if (curr_stn->cars[i] == car){
                    curr_stn->cars[i] = -1;
                    len_cars = curr_stn->len_cars - 1;
                    new_cars = (int *) calloc(len_cars, sizeof(int));
                    for (int j = 0; j < curr_stn->len_cars; j++) {
                        if (curr_stn->cars[j] != -1) {
                            new_cars[k] = curr_stn->cars[j];
                            k++;
                        }
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

int search_path (stn *first_stn, int stn_start, int stn_end, int *path, int n_stations) {
    int direction = 0;

    if (stn_start < stn_end) {
        direction = 1;
    }
    else {
        direction = -1;
    }

    stn *strt_stn = NULL;
    stn *nd_stn = NULL;
    stn *curr_stn = first_stn;

    if (direction == 1) {
        while (curr_stn != NULL) {
            curr_stn->color = 0;
            if (curr_stn->id == stn_start) {
                strt_stn = curr_stn;
            }
            else if (curr_stn->id == stn_end) {
                nd_stn = curr_stn;
                break;
            }
            curr_stn = curr_stn->nxt;
        }
    }
    else {
        while (curr_stn != NULL) {
            curr_stn->color = 0;
            if (curr_stn->id == stn_end) {
                nd_stn = curr_stn;
            }
            else if (curr_stn->id == stn_start) {
                strt_stn = curr_stn;
                break;
            }
            curr_stn = curr_stn->nxt;
        }
    }

    q_str *queue = NULL;
    int len_queue = 0;
    queue = (q_str *) calloc(n_stations, sizeof(q_str));

    queue->id = strt_stn;
    queue->father = NULL;
    len_queue = bfs(nd_stn, queue, direction);

    if (len_queue != -1) {
        iq_str *int_queue = (iq_str *) calloc(len_queue, sizeof(iq_str));
        for (int i = 0; i < len_queue; i++) {
            (int_queue + i)->id = (queue + i)->id->id;
            if ((queue + i)->father != NULL) {
                (int_queue + i)->father = (queue + i)->father->id;
            }
            else {
                (int_queue + i)->father = -1;
            }
        }
        free(queue);
        queue = NULL;
        int len_path = 0;
        int stn = stn_end;
        for (int i = len_queue - 1; i >= 0; i--) {
            if ((int_queue + i)->id == stn) {
                path[len_path] = stn;
                stn = (int_queue + i)->father;
                len_path++;
            }
        }
        #if debug_path
            if (len_path > 0) {
                printf("path: ");
                for (int i = 0; i < len_path; i++) {
                    printf("%d ", path[i]);
                }
                printf("\n\n-------\n\n");
            }
            else {
                printf("no path found\n\n-------\n\n");
            }
        #endif
        free(int_queue);
        int_queue = NULL;
        return len_path;
    }
    else {
        return 0;
    }
}

int bfs (stn *end_stn, q_str *queue, int direction) {
    stn *index = NULL;
    stn *curr_stn = NULL;
    q_str *v;
    int len_queue = 1;
    int i = 0;
    while (i < len_queue) {
        v = queue + i;
        index = v->id;
        #if debug_queue
            if ((queue + i)->father != NULL) {
                printf("[%d, %d]", (queue + i)->id->id, (queue + i)->father->id);
            }
            else {
                printf("[%d, %d]", (queue + i)->id->id, -1);
            }
            printf("\n");
        #endif
        if (index == end_stn) {
            return i + 1;
        }
        if (direction == 1) {
            curr_stn = index->nxt;
            while (curr_stn != NULL && curr_stn->id <= index->id + max_car(index->cars, index->len_cars) && curr_stn->id > index->id) {
                if (curr_stn->color == 0) {
                    curr_stn->color = 1;
                    (queue + len_queue)->id = curr_stn;
                    (queue + len_queue)->father = index;
                    len_queue++;
                }
                curr_stn = curr_stn->nxt;
            }
        }
        else {
            curr_stn = index->prv;
            while (curr_stn != NULL && curr_stn->id >= index->id - max_car(index->cars, index->len_cars) && curr_stn->id < index->id) {
                curr_stn = curr_stn->prv;
            }
            if (curr_stn != NULL) {
                while (curr_stn->id < index->id) {
                    if (curr_stn->color == 0) {
                        curr_stn->color = 1;
                        (queue + len_queue)->id = curr_stn;
                        (queue + len_queue)->father = index;
                        len_queue++;
                    }
                    curr_stn = curr_stn->nxt;
                }
            }
        }
        i++;
    }
    return -1;
}

int max_car (int *cars, int len_cars) {
    int car_m = 0;
    for (int i = 0; i < len_cars; i++) {
        if (cars[i] > car_m) 
            car_m = cars[i];
    }
    return car_m;
}

// ./test_gen_2023_macos -s 3000 -c 20 -C 100 -r 30
// Test/test_gen_2023/test_gen_2023_macos -s 5000 -c 20 -C 200 -r 30 > Test/archivio_test_aperti/open_200.txt

// open_104 / open_105 ha stesso tempo di parsing del verificatore --> open_104->612s / open_105->296s --> 104 sono meno istruzioni e ci impiega il triplo del tempo

// -> implementare cars come una lista
// -> fix path calloc
// -> fix memory leaks
// -> better memory usage when freeing immediately after pianifica percorso