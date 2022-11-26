//---------------------------------------------------------------------------------------------------------------------
// Game.cpp
//
// Implementation of the Game class.
//
// Group: EN-13
//
// Authors:
//   01640850
//   11933108
//   11934620
//---------------------------------------------------------------------------------------------------------------------
//

#include "Game.hpp"
#include "Command.hpp"
#include "CommandException.hpp"
#include "GameException.hpp"
#include "Random.hpp"
#include "RoomInfo.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

const std::string Game::UP = "UP";
const std::string Game::RIGHT = "RIGHT";
const std::string Game::DOWN = "DOWN";
const std::string Game::LEFT = "LEFT";
const std::vector<std::string> Game::CARDS = {Game::UP, Game::RIGHT, Game::DOWN, Game::LEFT};
const std::string Game::HORIZONTAL_ROOM_SEPARATOR = "═══════════════════════════════════╬";
const std::string Game::HORIZONTAL_ROOM_SEPARATOR_START = "╬";
const std::string Game::VERTICAL_ROOM_SEPARATOR = "║";
const int Game::NUMBER_OF_LINES_IN_ROOM = 15;

//---------------------------------------------------------------------------------------------------------------------
Game::Game() :
    fighter_{CharacterType::FIGHTER},
    seer_{CharacterType::SEER},
    thief_{CharacterType::THIEF},
    flip_counter_{-1},
    map_activated_{true}
{
}

