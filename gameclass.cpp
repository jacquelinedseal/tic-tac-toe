#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <array>

#include "gameclass.h"

//Name: save
//Argument: integer 'num', string 'name', 2D array representing tic-tac-toe board, fstream object 'file'
//Processing: saves a particular game by printing relevant information to file
//Output: none (void)
void save(int num, std::string name, std::array< std::array<char, 3>, 3> board, std::fstream& file)
{
   file << num << " " << name;
   for(auto row : board)
   {
       for(auto elem : row)
          file << " " << elem;
   }

   file << std::endl;
}

//Name: readRecord
//Argument: istream object 'fileIn', bool 'validValue'
//Processing: analyzes error states of input stream to determine whether record from file is valid or corrupted
//Output: keepGoing (a bool representing whether the program should continue to read from the file)
bool readRecord(std::istream& fileIn, bool& validValue)
{
    char dummy;
    bool keepGoing;
    validValue = false;

    if(fileIn.good() == true) //if input is valid...
    {
        keepGoing = true;
        validValue = true;
    }
    else if(fileIn.eof() == true) //if eof detected…
    {
        keepGoing = false; //prevents program from continuing to read from file
    }
    else if(fileIn.bad() == true) //if bad() bit set… (significant problem)
    {
        keepGoing = false; //prevents program from reading further values from file
    }
    else if(fileIn.fail() == true) //if fail() bit set… (incorrect input type)
    {
        keepGoing = true; //allow program to continue reading from file
        fileIn.clear(); //clear error states
        while(fileIn.get(dummy) && dummy != '\n'); //ignore invalid fileIn

        if(!fileIn) //if additional error encountered
        {
            std::cout << "Encountered another error. Exiting." << std::endl;
            keepGoing = false;
        }
    }

    return keepGoing;
}

//Name: dataSanitation
//Arguments: istream object 'fileIn'
//Processing: analyzes error states of stream to determine whether user input is valid
//Output: validValue (a bool representing whether the input is valid)
bool dataSanitation(std::istream& fileIn)
{
    char dummy;
    bool validValue;

    if(fileIn.good() == true) //if input is valid...
    {
        validValue = true; //valid
        while(fileIn.get(dummy) && dummy != '\n'); //removes excess, unnecessary characters appended to input
    }
    else //if input is invalid (ie. bad / eof / fail)...
    {
        fileIn.clear(); //clear error states
        while(fileIn.get(dummy) && dummy != '\n'); //ignore invalid fileIn
        validValue = false; //invalid
    }

    return validValue;
}

//Name: printLocationDemo
//Argument: none
//Processing: prints a formatted grid with position numbers labelled (for user convenience)
//Output: none (void)
void printLocationDemo()
{
    std::cout << "Position Numbers:" << std::endl;
    std::cout << "     |     |     " << std::endl;
    std::cout << "  " << 1 << "  |  " << 2 << "  |  " << 3 << "  " << std::endl;
    std::cout << "_____|_____|_____" << std::endl;
    std::cout << "     |     |     " << std::endl;
    std::cout << "  " << 4 << "  |  " << 5 << "  |  " << 6 << "  " << std::endl;
    std::cout << "_____|_____|_____" << std::endl;
    std::cout << "     |     |    " << std::endl;
    std::cout << "  " << 7 << "  |  " << 8 << "  |  " << 9 << "  " << std::endl;
    std::cout << "     |     |    " << std::endl;
}

//Name: getRowI
//Argument: location (integer)
//Processing: determines the row number of particular integer location in 2D array
//Output: row(the row index of the location in the 2D array)
int getRowI(int location)
{
    int row;

    if(location == 1 || location == 2 || location == 3)
        row = 0;
    else if(location == 4 || location == 5 || location == 6)
        row = 1;
    else if(location == 7 || location == 8 || location == 9)
        row = 2;

    return row;
}

//Name: getColumnI
//Argument: location (integer)
//Processing: determines the column number of particular integer location in 2D array
//Output: column(the column index of the location in the 2D array)
int getColumnI(int location)
{
    int column;

    if(location == 1 || location == 4 || location == 7)
        column = 0;
    else if(location == 2 || location == 5 || location == 8)
        column = 1;
    else if(location == 3 || location == 6 || location == 9)
        column = 2;

    return column;
}

//constructor for TicTacToe (fills grid with '-', sets default values)
TicTacToe::TicTacToe()
{
   for(int row{0}; row < 3; row++)
   {
       for(int column{0}; column < 3; column++)
       {
           board[row][column] = '-';
       }
   }

   gameName = "<untitled>";
   gameNum = 0; // what to initialize as?
}

//Name: setNum
//Argument: integer 'num'
//Processing: sets the private data member gameNum according to 'num'
//Return: none (void)
void TicTacToe::setNum(int num)
{
    gameNum = num;
}

//Name: setName
//Argument: string 'name'
//Processing: sets the private data member gameName according to 'name'
//Return: none (void)
void TicTacToe::setName(std::string name)
{
    gameName = name;
}

//Name: setPos
//Argument: integers representing row/column index in 2D array, XorO char representing contents of location in array
//Processing: fills in the private data member board with location-specific char
//Return: none (void)
void TicTacToe::setPos(int rowI, int columnI, char XorO)
{
    board[rowI][columnI] = XorO;
}

//Name: getNum
//Argument: none
//Processing: returns private data member 'gameNum'
//Return: gameNum
int TicTacToe::getNum()
{
    return gameNum;
}

