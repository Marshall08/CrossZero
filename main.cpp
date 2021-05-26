#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>
using namespace std;

enum TCell {
    EMPTY='_',
    CROSS='X',
    ZERO='0'
};

enum TProgress {
    IN_PROGRESS,
    WON_HUMAN,
    WON_AI,
    DRAW
};

struct TField{
    const size_t SIZE=3;
    TCell** ppFiled=nullptr;
    size_t nTurn=0;
    TCell human;
    TCell ai;
    TProgress progress=IN_PROGRESS;
};

struct TCoord {
    size_t y=0;
    size_t x=0;
};

void clearScr() {
    system("cls");
}

int32_t getRandomNum(int32_t min, int32_t max) {
    const static auto seed=chrono::system_clock::now().time_since_epoch().count();
    static mt19937_64 generator(seed);
    uniform_int_distribution<int32_t> dis(min,max);
    return dis(generator);
}

void initField(TField & rF){
    rF.ppFiled=new TCell* [rF.SIZE];
    for (size_t y=0; y<rF.SIZE; y++){
        rF.ppFiled[y]=new TCell [rF.SIZE];
    }
    for (size_t y=0; y<rF.SIZE; y++){
        for (size_t x=0; x<rF.SIZE; x++){
            rF.ppFiled[y][x]=EMPTY;
        }
    }
    if (getRandomNum(0,1000)>500) {
        rF.nTurn=0;
        rF.human=CROSS;
        rF.ai=ZERO;
    } else {
        rF.nTurn=1;
        rF.human=ZERO;
        rF.ai=CROSS;
    }
}

void deinitField(TField & rF) {
    for (size_t y=0; y<rF.SIZE; y++){
        delete[] rF.ppFiled[y];
    }
    delete[] rF.ppFiled;
    rF.ppFiled=nullptr;
}

void drawFiled(const TField & rF) {
    cout<<"   ";
    for (size_t x=0; x<rF.SIZE; x++){
        cout<<x+1<<"  ";
    }
    cout<<endl;
    for (size_t y=0; y<rF.SIZE; y++){
        cout<<y+1<<" | ";
        for (size_t x=0; x<rF.SIZE; x++){
            cout<<(char)rF.ppFiled[y][x]<<" | ";
        }
        cout<<endl;
    }
    cout<<endl<<"Human: "<<(char)rF.human<<endl;
    cout<<"AI: "<<(char)rF.ai<<endl<<endl;
}

TProgress isWon(const TField & rF){
    for (size_t y=0; y<rF.SIZE; y++){
        if (rF.ppFiled[y][0]==rF.ppFiled[y][1] && rF.ppFiled[y][0]==rF.ppFiled[y][2]){
            if(rF.ppFiled[y][0]==rF.human){
                return WON_HUMAN;
            }
            if(rF.ppFiled[y][0]==rF.ai){
                return WON_AI;
            }
        }
    }

    for (size_t x=0; x<rF.SIZE; x++){
        if (rF.ppFiled[0][x]==rF.ppFiled[1][x] && rF.ppFiled[0][x]==rF.ppFiled[2][x]){
            if(rF.ppFiled[0][x]==rF.human){
                return WON_HUMAN;
            }
            if(rF.ppFiled[0][x]==rF.ai){
                return WON_AI;
            }
        }
    }

    if (rF.ppFiled[0][0]==rF.ppFiled[1][1] && rF.ppFiled[0][0]==rF.ppFiled[2][2]){
        if(rF.ppFiled[0][0]==rF.human){
            return WON_HUMAN;
        }
        if(rF.ppFiled[0][0]==rF.ai){
            return WON_AI;
        }
    }

    if (rF.ppFiled[2][0]==rF.ppFiled[1][1] && rF.ppFiled[2][0]==rF.ppFiled[0][2]){
        if(rF.ppFiled[1][1]==rF.human){
            return WON_HUMAN;
        }
        if(rF.ppFiled[1][1]==rF.ai){
            return WON_AI;
        }
    }

    bool draw=true;
    for (size_t y=0; y<rF.SIZE; y++){
        for (size_t x=0; x<rF.SIZE; x++){
            if (rF.ppFiled[y][x]==EMPTY){
                draw=false;
                break;
            }
        }
        if (!draw)
            break;
    }

    if (draw){
        return DRAW;
    }

    return IN_PROGRESS;
}

TCoord getHumanCoord(const TField & rF){
    TCoord c;
    do {
        cout<<"Enter x coord (1..3): ";
        cin>>c.x;
        cout<<"Enter y coord (1..3): ";
        cin>>c.y;
    } while (c.x>3 || c.y>3 || c.x<1 || c.y<1 || rF.ppFiled[c.y-1][c.x-1]!=EMPTY);

    c.x--;
    c.y--;
    return c;
}

TCoord getAICoord(TField & rF){
    TCoord c;

    for (size_t y=0; y<rF.SIZE; y++){
        for (size_t x=0; x<rF.SIZE; x++){
            if (rF.ppFiled[y][x]==EMPTY){
                rF.ppFiled[y][x]==rF.ai;
                if (isWon(rF)==WON_AI){
                    rF.ppFiled[y][x]=EMPTY;
                    return{y,x};
                }
                rF.ppFiled[y][x]=EMPTY;
            }
        }
    }

    for (size_t y=0; y<rF.SIZE; y++){
        for (size_t x=0; x<rF.SIZE; x++){
            if (rF.ppFiled[y][x]==EMPTY){
                rF.ppFiled[y][x]==rF.human;
                if (isWon(rF)==WON_HUMAN){
                    rF.ppFiled[y][x]=EMPTY;
                    return{y,x};
                }
                rF.ppFiled[y][x]=EMPTY;
            }
        }
    }

    if (rF.ppFiled[1][1]==EMPTY){
        return{1,1};
    }
    if (rF.ppFiled[0][0]==EMPTY){
        return{0,0};
    }
    if (rF.ppFiled[2][2]==EMPTY){
        return{2,2};
    }
    if (rF.ppFiled[0][2]==EMPTY){
        return{0,2};
    }
    if (rF.ppFiled[2][0]==EMPTY){
        return{2,0};
    }
    if (rF.ppFiled[0][1]==EMPTY){
        return{0,1};
    }
    if (rF.ppFiled[2][1]==EMPTY){
        return{2,1};
    }
    if (rF.ppFiled[1][0]==EMPTY){
        return{1,0};
    }
    if (rF.ppFiled[1][2]==EMPTY){
        return{1,2};
    }

    return c;
}

int main(){
    TField f;
    initField(f);
    clearScr();
    drawFiled(f);

    do {
        if (f.nTurn % 2==0){
            TCoord c=getHumanCoord(f);
            f.ppFiled[c.y][c.x]=f.human;
        } else {
            TCoord c=getAICoord(f);
            f.ppFiled[c.y][c.x]=f.ai;
        }

        clearScr();
        drawFiled(f);
        f.nTurn++;
        f.progress=isWon(f);
    } while (f.progress==IN_PROGRESS);

    if (f.progress==IN_PROGRESS){
        cout<<"Game in progress"<<endl;
    }
    else if (f.progress==WON_HUMAN){
        cout<<"Human won! Congrats!"<<endl;
    }
    else if (f.progress==WON_AI){
        cout<<"AI won! Congrats!"<<endl;
    }
    else if (f.progress==DRAW){
        cout<<"Draw!"<<endl;
    }

    deinitField(f);
    return 0;
}