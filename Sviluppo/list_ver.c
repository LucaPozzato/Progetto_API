#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct stn
{
    int id;
    int m_car;
    int *cars;
    int len_cars;
    bool color;
    struct stn *nxt;
    struct stn *prv;
} stn;

typedef struct q_str {
    struct stn *id;
    struct stn *father;
} q_str;

stn* add_station (stn *first_stn, stn *new_stn, stn *last_stn, int station, int *cars, int len_cars, int m_car, int *f);
stn* del_station (stn *first_stn, int station, int *f);
stn* add_car (stn *last_stn, int station, int car);
int del_car (stn *first_stn, int *new_cars, int station, int car);
int search_path (stn *first_stn, int stn_start, int stn_end, int *path, int n_stations);
int bfs_dx (stn *end_stn, q_str *queue, int direction);
int bfs_sx (stn *end_stn, q_str *queue, int direction, int m_car);
int rebuild_graph (stn *start_stn, stn *end_stn, stn **next_nodes);

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
    stn *last_stn = NULL;
    int *new_cars = NULL;
    int *path = NULL;
    int len_path = 0;
    int f = 0;
    int n_stations = 0;

    while ((line_read = getline(&line, &line_len, file_input)) != -1) {
        command = strtok(line, " ");
        if (strcmp(command, "aggiungi-stazione") == 0) {
            int m_car = 0;
            f = 0;
            station = (int)strtol(strtok(NULL, " "), NULL, 10);
            len_cars = (int)strtol(strtok(NULL, " "), NULL, 10);
            cars = (int *) calloc(len_cars, sizeof(int));
            for (int i = 0; i < len_cars; i++) {
                cars[i] = (int)strtol(strtok(NULL, " "), NULL, 10);
                if (cars[i] > m_car) {
                    m_car = cars[i];
                }
            }
            new_stn = (stn *) calloc(1, sizeof(stn));
            result = add_station(first_stn, new_stn, last_stn, station, cars, len_cars, m_car, &f);
            if (result != NULL) {
                if (f == 1) {
                    first_stn = result;
                    last_stn = first_stn;
                }
                else {
                    last_stn = result;
                }
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
                last_stn = first_stn;
                fprintf(file_output, "demolita\n"); 
                n_stations--;
            }
            else if (result != NULL) {
                first_stn = result;
                last_stn = first_stn;
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
            result = add_car(last_stn, station, car);
            if (result != NULL) {
                last_stn = result;
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
                free(path);
            }
        }
        else {
            return 0;
        }
    }

    stn *curr_stn = first_stn;
    while (curr_stn->nxt != NULL) {
        free(curr_stn->cars);
        free(curr_stn->prv);
        curr_stn = curr_stn->nxt;
    }
    free(curr_stn->cars);
    free(curr_stn->prv);
    free(curr_stn);

    fclose(file_input);
    fclose(file_output);
    if (line)
        free(line);
    return 0;
}

stn* add_station (stn *first_stn, stn *new_stn, stn *last_stn, int station, int *cars, int len_cars, int m_car, int *f) {
    stn *curr_stn = last_stn;

    new_stn->id = station;
    new_stn->m_car = m_car;
    new_stn->cars = cars;
    new_stn->len_cars = len_cars;

    if (first_stn == NULL) {
        first_stn = new_stn;
        *f = 1;
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
        *f = 1;
        return first_stn;
    }
    else if (first_stn->id > new_stn->id) {
        new_stn->nxt = first_stn;
        first_stn->prv = new_stn;
        first_stn = new_stn;
        *f = 1;
        return first_stn;
    }
    else {
        if (curr_stn->id <= new_stn->id) {
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
                return new_stn;
            }
            else {
                curr_stn->prv->nxt = new_stn;
                new_stn->prv = curr_stn->prv;
                curr_stn->prv = new_stn;
                new_stn->nxt = curr_stn;
                return new_stn;
            }
        }
        else {
            while (curr_stn->id > new_stn->id && curr_stn->prv != NULL) {
                if (curr_stn->id == new_stn->id) {
                    return NULL;
                }
                curr_stn = curr_stn->prv;
            }
            if (curr_stn->id == new_stn->id || curr_stn->nxt->id == new_stn->id) {
                return NULL;
            }
            else {
                curr_stn->nxt->prv = new_stn;
                new_stn->nxt = curr_stn->nxt;
                curr_stn->nxt = new_stn;
                new_stn->prv = curr_stn;
                return new_stn;
            }
        }
    }
}

stn* del_station (stn *first_stn, int station, int *f) {
    stn *curr_stn = first_stn;
    while (curr_stn != NULL) {
        if (curr_stn->id == station) {
            if (curr_stn->nxt == NULL && curr_stn->prv == NULL) {
                free(curr_stn->cars);
                free(curr_stn);
                *f = 1;
                first_stn = NULL;
            }
            else if (curr_stn->nxt == NULL) {
                curr_stn->prv->nxt = NULL;
                free(curr_stn->cars);
                free(curr_stn);
            }
            else if (curr_stn->prv == NULL) {
                curr_stn->nxt->prv = NULL;
                first_stn = curr_stn->nxt;
                free(curr_stn->cars);
                free(curr_stn);
            }
            else {
                curr_stn->nxt->prv = curr_stn->prv;
                curr_stn->prv->nxt = curr_stn->nxt;
                free(curr_stn->cars);
                free(curr_stn);
            }
            return first_stn;
        }
        curr_stn = curr_stn->nxt;
    }
    return NULL;
}

