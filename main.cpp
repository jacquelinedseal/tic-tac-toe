#include <iostream>
#include <fstream> //for file IO
#include <iomanip> //for formatting
#include <string>
#include <vector>
#include <array>

#include "gameclass.h"

using namespace std;

//NOTE: While the Tic-Tac-Toe class contains the same information as the 'Game' struct below, the struct was used to prevent major inefficiencies / waste of memory.
    //The struct - while not entirely necessary - functions as a temporary storage medium, preventing the program from having to copy out the entire class object (including all functions) every time a new game is added to the vector.
struct Game
{
    int num;
    string name;
    array< array<char, 3>, 3 > board;
};

int main()
{
    bool exit{false}, valid, keepReading, alreadySaved{false};
    TicTacToe current;

    int fileID{1}, numGame, location, lastFileID, turn;
    char newOrSaved, playerXO, saveOrDiscard;
    string gameName, saveName;

    Game gameRecord;
    vector<Game> gameInFile{}; //for storing contents of file (reads a record of all games)

    fstream savedGames("gamesFile.txt", ios::in | ios::out | ios::app); //opens fstream object 'savedGames' for input/output

    if(!savedGames)//ensures file is opened properly
    {
        cout << "Error opening file. Exiting." << endl;
        return 1;
    }

    cout << "Welcome to Tic-Tac-Toe!" << '\n' << endl;

    printLocationDemo();

    do
    {
        do //Reads from file with data sanitation
        {
            savedGames >> fileID >> gameName; //reads game # and name first

            keepReading = readRecord(savedGames, valid);

            if(valid == true)
            {
                gameRecord.num = fileID;
                gameRecord.name = gameName; //if values are valid, updates contents of struct

                for(int location{1}; location <= 9 && keepReading == true; location++)
                {
                    savedGames >> playerXO; //then reads Xs and Os
                    keepReading = readRecord(savedGames, valid);

                    if(valid == true && (playerXO == 'X' || playerXO == 'O' || playerXO == '-'))
                        gameRecord.board.at(getRowI(location)).at(getColumnI(location)) = playerXO; //if values are valid, updates contents of struct
                    else //if chain of Xs and Os contains invalid error, moves on to next line...
                    {
                        valid = false;
                        break;
                    }
                }

                if(valid == true) //if all values are valid, adds gameRecord struct to vector for storage
                    gameInFile.push_back(gameRecord);
            }

            lastFileID = fileID; //for use later on (in saving files with consecutive game #)
        }while(keepReading == true);

        savedGames.close();
        cout << '\n';

        do
        {
            cout << "Do you want to (s)tart a new game or (l)oad one from a saved file ('x' to exit)? ";
            cin >> newOrSaved;

            valid = dataSanitation(cin);

            if(valid == false || (newOrSaved != 's' && newOrSaved != 'l' && newOrSaved != 'x'))
                cout << "Invalid Input. ";
            else if(gameInFile.size() == 0 && newOrSaved == 'l') //user should not be able to access a loaded game if there are none
            {
                cout << "There are no saved games. Starting new game." << endl;
                newOrSaved = 's';
            }
        }while((newOrSaved != 's' && newOrSaved != 'l' && newOrSaved != 'x') || valid == false); //data sanitation

        if(newOrSaved == 'x') //exits program
            break;

        if(newOrSaved == 'l')
        {
            alreadySaved = true; //applicable later (when updating games that were previously saved)
            cout << '\n' << "Saved Games: " << endl;

            for(auto game : gameInFile)
                cout << game.num << " " << game.name << endl; //outputs existing saved games
            cout << endl;

            do
            {
                cout << "Please enter the number of the game that you want to open: ";
                cin >> numGame;

                valid = dataSanitation(cin);

                if((valid == false) || (numGame < 1 || numGame > fileID))
                    cout << "Invalid Input. ";
            }while((valid == false) || (numGame < 1 || numGame > fileID)); //data sanitation

            for(auto game : gameInFile) //selects element of struct vector that corresponds to user's number
            {
                if(game.num == numGame)
                {
                    current.setNum(game.num); //initializes an object of class TicTacToe to values stored in struct intermediary
                    current.setName(game.name);

                    for(int rowI{0}; rowI < 3; rowI++)
                    {
                        for(int columnI{0}; columnI < 3; columnI++)
                            current.setPos(rowI, columnI, game.board.at(rowI).at(columnI));
                    }
                }
            }

            newOrSaved = 's'; //allows user to play with saved board (just as one would with a new game)
        }

        if(newOrSaved == 's')
        {
            if(gameInFile.size() == 0) //for numbering of saved games (addresses obscure bug)
                lastFileID = 0;

            current.printBoard();
            cout << "Enter EOF at any point to exit game." << endl;

            do
            {
                if(current.calcTurn() == 1) //uses calcTurn function to determine who should start
                {
                    turn = 1;
                    playerXO = 'X';
                }
                else if(current.calcTurn() == 2)
                {
                    turn = 2;
                    playerXO = 'O';
                }

                do
                {
                    cout << '\n' << "Player " << turn << " - Enter the location number at which you want to place an " << playerXO << " (EOF to exit): ";
                    cin >> location;

                    if(cin.eof() == true) //eof is used to exit game
                        break;
                    else if((dataSanitation(cin) == false) || (location < 1 || location > 9))
                        cout << "Invalid Input. ";
                    else if(current.locationFilled(location) == true)
                        cout << "This location is already filled." << endl;
                }while((location < 1 || location > 9) || (current.locationFilled(location) == true)); //data sanitation

                if(cin.eof() == true && alreadySaved == false) //if game has not been saved already and user wants to end it...
                {
                    cin.clear();

                    do
                    {
                        cout << '\n' << "Do you want to (s)ave or (d)iscard? ";
                        cin >> saveOrDiscard;

                        valid = dataSanitation(cin);

                        if((valid == false) || (saveOrDiscard != 's' && saveOrDiscard != 'd'))
                            cout << "Invalid Input. ";
                    }while((valid == false) || (saveOrDiscard != 's' && saveOrDiscard != 'd')); //data sanitation

                    if(saveOrDiscard == 's') //if the user wants to save...
                    {
                        exit = true;
                        cin.clear();

                        do
                        {
                            cout << '\n' << "What do you want to save this game as? ";
                            cin >> saveName;

                            valid = dataSanitation(cin);

                            if(valid == false)
                                cout << "Invalid Input. ";
                        }while(valid == false); //data sanitation

                        //saves game to end of file (appended)
                        savedGames.close();
                        savedGames.open("gamesFile.txt", ios::out | ios::app);

                        if(!savedGames)
                        {
                            cout << "Error opening file. Exiting." << endl;
                            return 1;
                        }

                        lastFileID++;
                        save(lastFileID, saveName, current.getBoard(), savedGames);
                        savedGames.close();

                        cout << "Game saved. ";
                    }
                    else if(saveOrDiscard == 'd') //if the user wants to discard...
                    {
                        exit = true;
                        cin.clear();
                        cout << "Discarding game. " << endl;
                    }
                    cout << "Returning to the main screen." << endl;
                }
                else if(cin.eof() == true && alreadySaved == true) //if the user wants to exit game and the game has already been saved
                {
                    cin.clear();

                    do
                    {
                        cout << '\n' << "Do you want to (s)ave or (d)iscard? ";
                        cin >> saveOrDiscard;

                        valid = dataSanitation(cin);

                        if((valid == false) || (saveOrDiscard != 's' && saveOrDiscard != 'd'))
                            cout << "Invalid Input. ";
                    }while((valid == false) || (saveOrDiscard != 's' && saveOrDiscard != 'd')); //data sanitation

                    if(saveOrDiscard == 's') //if the user wants to save...
                    {
                        exit = true;
                        cin.clear();
                        for(auto& game : gameInFile) //updates value of original struct used (to edited value in TicTacToe object)
                        {
                            if(game.num == current.getNum())
                            {
                                game.num = current.getNum();
                                game.name = current.getName();

                                for(int rowI{0}; rowI < 3; rowI++)
                                {
                                    for(int columnI{0}; columnI < 3; columnI++)
                                        game.board.at(rowI).at(columnI) = current.getXorO(rowI, columnI);
                                }
                            }
                        }

                        savedGames.close();
                        savedGames.open("gamesFile.txt", ios::out | ios::trunc); //overwrite contents of file (with updated vector of structs)

                        if(!savedGames)
                        {
                            cout << "Error opening file. Exiting." << endl;
                            return 1;
                        }

                        for(auto game : gameInFile)
                        {
                            save(game.num, game.name, game.board, savedGames); //prints information on each of stored games to file (including updated file)

                        }

                        cout << "Saving game." << endl;
                        savedGames.close();
                    }
                    else if(saveOrDiscard == 'd') //if the user wants to discard...
                    {
                        exit = true;
                        cin.clear();
                        cout << "Discarding game. ";
                    }
                }

                if(exit == false) //if the user did not enter EOF to exit
                {
                    current.updateBoard(location, playerXO); //updates board with new placement of X / O
                    current.printBoard();

                    if(current.winCheck() == true) //if a player has won...
                    {
                        cout << "Player " << turn << " wins! Exiting." << endl;

                        if(alreadySaved == true) //if the player won on an already saved game, deletes game from file
                        {
                            savedGames.close();
                            savedGames.open("gamesFile.txt", ios::out | ios::trunc);

                            if(!savedGames)
                            {
                                cout << "Error opening file. Exiting." << endl;
                                return 1;
                            }

                            for(auto game : gameInFile)
                            {
                                if(game.num != current.getNum())
                                {
                                    save(game.num, game.name, game.board, savedGames);
                                }
                            }

                            cout << "Deleting saved copy." << endl;
                            savedGames.close();
                        }

                        break;
                    }
                    else if(current.drawCheck() == true) //if the players tied on an already saved game, deletes game from file
                    {
                        cout << "Player 1 and Player 2 have tied. Game over." << endl;

                        if(alreadySaved == true)
                        {
                            savedGames.close();
                            savedGames.open("gamesFile.txt", ios::out | ios::trunc);

                            if(!savedGames)
                            {
                                cout << "Error opening file. Exiting." << endl;
                                return 1;
                            }

                            for(auto game : gameInFile)
                            {
                                if(game.num != current.getNum())
                                {
                                    save(game.num, game.name, game.board, savedGames);
                                }
                            }

                            cout << "Deleting saved copy." << endl;
                            savedGames.close();
                        }

                        break;
                    }

                }
            }while(current.winCheck() == false && exit == false);
        }

        //resets variables / vectors / streams (etc.) for next cycle
        exit = false;
        alreadySaved = false;
        current.reset();
        gameInFile.clear();

        savedGames.close();
        savedGames.open("gamesFile.txt", ios::in); //opens file for next iteration

        if(!savedGames)
        {
            cout << "Error opening file. Exiting." << endl;
            return 1;
        }
    }while(newOrSaved != 'x');

    savedGames.close();
    cout << '\n' << "Thanks for playing!" << endl;

    return 0;
}