//---------------------------------------------------------------------------------------------------------------------
Game::~Game()
{
  for (auto &row : rooms_)
  {
    for (auto room : row)
    {
      delete room;
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
bool Game::getMapActivated() const
{
  return map_activated_;
}

//---------------------------------------------------------------------------------------------------------------------
void Game::setMapActivated(const bool map_activated)
{
  map_activated_ = map_activated;
}

//---------------------------------------------------------------------------------------------------------------------
TileMap *Game::getTileMap() const
{
  return tile_map_.get();
}

//---------------------------------------------------------------------------------------------------------------------
std::string Game::getCurrentDirection() const
{
  return current_direction_;
}

//---------------------------------------------------------------------------------------------------------------------
int Game::getFlipCounter() const
{
  return flip_counter_;
}

//---------------------------------------------------------------------------------------------------------------------
Character *Game::getFighter()
{
  return &fighter_;
}

//---------------------------------------------------------------------------------------------------------------------
Character *Game::getThief()
{
  return &thief_;
}

//---------------------------------------------------------------------------------------------------------------------
Character *Game::getSeer()
{
  return &seer_;
}

//---------------------------------------------------------------------------------------------------------------------
void Game::prepareGame()
{
  createCardOrder();
  flipCard();
  greetPlayer();
  printGameMap();
}

//---------------------------------------------------------------------------------------------------------------------
void Game::runGame()
{
  while (true)
  {
    printPrompt();
    auto input = readInput();
    auto command = Command(input);
    try
    {
      command.executeCommand();
    }
    catch (CommandException &exception)
    {
      std::cout << exception.what();
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Game::updateGameMap(const std::vector<std::string> &rooms)
{
  checkRoomValidity(rooms);
  auto &roomInfoInstance = Oop::RoomInfo::getInstance();
  for (const auto &row_of_rooms : rooms)
  {
    std::vector<Room *> row;
    for (char const &room_id : row_of_rooms)
    {
      auto room = new Room(room_id, roomInfoInstance.getRoomInfoString(room_id));
      if (room_id == 'S')
      {
        placeCharacters(room);
        setCharactersPosition(rooms_.size(), row.size());
      }

      row.push_back(room);
    }
    rooms_.push_back(row);
  }
  tile_map_ = std::make_unique<TileMap>(rooms_);
}

//---------------------------------------------------------------------------------------------------------------------
void Game::createCardOrder()
{
  std::vector<std::string> cards = Game::CARDS;
  while (!cards.empty())
  {
    auto direction_index = Oop::Random::getInstance().getRandomCard(cards.size()) - 1;
    auto direction = cards.at(direction_index);
    cards_order_.push_back(direction);
    cards.erase(cards.begin() + direction_index);
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Game::flipCard()
{
  flip_counter_++;
  current_direction_ = cards_order_.at(0);
  cards_order_.push_back(current_direction_);
  cards_order_.erase(cards_order_.begin());
}

//---------------------------------------------------------------------------------------------------------------------
void Game::checkRoomValidity(const std::vector<std::string> rooms) const
{
  auto start_room_counter = 0;
  auto first_row_length = (rooms.at(0)).length();
  std::vector<char> room_ids;
  if (rooms.size() < 1)
  {
    throw GameException(GameError::ERROR_CONFIGURATION);
  }
  for (const auto &row_of_rooms : rooms)
  {
    if (row_of_rooms.length() != first_row_length)
    {
      throw GameException(GameError::ERROR_CONFIGURATION);
    }
    if (row_of_rooms.length() > 5)
    {
      throw GameException(GameError::ERROR_CONFIGURATION);
    }
    for (char const &room_id : row_of_rooms)
    {
      if (room_id == 'S')
      {
        start_room_counter++;
      }
      if ((room_id < 'A') || (room_id > 'Z'))
      {
        throw GameException(GameError::ERROR_CONFIGURATION);
      }
      if (std::find(room_ids.begin(), room_ids.end(), room_id) != room_ids.end())
      {
        throw GameException(GameError::ERROR_CONFIGURATION);
      }
      room_ids.push_back(room_id);
    }
  }
  if (start_room_counter != 1)
  {
    throw GameException(GameError::ERROR_CONFIGURATION);
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Game::rollBackFlipCounter()
{
  if (flip_counter_ > 5)
  {
    flip_counter_ -= 5;
  }
  else
  {
    flip_counter_ = 0;
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Game::greetPlayer() const
{
  std::cout << "Welcome to the magical OOP1 Maze!!!" << std::endl;
}

//---------------------------------------------------------------------------------------------------------------------
void Game::printPrompt() const
{
  std::cout << " > ";
}

//---------------------------------------------------------------------------------------------------------------------
std::string Game::readInput() const
{
  std::string input;
  std::getline(std::cin, input);
  if (std::cin.eof())
  {
    throw GameException(GameError::ERROR_NONE);
  }
  return input;
}

//---------------------------------------------------------------------------------------------------------------------
void Game::printGameMap() const
{
  if (!ai_activated_)
  {
    std::cout << "Card Flip Counter: " << std::setw(3) << flip_counter_ << std::endl;
    if (map_activated_)
    {
      std::cout << HORIZONTAL_ROOM_SEPARATOR_START;
      for (size_t room_index = 0; room_index < rooms_.at(0).size(); room_index++)
      {
        std::cout << HORIZONTAL_ROOM_SEPARATOR;
      }
      std::cout << std::endl;

      for (auto &row_of_rooms : rooms_)
      {
        std::vector<std::stringstream> row_of_strings;
        for (auto &room : row_of_rooms)
        {
          row_of_strings.push_back(std::stringstream(room->getRoomString()));
        }
        for (int line = 0; line < NUMBER_OF_LINES_IN_ROOM; line++)
        {
          std::cout << VERTICAL_ROOM_SEPARATOR;
          for (auto &room_string : row_of_strings)
          {
            std::string room_line;
            std::getline(room_string, room_line);
            std::cout << room_line << VERTICAL_ROOM_SEPARATOR;
          }
          std::cout << std::endl;
        }
        std::cout << HORIZONTAL_ROOM_SEPARATOR_START;
        for (size_t room_index = 0; room_index < rooms_.at(0).size(); room_index++)
        {
          std::cout << HORIZONTAL_ROOM_SEPARATOR;
        }
        std::cout << std::endl;
      }
    }
    auto direction_to_print = current_direction_;
    for (auto &character : direction_to_print)
    {
      character = tolower(character);
    }
    std::cout << "Possible move: " << direction_to_print << std::endl;
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Game::placeCharacters(Room *room)
{
  auto room_map = room->getRoomMap();
  room_map.at(Character::FIGHTER_START_ROW_INDEX).at(Character::FIGHTER_START_COL_INDEX)->setCharacter(&fighter_);
  room_map.at(Character::THIEF_START_ROW_INDEX).at(Character::THIEF_START_COL_INDEX)->setCharacter(&thief_);
  room_map.at(Character::SEER_START_ROW_INDEX).at(Character::SEER_START_COL_INDEX)->setCharacter(&seer_);

}

//---------------------------------------------------------------------------------------------------------------------
void Game::setCharactersPosition(const int row_index, const int column_index)
{
  Position fighter_position;
  Position thief_position;
  Position seer_position;

  fighter_position.row_ = (row_index * Room::TILE_ROW_IN_ROOM) + Character::FIGHTER_START_ROW_INDEX;
  fighter_position.column_ = (column_index * Room::TILE_ROW_IN_ROOM) + Character::FIGHTER_START_COL_INDEX;
  thief_position.row_ = (row_index * Room::TILE_ROW_IN_ROOM) + Character::THIEF_START_ROW_INDEX;
  thief_position.column_ = (column_index * Room::TILE_ROW_IN_ROOM) + Character::THIEF_START_COL_INDEX;
  seer_position.row_ = (row_index * Room::TILE_ROW_IN_ROOM) + Character::SEER_START_ROW_INDEX;
  seer_position.column_ = (column_index * Room::TILE_ROW_IN_ROOM) + Character::SEER_START_COL_INDEX;

  start_fighter_ = fighter_position;
  start_seer_ = seer_position;
  start_thief_ = thief_position;

  fighter_.setPosition(fighter_position);
  thief_.setPosition(thief_position);
  seer_.setPosition(seer_position);
}

//---------------------------------------------------------------------------------------------------------------------
Character *Game::selectCharacter(const std::string character_string)
{
  Character *character = nullptr;
  if (character_string == std::string(1, static_cast<char>(CharacterType::FIGHTER)))
  {
    character = &fighter_;
  }
  else if (character_string == std::string(1, static_cast<char>(CharacterType::THIEF)))
  {
    character = &thief_;
  }
  else if (character_string == std::string(1, static_cast<char>(CharacterType::SEER)))
  {
    character = &seer_;
  }
  else
  {
    throw CommandException(CommandError::CHARACTER_SELECTION);
  }
  return character;
}

//---------------------------------------------------------------------------------------------------------------------
Position Game::findRoomById(const std::string room_id) const
{
  Position room_position;
  bool room_found = false;
  int room_map_row_size = rooms_.size();
  int room_map_col_size = rooms_.at(0).size();
  for (int row = 0; row < room_map_row_size; row++)
  {
    for (int col = 0; col < room_map_col_size; col++)
    {
      if (room_id == std::string(1, rooms_.at(row).at(col)->getRoomID()))
      {
        room_position.row_ = row;
        room_position.column_ = col;
        room_found = true;
      }
    }
  }
  if (!room_found)
  {
    throw CommandException(CommandError::SEER_CONFUSED);
  }
  return room_position;
}

//---------------------------------------------------------------------------------------------------------------------
Room *Game::getRoomAt(const Position position) const
{
  int room_map_row_size = rooms_.size();
  int room_map_col_size = rooms_.at(0).size();
  if ((position.row_ >= room_map_row_size) || (position.row_ < 0) || (position.column_ >= room_map_col_size) ||
      (position.column_ < 0))
  {
    throw CommandException(CommandError::ROOM_INVALID);
  }
  return rooms_.at(position.row_).at(position.column_);
}

void  Game::secretUnlock()
{
  doors_unlocked_ = true;
}

bool Game::getSecret()
{
  return doors_unlocked_;
}

Position Game::getStartPosition(Character* charchar)
{
  if(charchar->getCharacterType() == CharacterType::FIGHTER)
    return start_fighter_;
  else if(charchar->getCharacterType() == CharacterType::SEER)
    return start_seer_;
  else
    return start_thief_;
}

void Game::setAIActivated(bool enable)
{
  ai_activated_ = enable;
}