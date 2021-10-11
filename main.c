#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "rng.h"

FILE* pFile;
Unit *pMonster, *pEnemies;
UnitType *pMonsterType, *pEnemyType;
char *selMonster, *filename, name[UNIT_TYPE_NAME_LEN];
int enemCount, seed, idxEnemy, attackSwitch = 0, dmg, baseDMG, strength, defense, c1, c2, d1, d2, totalMdmg, totalEdmg, line, att, def;

void initMonster(char *argv[]) {
    // funkcia na nacitanie monstra z databazy monster_types
    selMonster = argv[1];
    for (int i = 0; i < MONSTER_TYPE_COUNT; i++) {
        if(strcmp(selMonster, monster_types[i].name) == 0) {
            // alokacia pamate
            pMonster = (Unit *) malloc(sizeof(Unit));
            pMonsterType = (UnitType *) malloc(sizeof(UnitType));
            // nacitanie udajov
            strcpy(pMonsterType->name, monster_types[i].name);
            pMonsterType->att = monster_types[i].att;
            pMonsterType->def = monster_types[i].def;
            pMonster->type = pMonsterType;
            pMonster->hp = MONSTER_INITIAL_HP;
            pMonster->level = 0;
            break;
        }
    }
}

void initEnemies(char *argv[]) {
    // funkcia na nacitanie nepriatelov z databazy enemy_types
    // ziskanie hodnoty z argumentu
    enemCount = strtol(argv[2], NULL, 10);
    seed = strtol(argv[3], NULL, 10);
    // nacitanie funkcie rnd
    srnd(seed);
    // alokacie pamate pre pole nepriatelov
    pEnemies = (Unit *) malloc(enemCount * sizeof(Unit));

    for (int i = 0; i < enemCount; i++) {
        // vyberanie nahodneho nepriatela z databazy
        int enemy = rnd(0, ENEMY_TYPE_COUNT-1);
        pEnemyType = (UnitType *) malloc(sizeof(UnitType));
        // nacitanie hodnot vybrateho nepriatela
        strcpy(pEnemyType->name, enemy_types[enemy].name);
        pEnemyType->att = enemy_types[enemy].att;
        pEnemyType->def = enemy_types[enemy].def;
        pEnemies[i].type = pEnemyType;
        pEnemies[i].hp = rnd(ENEMY_MIN_INIT_HP, ENEMY_MAX_INIT_HP);
        pEnemies[i].level = rnd(0, UNIT_MAX_LEVEL);
        // vypis nacitaneho nepriatela
        printf("[%d] %s, ATT:%d, DEF:%d, HP:%d, LVL:%d\n", i, pEnemies[i].type->name, pEnemies[i].type->att, pEnemies[i].type->def,
               pEnemies[i].hp, pEnemies[i].level);
    }
    printf("\n");
}

int findLowestHP() {
    // funkcia na najdenie nepriatela s najnizsim hp - vrati jeho index
    int tmp = -1, idx = -1, i, j;

    // nacitanie prveho ziveho nepriatela a jeho hp
    for (i = 0; i < enemCount; i++) {
        if (pEnemies[i].hp > 0) {
            tmp = pEnemies[i].hp;
            idx = i;
            break;
        }
    }

    // hladanie ci existuje nejaky nepriatel s nizsim hp
    for (j = i; j < enemCount - 1; j++) {
        if ((pEnemies[j + 1].hp > 0) && (pEnemies[j + 1].hp < tmp)) {
            tmp = pEnemies[j + 1].hp;
            idx = j + 1;
        } else {
            continue;
        }
    }

    // ak neexistuju ziadny zivy nepriatelia vrati hodnotu -1
    if((tmp < 0) && (idx < 0)) {
        return -1;
    }

    return idx;
}

void printUnitStatus() {
    // funkcia na vypisanie aktualnych hodnot monstra a nepriatelov
    printf("\n%s, ATT:%d, DEF:%d, HP:%d, LVL:%d\n", pMonster->type->name, pMonster->type->att, pMonster->type->def,
           pMonster->hp, pMonster->level);

    for (int i = 0; i < enemCount; i++) {
        printf("[%d] %s, ATT:%d, DEF:%d, HP:%d, LVL:%d\n", i, pEnemies[i].type->name, pEnemies[i].type->att, pEnemies[i].type->def,
               pEnemies[i].hp, pEnemies[i].level);
    }

    printf("\n");
}

void memCleanup() {
    // uvolnenie alokovanej pamate
    free(pMonster);
    free(pMonsterType);
    free(pEnemies);
    free(pEnemyType);
}

