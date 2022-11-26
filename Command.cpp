//---------------------------------------------------------------------------------------------------------------------
// Command.cpp
//
// Implementation of the Command class.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#include "Command.hpp"
#include "CommandException.hpp"
#include "Game.hpp"
#include "GameException.hpp"
#include "TileMap.hpp"
#include <sstream>
#include "AI.hpp"

//---------------------------------------------------------------------------------------------------------------------
const char *HELP_TEXT = "Commands:\n"
                        " - help\n"
                        "    Prints this help text.\n"
                        "\n"
                        " - quit\n"
                        "    Terminates the game.\n"
                        "\n"
                        " - map\n"
                        "    Activates or deactivates the map.\n"
                        "\n"
                        " - flip\n"
                        "    Changes the possible move direction.\n"
                        "\n"
                        " - move <CHARACTER> <DIRECTION> <DISTANCE>\n"
                        "    Moves a character.\n"
                        "    <CHARACTER>: the character to move, T/F/S\n"
                        "    <DIRECTION>: the direction to move, up/down/left/right\n"
                        "    <DISTANCE>: optional, the distance to move\n"
                        "\n"
                        " - unlock\n"
                        "    Unlocks a nearby door.\n"
                        "\n"
                        " - fight\n"
                        "    Fights a nearby monster.\n"
                        "\n"
                        " - scry <KNOWN_ROOM> <DIRECTION>\n"
                        "    Reveals an unknown room.\n"
                        "    <KNOWN_ROOM>: where to scry from\n"
                        "    <DIRECTION>: which direction to scry\n"
                        "\n";

//---------------------------------------------------------------------------------------------------------------------
Command::Command(const std::string &command_string) :
    command_tokens_{preprocessCommand(command_string)}, game_{Game::getInstance()}
{
}

//---------------------------------------------------------------------------------------------------------------------
std::vector<std::string> Command::preprocessCommand(const std::string &command_string)
{
  std::istringstream command_stream(command_string);
  std::vector<std::string> command_tokens_;
  std::string token;

  while (command_stream >> token)
  {
    for (auto &character : token)
    {
      character = std::toupper(character);
    }
    command_tokens_.push_back(token);
  }

  return command_tokens_;
}


//---------------------------------------------------------------------------------------------------------------------
void Command::executeCommand()
{
  if (command_tokens_.empty())
  {
    return;
  }

  auto &command = command_tokens_.at(0);
  auto params = std::vector<std::string>(command_tokens_.begin() + 1, command_tokens_.end());
  if (command == "PLAY")
  {
    checkIfParamsEmpty(params);
    executePlay();
    game_.printGameMap();
  }
  else if (command == "MAP")
  {
    checkIfParamsEmpty(params);
    bool map_activated = game_.getMapActivated();
    map_activated = !map_activated;
    game_.setMapActivated(map_activated);
    if (map_activated)
    {
      game_.printGameMap();
    }
  }
  else if (command == "FLIP")
  {
    checkIfParamsEmpty(params);
    game_.flipCard();
    game_.printGameMap();
  }
  else if (command == "HELP")
  {
    checkIfParamsEmpty(params);
    std::cout << HELP_TEXT;
  }
  else if (command == "QUIT")
  {
    checkIfParamsEmpty(params);
    throw GameException(GameError::ERROR_NONE);
  }
  else if (command == "MOVE")
  {
    executeMove(params);
    game_.printGameMap();
  }
  else if (command == "SCRY")
  {
    executeScry(params);
    game_.printGameMap();
  }
  else if (command == "FIGHT")
  {
    checkIfParamsEmpty(params);
    executeFight();
    game_.printGameMap();
  }
  else if (command == "UNLOCK")
  {
    checkIfParamsEmpty(params);
    executeUnlock();
    game_.printGameMap();
  }
  else if (command == "WHOAMI")
  {
    checkIfParamsEmpty(params);
    executeWhoami();
  }
  else
  {
    throw CommandException(CommandError::UNKNOWN_COMMAND);
  }
}

//---------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------
void Command::executePlay()
{
  auto ai = AI();
  ai.playARound();
}

//---------------------------------------------------------------------------------------------------------------------
void Command::executeWhoami() const
{
  std::cout << "Team: mommy sorry mommy sorry" << std::endl;
}


