//---------------------------------------------------------------------------------------------------------------------
// main.cpp
//
// Implementation of the main program.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#include "Game.hpp"
#include "GameException.hpp"
#include <iostream>
#include <string>
#include <vector>

//---------------------------------------------------------------------------------------------------------------------
///
/// The main program.
/// Plays a game of OOP1 Maze, based on the rules described in the assignment.
///
/// @param argc Number of command line parameters
/// @param argv Command line parameters
///
/// @return The execution status of the program
//
int main(int argc, char *argv[])
{
  auto &game = Game::getInstance();
  std::vector<std::string> rooms;

  for (int arg_index = 1; arg_index < argc; arg_index++)
  {
    std::string param(argv[arg_index]);
    rooms.push_back(param);
  }
  try
  {
    try
    {
      game.updateGameMap(rooms);
      game.prepareGame();
      game.runGame();
    }
    catch (std::bad_alloc &)
    {
      throw GameException(ERROR_MEMORY);
    }
  }
  catch (GameException &exception)
  {
    std::cout << exception.what();
    return exception.getReturnValue();
  }

  return 0;
}
