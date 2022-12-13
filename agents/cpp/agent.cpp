#include <stdlib.h>

#include <iostream>
#include <random>
#include <string>

#include "aStar.hpp"
#include "game_state.hpp"
#include "support.hpp"

const std::vector<std::string> _actions = {"up", "left", "right", "down", "bomb", "detonate"};
// TODO
// verificar o vortice
// se inimigo perto, solta bomba
// fugir de bomba
//  parar fugir se estiver longe do inimigo
class Agent {
 private:
  static std::string my_agent_id;
  static std::vector<std::string> my_units;
  static std::string enemy_agent_id;
  static std::vector<std::string> enemy_units;
  static std::vector<Coordinates> map;

  static int tick;

  static void on_game_tick(int tick, const json& game_state);

 public:
  static void run();
};

std::string Agent::my_agent_id;
std::vector<std::string> Agent::my_units;
std::string Agent::enemy_agent_id;
std::vector<std::string> Agent::enemy_units;

vector<Node> caminhoAStar;
AStar estrela;

const int tickCountDownCONST = 5;
int tickCountDown = tickCountDownCONST;
int maxX = 0, maxY = 0;
int Agent::tick;

void Agent::run() {
  const char* connection_string = getenv("GAME_CONNECTION_STRING");

  if (connection_string == NULL) {
    connection_string = "ws://127.0.0.1:3000/?role=agent&agentId=agentId&name=defaultName";
  }
  std::cout << "The current connection_string is: " << connection_string << std::endl;

  GameState::connect(connection_string);
  GameState::set_game_tick_callback(on_game_tick);
  GameState::handle_messages();
}

void Agent::on_game_tick(int tick_nr, const json& game_state) {
  // DEBUG("*************** on_game_tick");
  // DEBUG(game_state.dump());
  // TEST(tick_nr, game_state.dump());
  tick = tick_nr;
  std::cout << "Tick #" << tick << std::endl;
  if (tick == 1) {
    my_agent_id = game_state["connection"]["agent_id"];
    my_units = game_state["agents"][my_agent_id]["unit_ids"].get<std::vector<std::string>>();
    enemy_agent_id = (my_agent_id == "a" ? "b" : "a");
    enemy_units = game_state["agents"][enemy_agent_id]["unit_ids"].get<std::vector<std::string>>();
    // tamanho máximo do jogo
    maxX = game_state["world"]["width"];
    maxY = game_state["world"]["height"];
  }
  
  srand(1234567 * (my_agent_id == "a" ? 1 : 0) + tick * 100 + 13);
  // srand(1234567 * (enemy_agent_id == "a" ? 1 : 0) + tick * 100 + 13);

  // send each (alive) unit a random action
  for (const auto& unit_id : my_units) {
    const json& entities = game_state["entities"];
    const json& unit = game_state["unit_state"][unit_id];
    // como o desafio só terá um inimigo, então só pego o primeiro inimigo
    const json& enemy = game_state["unit_state"][enemy_units[0]];
    //action é a ação do agent
    std::string action;
    //passo é o próximo passo do agent
    Node passo;
    
    Support support;
    Node agent_node;
    agent_node.coordinates.x = unit["coordinates"][0];
    agent_node.coordinates.y = unit["coordinates"][1];
    Node enemy_node;
    enemy_node.coordinates.x = enemy["coordinates"][0];
    enemy_node.coordinates.y = enemy["coordinates"][1];
    Node seek_node;

    // verificando se o inimigo esta perto
    if (support.manhattanDistance(agent_node.coordinates, enemy_node.coordinates) > 3) {
      // seta o caminho de fuga
      Coordinates seek_coordianates;
      seek_coordianates.x = agent_node.coordinates.x - (enemy_node.coordinates.x - agent_node.coordinates.x);
      seek_coordianates.y = agent_node.coordinates.y - (enemy_node.coordinates.y - agent_node.coordinates.y);

      seek_node.coordinates = support.getFreeCoordinates(seek_coordianates, maxX, maxY, entities);
      std::cout << seek_coordianates.x << " : " << seek_coordianates.y << std::endl;
      std::cout << seek_node.coordinates.x << " : " << seek_node.coordinates.y << std::endl;
      std::cout << agent_node.coordinates.x << " : " << agent_node.coordinates.y << std::endl;
      //calcula o caminho com o a estrela a cada 5 ticks
      if (tickCountDown == 5) {
        caminhoAStar = estrela.aStar(agent_node, seek_node, maxX, maxY, entities);
        
        std::cout << " Tick " << std::endl;
      }
      //seta o passo para o próximo passo no caminho A*
      if (caminhoAStar.size() > 0) {
        std::cout << " Passo " << std::endl;
        passo = caminhoAStar.back();
        caminhoAStar.pop_back();
      }
    } else {//se o inimigo estiver perto, lança bomba
      action = "bomb";
      
    }

    // calculando caminho A*
    

    if (unit["hp"] <= 0) continue;
    // escolha aleatoria
    //  std::string action = _actions[rand() % _actions.size()];
    
    // testa se precisa se movimentar
    if (passo.coordinates.x != MAX_INT && passo.coordinates.y != MAX_INT) {
      cout << "agent " << agent_node.coordinates.x << " : " << agent_node.coordinates.y << endl;
      cout << "passo " << passo.coordinates.x << " : " << passo.coordinates.y << endl;
      if (passo.coordinates.x > agent_node.coordinates.x) {
        action = "right";
      } else if (passo.coordinates.x < agent_node.coordinates.x) {
        action = "left";
      } else if (passo.coordinates.y > agent_node.coordinates.y) {
        action = "up";
      } else if (passo.coordinates.y < agent_node.coordinates.y) {
        action = "down";
      }
    }
    std::cout << "action: " << unit_id << " -> " << action << std::endl;

    // envio do comando para o jogo
    if (action == "up" || action == "left" || action == "right" || action == "down") {
      GameState::send_move(action, unit_id);
    } else if (action == "bomb") {
      GameState::send_bomb(unit_id);
    } else if (action == "detonate") {
      for (const auto& entity : entities) {
        if (entity["type"] == "b" && entity["unit_id"] == unit_id) {
          int x = entity["x"], y = entity["y"];
          GameState::send_detonate(x, y, unit_id);
          break;
        }
      }
    } else {
      std::cerr << "Unhandled action: " << action << " for unit " << unit_id << std::endl;
    }
  }
  if (tickCountDown == 0) {
    tickCountDown = tickCountDownCONST;
  } else {
    --tickCountDown;
  }
}

int main() {
  Agent::run();
}
