#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


// aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n
// demolisci-stazione distanza
// aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere
// rottama-auto distanza-stazione autonomia-auto-da-rottamare
// pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo

// IN QUESTA VERSIONE implemento la hashmap, senza il resize di questa
#define VECTOR_SIZE 65536
#define HASH_UTIL 0xFFFF
#define LINE_LENGTH 1024


typedef struct station{
    int32_t distance;
    int32_t* cars;
    int number_of_cars;
    int32_t best_car;
    struct station* next_with_same_hash;
    // if station is modified, put_car, print_station, read_aggiungi_stazione are to modify
} station;

typedef struct{
    int32_t distance;
    station* pointer;
} ordered_elem;



// commands functions
void read_aggiungi_stazione();
void read_demolisci_stazione();
void read_aggiungi_auto();
void read_rottama_auto();
void read_pianifica_percorso();


// utils
void put_car(station*, int32_t);
int remove_car(station*, int32_t);
int can_I_get_better(int32_t*, int32_t, int32_t, ordered_elem*);
int can_I_get_last_better(int32_t*, int, int32_t, ordered_elem*);
void put_in_ordered(station*, ordered_elem*, int);
void order_ordered(ordered_elem*, int, int);
int find_index_in_ordered(int32_t, ordered_elem*);
void create_ordered(ordered_elem*);

// prints utils
void print_station(station*);
// hash utils
int32_t hash_station(int32_t);
void add_station_to_hashmap(int32_t, station*);
station* find_station_in_hashmap(int32_t);

// free
void free_station(station*);

// data structures
station* stations_hashmap[VECTOR_SIZE];
int number_of_stations = 0;

//testing
void print_index();
void print_ordered();


int main(){
    
    for (int i = 0; i < VECTOR_SIZE; i++){
        stations_hashmap[i] = NULL;
    }
    char command[19];
    // printf("\nInserisci comando:\t");

    //


    //
    
    while (scanf("%s", command) != EOF){

        if(command[0] == 'a'){
            if (command[9] == 's'){
                read_aggiungi_stazione();
            } else {
                read_aggiungi_auto();
            }
        } else if (command[0] == 'd'){
            read_demolisci_stazione();
        } else if (command[0] == 'r'){
            read_rottama_auto();
        } else if(command[0] == 'p'){
            read_pianifica_percorso();
        }


        // TODELETE
        // printf("\nInserisci comando:\t");
    }
    
    // printf("\nCLOSING\n");
    return 0;
}

void read_aggiungi_stazione(){
    // TODELETE
    // printf("In read_aggiungi_stazione\n");

    int32_t distance_to_add;
    if(scanf("%" SCNd32, &distance_to_add))
    ;
    if(find_station_in_hashmap(distance_to_add) != NULL){
        printf("non aggiunta\n");
        char* line = NULL;
        size_t lunghezza_max = 0;
        if(getline(&line, &lunghezza_max, stdin))
        ;
        free(line);
        return;
    }


    int number_of_cars_to_add;
    int32_t* temp_cars = NULL;
    int32_t temp_best_car = 0;
    if(scanf("%" SCNd32, &number_of_cars_to_add))
    ;

    if (number_of_cars_to_add > 0){
        temp_cars = (int32_t*) malloc(number_of_cars_to_add * sizeof(int32_t));
        for (int i = 0; i < number_of_cars_to_add; i++){
            if(scanf("%" SCNd32, &temp_cars[i]))
            ;
            if (temp_cars[i] > temp_best_car) temp_best_car = temp_cars[i];
        }
    } else temp_cars = NULL;
    
    station* new_station = (station*) malloc(sizeof(station));
    new_station->best_car = temp_best_car;
    new_station->cars = temp_cars;
    new_station->distance = distance_to_add;
    new_station->number_of_cars = number_of_cars_to_add;
    new_station->next_with_same_hash = NULL;

    add_station_to_hashmap(hash_station(distance_to_add), new_station);

    printf("aggiunta\n");
    number_of_stations++;
    return;
}