//---------------------------------------------------------------------------------------------------------------------
void Command::executeMove(const std::vector<std::string> params) const
{
  auto params_size = params.size();
  int distance = 0;
  checkMoveParameters(params, params_size, distance);
  Character *characterToMove = game_.selectCharacter(params.at(0));
  auto start_position = characterToMove->getPosition();
  auto tile_map = game_.getTileMap();
  auto direction = game_.getCurrentDirection();
  std::vector<Tile *> path;
  try
  {
    path = tile_map->calculatePath(distance, start_position, direction);
  }
  catch (const std::out_of_range &)
  {
    throw CommandException(CommandError::BLOCKED_DIRECTION, characterToMove->getCharacterName());
  }
  Position end_position = tile_map->calculatePosition(distance, start_position, direction);

  checkPath(path, characterToMove);

  characterToMove->setPosition(end_position);

  Tile *end_tile = tile_map->getTileAt(end_position);
  checkForSandWatch(end_tile);

  TileType fighter_tile_type = tile_map->getTileAt(game_.getFighter()->getPosition())->getTileType();
  TileType seer_tile_type = tile_map->getTileAt(game_.getSeer()->getPosition())->getTileType();
  TileType thief_tile_type = tile_map->getTileAt(game_.getThief()->getPosition())->getTileType();

  auto start_tile = tile_map->getTileAt(start_position);
  start_tile->setCharacter(NULL);
  end_tile->setCharacter(characterToMove);

  if(end_tile->getTileType() == TileType::LOOT)
    end_tile->gotTreasure();

  openSecretDoors(fighter_tile_type, seer_tile_type, thief_tile_type);
  checkNeighborhoodForReveal(end_position);
  checkWin(fighter_tile_type, seer_tile_type, thief_tile_type);
}

