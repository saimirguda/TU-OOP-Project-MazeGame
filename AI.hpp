//---------------------------------------------------------------------------------------------------------------------
// AI.hpp
//
// Definition of the AI class.
// The AI class represents an artifical intelligence, that can, based on current game state, calculate
// and execute a sequence of game commands for a single round.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef AI_HPP
#define AI_HPP

#include "Game.hpp"
#include "Tile.hpp"
#include <string>
#include <vector>
#include <deque>

class AI
{
public:
  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor that initializes command_tokens_ with the preprocessed command string and stores a reference
  /// to the singleton game object.
  ///
  /// @param command_string command string representing this command
  //
  AI();

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Copy construnctor is deleted.
  //
  AI(const AI &) = delete;

  //-----------------------------------------------------------------------------------------------------------------
  ///
  /// Copy assignment operator is deleted.
  //
  AI &operator=(const AI &) = delete;

  ~AI() = default;

  void playARound();

private:
  Game &game_;

  Position calculatePosition(const int distance, const Position start_position, const std::string direction) const;

  Position calculatePath(Character *someone, Tile* goal_tile);

  void executePath(std::deque<Position> champion_path, std::string allowed_direction, Character* selected_champ);

  Tile* chooseGoal(Character* hero);

  Tile* calculateNearestTile(Tile* char_tile);

  void executeCommand(std::string command_string);

};

#endif
