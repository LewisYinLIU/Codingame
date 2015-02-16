#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
enum DIR { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3};
struct player {
  int x;
  int y;
  int dir;
  int id;
  int distance, next;
  int des, wallsLeft, isDead;
};
struct wall{
    int x,y,dist,myDist, isValid;
    string o;
};
vector<player> players;
int c[86][86];
int dist[86];
int previ[86];  
int n, line;
int wallMap[9][9];
int defed;

void Dijkstra(int n, int v){
	bool s[85];
	for(int i=1; i<=n; ++i)
	{
		dist[i] = c[v][i];
		s[i] = 0;     
		if(dist[i] == 999)
			previ[i] = 0;
		else
			previ[i] = v;
	}
	dist[v] = 0;
	s[v] = 1;
 
	for(int i=2; i<=n; ++i){
		int tmp = 85;
		int u = v;
		for(int j=1; j<=n; ++j)
			if((!s[j]) && dist[j]<tmp)
			{
				u = j;            
				tmp = dist[j];
			}
		s[u] = 1;  
 
		for(int j=1; j<=n; ++j){
			if((!s[j]) && c[u][j]<85)
			{
				int newdist = dist[u] + c[u][j];
				if(newdist < dist[j])
				{
					dist[j] = newdist;
					previ[j] = u;
				}
			}
		}
	}
}

void buildMap(){
    n = 85;
//WallMap
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            wallMap[i][j] = 0;        
        }
    }
    for(int i=0; i<=8;i++){
        wallMap[8][i] = 2;
        wallMap[i][8] = 1;
        wallMap[0][i] = 1;
        wallMap[i][0] = 2;
    }
    wallMap[8][8] = 3;
    wallMap[0][0] = 3;
//C map        
	for(int i=0; i<=n; ++i)
		for(int j=0; j<=n; ++j)
			c[i][j] = 999;
	
	//middle
	for(int i=11; i<72; i++){ 
        if(i%9 != 1 && i%9 != 0){
            c[i][i+1] = 1;
            c[i][i-1] = 1;
            c[i][i+9] = 1;
            c[i][i-9] = 1;
        }
	}
	//up bar
	for(int i=2;i<=8;i++){
	    c[i][i-1] = 1;
	    c[i][i+1] = 1;
	    c[82][i] = 1;
	    c[i][i+9] = 1;
	}
	//right bar
	for(int i=18;i<=72;i=i+9){
	    c[i][i-1] = 1;
	    c[83][i] = 1;
	    c[i][i-9] = 1;
	    c[i][i+9] = 1;
	}
	//down bar
	for(int i=74;i<=80;i++){
	    c[i][i-1] = 1;
	    c[i][i+1] = 1;
	    c[i][i-9] = 1;
	    c[84][i] = 1;
	}
	//left bar
	for(int i=10;i<=64;i=i+9){
	    c[85][i] = 1;
	    c[i][i+1] = 1;
	    c[i][i-9] = 1;
	    c[i][i+9] = 1;
	}
	//four pts
	c[82][1]=1;c[1][2]=1;c[85][1]=1;c[1][10]=1;
	c[82][9]=1;c[83][9]=1;c[9][18]=1;c[9][8]=1;
	c[81][72]=1;c[83][81]=1;c[84][81]=1;c[81][80]=1;
	c[73][64]=1;c[73][74]=1;c[84][73]=1;c[85][73]=1;
	
	for(int i=1; i<=n; ++i){
	    dist[i] = 999;
	}
	//Dijkstra(n, 1, dist, prev, c);
}

void updateMap(int x, int y, string o){
    //0->ok 1->noV 2->noH 3->no
    if(o == "V"){
        c[y*9+x+1][y*9+x] = 999;c[y*9+x][y*9+x+1] = 999;
        c[y*9+x+10][y*9+x+9] = 999;c[y*9+x+9][y*9+x+10] = 999;
        if(y-1>=0)
            wallMap[x][y-1] =  wallMap[x][y-1] == 2||wallMap[x][y-1] == 3? 3 : 1;
        wallMap[x][y] =  wallMap[x][y] == 2||wallMap[x][y] == 3? 3 : 1;
        wallMap[x][y+1] =  wallMap[x][y+1] == 2||wallMap[x][y+1] == 3? 3 : 1;
        wallMap[x-1][y+1] =  wallMap[x-1][y+1] == 1||wallMap[x-1][y+1] == 3? 3 : 2;

    }
    if(o == "H"){
        c[y*9+x+1][y*9+x-8] = 999;c[y*9+x-8][y*9+x+1] = 999;
        c[y*9+x+2][y*9+x-7] = 999;c[y*9+x-7][y*9+x+2] = 999;
        if(x-1>=0)
             wallMap[x-1][y] =  wallMap[x-1][y] == 1||wallMap[x-1][y] == 3? 3 : 2;
        wallMap[x][y] =  wallMap[x][y] == 1||wallMap[x][y] == 3? 3 : 2;
        wallMap[x+1][y] =  wallMap[x+1][y] == 1||wallMap[x+1][y] == 3? 3 : 2;
        wallMap[x+1][y-1] =  wallMap[x+1][y-1] == 2||wallMap[x+1][y-1] == 3? 3 : 1;
    }
}

