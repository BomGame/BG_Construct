#include "SFML/Graphics.hpp"
#include "iostream"
#include "global.cpp"
#include "GamesParameters.cpp"
#include <cstring>
#include <fstream>
#include "vector"
#include "listdir.h"
#include "generator.cpp"


//шапка
class _Head
{
private:
    sf::Vector2i cords;
    sf::Vector2f BTSize;

public:
  _Head()
  {
    flag = 0;
    BTSize = sf::Vector2f(HEIGHT/6, WIDTH/30);
  }

  void Update()
  {
    //чтение координат указателя
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
      cords=sf::Mouse::getPosition(window);

    //обработка координат указателя
    if(cords.x>0 && cords.x<BTSize.x && cords.y>0 && cords.y<BTSize.y)
      flag=0;
    if(cords.x>BTSize.x+1 && cords.x<BTSize.x*2 && cords.y>0 && cords.y<BTSize.y)
    {
      flag=1;
      SearchFiles=false;
      cords.x=-1;
      cords.y=-1;
    }
    if(cords.x>2+BTSize.x*2 && cords.x<2+BTSize.x*3 && cords.y>0 && cords.y<BTSize.y)
      flag=2;
    if(cords.x>3+BTSize.x*3 && cords.x<2+BTSize.x*4 && cords.y>0 && cords.y<BTSize.y)
      flag=3;
    if(cords.x>HEIGHT-BTSize.x && cords.x<HEIGHT && cords.y>0 && cords.y<BTSize.y)
    {
      flag=4;
      //generationFlag=true;
      cords.x=-1;
      cords.y=-1;
    }
  }

  void Draw()
  {
    sf::RectangleShape BTOptions;
    BTOptions.setSize(BTSize);
    BTOptions.move(0, 0);
    BTOptions.setFillColor(color);
    if(flag == 0)
        BTOptions.setFillColor(ActiveColor);
    text.setString(L"Настройки");
    text.setPosition(5+0,0);
    window.draw(BTOptions);
    window.draw(text);


    sf::RectangleShape BTAddObj;
    BTAddObj.setSize(BTSize);
    BTAddObj.move(1+BTSize.x, 0);
    BTAddObj.setFillColor(color);
    if(flag == 1)
        BTAddObj.setFillColor(ActiveColor);
    text.setString(L"Добавить объект");
    text.setPosition(6+BTSize.x,0);
    window.draw(BTAddObj);
    window.draw(text);

    sf::RectangleShape BTAddNPC;
    BTAddNPC.setSize(BTSize);
    BTAddNPC.move(2+BTSize.x*2, 0);
    BTAddNPC.setFillColor(color);
    if(flag == 2)
        BTAddNPC.setFillColor(ActiveColor);
    text.setString(L"Добавить персонажа");
    text.setPosition(7+BTSize.x*2,0);
    window.draw(BTAddNPC);
    window.draw(text);

    sf::RectangleShape BTMap;
    BTMap.setSize(BTSize);
    BTMap.move(3+BTSize.x*3, 0);
    BTMap.setFillColor(color);
    if(flag == 3)
        BTMap.setFillColor(ActiveColor);
    text.setString(L"Редактор карты");
    text.setPosition(8+BTSize.x*3,0);
    window.draw(BTMap);
    window.draw(text);

    sf::RectangleShape BTCompile;
    BTCompile.setSize(BTSize);
    BTCompile.move(HEIGHT-BTSize.x, 0);
    BTCompile.setFillColor(color);
    text.setString(L"Собрать проект");
    text.setPosition(HEIGHT-BTSize.x+5,0);
    window.draw(BTCompile);
    window.draw(text);
  }
};

//вкладка "Настройки"
class _Options
{
private:
  sf::Vector2i cords;
  int x, y;//начальные координаты поля
  sf::Vector2f spriteSize, buttonSize;
  bool click;
  int layer;
  bool flagName, flagAge, flagSpeed, flagPower, flagSlides, flagNewRound;
  bool flagNameLevel, flagHeight, flagWidth, flagLevelList, flagObjectsList;
  bool flagTexturePlayer;
  char symbol;
  int tempX, tempY;//для записи уровня
  std::string tempName;//для записи уровня
  int moveX, moveY;//смещение по редактору карты
  int matrixHeight, matrixWidth;//размеры текущего уровня
  bool clickLeft, clickRight, clickUp, clickDown;
  int nowLevel;
  std::string nowObject; //адрес выбранной текстуры
  int nowObjectIndex; //индекс выбранного объекта
  sf::Texture textureSprite, textureSpriteNow, textureSpritePlayer;
  sf:: Vector2u vectorPlayer; //Размеры текстуры игрока
  sf::IntRect playerRect;

public:
  _Options()
  {
    layer=1;
    x=HEIGHT/20;
    y=WIDTH/10+WIDTH/30;
    spriteSize=sf::Vector2f(HEIGHT/27, HEIGHT/27);
    buttonSize=sf::Vector2f(HEIGHT/36, HEIGHT/36);
    click=false;
    flagName=false;
    flagAge=false;
    flagSpeed=false;
    flagPower=false;
    flagSlides=false;
    flagNewRound=false;
    flagNameLevel=false;
    flagHeight=false;
    flagWidth=false;
    flagLevelList=false;
    flagObjectsList=false;
    symbol='\0';
    tempX=0;
    tempY=0;
    tempName="";
    nowObject="";
    moveX=0;
    moveY=0;
    matrixHeight=12;
    matrixWidth=13;
    clickLeft=false;
    clickRight=false;
    clickUp=false;
    clickDown=false;
    nowLevel=0;
    nowObjectIndex=-1;
    flagTexturePlayer=false;
    player.Texture="";
  }

