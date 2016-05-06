//*****************************************************************************
// TITLE: Connect-4 Project
//
// NAME:  Hao Liang, Dan Wortmann
// LAB SECTION: 303
// LAB DAY: Monday
//
// CLASS: ECE 210
// DATE:  FALL 2013
//
// DESCRIPTION: This is a ConnectFour application that simulates the game 
// for two players. It is played wirelessly between two BB13 boards. The 
// first player to get 4 symbols in a row on the boards wins!
//*****************************************************************************

/* Initialization Functions */
void 	LEDBARInit(void);
void  DIPSWInit(void);
void 	PBSwInit(void);
void  RGB_LEDInit(void);
void  sysTickInit(void);
int 	readPotentiometer0(void);
int 	readPotentiometer1(void);
int 	readPotentiometer2(void);
int  	read_PBSwitchNum(int SwitchNumber);
int 	read_Switches(void);
void 	LEDBAROutput(int value);
void  turnOn(char color);
void  turnOff(char color);
void 	sysTickWait1mS(int waitTime);
void	RIT128x96x4Init(int freq);
void 	RIT128x96x4Clear(void);
void 	RIT128x96x4StringDraw();
void  Xbee_ConfigureAddresses(int destination, int ownAddr);
void  Xbee_Send(int message);
int		Xbee_Receive(void);
void 	potentiometersInit(void);

//custom functions for the game
int 	checkValidColumn(int column, int* board);
int 	checkWin();
void 	drawBoard(int* a);
int 	addPiece(int column, int* board, int piece);
void 	clearBoard(int* a);
char* convert(int code);
void 	blink(int c, int* board);

