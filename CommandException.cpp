//---------------------------------------------------------------------------------------------------------------------
// CommandException.cpp
//
// Implementation of the CommandException class.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#include "CommandException.hpp"

//---------------------------------------------------------------------------------------------------------------------
CommandException::CommandException(CommandError error_number, std::string character) :
    command_error_{error_number}
{
  std::string quote;
  switch (command_error_)
  {
    case UNKNOWN_COMMAND:
      error_message_ = "Please enter a KNOWN COMMAND to not confuse your treasure hunters!\n";
      break;
    case WRONG_NUM_PARAMS:
      error_message_ = "Please enter a correct NUMBER OF PARAMETERS to not confuse your treasure hunters!\n";
      break;
    case CHARACTER_SELECTION:
      error_message_ = "Who do you want to move?!?\n";
      break;
    case LOCATION_ERROR:
      quote =": \"I don't understand where I should go!\"\n";
      error_message_ = character + quote;
      break;
    case DISTANCE_ERROR:
      quote =": \"I don't understand how far I should go!\"\n";
      error_message_ = character + quote;
      break;
    case WORNG_DIRECTION:
      quote =": \"I can't go that way right now!\"\n";
      error_message_ = character + quote;
      break;
    case BLOCKED_DIRECTION:
      quote =": \"My way is blocked!\"\n";
      error_message_ = character + quote;
      break;
    case SPACE_ERROR:
      quote =": \"There is not enough space on that tile!\"\n";
      error_message_ = character + quote;
      break;
    case SCARY_ROOM:
      quote =": \"That room is too scary for me!\"\n";
      error_message_ = character + quote;;
      break;
    case THIEF_UNLOCK_ERROR:
      error_message_ = "Thief: \"Nothing to unlock here!\"\n";
      break;
    case FIGHTER_FIGHT_ERROR:
      error_message_ = "Fighter: \"Nothing to fight here!\"\n";
      break;
    case SEER_NO_BALL:
      error_message_ = "Seer: \"I can't scry without my magic crystal ball!\"\n";
      break;
    case SEER_CONFUSED:
      error_message_ = "Seer: \" don't understand which room I should scry!\"\n";
      break;
    case ROOM_INVALID:
      error_message_ = "Seer: \"There is no room I can reveal at this position!!\"\n";
      break;
    case ROOM_REVEALED_ALREADY:
      error_message_ = "Seer: \"We already know that room...\"\n";
      break;
    default:
      error_message_ = "\n";
      break;
  }
}

int CommandException::getCommandError() const noexcept
{
  return command_error_;
}

//---------------------------------------------------------------------------------------------------------------------
const char *CommandException::what() const noexcept
{
  return error_message_.c_str();
}