void read_demolisci_stazione(){

    // TODELETE
    // printf("In demolisci-stazione\n");

    int32_t distance_to_demolish;
    if(scanf("%" SCNd32, &distance_to_demolish))
    ;
    int32_t index = hash_station(distance_to_demolish);
    if(stations_hashmap[index] == NULL){
        printf("non demolita\n");
        return;
    }
    station* temp = stations_hashmap[index];
    
    if(stations_hashmap[index]->distance == distance_to_demolish){
        station* pointer_to_demolish = stations_hashmap[index];
        stations_hashmap[index] = pointer_to_demolish->next_with_same_hash;
        free(pointer_to_demolish->cars);
        free(pointer_to_demolish);
        printf("demolita\n");
        number_of_stations--;
        return;
    }

    while (temp->next_with_same_hash != NULL){
        if (temp->next_with_same_hash->distance == distance_to_demolish){
            station* pointer_to_demolish = temp->next_with_same_hash;
            temp->next_with_same_hash = pointer_to_demolish->next_with_same_hash;
            free(pointer_to_demolish->cars);
            free(pointer_to_demolish);
            printf("demolita\n");
            number_of_stations--;
            return;
        }
        temp = temp->next_with_same_hash;
        if(temp->distance < distance_to_demolish) break;
    }

    printf("non demolita\n");

}

void read_aggiungi_auto(){

    //TODELETE
    // printf("In aggiungi-auto\n");

    int32_t station_distance;
    int32_t autonomy;
    if(scanf("%" SCNd32, &station_distance))
    ;
    if(scanf("%" SCNd32, &autonomy))
    ;

    station* station_to_update = find_station_in_hashmap(station_distance);

    if(station_to_update == NULL){
        printf("non aggiunta\n");
        return;
    }
    
    station_to_update->number_of_cars ++;
    int new_cars_num = station_to_update->number_of_cars;
    if(new_cars_num == 1) station_to_update->cars = (int32_t*) malloc(sizeof(int32_t));
    else station_to_update->cars = (int32_t*) realloc(station_to_update->cars, new_cars_num*sizeof(int32_t));
    station_to_update->cars[new_cars_num-1] = autonomy;
    if (autonomy > station_to_update->best_car) station_to_update->best_car = autonomy;
    
    printf("aggiunta\n");
}

void read_rottama_auto(){

    //TODELETE
    // printf("In rottama-auto ");

    int32_t station_distance, autonomy;
    if(scanf("%" SCNd32, &station_distance))
    ;
    if(scanf("%" SCNd32, &autonomy))
    ;
    station* station_to_update = find_station_in_hashmap(station_distance);
    if (station_to_update == NULL){
        // to put \n
        printf("non rottamata\n");
        //TODELETE
        //printf("perchè la stazione non esiste\n");
        return;
    }
    
    int found = -1;
    int number_of_cars = station_to_update->number_of_cars;
    for (int i = 0; i < number_of_cars && found == -1; i++){
        if(station_to_update->cars[i] == autonomy) found = i;
    }
    if(found == -1){
        // to put \n
        printf("non rottamata\n");
        //TODELETE
        //printf("perchè la macchina non esiste\n");
        return;
    }
    for (int i = found; i < station_to_update->number_of_cars - 1; i++){
        station_to_update->cars[i] = station_to_update->cars[i+1];
    }
    station_to_update->number_of_cars--;
    station_to_update->cars = (int32_t*) realloc(station_to_update->cars, number_of_cars * sizeof(int32_t));
    if (autonomy == station_to_update->best_car){
        int32_t temp_best_car = 0;
        for (int i = 0; i < station_to_update->number_of_cars; i++){
            if(station_to_update->cars[i] > temp_best_car) temp_best_car = station_to_update->cars[i];
        }
        station_to_update->best_car = temp_best_car;
    }
    printf("rottamata\n");
}

