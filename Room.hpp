//---------------------------------------------------------------------------------------------------------------------
// Room.hpp
//
// Definition of the Room class.
// The Room class represents a room of the game map.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef ROOM_HPP
#define ROOM_HPP

#include "BasicTile.hpp"
#include "MagicTile.hpp"
#include "Tile.hpp"
#include <iostream>
#include <string>
#include <vector>

class Room
{
private:
  char room_id_;
  static const size_t SIZE_OF_ROOM = 25;
  static const std::string UNREVEALED_TILE_LINE;
  std::vector<std::vector<Tile *>>room_map_;
  bool revealed_;
  int number_of_monsters_in_the_room_ = 0;

public:
  //-----------------------------------------------------------------------------------------------------------------
  /// The constructor initializes the members with the parameters' values.
  /// @param room_id room id
  /// @param room_info_str string, that containts information about room and its items
  //
  Room(char room_id, std::string room_info_str);

  //-----------------------------------------------------------------------------------------------------------------
  /// Room class destructor, that deletes memory, allocated in constructor
  //
  ~Room();

  ///----------------------------------------------------------------------------------------------------------------
  /// Getter for member room_id_
  //
  char getRoomID() const;

  //-----------------------------------------------------------------------------------------------------------------
  /// Getter for member room_map_
  //
  std::vector<std::vector<Tile *>> &getRoomMap();

  //-----------------------------------------------------------------------------------------------------------------
  /// Getter for member revealed_
  //
  bool getRevealed() const;

  //-----------------------------------------------------------------------------------------------------------------
  /// Setter for member revealed_
  //
  void setRevealed();

  //-----------------------------------------------------------------------------------------------------------------
  /// Kill a monster in room by decreasing number_of_monsters_in_the_room_ by one
  //
  void killAMonster();

  //-----------------------------------------------------------------------------------------------------------------
  /// Returns true if there a monster in the room
  //
  bool isThereAMonster() const;

  //-----------------------------------------------------------------------------------------------------------------
  /// Returns room map as string variable
  //
  std::string getRoomString() const;

  static const size_t TILE_ROW_IN_ROOM = 5;
  static const size_t TILE_COLUMN_IN_ROOM = 5;
};

#endif