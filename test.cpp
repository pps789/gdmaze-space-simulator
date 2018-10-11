#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include "airship.h"
#include "battle.h"
using namespace std;

string name[60];
int A[60][4];
int E[4];
map<vector<int>, string> L[21], R[21];
int N, M;

Airship ally_default, enemy_default;
int additional, reflect, critical, reductional;

void parse() {
    ifstream airship_ifs("airship.txt"), config_ifs("config.txt");

    airship_ifs >> N;
    for(int i=0;i<N;i++) {
        airship_ifs >> name[i];
        for(int j=0;j<4;j++) airship_ifs >> A[i][j];
    }

    // TODO: fix this!
    string trash;
    config_ifs >> trash >> additional;
    config_ifs >> trash >> reflect;
    config_ifs >> trash >> critical;
    config_ifs >> trash >> reductional;
    config_ifs >> trash >> M;
    
    config_ifs >> trash >> ally_default.max_hp;
    config_ifs >> trash >> ally_default.att >> ally_default.def
        >> ally_default.spd >> ally_default.luk;

    config_ifs >> trash >> enemy_default.max_hp;
    config_ifs >> trash >> enemy_default.att >> enemy_default.def
        >> enemy_default.spd >> enemy_default.luk;
}

int Lsize, Rsize;
void half_gen() {
    Lsize = N/2, Rsize = N-Lsize;
    printf("LR: %d, %d\n", Lsize, Rsize);

    for(int i=0;i<(1<<Lsize);i++){
        int bitcnt = 0;
        vector<int> cur(4);
        string curname;
        for(int j=0;j<Lsize;j++) if(i&(1<<j)) {
            bitcnt++;
            for(int k=0;k<4;k++) cur[k] += A[j][k];
            curname += name[j];
        }
        if (bitcnt <= M) {
            L[bitcnt][cur] = curname;
        }
    }

    printf("Left generate done.\n");

    for(int i=0;i<(1<<Rsize);i++){
        int bitcnt = 0;
        vector<int> cur(4);
        string curname;
        for(int j=0;j<Rsize;j++) if(i&(1<<j)) {
            bitcnt++;
            for(int k=0;k<4;k++) cur[k] += A[Lsize+j][k];
            curname += name[Lsize+j];
        }
        if (bitcnt <= M) {
            R[bitcnt][cur] = curname;
        }
    }

    printf("Right generate done.\n");
}

string best_name;
vector<int> best_stat;
int best_hp = -0x3fFFffFF;

void MITM() {
    for(int i=0;i<=M;i++) {
        printf("Processing... %d of %d\n", i, M);

        // pick i from L, M-i from R
        for(const auto& lv:L[i]) for(const auto& rv:R[M-i]) {
            Airship ally = ally_default, enemy = enemy_default;
            ally.att += lv.first[0]+rv.first[0];
            ally.def += lv.first[1]+rv.first[1];
            ally.spd += lv.first[2]+rv.first[2];
            ally.luk += lv.first[3]+rv.first[3];

            auto result = battle(move(ally), move(enemy));
            if (result > best_hp) {
                best_hp = result;
                best_stat = vector<int>(4);
                for(int j=0;j<4;j++) best_stat[j] += lv.first[j]+rv.first[j];
                best_name = lv.second + rv.second;
            }
        }
    }
}

int main(){
    parse();
    printf("Parse done.\n");

    half_gen();
    printf("Data generated.\n");

    MITM();

    cout << best_name << endl;
    cout << "Stat: ";
    for(int i=0;i<4;i++) cout << best_stat[i] << ' ';
    cout << endl;
    cout << "Expected remaning hp: " << best_hp << endl;
}