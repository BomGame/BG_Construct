#include "SFML/Graphics.hpp"
#include "iostream"
#include "global.cpp"
#include "GamesParameters.cpp"
#include <cstring>
#include <fstream>
#include "vector"
#include "listdir.h"
#include "generator.cpp"

struct Player
{
  int HP, MP, Lvl, Score, Speed, Slides, Power, Damage;
  std::string Name, Texture;
};
extern struct Player player;
struct Player player;

struct Object
{
  int ID, HP, MP, StartX, StartY, ScaleX, ScaleY;
  bool Quest, Item, Type;
  std::string Texture;
  std::string TextureName;
  sf::Texture objTexture;
};

struct NPC
{
  int HP, Damage, Entity, Gold, XP, Protection, x, y;
  std::string Name, Texture;
};

class Level
{
private:
  int x, y;
  std::string Name;
  int **layer1, **layer2, **layer3;
public:
  Level(int a, int b, std::string c)
  {
    x=a;
    y=b;
    layer1 = new int*[x];
    layer2 = new int*[x];
    layer3 = new int*[x];
    for(int i = 0; i < x; i++)
    {
      layer1[i] = new int[y];
      layer2[i] = new int[y];
      layer3[i] = new int[y];
    }

    for(int i = 0; i < x; i++)
      for(int j = 0; j < y; j++)
      {
        layer1[i][j]=-1;
        layer2[i][j]=-1;
        layer3[i][j]=-1;
      }

      Name = c;
  }

  int Map(int indexX, int indexY, int layer)
  {
    if(layer==1) return layer1[indexY][indexX];
    if(layer==2) return layer2[indexY][indexX];
    if(layer==3) return layer3[indexY][indexX];
    return -1;
  }

  int Width()
  {
    return x;
  }

  int Height()
  {
    return y;
  }

  void NewTile(int indexX, int indexY, int layer, int tile)
  {
    if(layer==1) layer1[indexY][indexX]=tile;
    if(layer==2) layer2[indexY][indexX]=tile;
    if(layer==3) layer3[indexY][indexX]=tile;
  }

  std::string LevelName()
  {
    return Name;
  }
};

std::vector<Level> levelList;
std::vector<Object> objectList;

std::string ProjectName;