//*****************************************************************************
//
// Main Program:
//
//*****************************************************************************
int
main(void)
{
 //Initializing the variables used in the lab.
 int PB1,PB2; //Pushbutton variables
 int column; //column
 int destAddress, ownAddress;
 int main;
 int board[42];
 int lastPosition; //array index of the last spot with piece put into it
 int selectedCol;
 int player;
 int otherPlayer;
 int turn;
 int potenValue0;
 int win;
 int receivedMessage;
 int replay;
 
 //Initializing the LEDBAR, RGB LED, DIPSwitches and Pushbuttons, and a wait timer
 LEDBARInit();
 DIPSWInit();
 PBSwInit();
 RGB_LEDInit();
 sysTickInit();
 potentiometersInit();
 RIT128x96x4Init(1000000);
 board[0] = 0;

 /* Main Part of the Program starts here */
	turnOn('G'); //Status LED lets us know we are ready to go.		

	//First the	destination address should be set.
	PB1 = read_PBSwitchNum(1);
	RIT128x96x4StringDraw("Enter destination", 0,8,15);
	RIT128x96x4StringDraw("address on switches", 0,16,15);
	RIT128x96x4StringDraw("Confirm with PB1", 0,32,15);
	
	while(PB1 != 0x0)
	{
		 PB1 = read_PBSwitchNum(1);
		 //Waits until PB1 has been pressed.
	}
	destAddress = read_Switches();
	RIT128x96x4Clear();

	//LED Sequence lets us know we have recieved the destination properly.
	turnOff('G');
	turnOn('R');
	sysTickWait1mS(150);
	turnOff('R');

	//Next, the source (for this Xbee) address should be set.
	PB2 = read_PBSwitchNum(2);
	RIT128x96x4StringDraw("Enter recieving", 0,8,15);
	RIT128x96x4StringDraw("address on switches", 0,16,15);
	RIT128x96x4StringDraw("Confirm with PB2", 0,32,15);
	
	while(PB2 != 0x0)
	{
		 PB2 = read_PBSwitchNum(2);
		 //Waits until PB2 has been pressed.
	}
	ownAddress = read_Switches();
	RIT128x96x4Clear();

	//LED Sequence lets us know we have recieved the source properly.
	turnOn('B');
	sysTickWait1mS(150);
	turnOff('B');

	//Time to set the Xbee up.
	Xbee_ConfigureAddresses(destAddress, ownAddress);
	
	//determine who goes first, higher address is player 1
	player = 1;
	otherPlayer = 2;
	
	if(ownAddress < destAddress)
	{
		player = 2;
		otherPlayer = 1;
	}

  //LED Sequence lets us know we have configured the Xbee 
	turnOn('G');
	sysTickWait1mS(150);
	turnOff('G');
	sysTickWait1mS(150);
	turnOn('G');
	sysTickWait1mS(150);
	turnOff('G');
	sysTickWait1mS(150);

//////////////////////////////////////////////////////////////////////
////MAIN LOOP/////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

replay = 1;
	
while(replay)
{	
//initialize game
RIT128x96x4Clear();//clear screen
main = 1;	
lastPosition = -1;	// main game loop variable
clearBoard(board); //clear board for new game
drawBoard(board);  //draw empty board
turn = player;			//assign turn
win = 0;

while(main)
{
	//player 1 - START OF GAME ----------------------------------------------
	if(turn == 1)
	{
		PB1 = read_PBSwitchNum(1);
	  //confirm column with Pushbutton or use switches to select new column
		while(PB1 != 0x0)
		{
			//select column with potentiometer
			selectedCol = (readPotentiometer0() / 150); //divides 1024 to roughly 7 parts
			
			//only read button if we have a valid column
			if(checkValidColumn(selectedCol, board) == 1)
			{
				//blink
				blink(selectedCol, board);
				
				//check for button
				PB1 = read_PBSwitchNum(1);
			}
		}	
		
		RIT128x96x4Clear();
		
		//once selected update board by adding piece
		lastPosition = addPiece(selectedCol, board, player);
		
		//print board on screen
		drawBoard(board);
		
		///////////////////////////////////////////////////////////
		//send updated info to player 2 so he gets updated board, 
		//send few times for insurance
		Xbee_Send(selectedCol);
		turnOn('G');
		sysTickWait1mS(150);
		turnOff('G');
		///////////////////////////////////////////////////////////
		
		turn = 2; //switch turns
		
		//CHECK WIN // You WIN!
	win = checkWin(board, lastPosition);
	
	if(win == 1)
	{
		//win
		main = 0;
		sysTickWait1mS(150);
		RIT128x96x4StringDraw("YOU WIN", 39,88,15);
		sysTickWait1mS(150);
		break;
	}
	else if(win == 2)
	{
		//tie
		main = 0;
		sysTickWait1mS(150);
		RIT128x96x4StringDraw("TIE", 51,88,15);
		sysTickWait1mS(150);
		break;
	}
	
	}
	
	//player 2 - WAIT FOR TURN ----------------------------
	if(turn == 2)
	{
		RIT128x96x4StringDraw("waiting for turn...", 16,18,15);
		
		while(1)
		{
			//wait for received signal
			receivedMessage = Xbee_Receive();
		
			//We check that data is valid (0x5A5 is	a data invalid flag), and output
			if(receivedMessage != 0x5A5)
			{
					selectedCol = receivedMessage;	
					break;
			}
	 }
	 
		//update board based on received signal
		RIT128x96x4Clear();
	  lastPosition = addPiece(selectedCol, board, otherPlayer);
		drawBoard(board);
		
		turn = 1; //switch turns
	}
	
	//CHECK WIN // You LOSE!
	if(lastPosition != -1)
		win = checkWin(board, lastPosition);
	
	if(win == 1)
	{
		//win
		sysTickWait1mS(150);
    RIT128x96x4StringDraw("YOU LOSE", 36,88,15);
		sysTickWait1mS(150);
		main = 0;
		break;
	}
	else if(win == 2)
	{
		//tie
		main = 0;
		sysTickWait1mS(150);
		RIT128x96x4StringDraw("TIE", 51,88,15);
		sysTickWait1mS(150);
		break;
	}
	
}

	sysTickWait1mS(250);//wait to clear screen for replay

	if(player == 1)
	{
	//ask to replay only player 1//////////////////////////////////////////
  RIT128x96x4Clear();
	RIT128x96x4StringDraw("Play Again?", 30,0,15);
	RIT128x96x4StringDraw("PB1 = YES", 33,16,15);
	RIT128x96x4StringDraw("PB2 =  NO", 33,32,15);
  sysTickWait1mS(150);

	//read buttons for user input
	while(1)
	{
		PB1 = read_PBSwitchNum(1);
		PB2 = read_PBSwitchNum(2);
		
		if(PB1 == 0)
		{
			//yes, replay
			replay = 1;
			break;
		}
		else if(PB2 == 0)
		{
			//no, end game
			replay = 0;
			RIT128x96x4Clear();
			RIT128x96x4StringDraw("GOODBYE", 39,75,15);
			sysTickWait1mS(150);
			break;
		}
		//////////////////////////////////////////////////////////////
	}
	
	Xbee_Send(replay); // send 0 no play 1 to replay
}
else
{
	RIT128x96x4Clear();
	RIT128x96x4StringDraw("Waiting...", 0,0,15);
	//p2 waits for replay or not from player 1
	while(1)
	{
			//wait for received replay signal
			receivedMessage = Xbee_Receive();
		
			//We check that data is valid (0x5A5 is	a data invalid flag), and output
			if(receivedMessage != 0x5A5)
			{	
				  replay = receivedMessage;
				
					break;
			}
	}
}	
	
RIT128x96x4Clear();
sysTickWait1mS(500);

}
///////////////////////////////////////////////////////////////////////
}
/* Blinks the row to easily show the player his or her selection
 *
 *
 *
 */
 void blink(int column, int board[])
 {
	 int i = column*12;
	 
	 RIT128x96x4StringDraw(" ",18+i+6,36+  0,15);
	 RIT128x96x4StringDraw(" ",18+i+6,36+  8,15);
	 RIT128x96x4StringDraw(" ",18+i+6,36+ 16,15);
	 RIT128x96x4StringDraw(" ",18+i+6,36+ 24,15);
	 RIT128x96x4StringDraw(" ",18+i+6,36+ 32,15);
	 RIT128x96x4StringDraw(" ",18+i+6,36+ 40,15);
	 RIT128x96x4StringDraw(" ",18+i+6,36+ 48,15);
	 sysTickWait1mS(500);
	 
	 drawBoard(board);
 }
