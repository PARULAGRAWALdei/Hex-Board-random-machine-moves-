//Program: Implementation of  Hex board
//Author: Parul Agrawal
//Date: 9/8/19
//
//Important:
//	1.For storing Hex Board adjcency matrix is used.
//	2.Execution of program needs c++11 support.It is also mandetory for course lectures.
//	3.Read about hex game prior executing program.
//	5.Some changes are made according to implementation.
//  6.Excessive commenting is intentional.
//  7.Do not use comma(,) in between of coordinates,otherwise program will go in for infinite loop
//  8.There is not Artificial intelligent used in this assignment.Computer moves are completely random



#include<iostream>
#include<vector>
#include<string>
#include<ctime>
#include<utility>
#include<queue>

using namespace std;

enum class Player {BLUE,RED};                                                                            //for storing colors of two players

class hexBoard                                                                                           //contains all members and funtions related to hex board
{
	public:
		hexBoard(int);                                                                                   //constructor declaration
		void	printBoard();                                                                            //For printing hex board
        bool 	isValidMove(int,                                                                         //Check if any move is valid move or not
								int);
		void 	move(int,                                                                                //given coordintes, occupies the place in board for player
						int,
							Player);
 		bool 	win(int,                                                                                 //check if there is any winner
		 				int);
		inline  bool validPlace(int,                                                                     //check if position lies inside board
									int);
		void 	checkBorders(int x,                                                                      //finding a border move
								int y, 
									std::vector<bool> &flags, 
															char side);
	private:
		vector<vector<char> >	board;
		int 					n;
		const static char 		Blue='B';
		const static char 		Red='R';
		const int 				directions[6][2]={{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0}};               //stores 6 possible neighbour of a node
};

hexBoard::hexBoard(int n):n(n)																			  
{
	board.resize(n);                                                                                      //making board size as specified by user
	for(int i = 0; i < n; ++ i)
	{
		board[i].resize(n);                                                                      
	}
	for(int i=0; i < n; ++i)
	{
		for(int j=0; j < n; ++j)
		{
			board[i][j]='.';                                                                              //initially assingning whole board as blank(.)
		} 
	}
} 


void hexBoard::printBoard()
{
	for(int i=0; i < n; ++i) 
    {
		for(int j=0;j<i;j++)
		{	
			cout<<" ";
		}
		for(int j=0; j < n; ++j) 
        {
            cout<<board[i][j]<<" ";
        }       
		cout<<endl;
	}
}


//if position at x and y coordinates is blank then function return true else false
bool hexBoard::isValidMove(int x,
								int y)
{
	if(board[x][y]=='.')																					
	{
		return true;
	}
	else
	{
		return false;
	}
}

//fill the character corresponding to player in specified coordinates
void hexBoard::move(int x,
						int y,
							Player c)
{
	if(c==Player::BLUE)																 //checking for player and places character accordingly
	{
		board[x][y]=Blue;                                                           
	}
	else
	{
		board[x][y]=Red;
	}	
}


bool hexBoard::validPlace(int x,
								int y)
{
	return x>=0 && x<n && y>=0 && y<n;                                                //checks whether coordinated lies with in the board or not
}
void hexBoard::checkBorders(int x, 
								int y, 
									std::vector<bool> &flags,
															char side) 
{
	if (side == Red)                                                                  //if present player is red it will check for left to right boundry.Hence it is checking for y coordinate
	{
		if (y == 0)																	  //because y will be either 0 or n at left to right boundries
			flags[0] = true;
		if (y == n - 1)
			flags[1] = true;
	}
	else if (side == Blue)															//if present player is blue it will check for top to bottom boundry.Hence it is checking for x coordinate
	{																				//because x will be either 0 or n at top to bottom boundries
		if (x == 0)
			flags[0] = true;
		if (x == n - 1)
			flags[1] = true;
	}
}


//for winner finding we have used BFS i.e using a queue and travesing its child
bool hexBoard::win(int x,
						int y)
{
	if (validPlace(x, y) == false || board[x][y] == '.')                             //if position is not valid then function returns
	{
		return false;	
	}
	
	
	char 					side = board[x][y];                                      //finds the color of present move
	vector<bool> 			flags(2, false);                                         //if any boundry is  touched by its specified color then flag gets checked
	vector<vector<bool> > 	visited(n, vector<bool>(n));                             //keeping track og visited nodes
	queue<pair<int, int> > 	traces;                                                  //stores the path
	
	traces.push(make_pair(x, y));
	visited[x][y] = true;

	while (!traces.empty())
	{
		auto top = traces.front();                                                   //a queue is poped out 
		checkBorders(top.first, top.second, flags, side);                            //if it is edge move then it marks flags 1
		traces.pop(); 
		for (int n=0; n<6; n++)														 //visiting every of neighbour and if they are valid neighbours then pushing them to queue
		{ 
			int X = top.first + directions[n][0];
			int Y = top.second + directions[n][1];
			if (validPlace(X, Y) && 
								board[X][Y] == side && 
									visited[X][Y] == false)
			{
				visited[X][Y] = true;
				traces.push(make_pair(X, Y));
			}
		}
	}
	return flags[0] && flags[1];	
}


//Execution of program starts here
int main()
{
	srand(time(0));                        																		//seed for random no. generator  
	cout<<"Enter the dimension for board."<<endl;
	int n;
	cin>>n;
	cout<<"For choosing Blue color press 0, for Red press 1"<<endl;
	int color;
	cin>>color;
	Player user,comp;
	if(color==0)                                                                                                //specifying color according to color chosen by user
	{
		cout<<"Your boundry is Top to bottom"<<endl;
		user=Player::BLUE;
	 	comp=Player::RED;
	}
	else
	{
		cout<<"Your boundry is Left to right"<<endl;
		user=Player::RED;
		comp=Player::BLUE;
	}
	
	hexBoard h(n);                                                                                              //creating hex board
	h.printBoard();
	int turn=0;																									//storing turn of user and computer
	int steps=0;																								//stores total steps used befor winning game.It counts user and computer steps individually
	int x,y;
	while(1)  				
	{
		steps++;
		turn=!turn;                                                                                             //reversing the turn
		if(turn)
		{
			do                                                                                                  //executes untill a valid move comes
			{
				cout<<endl<<"Enter the coordinates of your move seprated by space."<<endl; 
				cin>>x>>y;																						//taking user input for move
			}while(!h.isValidMove(x,y));
			h.move(x,y,user);
			cout<<endl;
			h.printBoard();
				
		}
		else
		{
			cout<<endl<<"Computer move:"<<endl;
			do 
			{
				x=rand()%n;																						//random guess for x coordinate
				y=rand()%n;                                                                                     //random guess for y coordinate
			}while(!h.isValidMove(x,y));																		//executes untill a valid move comes
			cout<<x<<" "<<y<<endl;
			h.move(x,y,comp);                                                                          
			cout<<endl;
			h.printBoard();
        }
        if (h.win(x, y))																						//check if anybody wins
		{
			cout<<"Congratulations!! ";
			cout << (turn ? "user" : "computer") << " win " << endl<<endl;;                                     //prints winner
			cout << "Total steps = " << steps << endl<<endl;                                                    //prints no. of steps used in winning
			h.printBoard();
			break;
		}
		
	}	
	return 0;
}