//---------------------------------------------------------------------------------------------------------------------
void Command::executeScry(const std::vector<std::string> params) const
{
  if (params.size() != 2)
  {
    throw CommandException(CommandError::WRONG_NUM_PARAMS);
  }

  auto tile_map = game_.getTileMap();
  Position seer_position = game_.getSeer()->getPosition();
  auto seer_tile = tile_map->getTileAt(seer_position);
  TileType seer_position_tile_type = seer_tile->getTileType();
  if (seer_position_tile_type != TileType::CRYSTAL_BALL)
  {
    throw CommandException(CommandError::SEER_NO_BALL);
  }

  std::string room_id = params.at(0);
  Position room_position = game_.findRoomById(room_id);
  auto room = game_.getRoomAt(room_position);

  if (!room->getRevealed())
  {
    throw CommandException(CommandError::SEER_CONFUSED);
  }

  std::string direction = params.at(1);
  if ((direction != Game::UP) && (direction != Game::DOWN) && (direction != Game::LEFT) && (direction != Game::RIGHT))
  {
    throw CommandException(CommandError::SEER_CONFUSED);
  }
  Room *room_to_reveal = nullptr;

  if (direction == Game::UP)
  {
    room_position.row_ -= 1;
    room_to_reveal = game_.getRoomAt(room_position);
  }
  else if (direction == Game::DOWN)
  {
    room_position.row_ += 1;
    room_to_reveal = game_.getRoomAt(room_position);
  }
  else if (direction == Game::LEFT)
  {
    room_position.column_ -= 1;
    room_to_reveal = game_.getRoomAt(room_position);
  }
  else if (direction == Game::RIGHT)
  {
    room_position.column_ += 1;
    room_to_reveal = game_.getRoomAt(room_position);
  }

  if (room_to_reveal->getRevealed())
  {
    throw CommandException(CommandError::ROOM_REVEALED_ALREADY);
  }

  if (seer_position_tile_type == TileType::CRYSTAL_BALL)
  {
    room_to_reveal->setRevealed();
    dynamic_cast<MagicTile *>(seer_tile)->magicUsed();
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Command::executeFight() const
{
  int monster_near = 0;
  auto fighter_position = game_.getFighter()->getPosition();
  std::vector<Tile *> neighbors = game_.getTileMap()->getNeighborTiles(fighter_position);
  for (const auto& neighbor : neighbors)
  {
    if (neighbor->getTileType() == MONSTER)
    {
      dynamic_cast<MagicTile *>(neighbor)->magicUsed();
      monster_near++;
      neighbor->getRoomReference()->killAMonster();
    }
  }
  if (monster_near == 0)
  {
    throw CommandException(CommandError::FIGHTER_FIGHT_ERROR);
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Command::executeUnlock() const
{
  int door_is_near = 0;
  auto thief_position = game_.getThief()->getPosition();
  std::vector<Tile *> neighbors = game_.getTileMap()->getNeighborTiles(thief_position);
  for (auto neighbor : neighbors)
  {
    if (neighbor->getTileType() == VERTICAL_DOOR || neighbor->getTileType() == HORIZONTAL_DOOR)
    {
      dynamic_cast<MagicTile *>(neighbor)->magicUsed();
      door_is_near++;
    }
  }
  if (door_is_near == 0)
  {
    throw CommandException(CommandError::THIEF_UNLOCK_ERROR);
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Command::checkPath(std::vector<Tile *> path, Character *playing_character) const
{
  Tile *end_tile = path.back();
  path.pop_back();
  for (auto &tile : path)
  {
    if (!tile->isGoThroughAllowed() || (!tile->getRoomReference()->getRevealed()))
    {
      throw CommandException(CommandError::BLOCKED_DIRECTION, playing_character->getCharacterName());
    }
  }
  if ((!end_tile->isStayAllowed()) || (!end_tile->getRoomReference()->getRevealed()))
  {
    throw CommandException(CommandError::BLOCKED_DIRECTION, playing_character->getCharacterName());
  }
  if (end_tile->getCharacter() != NULL)
  {
    throw CommandException(CommandError::SPACE_ERROR, playing_character->getCharacterName());
  }
  if (playing_character->getCharacterType() != CharacterType::FIGHTER)
  {
    if (end_tile->getRoomReference()->isThereAMonster() == true)
    {
      throw CommandException(CommandError::SCARY_ROOM, playing_character->getCharacterName());
    }
    for (auto &tile : path)
    {
      if (tile->getRoomReference()->isThereAMonster() == true)
      {
        throw CommandException(CommandError::SCARY_ROOM, playing_character->getCharacterName());
      }
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Command::checkNeighborhoodForReveal(const Position position) const
{
  auto neighborTiles = game_.getTileMap()->getNeighborTiles(position);
  for (auto tile : neighborTiles)
  {
    tile->getRoomReference()->setRevealed();
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Command::openSecretDoors(const TileType fighter_tile_type, const TileType seer_tile_type,
                              const TileType thief_tile_type) const
{
  auto thief_position = game_.getThief()->getPosition();
  auto fighter_position = game_.getFighter()->getPosition();
  auto seer_position = game_.getSeer()->getPosition();
  auto tile_map = game_.getTileMap();
  auto tile_map_row_size = tile_map->getTileMapRowSize();
  auto tile_map_col_size = tile_map->getTileMapColumnSize();
  if ((fighter_tile_type == TileType::FIGHTER) && (seer_tile_type == TileType::SEER) &&
      (thief_tile_type == TileType::THIEF))
  {
    for (int row = 0; row < tile_map_row_size; row++)
    {
      for (int col = 0; col < tile_map_col_size; col++)
      {
        Position position{row, col};
        auto tile = tile_map->getTileAt(position);
        if (tile->getTileType() == TileType::SECRET_DOOR)
        {
          dynamic_cast<MagicTile *>(tile)->magicUsed();
        }
      }
    }
    dynamic_cast<MagicTile *>(tile_map->getTileAt(fighter_position))->magicUsed();
    dynamic_cast<MagicTile *>(tile_map->getTileAt(thief_position))->magicUsed();
    dynamic_cast<MagicTile *>(tile_map->getTileAt(seer_position))->magicUsed();

    game_.secretUnlock();
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Command::checkWin(const TileType fighter_tile_type, const TileType seer_tile_type,
                       const TileType thief_tile_type) const
{
  if ((fighter_tile_type == TileType::LOOT) && (seer_tile_type == TileType::LOOT) &&
      (thief_tile_type == TileType::LOOT))
  {
    game_.printGameMap();
    std::cout << "You win, congratulations! It took you " << game_.getFlipCounter()
              << " card flips to find the treasure.\n";
    throw GameException(GameError::ERROR_NONE);
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Command::checkMoveParameters(const std::vector<std::string> params, const size_t params_size, int &distance) const
{
  if ((params_size != 2) && (params_size != 3))
  {
    throw CommandException(CommandError::WRONG_NUM_PARAMS);
  }

  Character *character = game_.selectCharacter(params.at(0));
  std::string direction = params.at(1);
  if ((direction != Game::UP) && (direction != Game::DOWN) && (direction != Game::LEFT) && (direction != Game::RIGHT))
  {
    throw CommandException(CommandError::LOCATION_ERROR, character->getCharacterName());
  }
  distance = 1;
  if (params_size == 3)
  {
    size_t processed_number_chars = 0;
    try
    {
      distance = std::stoi(params.at(2), &processed_number_chars, 10);
    }
    catch (std::exception &)
    {
      throw CommandException(CommandError::DISTANCE_ERROR, character->getCharacterName());
    }
    if (processed_number_chars != params.at(2).length())
    {
      throw CommandException(CommandError::DISTANCE_ERROR, character->getCharacterName());
    }
    if (distance <= 0)
    {
      throw CommandException(CommandError::DISTANCE_ERROR, character->getCharacterName());
    }
  }
  if (direction != game_.getCurrentDirection())
  {
    throw CommandException(CommandError::WORNG_DIRECTION, character->getCharacterName());
  }
}

void Command::checkIfParamsEmpty(const std::vector<std::string> params) const
{
  if (!params.empty())
  {
    throw CommandException(CommandError::WRONG_NUM_PARAMS);
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Command::checkForSandWatch(Tile *tile) const
{
  auto tile_type = tile->getTileType();
  if (tile_type == TileType::SAND_WATCH)
  {
    game_.rollBackFlipCounter();
    dynamic_cast<MagicTile *>(tile)->magicUsed();
  }
}
