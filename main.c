#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ezxml.h"
#include "helper.h"

struct list{
    struct node* head;
    struct node* tail;
};

struct node{
    struct node* next;
    struct coord coord;
};

struct coord parseCoord(char* str){
    char *endPtr;
    struct coord struct_coord;
    struct_coord.lon = strtod(str, &endPtr);
    struct_coord.lat = strtod(endPtr+1, NULL);
    return struct_coord;
}

struct list* createList(){
    struct list* list = malloc(sizeof(struct list));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void freeList(struct list* list){
    struct node* node = list->head;
    while(node){
        struct node* tmp = node->next;
        free(node);
        node = tmp;
    }
    free(list);
}

void add(struct list* list, struct coord coord){
    struct node* node = malloc(sizeof(struct node));
    node->next = NULL;
    node->coord = coord;
    if (list->head){
        int dupe = 0;
        struct node* tmp = list->head;
        while (tmp){
            if (tmp->coord.lon == coord.lon && tmp->coord.lat == coord.lat){
                dupe = 1;
                break;
            }
            tmp = tmp->next;
        }
        if (dupe){
            free(node);
        }else{
            list->tail->next = node;
            list->tail = list->tail->next;
        }
    }else{
        list->head = node;
        list->tail = node;
    }
}

struct list* parseFile(char* file_name){
    ezxml_t xml, doc, placemark, point, line_string;
    struct list* list = createList();
    xml = ezxml_parse_file(file_name);
    if (!xml) exit(-1);
    doc = xml;
    if (!ezxml_child(doc, "Placemark")){
        doc = ezxml_child(doc, "Document");
        if (!ezxml_child(doc, "Placemark")){
            doc = ezxml_child(doc, "Folder");
        }
    }
    while (doc){
        placemark = ezxml_child(doc, "Placemark");
        while  (placemark){
            for (point = ezxml_child(placemark, "Point"); point; point = point->next){
                char *str = ezxml_child(point, "coordinates")->txt;
                struct coord struct_coord = parseCoord(str);
                add(list, struct_coord);
            }
            for (line_string = ezxml_child(placemark, "LineString"); line_string; line_string = line_string->next){
                char *str = strtok(ezxml_child(line_string, "coordinates")->txt, " ");
                while (str){
                    struct coord struct_coord = parseCoord(str);
                    add(list, struct_coord);
                    str = strtok(NULL, " \n");
                }
            }
            placemark = placemark->next;
        }
        doc = doc->next;
    }
    ezxml_free(xml);
    return list;
}

int overlap(char* file_name1, char* file_name2, double max_distance){
    struct list* list1 = parseFile(file_name1);
    struct list* list2 = parseFile(file_name2);
    struct node* head1 = list1->head;
    struct node* head2 = list2->head;
    struct node* node1 = head1;
    int result = 0;
    while (node1){
        struct node* node2 = head2;
        while (node2){
            if (distance(node1->coord, node2->coord, 'K') < max_distance){
                printf("Coord1: (%f, %f) Coord2: (%f, %f) overlaps within %.2f km\n", node1->coord.lon, node1->coord.lat, node2->coord.lon, node2->coord.lat, max_distance);
                result = 1;
            }
            node2 = node2->next;
        }
        node1 = node1->next;
    }
    freeList(list1);
    freeList(list2);
    return result;
}

int main(int argc, char* argv[])
{
    if (argc == 4){
        double dis = strtod(argv[3], NULL);
        if (dis == 0) {
            printf("Enter valid distance\n");
            return 0;
        }
        printf("%s\n", overlap(argv[1], argv[2], dis) ? "Routes overlap!" : "Routes does not overlap.");
    }else{
        printf("Arguments invalid\n");
    }
    return 0;
}