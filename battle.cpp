#include "battle.h"

int battle(Airship&& ally, Airship&& enemy) {
    while (true) {
        if (ally.spd >= enemy.spd) {
            auto dmg = ally.hit(enemy);
            enemy.cur_hp -= dmg.first;
            ally.cur_hp -= dmg.second;

            if (enemy.cur_hp == 0) return ally.cur_hp;
            if (ally.cur_hp == 0) return -enemy.cur_hp;

            auto dmg2 = enemy.hit(enemy);
            ally.cur_hp -= dmg2.first;
            enemy.cur_hp -= dmg2.second;

            if (enemy.cur_hp == 0) return ally.cur_hp;
            if (ally.cur_hp == 0) return -enemy.cur_hp;
        }
        else {
            auto dmg2 = enemy.hit(enemy);
            ally.cur_hp -= dmg2.first;
            enemy.cur_hp -= dmg2.second;

            if (enemy.cur_hp == 0) return ally.cur_hp;
            if (ally.cur_hp == 0) return -enemy.cur_hp;

            auto dmg = ally.hit(enemy);
            enemy.cur_hp -= dmg.first;
            ally.cur_hp -= dmg.second;

            if (enemy.cur_hp == 0) return ally.cur_hp;
            if (ally.cur_hp == 0) return -enemy.cur_hp;
        }
    }
}
