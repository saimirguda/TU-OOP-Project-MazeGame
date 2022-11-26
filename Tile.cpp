//---------------------------------------------------------------------------------------------------------------------
// Tile.cpp
//
// Implementation of the Tile class.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#include "Tile.hpp"

//---------------------------------------------------------------------------------------------------------------------
Tile::Tile(TileType tile_type, Room *tile_in_room, bool stay_allowed, bool go_through_allowed) :
    character_{NULL},
    tile_type_{tile_type},
    tile_in_room_{tile_in_room},
    stay_allowed_{stay_allowed},
    go_through_allowed_{go_through_allowed}
{
}

//---------------------------------------------------------------------------------------------------------------------
Room *Tile::getRoomReference() const
{
  return tile_in_room_;
}

//---------------------------------------------------------------------------------------------------------------------
bool Tile::isStayAllowed() const
{
  return stay_allowed_;
}

//---------------------------------------------------------------------------------------------------------------------
bool Tile::isGoThroughAllowed() const
{
  return go_through_allowed_;
}

//---------------------------------------------------------------------------------------------------------------------
TileType Tile::getTileType() const
{
  return tile_type_;
}

void Tile::setCharacter(Character *character)
{
  character_ = character;
  if(character_ != NULL)
  {
    tile_represent_.at(1).at(TILE_CHARACTER_POS) = static_cast <char>(character_->getCharacterType());
  }
  else
  {
    tile_represent_.at(1).at(TILE_CHARACTER_POS) = static_cast <char>(CharacterType::NONE);
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Tile::setRoomName(char room_name)
{
  tile_represent_.at(1) = "███";
  tile_represent_.at(1) += room_name;
  tile_represent_.at(1) += "███";
}

//---------------------------------------------------------------------------------------------------------------------
std::string Tile::getTileString()
{
  std::string tile_string;
  for(const auto& tile_line : tile_represent_)
  {
    tile_string += tile_line;
    tile_string += '\n';
  }
  return tile_string;
}

//---------------------------------------------------------------------------------------------------------------------
Character *Tile::getCharacter()
{
  return character_;
}


void Tile::setPosition(Position pos)
{
  tile_position_ = pos;
}

Position Tile::getPosition()
{
  return tile_position_;
}

void Tile::markVisit()
{
  visited_ = true;
}

void Tile::markUnvisit()
{
  visited_ = false;
}
bool Tile::getVisit()
{
  return visited_;
}

void Tile::gotTreasure()
{
  rich_ = true;
}

bool Tile::getRich()
{
  return rich_;
}