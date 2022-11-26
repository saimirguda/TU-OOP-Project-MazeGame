//---------------------------------------------------------------------------------------------------------------------
// MagicTile.cpp
//
// Implementation of the MagicTile class.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#include "MagicTile.hpp"
#include <string>
#include <vector>

//---------------------------------------------------------------------------------------------------------------------
MagicTile::MagicTile(TileType tile_type, Room *tile_in_room, bool stay_allowed, bool go_through_allowed) :
    Tile(tile_type, tile_in_room, stay_allowed, go_through_allowed)
{
  tile_type_ = tile_type;
  switch (static_cast<char>(tile_type_))
  {
    case TileType::SECRET_DOOR:
      tile_represent_ = { "███████", "███████", "███████"};
      break;
    case TileType::MONSTER:
      tile_represent_ = { "       ", "   M   ", "       "};
      break;
    case TileType::SAND_WATCH:
      tile_represent_ = {" \\   / ", "  | |  ", " /   \\ "};
      break;
    case TileType::CRYSTAL_BALL:
      tile_represent_ = {"  / \\  ", " |   | ", "  \\ /  "};
      break;
    case TileType::HORIZONTAL_DOOR:
      tile_represent_= {"       ", "███████", "       "};
      break;
    case TileType::VERTICAL_DOOR:
      tile_represent_= {"   █   ", "   █   ", "   █   "};
      break;
    case TileType::LOOT:
      tile_represent_ = {"$$$$$$$", "$     $", "$$$$$$$"};
      break;
    case TileType::THIEF:
      tile_represent_ = {"TTTTTTT", "T     T", "TTTTTTT"};
      break;
    case TileType::FIGHTER:
      tile_represent_ = {"FFFFFFF", "F     F", "FFFFFFF"};
      break;
    case TileType::SEER:
      tile_represent_ = {"SSSSSSS", "S     S", "SSSSSSS"};
      break;
  }
  magic_used_ = false;
}

//---------------------------------------------------------------------------------------------------------------------
void MagicTile::magicUsed()
{
  if (magic_used_ == true)
  {
    return;
  }

  for (int tile_line = 0; tile_line < TILE_LINES; tile_line++)
  {
    tile_represent_.at(tile_line) = "       ";
  }
  setCharacter(character_);
  tile_type_ = PASSAGE;
  magic_used_ = true;
  go_through_allowed_ = true;
  stay_allowed_= true;
}