//Name: getName
//Argument: none
//Processing: returns private data member 'gameName'
//Return: gameName
std::string TicTacToe::getName()
{
    return gameName;
}

//Name: getXorO
//Argument: integer row/column index
//Processing: returns private data member in 'board' 2D array (specified by argument parameters)
//Return: particular element of 'board' 2D array
char TicTacToe::getXorO(int rowI, int columnI)
{
    return board.at(rowI).at(columnI);
}

//Name: getBoard
//Argument: none
//Processing: returns entire 'board' (2D array of chars)
//Return: 'board'
std::array<std::array<char, 3>, 3> TicTacToe::getBoard()
{
    return board;
}

//Name: drawCheck
//Argument: none
//Processing: checks whether the player have tied (ie. the board is full and nobody has won)
//Output: bool representing whether a draw has occured (draw)
bool TicTacToe::drawCheck()
{
   bool full{true}, draw;

   for(int pos{1}; pos <= 9; pos++)
   {
       if(locationFilled(pos) == false)
           full = false;
   }

   if(winCheck() == false && full == true)
       draw = true;
   else
       draw = false;

   return draw;
}

//Name: winCheck
//Argument: none
//Processing: determines whether a player has won the game based on positioning of their X/Os
//Output: win (bool representing whether a win has occured)
bool TicTacToe::winCheck()
{
   bool win;
   TicTacToe current = *this;

   if(current.threeInRow(1, 2, 3) && locationFilled(1))
        win = true;
   else if(current.threeInRow(4, 5, 6) && locationFilled(4))
        win = true;
   else if(current.threeInRow(7, 8, 9) && locationFilled(7))
        win = true;
   else if(current.threeInRow(1, 4, 7) && locationFilled(1))
        win = true;
   else if(current.threeInRow(2, 5, 8) && locationFilled(2))
        win = true;
   else if(current.threeInRow(3, 6, 9) && locationFilled(3))
        win = true;
   else if(current.threeInRow(1, 5, 9) && locationFilled(1))
        win = true;
   else if(current.threeInRow(3, 5, 7) && locationFilled(3))
        win = true;
   else
        win = false;

   return win;
}

//Name: threeInRow
//Argument: three integers (representing positions on the board)
//Processing: used in winCheck(), compares contents of three specified locations to determine whether a win has occured
//Output: threeGroup (bool representing whether the three locations mentioned have the same contents)
bool TicTacToe::threeInRow(int pos1, int pos2, int pos3)
{
    bool threeGroup;
    int row1, column1, row2, column2, row3, column3;

    row1 = getRowI(pos1);
    column1 = getColumnI(pos1);

    row2 = getRowI(pos2);
    column2 = getColumnI(pos2);

    row3 = getRowI(pos3);
    column3 = getColumnI(pos3);

    if(board[row1][column1] == board[row2][column2] && board[row1][column1] == board[row3][column3])
        threeGroup = true;
    else
        threeGroup = false;

    return threeGroup;
}

//Name: calcTurn
//Argument: none
//Processing: counts the number of Xs/Os on the grid to determine whose turn it is after a game is loaded from file
//Output: playerTurn (int representing the number of the player whose turn it is)
int TicTacToe::calcTurn()
{
    int playerTurn, Xs{0}, Os{0};

    for(auto row : board)
    {
        for(auto elem : row)
        {
            if(elem == 'X')
                Xs++;
            else if(elem == 'O')
                Os++;
        }
    }

    if(Xs > Os)
        playerTurn = 2;
    else if(Xs == Os)
        playerTurn = 1;

    return playerTurn;
}

//Name: locationFilled
//Argument: integer (location)
//Processing: checks whether a specified location is filled with either an 'x' or and 'o'
//Output: filled (bool representing whether the position is occupied)
bool TicTacToe::locationFilled(int location)
{
   bool filled;
   int rowI{getRowI(location)}, columnI{getColumnI(location)};

   if(board[rowI][columnI] == 'X' || board[rowI][columnI] == 'O')
       filled = true;
   else
       filled = false;

   return filled;
}

//Name: reset
//Argument: none
//Processing: erases contents of grid and replaces with '-'
//Output: none (void)
void TicTacToe::reset()
{
   for(auto& row : board)
   {
       for(auto& elem : row)
           elem = '-';
   }
}

//Name: printBoard
//Argument: none
//Processing: outputs formatted grid with Xs, Os, and empty ('-') spaces
//Output: none (void)
void TicTacToe::printBoard()
{
   std::cout << '\n' << "Player 1: X's" << "  " << "Player 2: O's" << std::endl;
   std::cout << "     |     |     " << std::endl;
   std::cout << "  " << board[0][0] << "  |  " << board[0][1] << "  |  " << board[0][2] << "  " << std::endl;
   std::cout << "_____|_____|_____" << std::endl;
   std::cout << "     |     |     " << std::endl;
   std::cout << "  " << board[1][0] << "  |  " << board[1][1] << "  |  " << board[1][2] << "  " << std::endl;
   std::cout << "_____|_____|_____" << std::endl;
   std::cout << "     |     |    " << std::endl;
   std::cout << "  " << board[2][0] << "  |  " << board[2][1] << "  |  " << board[2][2] << "  " << std::endl;
   std::cout << "     |     |    " << std::endl;
}

//Name: updateBoard
//Argument: int (location), char (XorO)
//Processing: changes the contents of the board based on user moves
//Output: none (void)
void TicTacToe::updateBoard(int location, char XorO)
{
    int row{getRowI(location)}, column{getColumnI(location)};

    board[row][column] = XorO;
}