  void Update()
  {
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click)
      click=true;
    if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && click)
    {
      cords=sf::Mouse::getPosition(window);

      //переход по слоям
      if(cords.x>HEIGHT/20+20+buttonSize.x*4 && cords.x<HEIGHT/20+20+buttonSize.x*4+buttonSize.x*2
        &&cords.y>WIDTH-5-spriteSize.x*2 && cords.y<WIDTH-5-spriteSize.x*2+buttonSize.y)
        layer=1;

      if(cords.x>HEIGHT/20+20+buttonSize.x*6+5 && cords.x<HEIGHT/20+20+buttonSize.x*6+5+buttonSize.x*2
        &&cords.y>WIDTH-5-spriteSize.x*2 && cords.y<WIDTH-5-spriteSize.x*2+buttonSize.y)
        layer=2;

      if(cords.x>HEIGHT/20+20+buttonSize.x*8+10 && cords.x<HEIGHT/20+20+buttonSize.x*8+10+buttonSize.x*2
        &&cords.y>WIDTH-5-spriteSize.x*2 && cords.y<WIDTH-5-spriteSize.x*2+buttonSize.y)
        layer=3;

      //ввод имени
      if(cords.x>HEIGHT/6*4+140 && cords.y>spriteSize.x*2 &&
        cords.x<HEIGHT/6*4+140+buttonSize.x*5 && cords.y<spriteSize.x*2+buttonSize.y)
        flagName=true;
      else
        flagName=false;

      //ввод здоровья
      if(cords.x>HEIGHT/6*4+140 && cords.x<HEIGHT/6*4+140+buttonSize.x*5/2 &&
         cords.y>spriteSize.x*3 && cords.y<spriteSize.x*3+buttonSize.y)
         flagAge=true;
      else
        flagAge=false;

      //ввод скорости
      if(cords.x>HEIGHT/6*4+buttonSize.x*5+10 && cords.x<HEIGHT/6*4+buttonSize.x*5+10+buttonSize.x*5/2 &&
         cords.y>spriteSize.x*2+140 && cords.y<spriteSize.x*2+140+buttonSize.y)
         flagSpeed=true;
      else
        flagSpeed=false;

      //ввод скорости
      if(cords.x>HEIGHT/6*4 && cords.x<HEIGHT/6*4+buttonSize.x*5/2 &&
         cords.y>spriteSize.x*3+140 && cords.y<spriteSize.x*3+140+buttonSize.y)
         flagPower=true;
      else
        flagPower=false;

      //ввод слайдов
      if(cords.x>HEIGHT/6*4+buttonSize.x*5+10 && cords.x<HEIGHT/6*4+buttonSize.x*5+10+buttonSize.x*5/2 &&
         cords.y>spriteSize.x*3+140 && cords.y<spriteSize.x*3+140+buttonSize.y)
         flagSlides=true;
      else
        flagSlides=false;

      //новый уровень
      if(cords.x>HEIGHT/2-spriteSize.x && cords.x<HEIGHT/2-spriteSize.x+10+buttonSize.x*5 &&
         cords.y>spriteSize.x*5 && cords.y<spriteSize.x*5+buttonSize.y)
      {
        flagNewRound=!flagNewRound;
        flagLevelList=false;
        flagObjectsList=false;
      }

      //название нового уровня
      if(cords.x>HEIGHT/2-spriteSize.x && cords.x<HEIGHT/2-spriteSize.x+buttonSize.x*5 &&
         cords.y>spriteSize.x*7 && cords.y<spriteSize.x*7+buttonSize.y && flagNewRound)
         flagNameLevel=true;
      else
          flagNameLevel=false;

      //ширина и высота нового уровня
      if(cords.x>HEIGHT/2-spriteSize.x && cords.x<HEIGHT/2-spriteSize.x+buttonSize.x*2 &&
         cords.y>spriteSize.x*8 && cords.y<spriteSize.x*8+buttonSize.y && flagNewRound)
         flagHeight=true;
      else
          flagHeight=false;

      if(cords.x>HEIGHT/2+buttonSize.x*3 && cords.x<HEIGHT/2+buttonSize.x*3+buttonSize.x*2 &&
         cords.y>spriteSize.x*8 && cords.y<spriteSize.x*8+buttonSize.y && flagNewRound)
         flagWidth=true;
      else
          flagWidth=false;

      //создание файла с уровнем
      if(cords.x>HEIGHT/2-spriteSize.x && cords.x<HEIGHT/2-spriteSize.x+buttonSize.x*2 &&
         cords.y>spriteSize.x*9 && cords.y<spriteSize.x*9+buttonSize.y && flagNewRound)
      {
        flagNewRound=false;

        levelList.push_back(Level(tempX, tempY, tempName));

        std::ofstream File ("./"+Directory+"/levels/"+tempName+".dat");
        File << tempX << " " << tempY;
        symbol='\0';
        tempX=0;
        tempY=0;
        tempName="";
        File.close();
        nowLevel=levelList.size()-1;
        matrixHeight=levelList[nowLevel].Height();
        matrixWidth=levelList[nowLevel].Width();
      }

      //сдвиги матрицы
      //лево
      if(cords.x>HEIGHT/20 && cords.x<HEIGHT/20+buttonSize.x &&
         cords.y>WIDTH-5-spriteSize.x*2 && cords.y<WIDTH-5-spriteSize.x*2+buttonSize.y && !clickLeft)
      {
        clickLeft=true;
      }
      if(clickLeft)
      {
        clickLeft=false;
        moveX--;
        if(moveX<0)moveX=0;
      }

      //право
      if(cords.x>HEIGHT/20+5+buttonSize.x && cords.x<HEIGHT/20+5+buttonSize.x+buttonSize.x &&
         cords.y>WIDTH-5-spriteSize.x*2 && cords.y<WIDTH-5-spriteSize.x*2+buttonSize.y && !clickRight)
      {
        clickRight=true;
      }
      if(clickRight)
      {
        clickRight=false;
        moveX++;
        if((moveX+10)>matrixHeight)moveX=matrixHeight-10;
      }

      //верх
      if(cords.x>HEIGHT/20+10+buttonSize.x*2 && cords.x<HEIGHT/20+10+buttonSize.x*2+buttonSize.x &&
         cords.y>WIDTH-5-spriteSize.x*2 && cords.y<WIDTH-5-spriteSize.x*2+buttonSize.y && !clickUp)
      {
        clickUp=true;
      }
      if(clickUp)
      {
        clickUp=false;
        moveY--;
        if(moveY<0)moveY=0;
      }

      //низ
      if(cords.x>HEIGHT/20+15+buttonSize.x*3 && cords.x<HEIGHT/20+15+buttonSize.x*3+buttonSize.x &&
         cords.y>WIDTH-5-spriteSize.x*2 && cords.y<WIDTH-5-spriteSize.x*2+buttonSize.y && !clickDown)
      {
        clickDown=true;
      }
      if(clickDown)
      {
        clickDown=false;
        moveY++;
        if((moveY+10)>matrixWidth)moveY=matrixWidth-10;
      }

      //поле
      for(int i=0; i<10; i++)
      {
        for(int j=0; j<10; j++)
        {
          if(levelList.size()>0)
            if(cords.x>(x+(spriteSize.x+1)*i) && cords.x<(x+(spriteSize.x+1)*i+spriteSize.x) &&
            cords.y>(y+(spriteSize.x+1)*j) && cords.y<(y+(spriteSize.x+1)*j+spriteSize.y))
              levelList[nowLevel].NewTile(i+moveX, j+moveY, layer, nowObjectIndex);
        }
      }

      //список уровней
      if(cords.x>HEIGHT/2-spriteSize.x && cords.x<HEIGHT/2-spriteSize.x+buttonSize.x*5 &&
         cords.y>spriteSize.x*4 && cords.y<spriteSize.x*4+buttonSize.y)
      {
        flagLevelList=!flagLevelList;
        flagNewRound=false;
        flagObjectsList=false;
      }

      //выбор уровня
      if(flagLevelList)
      {
        for(int i=0; i<levelList.size(); i++)
        {
          if(cords.x>HEIGHT/2-spriteSize.x && cords.x<HEIGHT/2-spriteSize.x+buttonSize.x*5 &&
             cords.y>(spriteSize.x*7+(buttonSize.y+5)*i) && cords.y<(spriteSize.x*7+(buttonSize.y+5)*i)+buttonSize.y)
          {
            nowLevel=i;
            flagLevelList=false;
          }
        }
      }

      //список объектов
      if(cords.x>HEIGHT/2-spriteSize.x && cords.x<HEIGHT/2-spriteSize.x+buttonSize.x*5 &&
        cords.y>spriteSize.x*3 && cords.y<spriteSize.x*3+buttonSize.y)
      {
        flagObjectsList=!flagObjectsList;
        flagNewRound=false;
        flagLevelList=false;
      }

      //выбор объекта
      if(flagObjectsList)
      {
        for (size_t i = 0; i < objectList.size(); i++)
        {
          if(cords.x>HEIGHT/2-spriteSize.x && cords.x<HEIGHT/2-spriteSize.x+spriteSize.x &&
            cords.y>spriteSize.x*7+(spriteSize.y+1)*i && cords.y<spriteSize.x*7+(spriteSize.y+1)*i+spriteSize.y)
          {
            if(objectList[i].Texture!="")
            {
              nowObject=objectList[i].Texture;
              nowObjectIndex=objectList[i].ID;
              flagObjectsList=false;
              textureSpriteNow.loadFromFile(nowObject);
            }
            else
            {
              nowObjectIndex=-1;
              nowObject="";
            }
          }
        }
      }

      //текстура игрока
      if(cords.x>HEIGHT/6*4 && cords.x<HEIGHT/6*4+HEIGHT/15 &&
        cords.y>spriteSize.x*2 && cords.y<spriteSize.x*2+HEIGHT/15)
      {
        flagTexturePlayer=!flagTexturePlayer;
      }

      //выбор текстуры
      if(flagTexturePlayer)
      {
        for(size_t i=1; i<7; i++)
        {
          if(cords.x>HEIGHT/2-spriteSize.x && cords.x<HEIGHT/2-spriteSize.x+buttonSize.x*5 &&
            cords.y>spriteSize.x*7+(buttonSize.y+5)*i && cords.y<spriteSize.x*7+(buttonSize.y+5)*i+buttonSize.y)
          {
            player.Texture=TextureFiles[i-1];
            textureSpritePlayer.loadFromFile("./"+ProjectName+"/textures/"+player.Texture);
            vectorPlayer=textureSpritePlayer.getSize();
            vectorPlayer.y=vectorPlayer.y/4;
            if(player.Slides!=0) vectorPlayer.x=vectorPlayer.x/player.Slides;
            playerRect=(sf::IntRect(0,0,vectorPlayer.x, vectorPlayer.y));
            flagTexturePlayer=false;
          }
        }
      }

      click=false;
    }

    //чтение текстур из каталога текстур
    if(flagTexturePlayer)
    {
      std::string path = "./"+ProjectName+"/textures";
      list_dir(path.c_str(), TextureFiles);
    }

    //ввод имени
    if(event.type == sf::Event::TextEntered && flagName)
    {
      symbol=writeText();
    }
    if(symbol!= '\0' && event.type != sf::Event::TextEntered && flagName)
    {
      if(symbol == 8)
      {
        if(player.Name.length()>0)
          player.Name.pop_back();
      }
      else
      {
        player.Name += symbol;
      }
      symbol='\0';
    }

    //ввод здоровья
    if(event.type == sf::Event::TextEntered && flagAge)
    {
      symbol=writeText();
    }
    if(symbol!= '\0' && event.type != sf::Event::TextEntered && flagAge)
    {
      if(symbol == 8)
      {
        player.HP /= 10;
      }
      else
      {
        player.HP=player.HP*10+((int)symbol-48);
      }
      symbol='\0';
    }

    //ввод скорости
    if(event.type == sf::Event::TextEntered && flagSpeed)
    {
      symbol=writeText();
    }
    if(symbol!= '\0' && event.type != sf::Event::TextEntered && flagSpeed)
    {
      if(symbol == 8)
      {
        player.Speed /= 10;
      }
      else
      {
        player.Speed=player.Speed*10+((int)symbol-48);
      }
      symbol='\0';
    }

    //ввод силы
    if(event.type == sf::Event::TextEntered && flagPower)
    {
      symbol=writeText();
    }
    if(symbol!= '\0' && event.type != sf::Event::TextEntered && flagPower)
    {
      if(symbol == 8)
      {
        player.Power /= 10;
      }
      else
      {
        player.Power=player.Power*10+((int)symbol-48);
      }
      symbol='\0';
    }

    //ввод слайдов
    if(event.type == sf::Event::TextEntered && flagSlides)
    {
      symbol=writeText();
    }
    if(symbol!= '\0' && event.type != sf::Event::TextEntered && flagSlides)
    {
      if(symbol == 8)
      {
        player.Slides /= 10;
      }
      else
      {
        player.Slides=player.Slides*10+((int)symbol-48);
      }
      symbol='\0';
    }

    //ввод названия нового уровня
    if(event.type == sf::Event::TextEntered && flagNameLevel)
    {
        symbol=writeText();
    }
    if(symbol!= '\0' && event.type != sf::Event::TextEntered && flagNameLevel)
    {
      if(symbol == 8)
      {
        if(tempName.length()>0)
          tempName.pop_back();
      }
      else
      {
        tempName += symbol;
      }
      symbol='\0';
    }

    //ввод ширины и высоты
    if(event.type == sf::Event::TextEntered && flagHeight)
    {
      symbol=writeText();
    }
    if(symbol!= '\0' && event.type != sf::Event::TextEntered && flagHeight)
    {
      if(symbol == 8)
      {
        tempX /= 10;
      }
      else
      {
        tempX=tempX*10+((int)symbol-48);
      }
      symbol='\0';
    }

    if(event.type == sf::Event::TextEntered && flagWidth)
    {
      symbol=writeText();
    }
    if(symbol!= '\0' && event.type != sf::Event::TextEntered && flagWidth)
    {
      if(symbol == 8)
      {
        tempY /= 10;
      }
      else
      {
        tempY=tempY*10+((int)symbol-48);
      }
      symbol='\0';
    }
  }

  void Draw()
  {
    sf::RectangleShape Name;
    Name.setSize(sf::Vector2f(HEIGHT/6, WIDTH/30));
    Name.setPosition(HEIGHT/20, WIDTH/10);
    Name.setFillColor(color);
    text.setString(L"Текущий уровень");
    text.setPosition(HEIGHT/20+5, WIDTH/10);
    window.draw(Name);
    window.draw(text);

    //поле
    for(int l=1; l<=layer; l++)
      for(int i=0; i<10; i++)
      {
        for(int j=0; j<10; j++)
        {
          sf::RectangleShape sprite;
          sprite.setSize(spriteSize);
          if(levelList.size()>0)
          {
              if(levelList[nowLevel].Map(i+moveX, j+moveY, l)==-1 && l==1)
              {
                sprite.setFillColor(sf::Color(153, 153, 153));
                sprite.setPosition(x+(spriteSize.x+1)*i, y+(spriteSize.x+1)*j);
                window.draw(sprite);
              }
              else
                for (size_t k = 0; k < objectList.size(); k++)
                {
                  if(levelList[nowLevel].Map(i+moveX, j+moveY, l)==objectList[k].ID)
                  {
                    sprite.setTexture(&objectList[k].objTexture);
                    sprite.setPosition(x+(spriteSize.x+1)*i, y+(spriteSize.x+1)*j);
                    window.draw(sprite);
                    break;
                  }
                }
          }
          if(levelList.size()==0)
          {
            sprite.setPosition(x+(spriteSize.x+1)*i, y+(spriteSize.x+1)*j);
            window.draw(sprite);
          }
        }
      }
    sf::RectangleShape button;
    button.setSize(buttonSize);
    button.setFillColor(color);
    //лево
    button.setPosition(HEIGHT/20, WIDTH-5-spriteSize.x*2);
    window.draw(button);
    text.setString(L"Left");
    text.setPosition(HEIGHT/20+5, WIDTH-5-spriteSize.x*2);
    window.draw(text);
    //право
    button.setPosition(HEIGHT/20+5+buttonSize.x, WIDTH-5-spriteSize.x*2);
    window.draw(button);
    text.setString(L"Right");
    text.setPosition(HEIGHT/20+5+buttonSize.x, WIDTH-5-spriteSize.x*2);
    window.draw(text);
    //верх
    button.setPosition(HEIGHT/20+10+buttonSize.x*2, WIDTH-5-spriteSize.x*2);
    window.draw(button);
    text.setString(L"Up");
    text.setPosition(HEIGHT/20+10+buttonSize.x*2, WIDTH-5-spriteSize.x*2);
    window.draw(text);
    //низ
    button.setPosition(HEIGHT/20+15+buttonSize.x*3, WIDTH-5-spriteSize.x*2);
    window.draw(button);
    text.setString(L"Down");
    text.setPosition(HEIGHT/20+15+buttonSize.x*3, WIDTH-5-spriteSize.x*2);
    window.draw(text);
    //слой 1
    button.setSize(sf::Vector2f(buttonSize.x*2, buttonSize.y));
    button.setFillColor(color);
    if(layer==1)
      button.setFillColor(ActiveColor);
    text.setString(L"Слой 1");
    text.setPosition(HEIGHT/20+20+buttonSize.x*4+10, WIDTH-5-spriteSize.x*2);
    button.setPosition(HEIGHT/20+20+buttonSize.x*4, WIDTH-5-spriteSize.x*2);
    window.draw(button);
    window.draw(text);
    //слой 2
    button.setSize(sf::Vector2f(buttonSize.x*2, buttonSize.y));
    button.setFillColor(color);
    if(layer==2)
      button.setFillColor(ActiveColor);
    text.setString(L"Слой 2");
    text.setPosition(HEIGHT/20+20+buttonSize.x*6+15, WIDTH-5-spriteSize.x*2);
    button.setPosition(HEIGHT/20+20+buttonSize.x*6+5, WIDTH-5-spriteSize.x*2);
    window.draw(button);
    window.draw(text);
    //слой 3
    button.setSize(sf::Vector2f(buttonSize.x*2, buttonSize.y));
    button.setFillColor(color);
    if(layer==3)
      button.setFillColor(ActiveColor);
    text.setString(L"Слой 3");
    text.setPosition(HEIGHT/20+20+buttonSize.x*8+20, WIDTH-5-spriteSize.x*2);
    button.setPosition(HEIGHT/20+20+buttonSize.x*8+10, WIDTH-5-spriteSize.x*2);
    window.draw(button);
    window.draw(text);
    //жанр
    button.setSize(sf::Vector2f(buttonSize.x*2, buttonSize.y));
    button.setFillColor(color);
    text.setString(L"Жанр");
    text.setPosition(HEIGHT/20+20+buttonSize.x*10+25, WIDTH-5-spriteSize.x*2);
    button.setPosition(HEIGHT/20+20+buttonSize.x*10+15, WIDTH-5-spriteSize.x*2);
    window.draw(button);
    window.draw(text);
    //список персонажей
    button.setSize(sf::Vector2f(buttonSize.x*5, buttonSize.y));
    button.setFillColor(color);
    text.setString(L"Список персонажей");
    text.setPosition(HEIGHT/2-spriteSize.x+5, spriteSize.x*2);
    button.setPosition(HEIGHT/2-spriteSize.x, spriteSize.x*2);
    window.draw(button);
    window.draw(text);
    //список объектов
    button.setSize(sf::Vector2f(buttonSize.x*5, buttonSize.y));
    if(flagObjectsList) button.setFillColor(ActiveColor); else button.setFillColor(color);
    text.setString(L"Список объектов");
    text.setPosition(HEIGHT/2-spriteSize.x+5, spriteSize.x*3);
    button.setPosition(HEIGHT/2-spriteSize.x, spriteSize.x*3);
    window.draw(button);
    window.draw(text);
    //список уровней
    button.setSize(sf::Vector2f(buttonSize.x*5, buttonSize.y));
    if(flagLevelList)button.setFillColor(ActiveColor); else button.setFillColor(color);
    text.setString(L"Список уровней");
    text.setPosition(HEIGHT/2-spriteSize.x+5, spriteSize.x*4);
    button.setPosition(HEIGHT/2-spriteSize.x, spriteSize.x*4);
    window.draw(button);
    window.draw(text);
    //Добавить уровень
    button.setSize(sf::Vector2f(buttonSize.x*5, buttonSize.y));
    if(flagNewRound) button.setFillColor(ActiveColor); else button.setFillColor(color);
    text.setString(L"Добавить уровень");
    text.setPosition(HEIGHT/2-spriteSize.x+5, spriteSize.x*5);
    button.setPosition(HEIGHT/2-spriteSize.x, spriteSize.x*5);
    window.draw(button);
    window.draw(text);
    //блок настройки персонажа
    //иконка текстуры
    sf::RectangleShape texture;
    texture.setSize(sf::Vector2f(HEIGHT/15,HEIGHT/15));
    texture.setPosition(HEIGHT/6*4,spriteSize.x*2);
    text.setString(L"Текстура");
    text.setPosition(HEIGHT/6*4,spriteSize.x*2-spriteSize.x/2);
    if(player.Texture!="")
    {
      texture.setTexture(&textureSpritePlayer);
      texture.setTextureRect(playerRect);
    }
    window.draw(texture);
    window.draw(text);
    //имя персонажа
    button.setSize(sf::Vector2f(buttonSize.x*5, buttonSize.y));
    if(flagName)button.setFillColor(ActiveColor);
    else button.setFillColor(color);
    text.setString(L"Имя\nперсонажа");
    text.setPosition(HEIGHT/6*4+HEIGHT/13+5+buttonSize.x*5,spriteSize.x*2);
    window.draw(text);
    text.setString(player.Name);
    text.setPosition(HEIGHT/6*4+HEIGHT/13+5,spriteSize.x*2);
    button.setPosition(HEIGHT/6*4+HEIGHT/13,spriteSize.x*2);
    window.draw(button);
    window.draw(text);
    //здоровье
    button.setSize(sf::Vector2f(buttonSize.x*5/2, buttonSize.y));
    if(flagAge) button.setFillColor(ActiveColor);
    else button.setFillColor(color);
    text.setString(L"Здоровье\nперсонажа");
    text.setPosition(HEIGHT/6*4+HEIGHT/13+5+buttonSize.x*5/2,spriteSize.x*3);
    window.draw(text);
    text.setString(std::to_string(player.HP));
    text.setPosition(HEIGHT/6*4+HEIGHT/13+5,spriteSize.x*3);
    button.setPosition(HEIGHT/6*4+HEIGHT/13,spriteSize.x*3);
    window.draw(button);
    window.draw(text);
    //настройки
    button.setSize(sf::Vector2f(buttonSize.x*5, buttonSize.y));
    button.setFillColor(color);
    text.setString(L"Полные настройки");
    text.setPosition(HEIGHT/6*4+5,spriteSize.x*2+HEIGHT/13);
    button.setPosition(HEIGHT/6*4,spriteSize.x*2+HEIGHT/13);
    window.draw(button);
    window.draw(text);
    //скорость
    button.setSize(sf::Vector2f(buttonSize.x*5/2, buttonSize.y));
    if(flagSpeed)button.setFillColor(ActiveColor);
    else button.setFillColor(color);
    text.setString(L"Скорость\nперсонажа");
    text.setPosition(HEIGHT/6*4+15+buttonSize.x*5+buttonSize.x*5/2,spriteSize.x*2+HEIGHT/13);
    window.draw(text);
    text.setString(std::to_string(player.Speed));
    text.setPosition(HEIGHT/6*4+buttonSize.x*5+15,spriteSize.x*2+HEIGHT/13);
    button.setPosition(HEIGHT/6*4+buttonSize.x*5+10,spriteSize.x*2+HEIGHT/13);
    window.draw(button);
    window.draw(text);
    //Очки силы
    button.setSize(sf::Vector2f(buttonSize.x*5/2, buttonSize.y));
    if(flagPower) button.setFillColor(ActiveColor);
    else button.setFillColor(color);
    text.setString(L"Сила\nперсонажа");
    text.setPosition(HEIGHT/6*4+buttonSize.x*5/2+5,spriteSize.x*3+HEIGHT/13);
    window.draw(text);
    text.setString(std::to_string(player.Power));
    text.setPosition(HEIGHT/6*4+5,spriteSize.x*3+HEIGHT/13);
    button.setPosition(HEIGHT/6*4,spriteSize.x*3+HEIGHT/13);
    window.draw(button);
    window.draw(text);
    //количество слайдов
    button.setSize(sf::Vector2f(buttonSize.x*5/2, buttonSize.y));
    if(flagSlides)button.setFillColor(ActiveColor);
    else button.setFillColor(color);
    text.setString(L"Количество\nслайдов");
    text.setPosition(HEIGHT/6*4+15+buttonSize.x*5+buttonSize.x*5/2,spriteSize.x*3+HEIGHT/13);
    window.draw(text);
    text.setString(std::to_string(player.Slides));
    text.setPosition(HEIGHT/6*4+buttonSize.x*5+15,spriteSize.x*3+HEIGHT/13);
    button.setPosition(HEIGHT/6*4+buttonSize.x*5+10,spriteSize.x*3+HEIGHT/13);
    window.draw(button);
    window.draw(text);
    //индикатор выбраного
    sf::RectangleShape texture2;
    texture2.setSize(sf::Vector2f(HEIGHT/15,HEIGHT/15));
    texture2.setPosition(HEIGHT-HEIGHT/13,WIDTH-WIDTH/7);
    text.setString(L"Выбрано");
    text.setPosition(HEIGHT-HEIGHT/13,WIDTH-WIDTH/7-fontSize-5);
    if(nowObject!="")
    {
      texture2.setTexture(&textureSpriteNow);
    }
    window.draw(texture2);
    window.draw(text);
    //форма создания уровня
    if(flagNewRound)
    {
      //название уровня
      button.setSize(sf::Vector2f(buttonSize.x*5, buttonSize.y));
      if(flagNameLevel) button.setFillColor(ActiveColor);
      else button.setFillColor(color);
      text.setString(L"Название");
      text.setPosition(HEIGHT/2-spriteSize.x, spriteSize.x*7-fontSize-5);
      window.draw(text);
      text.setString(tempName);
      text.setPosition(HEIGHT/2-spriteSize.x+5, spriteSize.x*7);
      button.setPosition(HEIGHT/2-spriteSize.x, spriteSize.x*7);
      window.draw(button);
      window.draw(text);
      //размеры уровня
      //ширина
      button.setSize(sf::Vector2f(buttonSize.x*2, buttonSize.y));
      if(flagHeight)button.setFillColor(ActiveColor);
      else button.setFillColor(color);
      text.setString(L"Ширина");
      text.setPosition(HEIGHT/2-spriteSize.x+buttonSize.x*2+5, spriteSize.x*8);
      window.draw(text);
      text.setString(std::to_string(tempX));
      text.setPosition(HEIGHT/2-spriteSize.x+5, spriteSize.x*8);
      button.setPosition(HEIGHT/2-spriteSize.x, spriteSize.x*8);
      window.draw(button);
      window.draw(text);
      //высота
      button.setSize(sf::Vector2f(buttonSize.x*2, buttonSize.y));
      if(flagWidth)button.setFillColor(ActiveColor);
      else button.setFillColor(color);
      text.setString(L"Высота");
      text.setPosition(HEIGHT/2+buttonSize.x*5+5, spriteSize.x*8);
      window.draw(text);
      text.setString(std::to_string(tempY));
      text.setPosition(HEIGHT/2+buttonSize.x*3+5, spriteSize.x*8);
      button.setPosition(HEIGHT/2+buttonSize.x*3, spriteSize.x*8);
      window.draw(button);
      window.draw(text);
      //ОК
      button.setSize(sf::Vector2f(buttonSize.x*2, buttonSize.y));
      button.setFillColor(color);
      text.setString(L"ОК");
      text.setPosition(HEIGHT/2-spriteSize.x+(buttonSize.x*2)/2-(fontSize)/2, spriteSize.x*9+(buttonSize.y)/2-(fontSize)/1.5);
      button.setPosition(HEIGHT/2-spriteSize.x, spriteSize.x*9);
      window.draw(button);
      window.draw(text);
    }
    //форма списка уровней
    if(flagLevelList)
    {
      for(int i=0; i<levelList.size(); i++)
      {
        button.setSize(sf::Vector2f(buttonSize.x*5, buttonSize.y));
        button.setFillColor(color);
        text.setString(levelList[i].LevelName());
        text.setPosition(HEIGHT/2-spriteSize.x+5, spriteSize.x*7+(buttonSize.y+5)*i);
        button.setPosition(HEIGHT/2-spriteSize.x, spriteSize.x*7+(buttonSize.y+5)*i);
        window.draw(button);
        window.draw(text);
      }
    }
    //форма списка объектов
    if(flagObjectsList)
    {
      for (size_t i = 0; i < objectList.size(); i++)
      {
        button.setSize(spriteSize);
        button.setTexture(&objectList[i].objTexture);
        button.setPosition(HEIGHT/2-spriteSize.x, spriteSize.x*7+(spriteSize.y+1)*i);
        window.draw(button);
        window.draw(text);
      }
    }
    //форма выбора текстуры
    if(flagTexturePlayer)
    {
      for(size_t i=0; i<8; i++)
      {
        button.setSize(sf::Vector2f(buttonSize.x*5, buttonSize.y));
        button.setFillColor(color);
        if(i-1<TextureFiles.size() && i>0) text.setString(TextureFiles[i-1]);
        else text.setString("");
        if(i==0)text.setString(L"Вверх");
        if(i==7)text.setString(L"Вниз");
        text.setPosition(HEIGHT/2-spriteSize.x+5, spriteSize.x*7+(buttonSize.y+5)*i);
        button.setPosition(HEIGHT/2-spriteSize.x, spriteSize.x*7+(buttonSize.y+5)*i);
        window.draw(button);
        window.draw(text);
      }
    }
  }
};

