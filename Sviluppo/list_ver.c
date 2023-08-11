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

typedef struct bst {
    struct stn *id;
    struct bst *father;
    struct bst *right;
    struct bst *left;
} bst;

int get_int (FILE *file_input);
bst* add_bst (bst **root, bst *leaf);
bst* successor (bst *leaf);
bst* min_bst (bst *root);
bst* del_bst (bst **root, bst *leaf);
bst* find_bst (bst *root, int stn);
stn* add_station (stn *first_stn, stn *new_stn, stn *last_stn, int station, int *cars, int len_cars, int m_car, int *f);
stn* del_station (stn *first_stn, stn *to_del, int station, int *f);
int add_car (stn *last_stn, int station, int car);
int del_car (stn *first_stn, int *new_cars, int station, int car);
int search_path (stn *first_stn, int stn_start, int stn_end, int *path, int n_stations);
int bfs_dx (stn *end_stn, q_str *queue, int direction);
int bfs_sx (stn *end_stn, q_str *queue, int direction, int m_car);
int rebuild_graph (stn *start_stn, stn *end_stn, stn **next_nodes);

int main (int argc, char *argv[])
{
    char line[50];
    char c = 0;
    int index = 0;
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
    bst *root = NULL;
    bst *leaf = NULL;
    bst *result_bst = NULL;
    bst *found = NULL;

    while ((c = getc_unlocked(file_input)) != EOF) {
        index = 0;
        line[index] = c;
        index++;
        while ((c = getc_unlocked(file_input)) != ' ') {
            line[index] = c;
            index++;
        }
        line[index] = '\0';
        if (strcmp(line, "aggiungi-stazione") == 0) {
            int m_car = 0;
            f = 0;
            station = get_int(file_input);
            len_cars = get_int(file_input);
            cars = (int *) calloc(len_cars, sizeof(int));
            for (int i = 0; i < len_cars; i++) {
                cars[i] = get_int(file_input);
                if (cars[i] > m_car) {
                    m_car = cars[i];
                }
            }

            new_stn = (stn *) calloc(1, sizeof(stn));
            new_stn->id = station;
            new_stn->m_car = m_car;
            new_stn->cars = cars;
            new_stn->len_cars = len_cars;

            leaf = (bst *) calloc(1, sizeof(bst));
            leaf->id = new_stn;
            result_bst = add_bst(&root, leaf);
            
            if (result_bst != NULL) {
                last_stn = result_bst->id;
            }

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
        else if (strcmp(line, "demolisci-stazione") == 0) {
            station = get_int(file_input);
            f = 0;
            found = find_bst(root, station);
            if (found != NULL) {
                result = del_station(first_stn, found->id, station, &f);
                del_bst(&root, found);
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
            }
            else {
                fprintf(file_output, "non demolita\n");
            }
        }
        else if (strcmp(line, "aggiungi-auto") == 0) {
            station = get_int(file_input);
            car = get_int(file_input);
            found = find_bst(root, station);
            if (found != NULL) {
                if (add_car(found->id, station, car) == 1) {
                    fprintf(file_output, "aggiunta\n");
                }
            }
            else {
                fprintf(file_output, "non aggiunta\n");
            }
        }
        else if (strcmp(line, "rottama-auto") == 0) {
            station = get_int(file_input);
            car = get_int(file_input);
            found = find_bst(root, station);
            if (found != NULL) {
                if (del_car(found->id, new_cars, station, car) == 1) {
                    fprintf(file_output, "rottamata\n");
                }
                else {
                    fprintf(file_output, "non rottamata\n");
                }
            }
            else {
                fprintf(file_output, "non rottamata\n");
            }
        }
        else if (strcmp(line, "pianifica-percorso") == 0) {
            station = get_int(file_input);
            destination = get_int(file_input);

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
    return 0;
}

int get_int (FILE *input_file) {
    int number = 0;
    char c = 0;
    while ((c = getc_unlocked(input_file)) != ' ' && c != '\n') {
        number = (c - '0') + (number << 3) + (number << 1);
    }
    return number;
}

bst* add_bst (bst **root, bst *leaf) {
    bst *prv_st = NULL;
    bst *curr_stn = *root;
    while (curr_stn != NULL) {
        prv_st = curr_stn;
        if (leaf->id->id < curr_stn->id->id) {
            curr_stn = curr_stn->left;
        }
        else if (leaf->id->id == curr_stn->id->id) {
            return NULL;
        }
        else {
            curr_stn = curr_stn->right;
        }
    }
    leaf->father = prv_st;
    if (prv_st == NULL) {
        *root = leaf;
        return *root;
    }
    else if (leaf->id->id < prv_st->id->id) {
        prv_st->left = leaf;
    }
    else {
        prv_st->right = leaf;
    }
    return prv_st;
}

bst* successor (bst *leaf) {
    bst *prv = NULL;
    if (leaf->right != NULL) {
        return min_bst(leaf->right);
    }
    prv = leaf->father;
    while (prv != NULL && prv->right == leaf) {
        leaf = prv;
        prv = prv->father;
    }
    return prv;
}

bst* min_bst (bst *root) {
    bst *curr = root;
    while (curr->left != NULL) {
        curr = curr->left;
    }
    return curr;
}

bst* del_bst (bst **root, bst *leaf) {
    bst *del = NULL;
    bst *tree = NULL;
    if (leaf->left == NULL || leaf->right == NULL) {
        del = leaf;
    }
    else {
        del = successor(leaf);
    }
    if (del->left != NULL) {
        tree = del->left;
    }
    else {
        tree = del->right;
    }
    if (tree != NULL) {
        tree->father = del->father;
    }
    if (del->father == NULL) {
        *root = tree;
    }
    else if (del == del->father->left) {
        del->father->left = tree;
    }
    else {
        del->father->right = tree;
    }
    if (del->id != leaf->id) {
        leaf->id = del->id;
    }
    if (del != leaf) {
        leaf->id = del->id;
    }
    free(del);
    return NULL;
}

bst* find_bst (bst *root, int stn) {
    if (root == NULL || root->id->id == stn) {
        return root;
    }
    if (root->id->id < stn) {
        return find_bst(root->right, stn);
    }
    else {
        return find_bst(root->left, stn);
    }
}

stn* add_station (stn *first_stn, stn *new_stn, stn *last_stn, int station, int *cars, int len_cars, int m_car, int *f) {
    stn *curr_stn = last_stn;

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

stn* del_station (stn *first_stn, stn *to_del, int station, int *f) {
    stn *curr_stn = to_del;
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

int add_car (stn *last_stn, int station, int car) {
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
        return 1;
    }
    return 0;
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
            }
        #endif
        free(queue);
        # if debug_queue
            printf("\n\n----\n\n");
        # endif
        return len_path;
    }
    else {
        free(queue);
        # if debug_queue
            printf("\n\n----\n\n");
        # endif
        return 0;
    }
}

int bfs_dx (stn *end_stn, q_str *queue, int direction) {
    stn *index = NULL;
    stn *curr_stn = NULL;
    stn *last_stn = NULL;
    q_str *v;
    int len_queue = 1;
    int i = 0;
    int car = 0;
    last_stn = (queue + i)->id;
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
        curr_stn = last_stn;
        if (curr_stn->nxt != NULL) {
            car = index->m_car;
            curr_stn = curr_stn->nxt;
            while (curr_stn != NULL && curr_stn->id <= index->id + car && curr_stn->id > index->id) {
                if (curr_stn->color == false) {
                    curr_stn->color = true;
                    (queue + len_queue)->id = curr_stn;
                    (queue + len_queue)->father = index;
                    last_stn = curr_stn;
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
    stn *last_stn = NULL;
    q_str *v;
    int len_queue = 1;
    int i = 0;
    int car = 0;
    last_stn = (queue + i)->id;
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
        curr_stn = last_stn;
        if (curr_stn->nxt != NULL) {
            curr_stn = curr_stn->nxt;
            while (curr_stn != NULL && curr_stn->id <= index->id + m_car && curr_stn->id <= end_stn->id) {
                car = curr_stn->m_car;
                if (index->id >= curr_stn->id - car && index->id < curr_stn->id) {
                    if (curr_stn->color == false) {
                        curr_stn->color = true;
                        (queue + len_queue)->id = curr_stn;
                        (queue + len_queue)->father = index;
                        last_stn = curr_stn;
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
