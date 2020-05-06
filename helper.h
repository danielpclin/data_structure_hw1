#ifndef DATA_STRUCTURE_HW1_HELPER_H
#define DATA_STRUCTURE_HW1_HELPER_H

struct coord{
    double lon;
    double lat;
};

double distance(struct coord coord1, struct coord coord2, char unit);

#endif //DATA_STRUCTURE_HW1_HELPER_H