//вкладка "Добавить объект"
class _AddObj
{
private:
  sf::Vector2f buttonSize;
  bool flagList, click, Type, flagItem, flagQuest, flagHP, flagMP;
  sf::Vector2i cords;
  std::string TextureAddres, TextureName;
  int HP, MP;
  sf::Texture textureSprite;
  char c;
public:
  _AddObj()
  {
    buttonSize=sf::Vector2f(WIDTH/7+5, WIDTH/20);
    flagList=false;
    click=false;
    TextureAddres="";
    Type=false;
    flagItem=false;
    flagQuest=false;
    HP=0;
    MP=0;
    c='\0';
    flagHP=false;
    flagMP=false;
  }

  void Update()
  {
    //чтение файлов из каталога текстур
    if(!flagList)
    {
      std::string path = "./"+ProjectName+"/textures";
      list_dir(path.c_str(), TextureFiles);
    }
    //обработка нажатия
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click)
      click=true;
    if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && click)
    {
      cords=sf::Mouse::getPosition(window);

      //выбор текстуры
      if(cords.x>50 && cords.x<50+128 &&
        cords.y>WIDTH/5-128+buttonSize.y && cords.y<WIDTH/5-128+buttonSize.y+128)
        flagList=true;
      click=false;

      //выбор текстуры
      if(flagList)
      {
        for (size_t i = 1; i < 11; i++)
        {
          if(TextureFiles.size()>i-1 && cords.x>50+(5+buttonSize.x)*5 && cords.x<50+(5+buttonSize.x)*5+buttonSize.x &&
            cords.y>WIDTH/5+(buttonSize.y+5)*i && cords.y<WIDTH/5+(buttonSize.y+5)*i+buttonSize.y)
            {
              TextureAddres="./"+ProjectName+"/textures/"+TextureFiles[i-1];
              TextureName=TextureFiles[i-1];
              flagList=false;
            }
        }
      }

      //добавить объект
      if(cords.x>50+(5+buttonSize.x)*3 && cords.x<50+(5+buttonSize.x)*3+buttonSize.x &&
        cords.y>(7+buttonSize.y)*6+(buttonSize.y/2+3) && cords.y<(7+buttonSize.y)*6+(buttonSize.y/2+3)+buttonSize.y)
        {
          Object tempObject;
          tempObject.Texture=TextureAddres;
          tempObject.ID=IDObjects;
          tempObject.objTexture.loadFromFile(TextureAddres);
          tempObject.HP=HP;
          tempObject.MP=MP;
          tempObject.Quest=flagQuest;
          tempObject.Item=flagItem;
          tempObject.Type=Type;
          tempObject.TextureName=TextureName;
          IDObjects++;
          objectList.push_back(tempObject);
          TextureAddres="";
        }

      //тип объекта
      if(cords.x>55+buttonSize.x && cords.x<55+buttonSize.x+buttonSize.x &&
        cords.y>WIDTH/5 && cords.y<WIDTH/5+buttonSize.y)
        Type=!Type;

      //Предмет
      if(cords.x>50+(5+buttonSize.x)*2 && cords.x<50+(5+buttonSize.x)*2+buttonSize.x &&
        cords.y>WIDTH/5 && cords.y<WIDTH/5+buttonSize.y)
        flagItem=!flagItem;

      //квест
      if(cords.x>50+(5+buttonSize.x)*2 && cords.x<50+(5+buttonSize.x)*2+buttonSize.x &&
        cords.y>(5+buttonSize.y)*5+(buttonSize.y/2+3) && cords.y<(5+buttonSize.y)*5+(buttonSize.y/2+3)+buttonSize.y)
        flagQuest=!flagQuest;

      //Здоровье
      if(cords.x>50+(5+buttonSize.x)*3 && cords.x<50+(5+buttonSize.x)*3+buttonSize.x &&
        cords.y>WIDTH/5 && cords.y<WIDTH/5+buttonSize.y)
      {
        flagHP=!flagHP;
        flagMP=false;
      }
      else
      {
        flagHP=false;
      }

      //мана

      if(cords.x>50+(5+buttonSize.x)*3 && cords.x<50+(5+buttonSize.x)*3+buttonSize.x &&
        cords.y>(5+buttonSize.y)*5+(buttonSize.y/2+3) && cords.y<(5+buttonSize.y)*5+(buttonSize.y/2+3)+buttonSize.y)
      {
        flagMP=!flagMP;
        flagHP=false;
      }
      else
      {
        flagMP=false;
      }
    }
    //ввод здоровья
    if(event.type == sf::Event::TextEntered && flagHP)
    {
      c=writeText();
    }
    if(c!= '\0' && event.type != sf::Event::TextEntered && flagHP)
    {
      if(c == 8)
      {
        HP /= 10;
      }
      else
      {
        HP=HP*10+((int)c-48);
      }
      c='\0';
    }

    //ввод маны
    if(event.type == sf::Event::TextEntered && flagMP)
    {
      c=writeText();
    }
    if(c!= '\0' && event.type != sf::Event::TextEntered && flagMP)
    {
      if(c == 8)
      {
        MP /= 10;
      }
      else
      {
        MP=MP*10+((int)c-48);
      }
      c='\0';
    }
  }

  void Draw()
  {
    //Текстура
    text.setString(L"Текстура");
    text.setPosition(50, WIDTH/5-110);
    window.draw(text);
    sf::RectangleShape texture;
    texture.setSize(sf::Vector2f(128,128));
    texture.setPosition(50, WIDTH/5-128+buttonSize.y);
    if(TextureAddres!="")
    {
	    textureSprite.loadFromFile(TextureAddres);
      texture.setTexture(&textureSprite);
    }
    window.draw(texture);

    sf::RectangleShape button;
    button.setSize(buttonSize);
    button.setFillColor(color);
    //тип объекта
    text.setString(L"Тип объекта");
    text.setPosition(60+buttonSize.x, WIDTH/5-(buttonSize.y/2+3)-5);
    window.draw(text);
    if(Type) text.setString(L"Твердый"); else text.setString(L"Не твердый");
    text.setPosition(60+buttonSize.x, WIDTH/5);
    button.setPosition(55+buttonSize.x, WIDTH/5);
    button.setFillColor(color);
    window.draw(button);
    window.draw(text);
    //размер объекта
    text.setString(L"размер X");
    text.setPosition(55, (5+buttonSize.y)*5-5);
    window.draw(text);

    button.setPosition(50, (5+buttonSize.y)*5+(buttonSize.y/2+3));
    button.setFillColor(color);
    window.draw(button);
    button.setPosition(50, (7+buttonSize.y)*6+(buttonSize.y/2+3));
    button.setFillColor(color);
    window.draw(button);

    text.setString(L"размер Y");
    text.setPosition(60+buttonSize.x, (5+buttonSize.y)*5-5);
    window.draw(text);

    button.setPosition(55+buttonSize.x, (5+buttonSize.y)*5+(buttonSize.y/2+3));
    button.setFillColor(color);
    window.draw(button);
    button.setPosition(55+buttonSize.x, (7+buttonSize.y)*6+(buttonSize.y/2+3));
    button.setFillColor(color);
    window.draw(button);
    //флаг предмет
    text.setString(L"Предмет");
    text.setPosition(55+(5+buttonSize.x)*2, WIDTH/5-(buttonSize.y/2+3)-5);
    window.draw(text);
    button.setPosition(50+(5+buttonSize.x)*2, WIDTH/5);
    if(flagItem) text.setString(L"Да"); else text.setString(L"Нет");
    text.setPosition(55+(5+buttonSize.x)*2, WIDTH/5);
    button.setFillColor(color);
    window.draw(button);
    window.draw(text);
    //флаг квест
    text.setString(L"Квестовый");
    text.setPosition(55+(5+buttonSize.x)*2, (5+buttonSize.y)*5-5);
    window.draw(text);
    if(flagQuest) text.setString(L"Да"); else text.setString(L"Нет");
    button.setPosition(50+(5+buttonSize.x)*2, (5+buttonSize.y)*5+(buttonSize.y/2+3));
    text.setPosition(55+(5+buttonSize.x)*2, (5+buttonSize.y)*5+(buttonSize.y/2+3));
    button.setFillColor(color);
    window.draw(button);
    window.draw(text);
    //здоровье
    text.setString(L"Здоровье");
    text.setPosition(55+(5+buttonSize.x)*3, WIDTH/5-(buttonSize.y/2+3)-5);
    if(flagHP)button.setFillColor(ActiveColor); else button.setFillColor(color);
    window.draw(text);
    button.setPosition(50+(5+buttonSize.x)*3, WIDTH/5);
    window.draw(button);
    text.setString(std::to_string(HP));
    text.setPosition(55+(5+buttonSize.x)*3, WIDTH/5);
    window.draw(text);
    //мана
    text.setString(L"Мана");
    text.setPosition(55+(5+buttonSize.x)*3, (5+buttonSize.y)*5-5);
    window.draw(text);
    button.setPosition(50+(5+buttonSize.x)*3, (5+buttonSize.y)*5+(buttonSize.y/2+3));
    if(flagMP) button.setFillColor(ActiveColor); else button.setFillColor(color);
    window.draw(button);
    text.setString(std::to_string(MP));
    text.setPosition(55+(5+buttonSize.x)*3, (5+buttonSize.y)*5+(buttonSize.y/2+3));
    window.draw(text);
    //добавить
    text.setString(L"Добавить");
    button.setPosition(50+(5+buttonSize.x)*3, (7+buttonSize.y)*6+(buttonSize.y/2+3));
    text.setPosition(55+(5+buttonSize.x)*3, (7+buttonSize.y)*6+(buttonSize.y/2+3));
    button.setFillColor(color);
    window.draw(button);
    window.draw(text);
    //список
    if(flagList)
    {
      for (size_t i = 0; i < 12; i++)
      {
        if(TextureFiles.size()>i-1 && i>0 && i<11) text.setString(TextureFiles[i-1]);
        else text.setString("");
        if(i==0) text.setString(L"вверх");
        if(i==11) text.setString(L"вниз");
        button.setPosition(50+(5+buttonSize.x)*5, WIDTH/5+(buttonSize.y+5)*i);
        text.setPosition(50+(5+buttonSize.x)*5, WIDTH/5+(buttonSize.y+5)*i);
        window.draw(button);
        window.draw(text);
      }
    }
  }
};