/* Checks if the selected column by the player is valid by checking
 * if all the spots are filled up vertically.
 *
 * return - 1 if valid column otherwise 0
 */
int checkValidColumn(int column, int board[])
{
	if(column > 6 || column < 0)
		return 0;
	
	if(board[column] != 0)
		return 0;
	
	return 1;
}
	 
/* Searches the board in all possible winning directions from the last
 * piece put on the board. 
 *
 * return - 1 if a winner has been determined, 0 if no one won, 2 if tie
 */
int checkWin(int board[], int last)
{
	int curr = 0;
	int row = last / 7;
	int col;
	int winCount;
	int tempLast;
	int i;
	
	//check row/////////////////////////////////////
	curr = board[last];
	winCount = 0;
	
	for(col = 0; col < 7; col++)
	{
		if(board[(row*7) + col] == curr)
			winCount++;
		else
			winCount = 0;
		
		if(winCount == 4)
			return 1;
	}
	//check column iff row is 4th or higher/////////////////////
	if(row < 3)
	{
		winCount = 1;
		tempLast = last;
	
		while(tempLast < 42)
		{
			tempLast += 7;
			
			if(board[tempLast] == curr)
				winCount++;
			else
				winCount = 0;
			
			if(winCount == 4)
			return 1;
		}
	}
	//check diagnal right/////////////////////////////////////
	
	winCount = 0;
	//get leftmost poition
	tempLast = last;
	col = last % 7; //get current column
	
	//move down left of last position by adding 6 and 
	//keeping track of columns to stay in bounds
	while(tempLast < 35 && col > 0)
	{
		tempLast += 6;
		col--;
	}
	
	//now check the entire diagnal going back up
	while(col < 7 && tempLast >= 0)
	{
		if(board[tempLast] == curr)
			winCount++;
		else
			winCount = 0;
		
		if(winCount == 4)
			return 1;
		
		col++;
		tempLast -= 6;
	}
	
	//check diagnal left/////////////////////////////////////
	
	winCount = 0;
	
	//get leftmost poition
	tempLast = last;
	col = last % 7; //get current column
	
	//move up left of last position by subtracting 8 and 
	//keeping track of columns to stay in bounds
	while(tempLast > 7 && col > 0)
	{
		tempLast -= 8;
		col--;
	}
	
	//now check the entire diagnal going downwards
	while(col < 7 && tempLast < 42)
	{
		if(board[tempLast] == curr)
			winCount++;
		else
			winCount = 0;
		
		if(winCount == 4)
			return 1;
		
		col++;
		tempLast += 8;
	}
	
	//check full board, if full and no wins its a tie
	for(i = 0; i < 42; i++)
	{
		if(board[i] == 0)
			return 0;
	}
	
	//no wins/losses were found, tie game
	return 2;
}
 
	 
/* Given the game board for the game, prints out the 6x7 board 
 * with the current contents
 *
 *
 */
void drawBoard(int board[])
{
	int i;
	int j;
	int col;
	
	RIT128x96x4StringDraw("CONNECT FOUR", 30,0,15); //begining of each row
	
	for(i = 0; i < 6; i++)
	{
		RIT128x96x4StringDraw("|", 18,36+i*8,15); //begining of each row
		j = 0;
		col = 0;
		
		for(j = 0; j < 13; j+=2)
		{
			RIT128x96x4StringDraw(convert(board[(i*7)+col]),18+ 6*(j+1),36+i*8,15);
			RIT128x96x4StringDraw("|",18+ 6*(j+2),36+i*8,15);
			col++;
		}
	}
}

/* Used for determining the string representation of the players
 * piece such that the board can display it properly.
 *
 * return - string equivalent of the given case for the game
 */
char* convert(int code)
{
	switch(code)
	{
		case(1): return "O";
		case(2): return "X";
	}
	return "_";
}

/* Assigns each array index of the board to a '_' character
 * to prepare the board for a new game.
 *
 *
 */
void clearBoard(int board[])
{
	int i = 0;
	
	for(i = 0; i < 42; i++)
  {
		board[i] = 0;
  }	
}

/* Adds the players piece at the given column in the first
 * availabe slot from the top.
 *
 * return - array position of the last piece placed
 */
int addPiece(int col, int board[], int piece)
{
	//defaul at bottom of board
	int start = 35 + col;
	
	//loop upward the column checking for the first valid spot
	while(board[start] != 0 && start > 0)
  {
		start -= 7;
	}	

	//add the piece at the first valid spot
	if(start >= 0)
	{
		board[start] = piece;
	}
	
	return start;
}