stn* add_car (stn *last_stn, int station, int car) {
    stn *curr_stn = last_stn;
    int len_cars = 0;
    if (last_stn->id > station) {
        while (curr_stn != NULL && curr_stn->id != station) {
            curr_stn = curr_stn->prv;
        }
    }
    else {
        while (curr_stn != NULL && curr_stn->id != station) {
            curr_stn = curr_stn->nxt;
        }
    }
    if (curr_stn != NULL) {
        if (curr_stn->m_car < car) {
            curr_stn->m_car = car;
        }
        len_cars = curr_stn->len_cars;
        curr_stn->cars = (int *) realloc(curr_stn->cars, (curr_stn->len_cars + 1) * sizeof(int));
        curr_stn->cars[len_cars] = car;
        curr_stn->len_cars = len_cars + 1;
        return curr_stn;
    }
    return NULL;
}

int del_car (stn *first_stn, int *new_cars, int station, int car) {
    stn *curr_stn = first_stn;
    int len_cars = 0;
    int k = 0;
    int m_car = 0;
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
                            if (new_cars[k] > m_car) {
                                m_car = new_cars[k];
                            }
                            k++;
                        }
                    }
                    free(curr_stn->cars);
                    curr_stn->cars = new_cars;
                    curr_stn->len_cars = len_cars;
                    if (curr_stn->m_car == car) {
                        curr_stn->m_car = m_car;
                    }
                    return 1;
                }
            }
            return 0;
        }
        curr_stn = curr_stn->nxt;
    }
    return 0;
    // si potrebbe ottimizzare iniziando gia a copiare l'array independentemente che la macchina sia presente o meno
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
    int m_car = 0;
    int car = 0;

    if (direction == 1) {
        while (curr_stn != NULL) {
            curr_stn->color = false;
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
            curr_stn->color = false;
            if (curr_stn->id > stn_end) {
                car = curr_stn->m_car;
                if (car > m_car) {
                    m_car = car;
                }
            }
            if (curr_stn->id == stn_end) {
                nd_stn = curr_stn;
            }
            else if (curr_stn->id == stn_start) {
                strt_stn = curr_stn;
                // printf("max car: %d, percorso: %d -> %d\n", m_car, stn_end, stn_start);
                break;
            }
            curr_stn = curr_stn->nxt;
        }
    }

    q_str *queue = NULL;
    int len_queue = 0;
    queue = (q_str *) calloc(n_stations, sizeof(q_str));

    if (direction == 1) {
        queue->id = strt_stn;
        queue->father = NULL;
        len_queue = bfs_dx(nd_stn, queue, direction);
    }
    else {
        queue->id = nd_stn;
        queue->father = NULL;
        len_queue = bfs_sx(strt_stn, queue, direction, m_car);
    }

    if (len_queue != -1) {
        int len_path = 0;
        stn *stn = NULL;
        if (direction == 1) {
            stn = nd_stn;
        }
        else {
            stn = strt_stn;
        }
        for (int i = len_queue - 1; i >= 0; i--) {
            if ((queue + i)->id == stn) {
                path[len_path] = stn->id;
                stn = (queue + i)->father;
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
        free(queue);
        return len_path;
    }
    else {
        free(queue);
        return 0;
    }
}

int bfs_dx (stn *end_stn, q_str *queue, int direction) {
    stn *index = NULL;
    stn *curr_stn = NULL;
    q_str *v;
    int len_queue = 1;
    int i = 0;
    int car = 0;
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
        curr_stn = index;
        if (curr_stn->nxt != NULL) {
            curr_stn = index->nxt;
            car = index->m_car;
            while (curr_stn != NULL && curr_stn->id <= index->id + car && curr_stn->id > index->id) {
                if (curr_stn->color == false) {
                    curr_stn->color = true;
                    (queue + len_queue)->id = curr_stn;
                    (queue + len_queue)->father = index;
                    len_queue++;
                }
                curr_stn = curr_stn->nxt;
            }
        }
        i++;
    }
    return -1;
}

int bfs_sx (stn *end_stn, q_str *queue, int direction, int m_car) {
    stn *index = NULL;
    stn *curr_stn = NULL;
    q_str *v;
    int len_queue = 1;
    int i = 0;
    int car = 0;
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
        curr_stn = index;
        if (curr_stn->nxt != NULL) {
            curr_stn = index->nxt;
            while (curr_stn != NULL && curr_stn->id <= index->id + m_car && curr_stn->id <= end_stn->id) {
                car = curr_stn->m_car;
                if (index->id >= curr_stn->id - car && index->id < curr_stn->id) {
                    if (curr_stn->color == false) {
                        curr_stn->color = true;
                        (queue + len_queue)->id = curr_stn;
                        (queue + len_queue)->father = index;
                        len_queue++;
                    }
                }
                curr_stn = curr_stn->nxt;
            }
        }
        i++;
    }
    return -1;
}

// bottle neck:
// -> iterare per trovare stazione
// -> range per stazioni sx, controllo più stazioni del necessario
// -> strole si prende il 10%, strtok il 5%
// -> algoritmo più afficiente per aggiungi stazioni, meno stazioni da iterare, iterare solo 1/4 delle stazioni memorizzando inizio metà e fine 
//      -> ora algoritmo è quasi bilanciato sulla metà
//      -> oppure implementare una hashtable per la ricerca delle stazioni
// 