//вкладка "Добавить персонажа"
class _AddNPC
{
private:
  sf::Vector2f buttonSize;

public:
  _AddNPC()
  {
    buttonSize=sf::Vector2f(WIDTH/7+5, WIDTH/20);
  }

  void Update()
  {}

  void Draw()
  {
    //Текстура
    text.setString(L"Текстура");
    text.setPosition(50, WIDTH/5-110);
    window.draw(text);
    sf::RectangleShape texture;
    texture.setSize(sf::Vector2f(128,128));
    texture.setPosition(50, WIDTH/5-128+buttonSize.y);
    window.draw(texture);

    sf::RectangleShape button;
    button.setSize(buttonSize);
    button.setFillColor(color);
    //имя
    text.setString(L"Имя");
    button.setPosition(55+buttonSize.x, WIDTH/5-110);
    text.setPosition(60+buttonSize.x, WIDTH/5-110);
    window.draw(button);
    window.draw(text);
    //предмет
    text.setString(L"Предмет");
    button.setPosition(50, 5+buttonSize.y+230);
    text.setPosition(55, 5+buttonSize.y+230);
    window.draw(button);
    window.draw(text);
    //здоровье
    text.setString(L"Здоровье");
    button.setPosition(50, (5+buttonSize.y)*2+230);
    text.setPosition(55, (5+buttonSize.y)*2+230);
    window.draw(button);
    window.draw(text);
    //золото
    text.setString(L"Золото");
    button.setPosition(50+(5+buttonSize.x)*1, 5+buttonSize.y+230);
    text.setPosition(55+(5+buttonSize.x)*1, 5+buttonSize.y+230);
    window.draw(button);
    window.draw(text);
    //сила
    text.setString(L"Сила");
    button.setPosition(50+(5+buttonSize.x)*1, (5+buttonSize.y)*2+230);
    text.setPosition(55+(5+buttonSize.x)*1, (5+buttonSize.y)*2+230);
    window.draw(button);
    window.draw(text);
    //опыт
    text.setString(L"Опыт");
    button.setPosition(50+(5+buttonSize.x)*2, 5+buttonSize.y+230);
    text.setPosition(55+(5+buttonSize.x)*2, 5+buttonSize.y+230);
    window.draw(button);
    window.draw(text);
    //защита
    text.setString(L"Защита");
    button.setPosition(50+(5+buttonSize.x)*2, (5+buttonSize.y)*2+230);
    text.setPosition(55+(5+buttonSize.x)*2, (5+buttonSize.y)*2+230);
    window.draw(button);
    window.draw(text);
    //редактор диалогов
    text.setString(L"Редактор диалогов");
    button.setPosition(50, (5+buttonSize.y)*3+230);
    button.setSize(sf::Vector2f(buttonSize.x*2+5, buttonSize.y));
    text.setPosition(55, (5+buttonSize.y)*3+230);
    window.draw(button);
    window.draw(text);
  }
};

