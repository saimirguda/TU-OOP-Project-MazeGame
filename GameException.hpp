//---------------------------------------------------------------------------------------------------------------------
// GameException.hpp
//
// Definition of the GameException class.
// The GameException class is an exception that is thrown at a game ending error / command. It's responsible for
// providing an error message and exiting the program.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef GAMEEXCEPTION_HPP
#define GAMEEXCEPTION_HPP

#include <exception>

enum GameError
{
  ERROR_NONE = 0,
  ERROR_MEMORY = 1,
  ERROR_CONFIGURATION = 2
};

class GameException : public std::exception
{
public:
  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor that initializes return_value_ member based on the given parameter.
  ///
  /// @param return_value The return value that the game should exit with, when this exception is thrown
  //
  GameException(int return_value);

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Getter for the return_value_ member.
  ///
  /// @return The return value
  //
  int getReturnValue() const noexcept;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Error message to print out before exit, when this exception is thrown.
  ///
  /// @return The error message
  //
  const char *what() const noexcept override;

private:
  int return_value_;
};

#endif
