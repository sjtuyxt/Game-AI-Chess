#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include "network.h"
#include "player.h"

using namespace std;
using network::IO;

#define DEBUG_SINGAL 0
#define DEBUG(x) if(DEBUG_SINGAL)cout<<#x<<" : "<<x<<endl;

vector<pair<int, pair<pair<int ,int>, pair<int, int > > > > step;
int my_id;

Player *player;

void makeDecision(string& decision){
	pair<pair<int, int>, char> res=player->makeDecision();
	decision='0'+res.first.first;
	decision+=' ';
	decision+='0'+res.first.second;
	decision+=' ';
	decision+=res.second;
	decision+='\n';
}
extern ChessPiece board[9][7]; 
extern bool trap[3][9][7];

int main(int argc,char **argv)
{	
	board[8][6].set(2,6);
	board[8][0].set(2,7);
	board[6][6].set(2,8);
	board[7][5].set(2,2);
	board[6][4].set(2,3);
	board[6][2].set(2,5);
	board[7][1].set(2,4);
	board[6][0].set(2,1);
	board[0][0].set(1,6);
	board[0][6].set(1,7);
	board[2][0].set(1,8);
	board[1][1].set(1,2);
	board[2][2].set(1,3);
	board[2][4].set(1,5);
	board[1][5].set(1,4);
	board[2][6].set(1,1);
	trap[1][8][2]=1;
	trap[1][8][4]=1;
	trap[1][7][3]=1;
	trap[2][0][2]=1;
	trap[2][0][4]=1;
	trap[2][1][3]=1;
	srand(time(NULL)+800);
	if(argc!=3 && argc!=2){
		cerr<<"<usage>: client ip [port]"<<endl;
		return 1;
	}
	string port = argc == 3 ? argv[2] : "12345";
	IO client(argv[1],port.c_str());
	string message;
	int x_space,y_space,id,new_x,new_y;
	client.receive(message);
	my_id=message[0]-'0';
	player=new Player(my_id,&step);
	client.send(string(player->name()+'\n'));
	while(true){
		client.receive(message);
		DEBUG(message);
		if(message=="game end"){
			break;
		}else{
			if(message=="action"){
				string decision="";
				makeDecision(decision);
				DEBUG(decision);
				if(decision==""){
					client.send("None\n");
				}else{
					client.send(decision);
				}
			}else{
				if(message=="None")
					continue;
				id=message[0]-'0';
				x_space=message[2]-'0';
				y_space=message[4]-'0';
				new_x=message[6]-'0';
				new_y=message[8]-'0';
				step.push_back(make_pair(id,make_pair(make_pair(x_space,y_space),make_pair(new_x,new_y))));
				player->addOperators(id,x_space,y_space,new_x,new_y);
			}
		}
	}
	delete player;
}