//Вкладка "редактор карты"
class _Map
{
private:
  sf::Vector2f buttonSize;
  int maxY, maxX, start;

public:
  _Map()
  {
    buttonSize=sf::Vector2f(WIDTH/7+5, WIDTH/20);
    maxY = (WIDTH-50)/buttonSize.y-3;
    start = buttonSize.x;
    maxX = (HEIGHT-start)/buttonSize.y-2;
  }

  void Update()
  {}

  void Draw()
  {
    sf::RectangleShape button;
    button.setSize(buttonSize);
    button.setFillColor(color);
    //интерфейс
    text.setString(L"Список персонажей");
    button.setSize(sf::Vector2f(buttonSize.x*1.5, buttonSize.y));
    text.setPosition(50+(10+buttonSize.x)*1, 50);
    button.setPosition(50+(5+buttonSize.x)*1, 50);
    window.draw(button);
    window.draw(text);

    text.setString(L"Список объектов");
    button.setSize(sf::Vector2f(buttonSize.x*1.5, buttonSize.y));
    text.setPosition(50+(10+buttonSize.x)*2+buttonSize.x*0.5, 50);
    button.setPosition(50+(5+buttonSize.x)*2+buttonSize.x*0.5, 50);
    window.draw(button);
    window.draw(text);

    text.setString(L"Список уровней");
    button.setSize(sf::Vector2f(buttonSize.x*1.5, buttonSize.y));
    text.setPosition(50+(10+buttonSize.x)*3+buttonSize.x, 50);
    button.setPosition(50+(5+buttonSize.x)*3+buttonSize.x, 50);
    window.draw(button);
    window.draw(text);

    button.setSize(sf::Vector2f(buttonSize.y, buttonSize.y));
    button.setPosition(50+buttonSize.x-buttonSize.y, 50+(10+50)*1);
    window.draw(button);

    button.setSize(sf::Vector2f(buttonSize.y, buttonSize.y));
    button.setPosition(50+buttonSize.x-buttonSize.y, 50+(10+50)*2);
    window.draw(button);

    button.setSize(sf::Vector2f(buttonSize.y, buttonSize.y));
    button.setPosition(50+buttonSize.x-buttonSize.y, 50+(10+50)*3);
    window.draw(button);

    button.setSize(sf::Vector2f(buttonSize.y, buttonSize.y));
    button.setPosition(50+buttonSize.x-buttonSize.y, 50+(10+50)*4);
    window.draw(button);

    text.setString(L"Слой 1");
    text.setPosition(60, 50+(10+50)*5);
    button.setSize(buttonSize);
    button.setPosition(50, 50+(10+50)*5);
    window.draw(button);
    window.draw(text);

    text.setString(L"Слой 2");
    text.setPosition(60, 50+(10+50)*6);
    button.setSize(buttonSize);
    button.setPosition(50, 50+(10+50)*6);
    window.draw(button);
    window.draw(text);

    text.setString(L"Слой 3");
    text.setPosition(60, 50+(10+50)*7);
    button.setSize(buttonSize);
    button.setPosition(50, 50+(10+50)*7);
    window.draw(button);
    window.draw(text);

    text.setString(L"Жанр");
    text.setPosition(60, 50+(10+50)*8);
    button.setSize(buttonSize);
    button.setPosition(50, 50+(10+50)*8);
    window.draw(button);
    window.draw(text);

    //Поле
    for(int i = 0; i<maxY; i++)
    {
      for(int j = 0; j<maxX; j++)
      {
        button.setFillColor(sf::Color::White);
        button.setSize(sf::Vector2f(buttonSize.y, buttonSize.y));
        button.setPosition(start+(1+buttonSize.y)*(j+1), 55+(1+buttonSize.y)*(i+1));
        window.draw(button);
      }
    }
  }
};

