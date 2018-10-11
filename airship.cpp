#include "airship.h"
#include <utility>

std::pair<int,int> Airship::hit(const Airship& enemy) {
    int luck_factor = luk > enemy.luk ? (luk*1000/enemy.luk)/1000 : 1;
    double att_factor = att - enemy.def;

    double base_damage = (max_hp*0.75 + cur_hp*0.25)/5;
    int damage = base_damage * (luck_factor + (att_factor > 0 ? 1/(1 + 200/att_factor) : att_factor/(200 - 2*att_factor)));

    damage += additional;
    if (luk >= enemy.luk) damage += critical;

    return {damage, enemy.reflect};
}