void analysePlayer(int myID){
    cerr << "analysePlayer..." << endl;
    if (players[myID].x == -1){
        players[myID].isDead = 1;
        cerr << "P"<<  myID << " RIP"<< endl;
        return;
    }
    Dijkstra(85, players[myID].des);
    int me = players[myID].y*9+players[myID].x+1;
    int next = previ[me];
    players[myID].distance = dist[me];
    players[myID].next = next;
    players[myID].id = me;
}

void move(int myId){
    switch (players[myId].next - players[myId].id){
        case 1: {
            cout << "RIGHT" << endl;
            break;
        }
        case -1: {
            cout << "LEFT" << endl;
            break;
        }
        case 9: {
            cout << "DOWN" << endl;
            break;
        }
        case -9: {
            cout << "UP" << endl;
        }            
    }
}

int analyseDistance(int id, int x, int y, string o){
    int tmp[86][86];
    for(int i=0; i<=85; ++i)
		for(int j=0; j<=85; ++j)
			tmp[i][j] = c[i][j];
    if(o == "V"){
        c[y*9+x+1][y*9+x] = 999;c[y*9+x][y*9+x+1] = 999;
        c[y*9+x+10][y*9+x+9] = 999;c[y*9+x+9][y*9+x+10] = 999;
    }
    if(o == "H"){
        c[y*9+x+1][y*9+x-8] = 999;c[y*9+x-8][y*9+x+1] = 999;
        c[y*9+x+2][y*9+x-7] = 999;c[y*9+x-7][y*9+x+2] = 999;
    }
    Dijkstra(85, players[id].des);
    for(int i=0; i<=85; ++i)
		for(int j=0; j<=85; ++j)
			c[i][j] = tmp[i][j];
    return dist[players[id].id];
}

