#include "exerciser.h"

void exercise(connection *C)
{
    query1(C,1,35, 40,\
    0, 13, 19,\
    0, 5, 9,\
    0, 1, 4,\
    0, 0.8, 1.5,\
    0, 0.4, 3.4);
    query1(C, 1, 10, 20, 1, 5, 10, 1, 7, 10, 0, 0, 0, 1, 0.2, 1.0, 1, 0.5, 3.0);
    query2(C, "Green");
    query3(C, "Miami");
    query4(C, "NC", "Green");
    query5(C, 12);


}
