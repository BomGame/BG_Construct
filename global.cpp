sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "BGConstruct");
sf::Font font;
int HEIGHT=window.getSize().x;
int WIDTH=window.getSize().y;
int fontSize=HEIGHT/74;
sf::Text text("", font, fontSize);
int flag;
sf::Color color(157,133,171);
sf::Color ActiveColor(157,100,120);
sf::Color fon(151, 13, 161);
bool startFlag=true;
sf::Event event;
std::string Directory;
bool SearchFiles;
std::vector<std::string> TextureFiles;
int IDObjects;
bool generationFlag=false;
std::wstring aboutGame=L"Управление:\nПеремещение: стрелки на клавиатуре\nДействие: пробел"; //описание
std::string Autor="";

char writeText()
{
  char symbol;
  if (event.text.unicode < 128)
  {
    symbol=static_cast<char>(event.text.unicode);
  }
  return symbol;
}

int testmatrix[13][12]=
{
  {1,0,1,0,1,0,1,0,1,0,1,0},
  {1,0,1,0,1,0,1,0,1,0,1,0},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,1,0,1,0,1,0,1,0,1,0},
  {1,0,1,0,1,0,1,0,1,0,1,0},
  {1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,1,0,1,0,1,0,1,0,1,0},
  {1,0,1,0,1,0,1,0,1,0,1,0},
  {1,0,1,0,1,0,1,0,1,0,1,0},
  {1,0,1,0,1,0,1,0,1,0,1,0},
  {1,0,1,0,1,0,1,0,1,0,1,0},
  {1,0,1,0,1,0,1,0,1,0,1,0},
  {1,1,1,1,1,1,1,1,1,1,1,1}
};
