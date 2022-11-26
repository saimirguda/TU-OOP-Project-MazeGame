//---------------------------------------------------------------------------------------------------------------------
// Character.hpp
//
// Definition of the Character class.
// The Character class represents a character of the game, and holds all information about the character.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <iostream>
#include <string>
#include <vector>

enum class CharacterType
{
  FIGHTER = 'F',
  THIEF = 'T',
  SEER = 'S',
  NONE = ' '
};

struct Position
{
  int row_;
  int column_;
};

class Character
{
private:
  CharacterType type_;
  Position position_;

public:
  //-----------------------------------------------------------------------------------------------------------------
  /// Constructor that sets character type to NONE
  //
  Character();

  //-----------------------------------------------------------------------------------------------------------------
  /// Constructor that sets character type to type
  ///
  /// @param type type of the character (can be FIGHTER, THIEF, SEER or NONE)
  //
  Character(CharacterType type);

  //-----------------------------------------------------------------------------------------------------------------
  /// Getter for member type_
  //
  CharacterType getCharacterType() const;

  //-----------------------------------------------------------------------------------------------------------------
  /// Setter for member position_
  //
  void setPosition(Position position);

  //-----------------------------------------------------------------------------------------------------------------
  /// Getter for position_
  ///
  Position getPosition() const;

  //-----------------------------------------------------------------------------------------------------------------
  /// Returns the character name as a string
  //
  std::string getCharacterName() const;

  static const int FIGHTER_START_ROW_INDEX = 1;
  static const int FIGHTER_START_COL_INDEX = 1;
  static const int THIEF_START_ROW_INDEX = 1;
  static const int THIEF_START_COL_INDEX = 3;
  static const int SEER_START_ROW_INDEX = 3;
  static const int SEER_START_COL_INDEX = 1;
};

#endif