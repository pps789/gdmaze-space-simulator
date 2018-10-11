#ifndef __GDMAZE_SIMULATOR__AIRSHIP_H__
#define __GDMAZE_SIMULATOR__AIRSHIP_H__

#include <utility>

struct Airship {
    int att, def, spd, luk;
    int additional, reflect, critical, reductional;

    int max_hp, cur_hp;

    std::pair<int,int> hit(const Airship& enemy);
};

#endif