//Первый запуск
class _FirstStart
{
private:
  sf::Vector2f buttonSize;
  bool click, write,focus;
  sf::Vector2i cords;
  std::string Text;
  char symbol;
  bool testflag;

public:
  _FirstStart()
  {
    buttonSize=sf::Vector2f(WIDTH/7+5, WIDTH/20);
    click=false;
    write=false;
    Text="";
    focus=false;
    SearchFiles=true;
  }

  void Update()
  {
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click)
      click=true;
    if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && click)
    {
      //создать проект
      cords=sf::Mouse::getPosition(window);
      if(cords.x>100 && cords.x<100+buttonSize.x*1.5
      &&cords.y>100 && cords.y<100+buttonSize.y)
      {
        startFlag=false;

        std::string command;
        command = "mkdir -p ./"+Text;
        Directory=Text;
        system(command.c_str());
        command="mkdir -p ./"+Text+"/levels";
        system(command.c_str());
        command="mkdir -p ./"+Text+"/textures";
        system(command.c_str());
        ProjectName = Text;
      }
      else if(cords.x>100 && cords.x<100+buttonSize.x*1.5
      &&cords.y>200 && cords.y<200+buttonSize.y)
      {
        focus=true;
        click=false;
      }
      else
      {
        click=false;
        focus=false;
      }
    }
    //копирование из буфера обмена
    if(event.key.control && event.key.code == sf::Keyboard::V)
            Text = sf::Clipboard::getString();
    //ввод текста
    if(event.type == sf::Event::TextEntered && focus)
    {
      symbol=writeText();
    }
    if(symbol!= '\0' && event.type != sf::Event::TextEntered)
    {
      if(symbol == 8){if(Text.length()>0)Text.pop_back();} else Text += symbol;
      symbol='\0';
    }
  }

  void Draw()
  {
    sf::RectangleShape button;
    button.setSize(buttonSize);
    button.setFillColor(color);

    text.setString(L"Создать проект");
    button.setSize(sf::Vector2f(buttonSize.x*1.5, buttonSize.y));
    text.setPosition(105, 100);
    button.setPosition(100, 100);
    window.draw(button);
    window.draw(text);

    if(focus) button.setFillColor(ActiveColor);
    text.setString(Text);
    //text.setString(sf::String::fromUtf8(Text.begin(), Text.end()));
    button.setSize(sf::Vector2f(buttonSize.x*1.5, buttonSize.y));
    text.setPosition(105, 200);
    button.setPosition(100, 200);
    window.draw(button);
    window.draw(text);
  }
};

