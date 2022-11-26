//---------------------------------------------------------------------------------------------------------------------
// Character.cpp
//
// Implementation of the Character class.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#include "Character.hpp"

//---------------------------------------------------------------------------------------------------------------------
Character::Character() : type_{CharacterType::NONE} {}

//---------------------------------------------------------------------------------------------------------------------
Character::Character(CharacterType type) : type_{type} {}

//---------------------------------------------------------------------------------------------------------------------
CharacterType Character::getCharacterType() const
{
  return type_;
}

//---------------------------------------------------------------------------------------------------------------------
void Character::setPosition(Position position)
{
  position_ = position;
}

//---------------------------------------------------------------------------------------------------------------------
Position Character::getPosition() const
{
  return position_;
}

//---------------------------------------------------------------------------------------------------------------------
std::string Character::getCharacterName() const
{
  switch (type_)
  {
    case CharacterType::FIGHTER:
    {
      return "Fighter";
    }
    case CharacterType::THIEF:
    {
      return "Thief";
    }
    case CharacterType::SEER:
    {
      return "Seer";
    }
    default:
    {
      return "";
    }
  }
}
