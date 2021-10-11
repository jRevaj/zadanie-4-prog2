#include "data.h"

UnitType monster_types[] = {
        {"Godzilla",      200, 220},
        {"Kong",          220, 180},
        {"Mechagodzilla", 230, 230}
};

UnitType enemy_types[] = {
        // land
        {"Infantry",   45,  0}, // pechota
        {"Tank",       65,  100}, // tank
        {"Artillery",  75,  70}, // delostrelectvo

        // water
        {"Cruiser",    100, 75}, // kriznik
        {"Submarine",  105, 65}, // ponorka

        // air
        {"Helicopter", 55,  40}, // helikoptera
        {"Fighter",    110, 110}, // stihacka
        {"Bomber",     120, 100} // bombarder
};