int main()
{
  //font.loadFromFile("./Fonts/CyrilicOld.TTF");
  //font.loadFromFile("./Fonts/ofont.ru_FRACTAL.ttf");
  //font.loadFromFile("./Fonts/ofont.ru_Secession-Afisha .ttf");
  font.loadFromFile("./Fonts/ofont.ru_Stanberry.ttf");

  _Options options;
  _Head head;
  _AddObj addObj;
  _AddNPC addNPC;
  _Map map;
  _FirstStart firstStart;
  _Generator generator;

  window.setVerticalSyncEnabled(true);

  IDObjects=1;

  while(window.isOpen())
  {

    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    if(event.type == sf::Event::LostFocus)
      continue;

    window.clear(fon);

    if(startFlag)
    {
      firstStart.Update();
      firstStart.Draw();
    }
    else
    {
      head.Update();
      switch(flag)
      {
        case 0:
          options.Update();
          break;
        case 1:
          addObj.Update();
          break;
        case 2:
          addNPC.Update();
          break;
        case 3:
          map.Update();
          break;
        case 4:
          generator.Update();
          break;

        default:
          break;
      }

      head.Draw();
      switch(flag){
        case 0:
          options.Draw();
          break;
        case 1:
          addObj.Draw();
          break;
        case 2:
          addNPC.Draw();
          break;
        case 3:
          map.Draw();
          break;
        case 4:
          generator.Draw();
          break;

        default:
          break;
      }
    }

    window.display();
  }

  return 0;
}
