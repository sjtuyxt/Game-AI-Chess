#include "player.h"
#include "tuple"
#include "vector"
#include<ctime>
#include<cstdlib>
#include<algorithm>
#include<iostream>
#define abs(x) (((x)>0)?(x):(-(x)))

string Player::name() const {
	//decide your name
	return "辣子鸡";
}

ChessPiece board[9][7]; 

int random(int x){
	return rand()%x;
}

inline bool InBoard(int x,int y){
	return x>=0 && x<9 && y>=0 && y<7;
}

inline bool InWater(int x,int y){
	return x>=3 && x<=5 && (y>=1 && y<=2 || y>=4 && y<=5);
}

int dx[4]={1,0,0,-1};
int dy[4]={0,1,-1,0};
bool trap[3][9][7];
char dir[4]={'U','R','L','D'};
Point chess[3][10];

inline bool CanEat(int x,int y,int nx,int ny){
	if (!(board[x][y].kind==8 && board[nx][ny].kind==1) && board[nx][ny].kind<=board[x][y].kind || board[nx][ny].kind==8 && board[x][y].kind==1) return 1;
	if (trap[board[nx][ny].owner][nx][ny]) return 1;
	return 0;
}

int check(int x,int y,int d,int owner){
	int nx=x+dx[d];
	int ny=y+dy[d];
	if (!InBoard(nx,ny)) return -1e8;
	if (InWater(nx,ny) && !(board[x][y].kind==1 || board[x][y].kind==6 || board[x][y].kind==7)) return -1e8;
	else if (InWater(nx,ny) && board[x][y].kind!=1){
		while (InWater(nx,ny)){
			if (board[nx][ny].owner!=0) return -1e8;
			nx+=dx[d];
			ny+=dy[d];
		}
	}
	else if (InWater(nx,ny)^InWater(x,y) && board[x][y].kind==1){
		if (board[nx][ny].owner!=0) return -1e8;
	}
	if (nx==(board[x][y].owner-1)*8 && ny==3) return -1e8;
	if (board[x][y].owner==board[nx][ny].owner) return -1e8;
	int Max=-1e8;
	if (nx==(2-board[x][y].owner)*8 && ny==3) return 2e8;
	if (trap[board[x][y].owner][nx][ny]){
		int cnt1=0,cnt2=0;
		for (int k=0;k<=3;++k){
			if (InBoard(nx+dx[k],ny+dy[k]) && board[nx+dx[k]][ny+dy[k]].owner==3-board[x][y].owner)
				cnt2++;
			else if (InBoard(nx+dx[k],ny+dy[k]) && board[nx+dx[k]][ny+dy[k]].owner==board[x][y].owner)
				cnt1++;
		}	
		if (cnt1==0 && cnt2>0) return -1e8;
	}
	if (owner<=4){
		ChessPiece Temp=board[nx][ny];
		chess[board[nx][ny].owner][board[nx][ny].kind].set(-1,-1);
		chess[board[x][y].owner][board[x][y].kind].set(nx,ny);
		board[nx][ny]=board[x][y];
		board[x][y].set(0,0);
		for (int p=1;p<=8;p++){
			int i=chess[3-board[nx][ny].owner][p].x;
			int j=chess[3-board[nx][ny].owner][p].y;
			if (i==-1) continue;
			for (int k=0;k<4;++k){
				int temp=check(i,j,k,owner+1);
				if (temp>Max)
					Max=temp;
			}
		}		
		board[x][y]=board[nx][ny];
		board[nx][ny]=Temp;		
		chess[board[x][y].owner][board[x][y].kind].set(x,y);
		chess[board[nx][ny].owner][board[nx][ny].kind].set(nx,ny);
	}
	else Max=0;
	if (!CanEat(x,y,nx,ny)) return -1e8;
	int a[10];
	for (int i=0;i<9;++i)
		a[i]=0;
	int temp=0;
	int cnt=0;
	int pp=50;
	if (board[x][y].owner==2) pp=70;
	for (int p=1;p<=8;p++){
		int i=chess[board[x][y].owner][p].x;
		int j=chess[board[x][y].owner][p].y;
		if (i==-1) continue;
		if (abs(i-(board[x][y].owner-1)*8+abs(j-3)<=2 && !(board[i][j].kind==6 || board[i][j].kind==7 || board[i][j].kind==8))) 
			cnt+=((board[i][j].kind==8)?3:1);
		if (board[i][j].kind==6 || board[i][j].kind==7) 
			temp+=(pp+30)/(abs(i-(2-board[x][y].owner)*8)+abs(j-3));
		else if (board[i][j].kind!=8) 
			a[++a[0]]=pp-pp/(abs(i-(2-board[x][y].owner)*8)+abs(j-3));
	}	
	std::sort(a+1,a+1+a[0]);
	temp+=pp-a[1]+cnt*7;	
	if (chess[3-board[x][y].owner][6].x!=-1) temp+=23/(abs(chess[board[x][y].owner][8].x-chess[3-board[x][y].owner][6].x)+abs(chess[board[x][y].owner][8].y-chess[3-board[x][y].owner][6].y));
	if (chess[3-board[x][y].owner][8].x!=-1) temp+=20/(abs(chess[board[x][y].owner][1].x-chess[3-board[x][y].owner][8].x)+abs(chess[board[x][y].owner][1].y-chess[3-board[x][y].owner][8].y));
	if (chess[3-board[x][y].owner][7].x!=-1) temp+=23/(abs(chess[board[x][y].owner][8].x-chess[3-board[x][y].owner][7].x)+abs(chess[board[x][y].owner][8].y-chess[3-board[x][y].owner][7].y));
	if (chess[board[x][y].owner][6].x!=-1 && chess[board[x][y].owner][7].x!=-1) temp+=80/(2+abs(chess[board[x][y].owner][7].y-chess[board[x][y].owner][6].y));
	int tx=1,ty=3;
	if (chess[chess[board[x][y].owner][8].x!=-1] && (chess[3-board[x][y].owner][7].x==6 && (chess[3-board[x][y].owner][7].y==1 || chess[3-board[x][y].owner][7].y==2))){
		tx=2;
		ty=1;
	}
	if (chess[board[x][y].owner][5].x!=-1 && chess[3-board[x][y].owner][1].x!=-1) temp+=20/(1+abs(chess[board[x][y].owner][5].x-chess[3-board[x][y].owner][1].x)+abs(chess[board[x][y].owner][5].y-chess[3-board[x][y].owner][1].y));
	if (chess[board[x][y].owner][5].x!=-1 && chess[board[x][y].owner][8].x!=-1) temp+=20/(1+abs(chess[board[x][y].owner][5].x-chess[board[x][y].owner][8].x)+abs(chess[board[x][y].owner][5].y-chess[board[x][y].owner][1].y));
	if (chess[board[x][y].owner][6].x>=4 && chess[board[x][y].owner][7].x>=4 && chess[board[x][y].owner][8].x!=-1 && ((chess[3-board[x][y].owner][6].x!=-1) || (chess[3-board[x][y].owner][7].x!=-1))) temp+=80/(2+abs(chess[board[x][y].owner][8].x-tx)+abs(chess[board[x][y].owner][8].y-ty));
	if (InWater(chess[board[x][y].owner][1].x,chess[board[x][y].owner][1].y)) temp+=10;
	if (board[nx][ny].owner==0) return temp-Max;
	int kk=20;
	if (board[x][y].owner==2) {
		kk=60;
		if (board[nx][ny].kind==1) kk+=30;
	}	
	if (CanEat(x,y,nx,ny)) return temp-Max+board[nx][ny].kind*5+kk;
	return -1e8;
}

