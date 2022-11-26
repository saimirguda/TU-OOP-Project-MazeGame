//---------------------------------------------------------------------------------------------------------------------
// CommandException.hpp
//
// Definition of the CommandException class.
// The CommandException class is responsible for signaling if there is a command related problem and
// printing the error message.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef COMMANDEXCEPTION_HPP
#define COMMANDEXCEPTION_HPP

#include <exception>
#include <string>

enum CommandError
{
  UNKNOWN_COMMAND = 1,
  WRONG_NUM_PARAMS = 2,
  CHARACTER_SELECTION = 3,
  LOCATION_ERROR = 4,
  DISTANCE_ERROR = 5,
  WORNG_DIRECTION = 6,
  BLOCKED_DIRECTION = 7,
  SPACE_ERROR = 8,
  SCARY_ROOM = 9,
  THIEF_UNLOCK_ERROR = 10,
  FIGHTER_FIGHT_ERROR = 11,
  SEER_NO_BALL = 12,
  SEER_CONFUSED = 13,
  ROOM_INVALID = 14,
  ROOM_REVEALED_ALREADY = 15
};

class CommandException : public std::exception
{
public:
  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor that initializes return_value_ and character_ members based on the given parameters.
  ///
  /// @param command_error The command error value that indicates which message is to print,
  ///   when this exception is thrown
  /// @param character The character with whom the error occurred (by default empthy string)
  //
  CommandException(CommandError command_error, std::string character = "");

  int getCommandError() const noexcept;

  const char *what() const noexcept override;

private:
  CommandError command_error_;
  std::string error_message_;
};

#endif