void read_pianifica_percorso(){

    //TODELETE
    // printf("In pianifica-percorso ");

    int32_t start, destination;
    if(scanf("%" SCNd32, &start))
    ;
    if(scanf("%" SCNd32, &destination))
    ;
    if (start == destination){
        printf("%" SCNd32, start);
        printf("\n");
        return;
    }
    int32_t* path = (int32_t*) malloc(32 * sizeof(int32_t));
    path[0] = destination;
    if(start < destination){
        // start < destination            ---- start --------> destination ------

        int32_t start_best_car = find_station_in_hashmap(start)->best_car;
        int32_t reached_by_start;
        if(start_best_car > 2147483647-start) reached_by_start = 2147483647;
        else reached_by_start = start + start_best_car;

        if(reached_by_start >= destination){
            printf("%" SCNd32, start);
            printf(" %" SCNd32, destination);
            printf("\n");
            return;
        }
        
        // creo un array ordinato di #stazioni stazioni*
        ordered_elem ordered_stations[number_of_stations];
        // for(int i=0; i<number_of_stations; i++) ordered_stations[i] = NULL;
        int inserted = 0;
        station* temp_pointer = NULL;
        for(int i=0; inserted < number_of_stations; i++){
            temp_pointer = stations_hashmap[i];
            while(temp_pointer != NULL){
                put_in_ordered(temp_pointer, ordered_stations, inserted);
                inserted++;
                temp_pointer = temp_pointer->next_with_same_hash;
            }
        }
        order_ordered(ordered_stations, 0, number_of_stations-1);

        int index_of_start = find_index_in_ordered(start, ordered_stations);
        int index_of_destination = find_index_in_ordered(destination, ordered_stations);
        int curr_dest = destination;
        int index_of_curr_dest = index_of_destination;
        int i=index_of_start;

        int32_t* path = (int32_t*) malloc(32 * sizeof(int32_t));
        int stops = 1;
        
        while(1){
            int found_this = 0;
            i=index_of_start;
            while(i<index_of_curr_dest && !found_this){
                int32_t curr_distance = ordered_stations[i].distance;
                int32_t curr_best_car = ordered_stations[i].pointer->best_car;
                
                int32_t curr_reached;
                if(2147483647 - curr_distance <= curr_best_car) curr_reached = 2147483647;
                else curr_reached = curr_distance + curr_best_car;

                if(curr_reached >= curr_dest){
                    if(i==index_of_start){
                        printf("%" SCNd32, start);
                        for (int j = stops-1; j > 0; j--){
                            printf(" %" SCNd32, path[j]);
                        }
                        printf(" %" SCNd32, destination);
                        printf("\n");
                        free(path);
                        return;
                    }
                    found_this = 1;
                    path[stops] = curr_distance;
                    stops++;
                    if (stops % 32 == 0){
                        path = (int32_t*) realloc(path, (stops+32) * sizeof(int32_t));
                    }
                    curr_dest = curr_distance;
                    index_of_curr_dest = find_index_in_ordered(curr_dest, ordered_stations);
                }
                i++;
            }
            if(found_this == 0){
                printf("nessun percorso\n");
                return;
            }

        }
        
    } else {

        //useful things
        int32_t distance_to_reach = destination;
        int32_t reached_by_start;
        if(start <= find_station_in_hashmap(start)->best_car) reached_by_start = 0; // maybe here I print and return; same problem can happen with positive way and overflow
        else reached_by_start = start - find_station_in_hashmap(start)->best_car;
        if(reached_by_start <= destination){
            printf("%" SCNd32, start);
            printf(" %" SCNd32, destination);
            printf("\n");
            return;
        }
        
        // creo un array ordinato di #stazioni stazioni*
        ordered_elem ordered_stations[number_of_stations];
        // for(int i=0; i<number_of_stations; i++) ordered_stations[i] = NULL;
        int inserted = 0;
        station* temp_pointer = NULL;
        for(int i=0; inserted < number_of_stations; i++){
            temp_pointer = stations_hashmap[i];
            while(temp_pointer != NULL){
                put_in_ordered(temp_pointer, ordered_stations, inserted);
                inserted++;
                temp_pointer = temp_pointer->next_with_same_hash;
            }
        }
        order_ordered(ordered_stations, 0, number_of_stations-1);

        int dtr_index;
        int start_index = find_index_in_ordered(start, ordered_stations);


        // ORA CAMBIA PURE QUESTA PARTE, PERCHè SO ORDINATE
        // start > destination            ---- destination <-------- start ------
        int stops = 1;
        
        while(1){
            if(start < distance_to_reach){
                printf("nessun percorso\n");
                free(path);
                return;
            }
            if(reached_by_start <= distance_to_reach){
                int i=1;
                path[0] = destination;
                while(i<stops-1){
                    if(!can_I_get_better(path, i, start, ordered_stations)) i++;
                }
                while(can_I_get_last_better(path, stops-1, start, ordered_stations));
                
                printf("%" SCNd32, start);
                for (int i = stops-1; i > 0; i--){
                    printf(" %" SCNd32, path[i]);
                }
                printf(" %" SCNd32, destination);
                printf("\n");
                free(path);
                return;
            }
            int32_t next_dtr = distance_to_reach;
            dtr_index = find_index_in_ordered(next_dtr, ordered_stations);
            start_index = find_index_in_ordered(start, ordered_stations);
            for (int i = start_index-1; i > dtr_index ; i--){
            
                int32_t temp_distance = ordered_stations[i].pointer->distance;
                int32_t temp_reached;
                if(temp_distance <= ordered_stations[i].pointer->best_car) temp_reached = 0;
                else temp_reached = temp_distance - ordered_stations[i].pointer->best_car;

                if(temp_reached <= distance_to_reach){
                    next_dtr = temp_distance;
                    break;
                }

            }
            if(next_dtr == distance_to_reach){
                printf("nessun percorso\n");
                free(path);
                return;
            }
            if (stops % 32 == 0){
                path = (int32_t*) realloc(path, (stops+32) * sizeof(int32_t));
            }
            path[stops] = next_dtr;
            //printf("Putting %d\n", next_dtr);
            distance_to_reach = next_dtr;
            stops++;
        }
    }
}