int bushu;
int slt[40][4];
int ns,tail,R1=10,R2=10;

pair<pair<int, int>, char> Player::makeDecision() {
	for (int i=tail;i<(*operators).size();++i){
		pair<int,int> S=(*operators)[i].second.first,T=(*operators)[i].second.second;
		int X1=S.first,Y1=S.second,X2=T.first,Y2=T.second;
		board[X2][Y2]=board[X1][Y1];
		board[X1][Y1].set(0,0);
	}
	tail=(*operators).size();
	int Max=-1e8+1;
	ns=0;
	for (int i=1;i<=8;i++){
		chess[1][i].set(-1,-1);
		chess[2][i].set(-1,-1);
	}
	for (int i=0;i<9;++i)
		for (int j=0;j<7;++j)
			chess[board[i][j].owner][board[i][j].kind].set(i,j);
	if (bushu==0) {
		bushu=1;
		return make_pair(make_pair(2,0),'R');
	}
	else if (bushu==1){
		bushu=2;
		return make_pair(make_pair(1,1),'D');
	}
	else if (bushu==2){
		bushu=3;
		return make_pair(make_pair(2,6),'U');
	}
	 else if (bushu==3 && !(chess[2][1].x==2&&chess[2][1].y==0)){
		 bushu=4;
		 return make_pair(make_pair(3,6),'U');
	}
	if (chess[1][1].x!=-1 && !InWater(chess[1][1].x,chess[1][1].y)){
		for (int k=0;k<=3;k++){
			int nx=chess[1][1].x+dx[k];
			int ny=chess[1][1].y+dy[k];
			if (InBoard(nx,ny) && board[nx][ny].owner==2 && board[nx][ny].kind==8)
				return make_pair(make_pair(chess[1][1].x,chess[1][1].y),dir[k]);
		}
	}
	for (int p=8;p>=1;p--){
		int i=chess[1][p].x;
		int j=chess[1][p].y;
		for (int k=0;k<4;++k){
			if (i+dx[k]==8 && j+dy[k]==3)
			return make_pair(make_pair(i,j),dir[k]);
			int temp=check(i,j,k,1);
			if (temp>Max){
				ns=1;
				slt[ns][1]=i;
				slt[ns][2]=j;
				slt[ns][3]=k;
				Max=temp;
			}
			else if (temp==Max){
				++ns;
				slt[ns][1]=i;
				slt[ns][2]=j;
				slt[ns][3]=k;
			}
		}
	}
	int k=random(ns)+1;
	int x=slt[k][1];
	int y=slt[k][2];
	int d=slt[k][3];
	
	if (chess[1][7].x>=7 && random(R1)==0 && check(chess[1][7].x,chess[1][7].y,3,1)>Max-100){
		R1=2;
		return make_pair(make_pair(chess[1][7].x,chess[1][7].y),dir[3]);
	}
	else R1=10;
	if (chess[1][8].x>=7 && random(R2)==0 && check(chess[1][8].x,chess[1][8].y,3,1)>Max-100){
		R2=2;
		return make_pair(make_pair(chess[1][8].x,chess[1][8].y),dir[3]);
	}		
	else R2=10;
	return make_pair(make_pair(x,y),dir[d]);
}

void Player::addOperators(int e1,int e2,int e3,int e4,int e5){
	operators_tuple.push_back(make_tuple(e1,e2,e3,e4,e5));
}