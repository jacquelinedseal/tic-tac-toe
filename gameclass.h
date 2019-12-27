#ifndef GAMECLASS_H
#define GAMECLASS_H

#include <iostream>
#include <fstream> //for file IO
#include <iomanip> //for formatting
#include <string>
#include <vector>
#include <array>

void save(int num, std::string name, std::array<std::array<char, 3>, 3> board, std::fstream& file); //saves game information to file
bool readRecord(std::istream& fileIn, bool& validValue); //reads from file and performs data sanitation
bool dataSanitation(std::istream& fileIn); //performs generalized data sanitation on standard (cin) inputs

void printLocationDemo(); //prints tic-tac-toe grid with location numbers (for user reference)
int getRowI(int location); //based on the location number, determines corresponding row number (in array)
int getColumnI(int location); //based on the location number, determines corresponding column number (in array)

class TicTacToe
{
    private:
       std::array< std::array<char, 3>, 3 > board; //represents Tic-Tac-Toe board
       std::string gameName; //each saved game has a name
       int gameNum; //each saved game has a number
    public:
       TicTacToe(); //constructor

       //set functions
       void setNum(int num);
       void setName(std::string name);
       void setPos(int rowI, int columnI, char XorO);

       //get functions
       int getNum();
       std::string getName();
       char getXorO(int rowI, int columnI);
       std::array< std::array<char, 3>, 3 > getBoard();

       bool drawCheck(); //determines whether the players have tied
       bool winCheck(); //determines whether a player has won

       bool threeInRow(int pos1, int pos2, int pos3); //checks for groups of three Xs/Os in a row (used with winCheck())

       int calcTurn(); //determines whose turn it is
       bool locationFilled(int location); //determines whether a location on the board is filled

       void reset(); //clears the board for next game
       void printBoard(); //prints contents of board
       void updateBoard(int location, char XorO); //changes contents of board based on user moves
};

#endif // GAMECLASS_H