int can_I_get_better(int32_t* path, int32_t curr_stop, int32_t start, ordered_elem* ordered_stations){
    if(curr_stop == 0) return 0;
    int i_got_better = 0;
    // I get better when someone less than me reaches path[i-1] AND is reached by path[i+1]
    int32_t must_be_less_than = path[curr_stop];
    int index_must_be_less_than = find_index_in_ordered(must_be_less_than, ordered_stations);
    int32_t must_be_at_least = find_station_in_hashmap(path[curr_stop+1])->distance - find_station_in_hashmap(path[curr_stop+1])->best_car;
    //int index_must_be_at_least = find_index_in_ordered(must_be_at_least, ordered_stations);
    int32_t must_reach = find_station_in_hashmap(path[curr_stop-1])->distance;

    //TODELETE
    //printf("Looking for a station >= %d and < %d that reaches %d\n", must_be_at_least, must_be_less_than, must_reach);

    // scorro l'ordered array
    station* candidate_station;
    for (int i = index_must_be_less_than-1; (candidate_station = ordered_stations[i].pointer)->distance >= must_be_at_least; i--){
        
        int32_t candidate = candidate_station->distance;
        int32_t candidate_reaches;
        if(candidate_station->best_car > candidate) candidate_reaches = 0;
        else candidate_reaches = candidate - candidate_station->best_car;
        //TODELETE
        //printf("\tCandidate %d is at least %d and reaches %d\n", candidate, must_be_at_least, candidate_reaches);
        if(candidate_reaches <= must_reach){
            path[curr_stop] = candidate;
            i_got_better = 1;
            
            //printf("Putting in %d : %d\n\tcause in %d there is %d\n\tin %d there is %d\n", curr_stop, candidate, curr_stop-1, path[curr_stop-1], curr_stop+1, path[curr_stop+1]);
            break;
        }
    }
    if(i_got_better == 1){
        while(can_I_get_better(path, curr_stop-1, start, ordered_stations));
    }
    return i_got_better;
}

int can_I_get_last_better(int32_t* path, int curr_stop, int32_t start, ordered_elem* ordered_stations){
    if(curr_stop == 0) return 0;
    int i_got_better = 0;
    // I get better when someone less than me reaches path[i-1] AND is reached by path[i+1]
    int32_t must_be_less_than = path[curr_stop];
    int index_must_be_less_than = find_index_in_ordered(must_be_less_than, ordered_stations);
    int32_t must_be_at_least = start - ordered_stations[find_index_in_ordered(start, ordered_stations)].pointer->best_car;
    //int index_must_be_at_least = find_index_in_ordered(must_be_at_least, ordered_stations);
    int32_t must_reach = find_station_in_hashmap(path[curr_stop-1])->distance;

    //TODELETE
    //printf("Looking for a station >= %d and < %d that reaches %d\n", must_be_at_least, must_be_less_than, must_reach);

    station* candidate_station;
    for (int i = index_must_be_less_than-1; (candidate_station = ordered_stations[i].pointer)->distance >= must_be_at_least; i--){
        
        int32_t candidate = candidate_station->distance;
        int32_t candidate_reaches;
        if(candidate_station->best_car > candidate) candidate_reaches = 0;
        else candidate_reaches = candidate - candidate_station->best_car;
        //TODELETE
        //printf("\tCandidate %d reaches %d\n", candidate, candidate_reaches);
        if(candidate_reaches <= must_reach){
            path[curr_stop] = candidate;
            i_got_better = 1;
            break;
        }
    }
    if(i_got_better == 1){
        while(can_I_get_better(path, curr_stop-1, start, ordered_stations));
    }
    return i_got_better;
}

void put_in_ordered(station* temp_pointer, ordered_elem* ordered_stations, int inserted){
    ordered_elem new_elem;
    new_elem.distance = temp_pointer->distance;
    new_elem.pointer = temp_pointer;
    ordered_stations[inserted] = new_elem;
}