void putWall(int id, int x, int y, int myId){
    cerr << "slowin' "<< id << endl;
    int dist1 = 999;
    int dist2 = 999;
    switch (players[id].next - players[id].id){
        case 1: {
            //return "RIGHT";
            if(y+1<=8){
                if(wallMap[x+1][y]%2 == 0){
                    dist1 = analyseDistance(id, x+1, y, "V");
                    cerr<<"dist1 "<<dist1<<endl;
                    //cout << x+1 <<" "<< y << " V" << endl;
                    //return;
                }
            }
            if(y-1>=0){
                if(wallMap[x+1][y-1]%2 == 0){
                    dist2 = analyseDistance(id, x+1, y-1, "V"); 
                    cerr<<"dist2 "<<dist2<<endl;
                    //cout << x+1 <<" "<< y-1 << " V" << endl;
                    //return;
                }
            }
            if(dist1>998 && dist2>998){
                move(myId);
                return;
            }
            if(dist1>998 && dist2<998){
                cout << x+1 <<" "<< y-1 << " V" << endl;
                return;
            }
            if(dist1<998 && dist2>998){
                cout << x+1 <<" "<< y << " V" << endl;
                return;
            }
            if(dist1<998 && dist2<998){
                if(dist1>=dist2){
                    if(dist1 == 0){
                        move(myId);
                        return;
                    }
                    cout << x+1 <<" "<< y << " V" << endl;
                    return;
                }else{
                    cout << x+1 <<" "<< y-1 << " V" << endl;
                    return;
                }
            }
        }
        
        case -1: {
            //return "LEFT";
            
            if(y+1<=8){
                if(wallMap[x][y]%2 == 0){
                    dist1 = analyseDistance(id, x, y, "V");
                    cerr<<"dist1 "<<dist1<<endl;
                    //cout << x <<" "<< y << " V" << endl;
                    //return;
                }
            }
            if(y-1>=0){
                if(wallMap[x][y-1]%2 == 0){
                    dist2 = analyseDistance(id, x, y-1, "V");
                    cerr<<"dist2 "<<dist2<<endl;
                    //cout << x <<" "<< y-1 << " V" << endl;
                    //return;
                }
            }
            if(dist1>998 && dist2>998){
                move(myId);
                return;
            }
            if(dist1>998 && dist2<998){
                cout << x <<" "<< y-1 << " V" << endl;
                return;
            }
            if(dist1<998 && dist2>998){
                cout << x <<" "<< y << " V" << endl;
                return;
            }
            if(dist1<998 && dist2<998){
                if(dist1>=dist2){
                    if(dist1 == 0){
                        move(myId);
                        return;
                    }
                    cout << x <<" "<< y << " V" << endl;
                    return;
                }else{
                    cout << x <<" "<< y-1 << " V" << endl;
                    return;
                }
            }
        }
        case 9: {
            //return "DOWN";
            if(x+1<=8){
                if(wallMap[x][y+1]<=1){
                    dist1 = analyseDistance(id, x, y+1, "H");
                    cerr<<"dist1 "<<dist1<<endl;
                    //cout << x <<" "<< y+1 << " H" << endl;
                    //return;
                }
            }
            if(x-1>=0){
                if(wallMap[x-1][y+1]<=1){
                    dist2 = analyseDistance(id, x-1, y+1, "H");
                    cerr<<"dist2 "<<dist2<<endl;
                    //cout << x-1 <<" "<< y+1 << " H" << endl;
                    //return;
                }
            }
            if(dist1>998 && dist2>998){
                move(myId);
                return;
            }
            if(dist1>998 && dist2<998){
                cout << x-1 <<" "<< y+1 << " H" << endl;
                return;
            }
            if(dist1<998 && dist2>998){
                cout << x <<" "<< y+1 << " H" << endl;
                return;
            }
            if(dist1<998 && dist2<998){
                if(dist1>=dist2){
                    if(dist1 == 0){
                        move(myId);
                        return;
                    }
                    cout << x <<" "<< y+1 << " H" << endl;
                    return;
                }else{
                    cout << x-1 <<" "<< y+1 << " H" << endl;
                    return;
                }
            }
        }
        case -9: {
            //return "UP";
            if(x+1<=8){
                if(wallMap[x][y]<=1){
                    dist1 = analyseDistance(id, x, y, "H");
                    cerr<<"dist1 "<<dist1<<endl;
                    //cout << x <<" "<< y << " H" << endl;
                    //return;
                }
            }
            if(x-1>=0){
                if(wallMap[x-1][y]<=1){
                    dist2 = analyseDistance(id, x-1, y, "H");
                    cerr<<"dist2 "<<dist2<<endl;
                    //cout << x-1 <<" "<< y << " H" << endl;
                    //return;
                }
            }
            if(dist1>998 && dist2>998){
                move(myId);
                return;
            }
            if(dist1>998 && dist2<998){
                cout << x-1 <<" "<< y << " H" << endl;
                return;
            }
            if(dist1<998 && dist2>998){
                cout << x <<" "<< y << " H" << endl;
                return;
            }
            if(dist1<998 && dist2<998){
                if(dist1>=dist2){
                    if(dist1 == 0){
                        move(myId);
                        return;
                    }
                    cout << x <<" "<< y << " H" << endl;
                    return;
                }else{
                    cout << x-1 <<" "<< y << " H" << endl;
                    return;
                }
            }
        }
    }
}
void putWall2(int id, int x, int y, int myId){
    cerr<<"slowin' "<<id<<endl;
    int third = 3 - id - myId;
    vector<wall> wallList;
    //4X4 wall list
    for(int i=x-1; i<=x+2; i++){
		for(int j=y-1; j<=y+2; j++){
		    if(i>=0 && i<=8 && j>=0 && j<=8){
		        if(wallMap[i][j]<=1){
    		        struct wall wh;
        	        wh.x = i; wh.y = j; wh.o = "H";wh.isValid = 1; 
        	        wallList.push_back(wh);
		        }
    	        if(wallMap[i][j]%2 == 0){
    	            struct wall wv;
        	        wv.x = i; wv.y = j; wv.o = "V";wv.isValid = 1; 
        	        wallList.push_back(wv);
    	        }
		    }
		}
    }
    cerr<<"ls done "<<wallList.size()<<endl;
    for(int i = 0;i<wallList.size();i++){
        wallList[i].dist = analyseDistance(id, wallList[i].x, wallList[i].y, wallList[i].o);
        wallList[i].myDist = analyseDistance(myId, wallList[i].x, wallList[i].y, wallList[i].o);
        int thirdDist = -1;
        if(players.size() == 3)
            if(players[third].x != -1)
                thirdDist = analyseDistance(third, wallList[i].x, wallList[i].y, wallList[i].o);
        if(wallList[i].dist>998 || wallList[i].myDist>998 || thirdDist> 998)
            wallList[i].isValid = 0;
    }
    cerr<<"Da done"<<endl;
    int maxDist = -50;
    int wallNumber = -1;
    for(int i = 0;i<wallList.size();i++){
        cerr<<wallList[i].x<<wallList[i].y<<wallList[i].o<<endl;
        if(wallList[i].isValid == 1){
            if((wallList[i].dist - wallList[i].myDist)>=maxDist){
                
                if((wallList[i].dist - wallList[i].myDist) == maxDist){
                    if(wallList[i].x == wallList[wallNumber].x){
                        if(abs(wallList[i].y-4)>abs(wallList[wallNumber].y-4)){
                            maxDist = wallList[i].dist - wallList[i].myDist;
                            wallNumber = i;
                        }
                    }
                    if(wallList[i].x == wallList[wallNumber].x){
                        if(abs(wallList[i].x-4)>abs(wallList[wallNumber].x-4)){
                            maxDist = wallList[i].dist - wallList[i].myDist;
                            wallNumber = i;
                        }
                    }
                }else{
                    maxDist = wallList[i].dist - wallList[i].myDist;
                    wallNumber = i;  
                }
            }
        }
    }
    if((players[id].distance-players[myId].distance)<maxDist){
        cout << wallList[wallNumber].x <<" "<< wallList[wallNumber].y << " " << wallList[wallNumber].o << endl;
        return;
    }else{
        move(myId);
        return;
    }
}
void Def(int myId, int x, int y){
    if(defed == 0){
        switch (myId){
            case 0: {
                if(x == 6){
                    if(wallMap[x+1][y]<=1){
                        cout<<x+1<<" "<<y<<" H"<<endl;
                        defed = 1;
                        return;
                    }
                    if(y+1<=8){
                        if(wallMap[x+1][y+1]<=1){
                        cout<<x+1<<" "<<y+1<<" H"<<endl;
                        defed = 1;
                        return;
                        }
                    }
                    if(y-1>0){
                        if(wallMap[x+1][y-1]<=1){
                        cout<<x+1<<" "<<y-1<<" H"<<endl;
                        defed = 1;
                        return;
                        }
                    }
                }    
            }
            case 1: {
                break;
            }
            case 2:{
            }
        }
    }
    
    move(myId);
}
int main()
{
    int w; // width of the board
    int h; // height of the board
    int playerCount; // number of players (2,3, or 4)
    int myId; // id of my player (0 = 1st player, 1 = 2nd player, ...)
    cin >> w >> h >> playerCount >> myId; cin.ignore();
    for(int i = 0; i<playerCount; i++){
        if (i == 0){
            struct player p;
            p.id = i;
            p.dir = DIR::RIGHT;
            p.des = 83;
            p.isDead = 0;
            players.push_back(p);
        }
         if (i == 1){
            struct player p;
            p.id = i;
            p.dir = DIR::LEFT;
            p.des = 85;
            p.isDead = 0;
            players.push_back(p);
        }
         if (i == 2){
            struct player p;
            p.id = i;
            p.dir = DIR::DOWN;
            p.des = 84;
            p.isDead = 0;
            players.push_back(p);
        }
    }
    defed = 0;
    buildMap();
    // game loop
    int count = 0;
    while (1) {
        for (int i = 0; i < playerCount; i++) {
            int x; // x-coordinate of the player
            int y; // y-coordinate of the player
            int wallsLeft; // number of walls available for the player
            cin >> x >> y >> wallsLeft; cin.ignore();
            players[i].x = x; players[i].y = y; players[i].wallsLeft = wallsLeft;
        }
        int wallCount; // number of walls on the board
        cin >> wallCount; cin.ignore();
 
        for (int i = 0; i < wallCount; i++) {
            int wallX; // x-coordinate of the wall
            int wallY; // y-coordinate of the wall
            string wallOrientation; // wall orientation ('H' or 'V')
            cin >> wallX >> wallY >> wallOrientation; cin.ignore();
            updateMap(wallX, wallY, wallOrientation);
        }
        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        int min = 999;
        int badAss;
        for (int i = playerCount-1; i >=0; i--) {
            analysePlayer(i);
            if (players[i].distance <= min && players[i].isDead != 1){
                min = players[i].distance;
                badAss = i;
            }
        }
        if (players[myId].distance <= min || players[myId].wallsLeft == 0 || count <6){
            count++;
            Def(myId, players[myId].x, players[myId].y);
        }else{
            //cerr << "dead..." << endl;
            putWall2(badAss, players[badAss].x, players[badAss].y, myId);
        }
        //cout << out << endl; // action: LEFT, RIGHT, UP, DOWN or "putX putY putOrientation" to place a wall
    }
}