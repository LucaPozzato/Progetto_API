#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct stn
{
    int id;
    int *cars;
    int len_cars;
    int color;
    struct stn **right_stn;
    int len_right_stn;
    int right_index;
    struct stn **left_stn;
    int left_index;
    int len_left_stn;
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
int search_path (stn *first_stn, int stn_start, int stn_end, int *path);
int graph_rebuild (stn *start_stn, stn *end_stn);
int bfs (stn *end_stn, q_str *queue, int len_queue, int direction);
int max_car (int *cars, int len_cars);

int main (int argc, char *argv[])
{
    char *line = NULL;
    size_t line_len = 0;
    ssize_t line_read;
    FILE *file_input = NULL;
    FILE *file_output = NULL;

    #if local
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
        file_input = fopen("/Users/luca/Documents/Progetto-API/Test/archivio_test_aperti/open_65.txt", "r");
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
    #else
        file_input = stdin;
        file_output = stdout;
    #endif

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
            }
            else if (result != NULL) {
                first_stn = result;
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
                len_path = search_path(first_stn, station, destination, path);
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
        free(curr_stn->right_stn);
        free(curr_stn->left_stn);
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

int graph_rebuild (stn *start_stn, stn *end_stn) {
    stn *range_start = NULL;
    stn *range_end = NULL;
    stn *curr_stn = NULL;
    stn **stations = NULL;
    int len_stations = 0;

    if (start_stn->id < end_stn->id) {
        range_start = start_stn;
        curr_stn = range_start;
        range_end = end_stn;
    }
    else {
        range_start = end_stn;
        curr_stn = range_start;
        range_end = start_stn;
    }
    while (curr_stn != NULL && curr_stn->id <= range_end->id) {
        if (start_stn->id < end_stn->id) {
            curr_stn->color = 0;
            free(curr_stn->right_stn);
            curr_stn->right_stn = (stn **) calloc(32, sizeof(stn *));
            curr_stn->right_index = 0;
            curr_stn->len_right_stn = 32;
        }
        else {
            curr_stn->color = 0;
            free(curr_stn->left_stn);
            curr_stn->left_stn = (stn **) calloc(32, sizeof(stn *));
            curr_stn->left_index = 0;
            curr_stn->len_left_stn = 32;
        }
        curr_stn = curr_stn->nxt;
        len_stations++;
    }
    stations = (stn **) calloc(len_stations, sizeof(stn *));
    curr_stn = range_start;
    for (int i = 0; i < len_stations; i++) {
        stations[i] = curr_stn;
        curr_stn = curr_stn->nxt;
    }
    #if debug_graph
        printf("percorso: %d -> %d\n\n", range_start->id, range_end->id);
        printf("stations: ");
        for (int i = 0; i < len_stations; i++) {
            printf("%d ", stations[i]->id);
        }
        printf("\n\n");
    #endif

    int len_cars = 0;
    int car_m;
    int index = 0;

    for (int i = 0; i < len_stations; i++) {
        len_cars = stations[i]->len_cars;
        if (len_cars > 0) {
            car_m = max_car(stations[i]->cars, len_cars);
            if (start_stn->id < end_stn->id) {
                for (int j = 0; j < len_stations; j++) {
                    if (stations[j]->id <= stations[i]->id + car_m && stations[j]->id > stations[i]->id) {
                        index = stations[i]->right_index;
                        if (index >= stations[i]->len_right_stn - 1) {
                            stations[i]->len_right_stn *= 2;
                            stations[i]->right_stn = (stn **) realloc(stations[i]->right_stn, stations[i]->len_right_stn * sizeof(stn *));
                        }
                        stations[i]->right_stn[index] = stations[j];
                        stations[i]->right_index++;
                    }
                }
            }
            else{
                for (int j = 0; j < len_stations; j++) {
                    if (stations[j]->id >= stations[i]->id - car_m && stations[j]->id < stations[i]->id) {
                        index = stations[j]->left_index;
                        if (index >= stations[j]->len_left_stn - 1) {
                            stations[j]->len_left_stn *= 2;
                            stations[j]->left_stn = (stn **) realloc(stations[j]->left_stn, stations[j]->len_left_stn * sizeof(stn *));
                        }
                        stations[j]->left_stn[index] = stations[i];
                        stations[j]->left_index++;
                    }
                }
            }
        }
    }
    #if debug_graph
        for (int i = 0; i < len_stations; i++) {
            printf("stn: %d -> ", stations[i]->id);
            if (stations[i]->right_index != 0) {
                printf("right_stn: ");
                for (int j = 0; j < stations[i]->right_index; j++) {
                    printf("%d ", stations[i]->right_stn[j]->id);
                }
                printf("\n");
            }
            else {
                printf("left_stn: ");
                for (int j = 0; j < stations[i]->left_index; j++) {
                    printf("%d ", stations[i]->left_stn[j]->id);
                }
                printf("\n");
            }
        }
        printf("\n");
    #endif
    return 1;
}

int search_path (stn *first_stn, int stn_start, int stn_end, int *path) {
    int direction = 0;
    int stn_count = 0;

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
            if (curr_stn->id == stn_start) {
                strt_stn = curr_stn;
            }
            else if (curr_stn->id == stn_end) {
                nd_stn = curr_stn;
                break;
            }
            curr_stn = curr_stn->nxt;
            stn_count++;
        }
    }
    else {
        while (curr_stn != NULL) {
            if (curr_stn->id == stn_end) {
                nd_stn = curr_stn;
            }
            else if (curr_stn->id == stn_start) {
                strt_stn = curr_stn;
                break;
            }
            curr_stn = curr_stn->nxt;
            stn_count++;
        }
    }

    graph_rebuild(strt_stn, nd_stn);
    q_str *queue = NULL;
    int len_queue = 1;
    queue = (q_str *) calloc(stn_count * stn_count, sizeof(q_str));
    if (direction == 1) {
        queue->id = strt_stn;
        queue->father = NULL;
        len_queue = bfs(nd_stn, queue, len_queue, direction);
    }
    else {
        queue->id = nd_stn;
        queue->father = NULL;
        len_queue = bfs(strt_stn, queue, len_queue, direction);
    }
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
        #if debug_queue
            for (int i = 0; i < len_queue; i++) {
                printf("[%d, %d]\n", (int_queue + i)->id, (int_queue + i)->father);
            }
            printf("\n");
        #endif
        int len_path = 0;
        int stn = 0;
        if (direction == 1) {
            stn = stn_end;
        }
        else {
            stn = stn_start;
        }
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

int bfs (stn *end_stn, q_str *queue, int len_queue, int direction) {
    stn **next_nodes = NULL;
    int len_next_nodes = 0;
    stn *index = NULL;
    q_str *v;
    int i = 0;
    while (i < len_queue) {
        v = queue + i;
        index = v->id;
        if (direction == 1) {
            next_nodes = index->right_stn;
            len_next_nodes = index->right_index;
        }
        else {
            next_nodes = index->left_stn;
            len_next_nodes = index->left_index;
        }
        if (v->id == end_stn) {
            return i + 1;
        }
        if (len_next_nodes > 0) {
            for (int j = 0; j < len_next_nodes; j++) {
                if (next_nodes[j]->color == 0) {
                    next_nodes[j]->color = 1;
                    (queue + len_queue)->id = next_nodes[j];
                    (queue + len_queue)->father = v->id;
                    len_queue++;
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
// -> implementare stn_right or stn_left come lista
// -> non serve rifare grafo per stn_right, si puô fare tutto in bfs
// -> fix path calloc
// -> fix memory leaks
// -> better memory usage when freeing immediately after pianifica percorso
// -> fix queue alloc, smaller size than n^2