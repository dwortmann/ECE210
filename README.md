# ECE210
UW-Madison

Project Title: ConnectFour
Your Name: Dan Wortmann
Team Members: Hao Liang

Project Instructions: 

	Our project is a digital version of the ConnectFour game that is played between two players.
The object of the game is to place four pieces belonging to the player in a line. This line can be
either horizontal, vertical, or diagnal. The piece placement is done by selecting a column and the 
piece takes the first available spot starting at the bottom of the board.

	On start up of the game, follow the on screen instructions to set up the wireless capability
of the BB13 that is used for the game. As the game begins one player will be prompted to place a piece
in a column of his or her choice. The column selection is done by the leftmost potentiometer and based
on the on screen "selection" blink, the player will know which column is selected. Note, that only a
valid column may be selected, otherwise the game will ignore the choice and wait until a valid move is
made. At the end of the players turn, the game switches to the opponent to take his or her turn. This
cycle continues until one of the players has won, or a draw has been determined by filling up the board
with no winning arrangement. At the end the game asks player 1, whether or not he or she would like to
replay.
			

My role in the project: 

	I did a lot of the initial and final coding of the program for connect four. I also contributed
in brainstorming of ideas, as well as planning of the program, its parameters, and functions, and finally
the testing of the  code on the BB13.

Workload Breakdown:    

	The work was split pretty evenly between the two of us. I did most of the coding in the project,
and my partner did the power point and helped a lot in play testing and the finishing touches. In the 
beginning we came together to plan out the program, the functions that will be needed, as well as any
problems to watch out for while coding. From there we worked I worked at home to finish the first draft
of the program. Once the initial code was done we came together again and debugged most of the problems
as well as tested the wireless capability. Overall I think I spent an extra two hours or so on the project
just on my own, which is offset by my partner being completely in charge of the power point. For the most
part it's 50/50 ,or if anything 60/40 (me doing a little more).			

What I learned from the project: 

	This project, as well as the class itself, helped to reinforce of all of topics stressed through-
out the semester. Starting from the basic functioning hardware such as push buttons, potentiometers, and 
the switches, I got good experience working with them, as well as dealing with issues that may arise from
implementing code using them. Some particulars dealth with return values, and the range of values that
certain hardware may return. For instance the potentiometer returns 0 - 1024 so in order to use it for
column selection we can use integer division and get values 0 - 6 (for 7 columns). Also one thing I got
from the experience-as well as from other classes- is to pay attention to specification of hardware,
especially when dealing with active-low or active-high push buttons. Finally, working with C code has 
helped me become a better programmer, and helped me reinforce the language. 