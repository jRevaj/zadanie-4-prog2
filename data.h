#ifndef Z4_DATA_H
#define Z4_DATA_H

#define UNIT_MAX_LEVEL 10 // max. level, ktory mozu mat jednotky
#define ENEMY_MIN_INIT_HP 50 // min. pociatocne HP nepriatelskych jednotiek
#define ENEMY_MAX_INIT_HP 100 // max. pociatocne HP nepriatelskych jednotiek
#define MONSTER_INITIAL_HP 500 // pociatocne HP monster jednotiek

#define UNIT_TYPE_NAME_LEN 30 // max. dlzka nazvu typu jednotky

// struktura definujuca typ jednotky
typedef struct {
    char name[UNIT_TYPE_NAME_LEN + 1];
    int att; // attack atribut
    int def; // defense atribut
} UnitType;

// struktura definujuca jednotku
typedef struct {
    const UnitType *type; // typ jednotky
    int hp; // hit points
    int level;
} Unit;

#define MONSTER_TYPE_COUNT 3
// databaza typov monster jednotiek
extern UnitType monster_types[MONSTER_TYPE_COUNT];

#define ENEMY_TYPE_COUNT 8
// databaza typov nepriatelskych jednotiek
extern UnitType enemy_types[ENEMY_TYPE_COUNT];

#endif //Z4_DATA_H
