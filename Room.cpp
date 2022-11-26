//---------------------------------------------------------------------------------------------------------------------
// Room.cpp
//
// Implementation of the Room class.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#include "Room.hpp"
#include "GameException.hpp"
#include <sstream>

//---------------------------------------------------------------------------------------------------------------------
const std::string Room::UNREVEALED_TILE_LINE = "UUUUUUU\nUUUUUUU\nUUUUUUU\n";

//---------------------------------------------------------------------------------------------------------------------
Room::Room(char room_id, std::string room_info_str) :room_id_{room_id}
{
  std::string room_info_buf = room_info_str;
  std::string tile;
  Tile *created_tile;
  if (room_info_str.length() != SIZE_OF_ROOM)
  {
    throw GameException(GameError::ERROR_CONFIGURATION);
  }
  for (size_t row = 0; row < TILE_ROW_IN_ROOM; row++)
  {
    std::vector<Tile *> tile_row;
    for (size_t column = 0; column < TILE_COLUMN_IN_ROOM; column++)
    {
      tile = room_info_buf.substr(0, 1);
      room_info_buf.erase(0, 1);
      if (tile == "W")
      {
        created_tile = new BasicTile(WALL, this, false, false);
      }
      else if (tile == "P")
      {
        created_tile = new BasicTile(PASSAGE, this, true, true);
      }
      else if (tile == "X")
      {
        created_tile = new MagicTile(SECRET_DOOR, this, false, false);
      }
      else if (tile == "M")
      {
        created_tile = new MagicTile(MONSTER, this, false, false);
        number_of_monsters_in_the_room_++;
      }
      else if (tile == "G")
      {
        created_tile = new MagicTile(SAND_WATCH, this, true, true);
      }
      else if (tile == "C")
      {
        created_tile = new MagicTile(CRYSTAL_BALL, this, true, true);
      }
      else if (tile == "H")
      {
        created_tile = new MagicTile(HORIZONTAL_DOOR, this, false, false);
      }
      else if (tile == "V")
      {
        created_tile = new MagicTile(VERTICAL_DOOR, this, false, false);
      }
      else if (tile == "L")
      {
        created_tile = new MagicTile(LOOT, this, true, true);
      }
      else if (tile == "T")
      {
        created_tile = new MagicTile(THIEF, this, true, true);
      }
      else if (tile == "F")
      {
        created_tile = new MagicTile(FIGHTER, this, true, true);
      }
      else if (tile == "S")
      {
        created_tile = new MagicTile(SEER, this, true, true);
      }
      tile_row.push_back(created_tile);
    }
    room_map_.push_back(tile_row);
  }
  room_map_.at(0).at(0)->setRoomName(room_id_);
  revealed_ = (room_id == 'S') ? true : false;
}

//---------------------------------------------------------------------------------------------------------------------
Room::~Room()
{
  for (auto room : room_map_)
  {
    for (size_t room_part = 0; room_part < TILE_ROW_IN_ROOM; room_part++)
    {
      delete (room[room_part]);
    }
  }
}

///--------------------------------------------------------------------------------------------------------------------
char Room::getRoomID() const
{
  return room_id_;
}

//---------------------------------------------------------------------------------------------------------------------
std::vector<std::vector<Tile *>> &Room::getRoomMap()
{
  return room_map_;
}

//---------------------------------------------------------------------------------------------------------------------
bool Room::getRevealed() const
{
  return revealed_;
}

//---------------------------------------------------------------------------------------------------------------------
void Room::setRevealed()
{
  revealed_ = true;
}

//---------------------------------------------------------------------------------------------------------------------
std::string Room::getRoomString() const
{
  std::string room_string;
  for (auto tile_row : room_map_)
  {
    std::vector<std::stringstream> row_of_strings;
    for (int tile_index = 0; tile_index < 5; tile_index++)
    {
      if(revealed_)
      {
      row_of_strings.push_back(std::stringstream(tile_row[tile_index]->getTileString()));
      }
      else
      {
        std::string unrevealed_line = UNREVEALED_TILE_LINE;
        row_of_strings.push_back(std::stringstream(unrevealed_line));
      }
    }
    for (int line = 0; line < 3; line++)
      {
        for (auto &tile_string : row_of_strings)
        {
          std::string tile_line;
          std::getline(tile_string, tile_line);
          room_string += tile_line;
        }
        room_string += '\n';
      }
  }
  return room_string;
}

//---------------------------------------------------------------------------------------------------------------------
bool Room::isThereAMonster() const
{
  return number_of_monsters_in_the_room_ != 0;
}

//---------------------------------------------------------------------------------------------------------------------
void Room::killAMonster()
{
  if (number_of_monsters_in_the_room_ > 0)
  {
    number_of_monsters_in_the_room_--;
  }
}