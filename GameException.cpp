//---------------------------------------------------------------------------------------------------------------------
// GameException.cpp
//
// Implementation of the GameException class.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#include "GameException.hpp"

//---------------------------------------------------------------------------------------------------------------------
GameException::GameException(int return_value) : return_value_{return_value} {}

//---------------------------------------------------------------------------------------------------------------------
int GameException::getReturnValue() const noexcept
{
  return return_value_;
}

//---------------------------------------------------------------------------------------------------------------------
const char *GameException::what() const noexcept
{
  switch (return_value_)
  {
    case ERROR_MEMORY:
      return "Error: Not enough memory!\n";
      break;
    case ERROR_CONFIGURATION:
      return "Error: Invalid configuration!\n";
      break;
    default:
      return "";
      break;
  }
}