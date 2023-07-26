#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define hash_const (sqrtl(5)-1)/2

typedef struct station
{
    int id;
    int *cars;
    int len_cars;
    int *right_queue;
    int right_index;
    int len_rqueue;
    int *left_queue;
    int left_index;
    int len_lqueue;
    int color;
} station;

typedef struct list_path 
{
    int *path;
    int len_path;
} list_path;

typedef struct queue_str {
    int id;
    int father;
} queue_str;

int get_index (int key, int highway_len, station* highway);
int in_highway (station *highway, int highway_len, int key);
int re_hash (station *highway, station *new_highway,int *highway_len);
int car_found (station *highway, int index, int car);
int max_car (station *highway, int index);
int compare (const void *elem1, const void *elem2);
int in_array (int *array, int len, int elem);
int shortest_path(station *highway, int highway_len, int distance, int arrival, int *path, int n_stations);
int initialize_highway (station *highway, int highway_len);
int delete_station(station *highway, int index);
int recalc_station (station *highway, int highway_len, int distance, int arrival);
int add_car (station *highway, int highway_len, int car, int index);
int remove_car (station *highway, int *curr_ptr, int car, int index);
int find_shortest_path (station *highway, int highway_len, queue_str *queue, int queue_len, int direction, int end_node, int *index);

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
    char *distance_c = NULL;
    int distance = 0;
    int arrival = 0;
    int highway_len = 64;
    int n_stations = 0;
    int index = 0;
    int len_cars = 0;
    int car = 0;
    int *curr_ptr = NULL;
    station* highway = NULL;
    highway = (station *) calloc(highway_len, sizeof(station));
    initialize_highway(highway, highway_len);

    int first_node = 0;
    int end_node = 0;
    int *path = NULL;
    int len_path = 0;

    while ((line_read = getline(&line, &line_len, file_input)) != -1) {
        command = strtok(line, " ");
        if (strcmp(command, "aggiungi-stazione") == 0) {
            if (n_stations > highway_len/10*7) {
                station *new_highway = (station *) calloc(highway_len * 2, sizeof(station));
                initialize_highway(new_highway, highway_len * 2);
                re_hash(highway, new_highway, &highway_len);
                free(highway);
                highway = new_highway;
                new_highway = NULL;
            }
            distance_c = strtok(NULL, " ");
            distance = (int)strtol(distance_c, NULL, 10);
            if (in_highway (highway, highway_len, distance) != -1) {
                fprintf(file_output, "non aggiunta\n");
            }
            else {
                index = get_index(distance, highway_len, highway);
                (highway + index)->id = distance;
                len_cars = (int)strtol(strtok(NULL, " "), NULL, 10);
                (highway + index)->cars = (int *) calloc(len_cars, sizeof(int));
                (highway + index)->len_cars = len_cars;
                curr_ptr = (highway + index)->cars;
                for (int i = 0; i < len_cars; i++) {
                    (highway + index)->cars[i] = (int)strtol(strtok(NULL, " "), NULL, 10); 
                }
                n_stations++;
                fprintf(file_output, "aggiunta\n");
            }
            
        }
        else if (strcmp(command, "demolisci-stazione") == 0) {
            distance_c = strtok(NULL, " ");
            distance = (int)strtol(distance_c, NULL, 10);
            index = in_highway (highway, highway_len, distance);
            if (delete_station(highway, index)) {
                fprintf(file_output, "demolita\n");
                n_stations--;
            }
            else {
                fprintf(file_output, "non demolita\n");
            }
        }
        else if (strcmp(command, "aggiungi-auto") == 0) {
            distance_c = strtok(NULL, " ");
            distance = (int)strtol(distance_c, NULL, 10);
            index = in_highway(highway, highway_len, distance);
            car = (int)strtol(strtok(NULL, " "), NULL, 10);
            if (index == -1) {
                fprintf(file_output, "non aggiunta\n");
            }
            else {
                add_car(highway, highway_len, car, index);
                fprintf(file_output, "aggiunta\n");
            }
        }
        else if (strcmp(command, "rottama-auto") == 0) {
            distance_c = strtok(NULL, " ");
            distance = (int)strtol(distance_c, NULL, 10);
            index = in_highway(highway, highway_len, distance);
            car = (int)strtol(strtok(NULL, " "), NULL, 10);
            if (index == -1) {
                fprintf(file_output, "non rottamata\n");
            }
            else {
                if (remove_car(highway, curr_ptr, car, index)) {
                    fprintf(file_output, "rottamata\n");
                }
                else {
                    fprintf(file_output, "non rottamata\n");
                }
            }
        }
        else if (strcmp(command, "pianifica-percorso") == 0) {
            distance_c = strtok(NULL, " ");
            distance = (int)strtol(distance_c, NULL, 10);
            arrival = (int)strtol(strtok(NULL, " "), NULL, 10);
            first_node = 0;
            end_node = 0;
            if (distance < arrival) {
                first_node = distance;
                end_node = arrival;
            } 
            else {
                first_node = arrival;
                end_node = distance;
            }
            if (first_node == end_node) {
                fprintf(file_output, "%d\n", first_node);
            }
            else {
                recalc_station(highway, highway_len, distance, arrival);
                path = (int *) calloc(n_stations, sizeof(int));
                len_path = 0;
                len_path = shortest_path(highway, highway_len, distance, arrival, path, n_stations);
                if (len_path > 0) {
                    if (distance > arrival) {
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
    for (int i = 0; i < highway_len; i++) {
        free((highway + i)->cars);
        free((highway + i)->right_queue);
        free((highway + i)->left_queue);
    }
    free(highway);
    fclose(file_input);
    fclose(file_output);
    if (line)
        free(line);
    return 0;
}

int get_index (int key, int highway_len, station* highway) {
    int index = floor(highway_len*(hash_const*key-floor(hash_const*key)));
    int nxt_index = index;
    int i = 1;
    while ((highway + nxt_index)->id != -2 && (highway + nxt_index)->id != -1) {
        nxt_index = (index + (int)((1/((double)2))*i + (1/((double)2))*(i*i)))%highway_len;
        i++;
    }
    return nxt_index;
}

int in_highway (station *highway, int highway_len, int key) {
    int index = floor(highway_len*(hash_const*key-floor(hash_const*key)));
    int nxt_index = index;
    int i = 1;
    while ((highway + nxt_index)->id != -1) {
        if ((highway + nxt_index)->id == key) {
            return nxt_index;
        }
        nxt_index = (index + (int)((1/((double)2))*i + (1/((double)2))*(i*i)))%highway_len;
        i++;
    }
    return -1;
}

int re_hash (station *highway, station *new_highway,int *highway_len) {
    int new_len = *highway_len * 2;
    int new_index = 0;
    station *curr_station = NULL;
    for (int i = 0; i < *highway_len; i++) {
        curr_station = highway + i;
        if (curr_station->id != -2 && curr_station->id != -1) {
            new_index = get_index(curr_station->id, new_len, new_highway);
            (new_highway + new_index)->id = curr_station->id;
            (new_highway + new_index)->cars = curr_station->cars;
            (new_highway + new_index)->len_cars = curr_station->len_cars;
            (new_highway + new_index)->right_queue = curr_station->right_queue;
            (new_highway + new_index)->len_rqueue = curr_station->len_rqueue;
            (new_highway + new_index)->left_queue = curr_station->left_queue;
            (new_highway + new_index)->len_lqueue = curr_station->len_lqueue;
        }
    }
    *highway_len = *highway_len * 2;
    return 1;
}

int car_found (station *highway, int index, int car) {
    int len_cars = (highway + index)->len_cars;
    for (int i = 0; i < len_cars; i++) {
        if ((highway + index)->cars[i] == car) {
            (highway + index)->cars[i] = -1;
            return 1;
        }
    }
    return 0;
}

int max_car (station *highway, int index) {
    int len_cars = (highway + index)->len_cars;
    int car = 0;
    for (int i = 0; i < len_cars; i++) {
        if ((highway + index)->cars[i] > car) {
            car = (highway + index)->cars[i];
        }
    }
    return car;
}

int compare (const void *elem1, const void *elem2) {
    int i = *(int *)elem1;
    int j = *(int *)elem2;
    if (i < j) {
        return -1;
    }  
    else if (i > j) {
        return 1;
    }
    return 0;
}

int in_array (int *array, int len, int elem) {
    for (int i = 0; i < len; i++) {
        if (array[i] == elem) {
            return 1;
        }
    }
    return 0;
}

int shortest_path (station *highway, int highway_len, int distance, int arrival, int *path, int n_stations) {
    int first_node = 0;
    int end_node = 0;
    int direction = 0;
    if (distance < arrival) {
        first_node = distance;
        end_node = arrival;
        direction = 1;
    } 
    else {
        first_node = arrival;
        end_node = distance;
    }

    int found = -1;
    int queue_len = n_stations;
    queue_str *queue = (queue_str *) malloc(queue_len * sizeof(int*));
    
    for (int i = 0; i < queue_len; i++) {
        (queue + i)->id = -1;
        (queue + i)->father = -1;
    }

    (queue + 0)->id = first_node;
    int index = in_highway(highway, highway_len, first_node);
    (highway + index)->color = 1;

    index = 0;
    found = find_shortest_path(highway, highway_len, queue, queue_len, direction, end_node, &index);

    int path_index = 0;
    if (found != -1) {
        for (int i = 0; i <= index; i++) {
            for (int j = 0; j <= index; j++) {
                if ((queue + j)->id == found) {
                    path[path_index] = found;
                    found = (queue + j)->father;
                    path_index++;
                }
            }
        }
        free(queue);
        return path_index;
    }
    else {
        free(queue);
        return 0;
    }
}

int find_shortest_path (station *highway, int highway_len, queue_str *queue, int queue_len, int direction, int end_node, int *index) {
    queue_str *curr_node = NULL;
    int *next_nodes = (int *) calloc(queue_len, sizeof(int));
    int next_nodes_len = 0;
    int station_index = 0;
    int next_node_index = 0;
    int index_last_node = 1;
    while (*index < index_last_node) {
        curr_node = (queue + *index);
        station_index = in_highway(highway, highway_len, curr_node->id);
        if (direction == 1) {
            next_nodes = (highway + station_index)->right_queue;
            next_nodes_len = (highway + station_index)->right_index;
        }
        else {
            next_nodes = (highway + station_index)->left_queue;
            next_nodes_len = (highway + station_index)->left_index;
        }

        if (curr_node->id == end_node) {
            return curr_node->id;
        }

        if (next_nodes_len > 0) {
            for (int i = 0; i < next_nodes_len; i++) {
                next_node_index = in_highway(highway, highway_len, next_nodes[i]);
                if ((highway + next_node_index)->color == 0) {
                    (highway + next_node_index)->color = 1;
                    (queue + index_last_node)->id = next_nodes[i];
                    (queue + index_last_node)->father = curr_node->id;
                    index_last_node++;
                }
            }   
        }
        *index = *index + 1;
    }
    return -1;
}

int initialize_highway (station *highway, int highway_len) {
    for (int i = 0; i < highway_len; i++) {
        (highway + i)->id = -1;
    }
    return 1;
}

int delete_station (station *highway, int index) {
    if (index != -1) {
        (highway + index)->id = -2;
        free((highway + index)->cars);
        (highway + index)->cars = NULL; // per evitare errore con double free
        (highway + index)->len_cars = 0;
        free((highway + index)->right_queue);
        (highway + index)->right_queue = NULL;
        (highway + index)->len_rqueue= 0;
        free((highway + index)->left_queue);
        (highway + index)->left_queue = NULL;
        (highway + index)->len_lqueue = 0;
        return 1;
    }
    else {
        return 0;
    }
}

int recalc_station (station *highway, int highway_len, int distance, int arrival) {
    int len_cars = 0;
    int max_dist = 0;
    int first_node = 0;
    int end_node = 0;
    // int len_station = 32;

    if (distance < arrival) {
        first_node = distance;
        end_node = arrival;
    } 
    else {
        first_node = arrival;
        end_node = distance;
    }

    for (int i = 0; i < highway_len; i++) {
        if ((highway + i)->id >= first_node && (highway + i)->id <= end_node) {
            if (distance < arrival) {
                free((highway + i)->right_queue);
                (highway + i)->len_rqueue = 32;
                (highway + i)->right_queue = (int *) calloc((highway + i)->len_rqueue, sizeof(int));
                (highway + i)->color = 0;
                (highway + i)->right_index = 0;
            }
            else {
                free((highway + i)->left_queue);
                (highway + i)->len_lqueue = 32;
                (highway + i)->left_queue = (int *) calloc((highway + i)->len_lqueue, sizeof(int));
                (highway + i)->color = 0;
                (highway + i)->left_index = 0;
            }
        }
    }
    for (int i = 0; i < highway_len; i++) {
        if ((highway + i)->id >= first_node && (highway + i)->id <= end_node) {
            len_cars = (highway + i)->len_cars;
            if (len_cars > 0) {
                max_dist = max_car(highway, i);
                for (int j = 0; j < highway_len; j++) {
                    if ((highway + j)->id >= first_node && (highway + j)->id <= end_node) {
                        if (distance < arrival ) {
                            if ((highway + j)->id <= (highway + i)->id + max_dist && (highway + j)->id > (highway + i)->id) {
                                // si può utilizzare calloc e poi ingrandirla al bisogno
                                if ((highway + i)->right_index >= (highway + i)->len_rqueue - 1) {
                                    (highway + i)->len_rqueue = (highway + i)->len_rqueue * 2;
                                    (highway + i)->right_queue = (int *) realloc((highway + i)->right_queue, (highway + i)->len_rqueue * sizeof(int));
                                }
                                (highway + i)->right_queue[(highway + i)->right_index] = (highway + j)->id;
                                (highway + i)->right_index++;
                            }
                        }
                        else {
                            if ((highway + j)->id >= (highway + i)->id - max_dist && (highway + j)->id < (highway + i)->id) {
                                // si può utilizzare calloc e poi ingrandirla al bisogno
                                if ((highway + j)->left_index >= (highway + j)->len_lqueue - 1) {
                                    (highway + j)->len_lqueue = (highway + j)->len_lqueue * 2;
                                    (highway + j)->left_queue = (int *) realloc((highway + j)->left_queue, (highway + j)->len_lqueue * sizeof(int));
                                }
                                (highway + j)->left_queue[(highway + j)->left_index] = (highway + i)->id;
                                (highway + j)->left_index++;

                            }
                        }
                        // si può ottimizzare che stazione è già contenuta in right or left queue, e in caso aggiungerla
                        // a seconda di dove andare si può ricalcolare grafo solo da un lato
                    }
                }
            }
        }
    }
    for (int i = 0; i < highway_len; i++) {
        if ((highway + i)->id >= first_node && (highway + i)->id <= end_node) {
            if (distance < arrival) {
                qsort((highway + i)->right_queue, (highway + i)->right_index, sizeof(int), compare);
            }
            else {
                qsort((highway + i)->left_queue, (highway + i)->left_index, sizeof(int), compare);
            }
        }
    }
    return 1;
}

int add_car (station *highway, int highway_len, int car, int index) {
    int len_cars = 0; 
    (highway + index)->len_cars++;
    len_cars = (highway + index)->len_cars;
    (highway + index)->cars = (int *) realloc((highway + index)->cars, len_cars * sizeof(int));
    (highway + index)->cars[len_cars-1] = car;
    return 1;
}

int remove_car (station *highway, int *curr_ptr, int car, int index) {
    int old_len = 0;
    int len_cars = 0;
    int j = 0;
    old_len = (highway + index)->len_cars;
    len_cars = old_len;
    if (car_found(highway, index, car) == 1) {
        len_cars--;
        curr_ptr = (int *) calloc(len_cars, sizeof(int));
        for (int i = 0; i < old_len; i++) {
            if ((highway + index)->cars[i] != -1) {
                curr_ptr[j] = (highway + index)->cars[i];
                j++;
            }
        }
        free((highway + index)->cars);
        (highway + index)->len_cars = len_cars;
        (highway + index)->cars = curr_ptr;
        return 1;
        // da verificare curr_ptr utilizzo di memoria, e se si può fare una free
    }
    else {
        return 0;
    }
}

// ./test_gen_2023_macos -s 3000 -c 20 -C 100 -r 30
// Test/test_gen_2023/test_gen_2023_macos -s 5000 -c 20 -C 200 -r 30 > Test/archivio_test_aperti/open_200.txt

// open_104 / open_105 ha stesso tempo di parsing del verificatore --> open_104->612s / open_105->296s --> 104 sono meno istruzioni e ci impiega il triplo del tempo