int main(int argc, char *argv[]) {
    // zistovanie ci bol zadany prepinac -i
    if (argc == 6 || argc == 5) {
        // test ci bol zadany spravne
        if (strcmp(argv[4], "-i") != 0) {
            return 4;
        } else {
            filename = argv[5];

            pFile = fopen(filename, "r");

            // test ci bola zadana spravna cesta k suboru
            if (pFile == NULL) { return 2; }
            else {
                int cnt = 0;
                while (1) {
                    // nacitavanie riadkov zo suboru a ich testovanie ci su validne
                    line = fscanf(pFile, "%s %d %d\n", name, &att, &def);
                    if (line == EOF) {
                        break;
                    } else if ((line < 3) || (line > 3)) {
                        return 3;
                    } else {
                        // zapisovanie do databazy enemy_types
                        strcpy(enemy_types[cnt].name, name);
                        enemy_types[cnt].att = att;
                        enemy_types[cnt].def = def;
                    }
                    cnt++;
                }
            }

            fclose(pFile);
        }
    }

    // nacitanie monstra a test ci bolo nacitane spravne
    initMonster(argv);
    if (pMonster) {
        printf("%s, ATT:%d, DEF:%d, HP:%d, LVL:%d\n", pMonster->type->name, pMonster->type->att, pMonster->type->def,
               pMonster->hp, pMonster->level);
    } else {
        return 1;
    }

    // nacitanie a uvodny vypis nepriatelov
    initEnemies(argv);

    while (1) {
        // urcenie kto utoci
        if (attackSwitch == 0) {
            // utok monstra
            // najdenie nepriatela s najnizsim hp
            if ((idxEnemy = findLowestHP()) > -1) {
                // vypocet dmg
                c1 = rnd(1, pMonster->hp);
                c2 = pMonster->type->att;
                d1 = rnd(1, pEnemies[idxEnemy].hp);
                d2 = pEnemies[idxEnemy].type->def;
                baseDMG = 30 + pMonster->level - pEnemies[idxEnemy].level;
                strength = 100 + c1 + c2;
                defense = 100 + d1 + d2;
                dmg = (baseDMG * strength) / defense;

                // pripocitanie dmg k statistike celkoveho dmg ktore dalo monstrum
                totalMdmg += dmg;

                // odpocitanie dmg od hp nepriatela
                pEnemies[idxEnemy].hp = pEnemies[idxEnemy].hp - dmg;

                //vypis utoku
                printf("%s => %d => [%d] %s\n", pMonster->type->name, dmg, idxEnemy, pEnemies[idxEnemy].type->name);
            } else {
                // ak nenajde ziadneho nepriatela tak vypise vitaza a statistiku
                printf("Winner: %s\n", pMonster->type->name);
                printf("Total monster DMG: %d\n", totalMdmg);
                printf("Total enemies DMG: %d", totalEdmg);
                break;
            }
            attackSwitch = 1;
        } else {
            // utok armady
            // prepinanie nepriatelov zaradom ako su nacitany
            for (int i = 0; i < enemCount; i++) {
                // ak ma monstrum hp 0 alebo mensie ukonci utok a vypise vitaza
                if (pMonster->hp <= 0) {
                    break;
                } else if (pEnemies[i].hp <= 0) {
                    // ak ma nepriatel hp 0 alebo mensie nemoze zautocit - preskoci na dalsieho
                    continue;
                } else {
                    // vypocet dmg
                    c1 = rnd(1, pEnemies[i].hp);
                    c2 = pEnemies[i].type->att;
                    d1 = rnd(1, pMonster->hp);
                    d2 = pMonster->type->def;
                    baseDMG = 30 + pEnemies[i].level - pMonster->level;
                    strength = 100 + c1 + c2;
                    defense = 100 + d1 + d2;
                    dmg = (baseDMG * strength) / defense;

                    // pripocitanie dmg k statistike celkoveho dmg ktore dali nepriatelia
                    totalEdmg += dmg;

                    // odpocitanie dmg od hp monstra
                    pMonster->hp = pMonster->hp - dmg;

                    //vypis utoku
                    printf("[%d] %s => %d => %s\n", i, pEnemies[i].type->name, dmg, pMonster->type->name);
                }
            }
            // test ci monstrum zije, ak ano test ci dosiahlo max level, ak zvysenie levelu o 1
            if ((pMonster->hp > 0) && (pMonster->level < UNIT_MAX_LEVEL)) {
                pMonster->level = pMonster->level + 1;
            } else {
                // ak ma monstrum hp 0 alebo nizsie vypise vitaza a statistiku
                if (pMonster->hp <= 0) {
                    printUnitStatus();
                    printf("Winner: Enemy\n");
                    printf("Total monster DMG: %d\n", totalMdmg);
                    printf("Total enemies DMG: %d", totalEdmg);
                    break;
                }
            }

            // vypisanie statistiky po kole
            printUnitStatus();
            attackSwitch = 0;
        }
    }

    // uvolnenie alokovanej pamate
    memCleanup();
    return 0;
}