int find_index_in_ordered(int32_t distance, ordered_elem* ordered_stations){
    int low = 0;
    int high = number_of_stations - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (ordered_stations[mid].distance == distance) {
            return mid;
        } else if (ordered_stations[mid].distance < distance) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}

int32_t hash_station(int32_t distance){
    return distance & HASH_UTIL;
}

// inserisce nella hashmap in modo che le collisioni siano ordinate in modo decrescente
void add_station_to_hashmap(int32_t hash, station* pointer){
    if(stations_hashmap[hash] == NULL){
        stations_hashmap[hash] = pointer;
        return;
    }
    station* temp = stations_hashmap[hash];
    if(temp->distance < pointer->distance){
        pointer->next_with_same_hash = temp;
        stations_hashmap[hash] = pointer;
        return;
    }
    while(temp->next_with_same_hash != NULL){
        if(temp->next_with_same_hash->distance < pointer->distance){
            pointer->next_with_same_hash = temp->next_with_same_hash;
            temp->next_with_same_hash = pointer;
            return;
        }
        temp = temp->next_with_same_hash;
    }
    temp->next_with_same_hash = pointer;
}

station* find_station_in_hashmap(int32_t distance){
    station* temp = stations_hashmap[hash_station(distance)];
    if (temp == NULL) return NULL;
    if (temp->distance == distance){
        return temp;
    }
    while (temp->next_with_same_hash != NULL){
        if(temp->next_with_same_hash->distance == distance){
            return temp->next_with_same_hash;
        }
        temp = temp->next_with_same_hash;
        if(temp->distance < distance) return NULL;
    }
    return NULL;    
}

void create_ordered(ordered_elem* ordered_stations){
    // creo un array ordinato di #stazioni stazioni*
    
    // for(int i=0; i<number_of_stations; i++) ordered_stations[i] = NULL;
    int inserted = 0;
    station* temp_pointer = NULL;
    for(int i=0; inserted < number_of_stations; i++){
        temp_pointer = stations_hashmap[i];
        while(temp_pointer != NULL){
            put_in_ordered(temp_pointer, ordered_stations, inserted);
            inserted++;
            temp_pointer = temp_pointer->next_with_same_hash;
        }
    }
}

// Merge two sorted subarrays into one sorted array
void merge(ordered_elem* arr, int start, int mid, int end) {
    int leftSize = mid - start + 1;
    int rightSize = end - mid;

    ordered_elem left[leftSize];
    ordered_elem right[rightSize];

    // Copy data to temporary arrays
    for (int i = 0; i < leftSize; i++) {
        left[i] = arr[start + i];
    }
    for (int i = 0; i < rightSize; i++) {
        right[i] = arr[mid + 1 + i];
    }

    int i = 0, j = 0, k = start;

    // Merge the temporary arrays back into arr[]
    while (i < leftSize && j < rightSize) {
        if (left[i].distance <= right[j].distance) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of left[], if any
    while (i < leftSize) {
        arr[k] = left[i];
        i++;
        k++;
    }

    // Copy the remaining elements of right[], if any
    while (j < rightSize) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void order_ordered(ordered_elem* arr, int start, int end) {
    if (start < end) {
        int mid = start + (end - start) / 2;

        // Recursively sort the two halves
        order_ordered(arr, start, mid);
        order_ordered(arr, mid + 1, end);

        // Merge the sorted halves
        merge(arr, start, mid, end);
    }
}

void print_index(){
    int32_t distance;
    if(scanf("%" SCNd32, &distance))
    ;
    ordered_elem ordered_stations[number_of_stations];
    create_ordered(ordered_stations);
    int index = find_index_in_ordered(distance, ordered_stations);
    printf("Index is %d\n", index);
}
void print_ordered(){
    ordered_elem ordered_stations[number_of_stations];
    create_ordered(ordered_stations);
    order_ordered(ordered_stations, 0, number_of_stations-1);
    for(int i=0; i<number_of_stations; i++){
        print_station(ordered_stations[i].pointer);
    }
    printf("\n");
}
void print_station(station* station){
    if(station == NULL) {
        printf("Stazione nulla\n");
        return;
    }
    printf("Station %d : best car %d \n", station->distance, station->best_car);
}

//METTERLO UN PO' OVUNQUE
//int end = (start + 2 * i - 1) < (number_of_stations - 1) ? (start + 2 * i - 1) : (number_of_stations - 1);


