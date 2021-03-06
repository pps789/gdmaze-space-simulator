#include <string>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <set>
#include "airship.h"
#include "battle.h"
using namespace std;

int A[60][4];
map<vector<int>, long long> L[21], R[21];
vector<pair<vector<int>, long long>> Lv[21], Rv[21];

const int SMAX = 700;
unordered_map<int,int> cache_up[SMAX][SMAX], cache_down[SMAX][SMAX];
int N, M;

bool cache_check(const unordered_map<int,int>& cache, int x) {
    return cache.count(x);
}

Airship ally_default, enemy_default;

void parse() {
    ifstream airship_ifs("airship.txt"), config_ifs("config.txt");

    airship_ifs >> N;
    printf("Number of airships: %d\n", N);
    for(int i=0;i<N;i++) {
        for(int j=0;j<4;j++) airship_ifs >> A[i][j];
    }
    printf("Read %d airships.\n", N);

    // TODO: fix this!
    string trash;
    config_ifs >> trash >> ally_default.additional;
    config_ifs >> trash >> ally_default.reflect;
    config_ifs >> trash >> ally_default.critical;
    config_ifs >> trash >> ally_default.reductional;
    config_ifs >> trash >> M;
    
    config_ifs >> trash >> ally_default.max_hp;
    config_ifs >> trash >> ally_default.att >> ally_default.def
        >> ally_default.spd >> ally_default.luk;

    config_ifs >> trash >> enemy_default.max_hp;
    config_ifs >> trash >> enemy_default.att >> enemy_default.def
        >> enemy_default.spd >> enemy_default.luk;

    ally_default.cur_hp = ally_default.max_hp;
    enemy_default.cur_hp = enemy_default.max_hp;
}

int Lsize, Rsize;
void half_gen() {
    Lsize = N/2, Rsize = N-Lsize;
    printf("LR: %d, %d\n", Lsize, Rsize);

    for(long long i=0;i<(1<<Lsize);i++){
        int bitcnt = 0;
        vector<int> cur(4);
        string curname;
        for(int j=0;j<Lsize;j++) if(i&(1<<j)) {
            bitcnt++;
            for(int k=0;k<4;k++) cur[k] += A[j][k];
        }
        if (bitcnt <= M) {
            L[bitcnt][cur] = i;
        }
    }

    for(int i=0;i<=M;i++){
        Lv[i].insert(Lv[i].begin(), L[i].begin(), L[i].end());
        L[i].clear();
    }

    printf("Left generate done.\n");

    for(long long i=0;i<(1<<Rsize);i++){
        int bitcnt = 0;
        vector<int> cur(4);
        string curname;
        for(int j=0;j<Rsize;j++) if(i&(1<<j)) {
            bitcnt++;
            for(int k=0;k<4;k++) cur[k] += A[Lsize+j][k];
        }
        if (bitcnt <= M) {
            R[bitcnt][cur] = i<<Lsize;
        }
    }

    for(int i=0;i<=M;i++){
        Rv[i].insert(Rv[i].begin(), R[i].begin(), R[i].end());
        R[i].clear();
    }

    printf("Right generate done.\n");
}

string best_name;
vector<int> best_stat;
const int INF = 0x3fFFffFF;
int best_hp = -INF;

void MITM() {
    for(int i=0;i<=M;i++) {
        printf("Processing... %d of %d\n", i, M);
        printf("Investigating %lld pairs...\n", 1LL*Lv[i].size()*Rv[M-i].size());

        // pick i from L, M-i from R
        for(const auto& lv:Lv[i]) for(const auto& rv:Rv[M-i]) {
            int att = lv.first[0]+rv.first[0];
            int def = lv.first[1]+rv.first[1];
            int spd = lv.first[2]+rv.first[2];
            int luk = lv.first[3]+rv.first[3];
            bool fast = ally_default.spd + spd >= enemy_default.spd;

            if (fast && cache_check(cache_up[att][def], luk)) continue;
            if (!fast && cache_check(cache_down[att][def], luk)) continue;

            Airship ally = ally_default, enemy = enemy_default;
            ally.att += att;
            ally.def += def;
            ally.spd += spd;
            ally.luk += luk;

            auto result = battle(move(ally), move(enemy));
            if (result > best_hp) {
                best_hp = result;
                best_stat = vector<int>(4);
                for(int j=0;j<4;j++) best_stat[j] += lv.first[j]+rv.first[j];
                best_name = lv.second | rv.second;
            }

            if (fast) cache_up[att][def][luk] = result;
            else cache_down[att][def][luk] = result;
        }
    }
}

int main(){
    parse();
    printf("Parse done.\n");
    printf("Ally\n"); ally_default.print();
    printf("Enemy\n"); enemy_default.print();

    half_gen();
    printf("Data generated.\n");

    MITM();

    cout << best_name << endl;
    cout << "Stat: ";
    for(int i=0;i<4;i++) cout << best_stat[i] << ' ';
    cout << endl;
    cout << "Expected remaning hp: " << best_hp << endl;
}
