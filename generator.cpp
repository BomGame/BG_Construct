#include <fstream>
#include <codecvt>
//#include "global.cpp"

class _Generator
{
private:
  sf::Vector2f buttonSize;
  //для защиты
  bool BlockCopy;
  bool BlockFrame;
  bool BlockDirectory;
  bool EnterAutor;
  bool click;
  sf::Vector2i cords;
  char symbol;
  std::wstring work;

public:
  _Generator()
  {
    buttonSize=sf::Vector2f(WIDTH/7+5, WIDTH/20);
    BlockCopy=false;
    BlockFrame=false;
    BlockDirectory=false;
    EnterAutor=false;
    click=false;
    symbol='\0';
    work=L"Сборка не начата";
  }

  void Draw()
  {
    text.setString(work);
    text.setPosition(HEIGHT/2,WIDTH-(buttonSize.y*2));
    window.draw(text);

    sf::RectangleShape button;
    button.setSize(buttonSize);
    button.setFillColor(color);

    text.setString(L"Запретить копировать \nтекст");
    text.setPosition(10, buttonSize.y-5);
    window.draw(text);
    if(BlockCopy) text.setString(L"Да"); else text.setString(L"Нет");
    text.setPosition(15, buttonSize.y*2);
    button.setSize(sf::Vector2f(buttonSize.x*1.5, buttonSize.y));
    button.setPosition(10, buttonSize.y*2);
    window.draw(button);
    window.draw(text);

    text.setString(L"Запретить встраивать \nстраницу");
    text.setPosition(10+buttonSize.x*2, buttonSize.y-5);
    window.draw(text);
    if(BlockFrame) text.setString(L"Да"); else text.setString(L"Нет");
    text.setPosition(15+buttonSize.x*2, buttonSize.y*2);
    button.setSize(sf::Vector2f(buttonSize.x*1.5, buttonSize.y));
    button.setPosition(10+buttonSize.x*2, buttonSize.y*2);
    button.setFillColor(color);
    window.draw(button);
    window.draw(text);

    text.setString(L"Запретить просматривать \nкаталоги страницы");
    text.setPosition(10+buttonSize.x*4, buttonSize.y-5);
    window.draw(text);
    if(BlockDirectory) text.setString(L"Да"); else text.setString(L"Нет");
    text.setPosition(15+buttonSize.x*4, buttonSize.y*2);
    button.setSize(sf::Vector2f(buttonSize.x*1.5, buttonSize.y));
    button.setPosition(10+buttonSize.x*4, buttonSize.y*2);
    button.setFillColor(color);
    window.draw(button);
    window.draw(text);

    text.setString(L"Указать псевдоним");
    text.setPosition(10+buttonSize.x*7, buttonSize.y-5);
    window.draw(text);
    text.setString(Autor);
    text.setPosition(15+buttonSize.x*7, buttonSize.y*2);
    button.setSize(sf::Vector2f(buttonSize.x*1.5, buttonSize.y));
    button.setPosition(10+buttonSize.x*7, buttonSize.y*2);
    if(EnterAutor)button.setFillColor(ActiveColor); else button.setFillColor(color);
    window.draw(button);
    window.draw(text);

    text.setString(L"Собрать");
    text.setPosition(15, WIDTH-(buttonSize.y*2));
    button.setSize(sf::Vector2f(buttonSize.x*1.5, buttonSize.y));
    button.setPosition(10, WIDTH-(buttonSize.y*2));
    button.setFillColor(color);
    window.draw(button);
    window.draw(text);

    text.setString(L"Описание");
    text.setPosition(10, buttonSize.y*3.8);
    window.draw(text);
    text.setString(aboutGame);
    text.setPosition(10, buttonSize.y*4.5);
    button.setSize(sf::Vector2f(HEIGHT-20, WIDTH-(buttonSize.y*4.5+buttonSize.y*3)));
    button.setPosition(10, buttonSize.y*4.5);
    button.setFillColor(color);
    window.draw(button);
    window.draw(text);
  }

  void Update()
  {
    if(generationFlag)
    {
      //создание каталогов
      //------------------
      std::string command;
      command = "mkdir -p ./"+ProjectName+"/Game/Engen";
      system(command.c_str());
      command = "mkdir -p ./"+ProjectName+"/Game/"+ProjectName;
      system(command.c_str());
      command = "mkdir -p ./"+ProjectName+"/Game/"+ProjectName+"/levels";
      system(command.c_str());
      command = "mkdir -p ./"+ProjectName+"/Game/"+ProjectName+"/scripts";
      system(command.c_str());
      command = "mkdir -p ./"+ProjectName+"/Game/"+ProjectName+"/sprites";
      system(command.c_str());

      std::ofstream File;
      if(BlockDirectory)
      {
        File.open ("./"+ProjectName+"/Game/"+ProjectName+"/.htaccess");
        File<<"Options -Indexes";
        File.close();

        File.open("./"+ProjectName+"/Engen/"+ProjectName+"/.htaccess");
        File<<"Options -Indexes";
        File.close();
      }

      //создание файла инициализации
      //----------------------------
      File.open("./"+ProjectName+"/Game/"+ProjectName+"/levels/"+"levelsinit.js");
      File<<"function levelsinit(level){";
      for(int i=0; i<levelList.size(); i++)
       File<<"if(level=="<<i+1<<")"<<levelList[i].LevelName()<<"();"<<std::endl;
      File<<"}";
      File.close();

      //создание страницы с игрой
      //-------------------------
      File.open ("./"+ProjectName+"/Game/"+ProjectName+"/"+ProjectName+".html");
      File << "<!DOCTYPE html>"<<std::endl<<
      "<html>"<<std::endl<<"<head>"<<std::endl<<"<meta charset=\"utf-8\"/>"<<std::endl
      <<"<title>"<<ProjectName<<"</title>"<<"<style>"<<std::endl<<"*{padding: 0; margin: 0;}"<<std::endl
      <<"canvas { background: #eee; display: block; margin: 0 auto; }"<<std::endl<<"</style>"<<std::endl;

      if(BlockFrame)
        File<<"<script>"<<"if(window.parent.frames.length>0){window.stop();}"<<"</script>";

      File<<"</head>"<<std::endl<<"<body>"<<std::endl<<"<canvas id=\"myCanvas\" width=\"1024\" height=\"768\" style=\"background: #000;\"></canvas>";

      if(BlockCopy)
        File<<"<script>"<<"document.oncontextmenu = noselect;function noselect() {return false;};"<<"</script>";
      else
        File<<"<script>"<<"document.oncopy = function(){var body = document.getElementsByTagName('body')[0];var selection = window.getSelection();"<<
        "var div = document.createElement('div'); div.style.position = 'absolute';div.style.left = '-99999px';body.appendChild(div);div.innerHTML=selection+' Источник: '+document.location.host;"<<
        "selection.selectAllChildren(div);window.setTimeout(function(){body.removeChild(div);},0);}"<<"</script>";

      File<<std::endl<<"<script src=\"../Engen/BG_Parameters.js\"></script>"<<std::endl<<"<script src=\"./scripts/parameters.js\"></script>"
      <<std::endl;

      for (int i = 0; i<levelList.size(); i++)
        File<<"<script src=\"./levels/"<<levelList[i].LevelName()<<".js\"></script>"<<std::endl;

      File<<"<script src=\"./levels/levelsinit.js\"></script>"<<std::endl;

      File<<"<script src=\"../Engen/BG_Engen.js\"></script>"<<std::endl;

      if(BlockCopy)File<<"<p oncontextmenu=\"return false;\" onselectstart=\"return false\">";
      else File<<"<p>"<<std::endl;

      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
      std::string utf8String = convert.to_bytes( aboutGame );
      File<<utf8String<<"</p>";

      File<<std::endl<<"</body>"<<std::endl<<"</html>";
      File.close();

      //создание скрипта с параметрами
      //------------------------------
      File.open ("./"+ProjectName+"/Game/"+ProjectName+"/scripts/parameters.js");
      File << "playerImage=new Image();" << std::endl <<"playerImage.src='./sprites/"<<player.Texture<<"';"<<std::endl<<
      "playerSlides="<<player.Slides<<";"<<std::endl<<"playerX=size*6;"<<std::endl<<"playerY=size*4;"<<std::endl<<
      "playerSpeed="<<player.Speed<<";"<<std::endl<<"playerMove=0;"<<std::endl<<"playerDirection=0;"<<std::endl<<
      "colisX=size*6, colisY=size*4";
      File.close();

      command = "cp ./"+ProjectName+"/textures/"+player.Texture+" ./"+ProjectName+"/Game/"+ProjectName+"/sprites";
      system(command.c_str());

      //создание скрипта движка
      //-----------------------
      File.open("./"+ProjectName+"/Game/Engen/BG_Engen.js");

      // File<<"document.addEventListener(\"keydown\", keyDownHandler, false);"<<std::endl<<"document.addEventListener(\"keyup\", keyUpHandler, false);";
      //
      // //function keydown
      // File<<"function keyDownHandler(e){if(e.key == \" \"){}"<<std::endl<<"else if(e.key==\"ArrowRight\"){"<<std::endl<<
      // "if(layer2[parseInt((colisY+size/2)/size)][parseInt(colisX/size)+1]!=-1)"<<std::endl<<"if(objects[layer2[parseInt((colisY+size/2)/size)][parseInt(colisX/size)+1]-1].solid  &&"<<
      // "objects[layer2[parseInt((colisY+size/2)/size)][parseInt(colisX/size)+1]-1].ID==layer2[parseInt((colisY+size/2)/size)][parseInt(colisX/size)+1]){"<<std::endl<<
      // "walkX+=playerSpeed; colisX-=playerSpeed;}"<<std::endl<<"walkX-=playerSpeed; colisX+=playerSpeed;playerDirection=playerImage.height/4*2;"<<std::endl<<
      // "if(walkX<(size*-1) && (tileX<(layer1[0].length-14))){tileX++;walkX=0;}"<<std::endl<<"timer++;}"<<"else if(e.key==\"ArrowLeft\"){"<<std::endl<<
      // "if(layer2[parseInt((colisY+size/2)/size)][parseInt(colisX/size)]!=-1)"<<std::endl<<"if(objects[layer2[parseInt((colisY+size/2)/size)][parseInt(colisX/size)]-1].solid  &&"<<
      // "objects[layer2[parseInt((colisY+size/2)/size)][parseInt(colisX/size)]-1].ID==layer2[parseInt((colisY+size/2)/size)][parseInt(colisX/size)]){"<<std::endl<<"walkX-=playerSpeed;colisX+=playerSpeed;}"<<std::endl<<
      // "walkX+=playerSpeed;colisX-=playerSpeed;playerDirection=playerImage.height/4;"<<std::endl<<"if(walkX>size && tileX>0){tileX--;walkX=0}"<<std::endl<<"timer++;}"<<std::endl<<
      // "else if(e.key==\"ArrowUp\"){"<<std::endl<<"if(layer2[parseInt((colisY+size)/size)-1][parseInt(colisX/size)]!=-1)"<<std::endl<<"if(objects[layer2[parseInt((colisY+size)/size)-1][parseInt(colisX/size)]-1].solid  &&"<<
      // "objects[layer2[parseInt((colisY+size)/size)-1][parseInt(colisX/size)]-1].ID==layer2[parseInt((colisY+size)/size)-1][parseInt(colisX/size)]){"<<std::endl<<"walkY-=playerSpeed;colisY+=playerSpeed;}"<<std::endl<<
      // "walkY+=playerSpeed;colisY-=playerSpeed;playerDirection=playerImage.height/4*3;"<<std::endl<<"if(walkY>size && tileY>0){tileY--;walkY=0}"<<std::endl<<"timer++;}"<<std::endl<<"else if(e.key==\"ArrowDown\"){"<<std::endl<<
      // "if(layer2[parseInt((colisY+size)/size)][parseInt(colisX/size)]!=-1)"<<std::endl<<"if(objects[layer2[parseInt((colisY+size)/size)][parseInt(colisX/size)]-1].solid  &&"<<"objects[layer2[parseInt((colisY+size)/size)][parseInt(colisX/size)]-1].ID==layer2[parseInt((colisY+size)/size)][parseInt(colisX/size)]){"<<
      // "walkY+=playerSpeed;colisY-=playerSpeed;}"<<std::endl<<"walkY-=playerSpeed;colisY+=playerSpeed;playerDirection=0;"<<std::endl<<"if(walkY<(size*-1) && (tileY<(layer1.length-11))){"<<"tileY++;walkY=0;}"<<std::endl<<
      // "timer++;}}"<<std::endl;
      //
      // //function keyup
      // File<<"function keyUpHandler(e){playerMove=0;playerDirection=0;}"<<std::endl;
      //
      // //function Draw_Level
      // File<<"function Draw_Level(){"<<std::endl<<"if(timer>50/playerSlides){playerMove+=playerImage.height/playerSlides;timer=0;}"<<std::endl<<"if(playerMove>=playerImage.width) playerMove=0;ctx.fillStyle=\"#000\";ctx.fillRect(0, 0, canvas.width, canvas.height);"<<std::endl<<
      // "for(var i = 0; i<14; i++){for(var j = 0; j<11; j++){if(layer1[j+tileY][i+tileX]!=-1) ctx.drawImage(tiles[layer1[j+tileY][i+tileX]-1], 0+size*i+walkX, 0+size*j+walkY, size, size);}}"<<std::endl<<"ctx.drawImage(playerImage, 0+playerMove, 0+playerDirection, playerImage.width/playerSlides, playerImage.height/4, playerX, playerY, size, size);"<<std::endl<<
      // "for(var l=1; l<3; l++){for(var i = 0; i<14; i++){for(var j = 0; j<11; j++){if(l==1){if(layer2[j+tileY][i+tileX]!=-1) ctx.drawImage(tiles[layer2[j+tileY][i+tileX]-1], 0+size*i+walkX, 0+size*j+walkY, size, size);}if(l==2){if(layer3[j+tileY][i+tileX]!=-1) ctx.drawImage(tiles[layer3[j+tileY][i+tileX]-1], 0+size*i+walkX, 0+size*j+walkY, size, size);}}}}"<<
      // "ctx.fillStyle=\"#2a172b\";ctx.strokeStyle=\"#af11ba\";ctx.font = \"italic 30pt Arial\";ctx.fillText(Autor, 20, 50);ctx.strokeText(Autor, 20, 50);"<<"}"<<std::endl;
      //
      // //function BG_RPG
      // File<<"function BG_RPG(){if(!flagInit){";
      // for(int i=0; i<levelList.size(); i++)
      //   File<<"if(level=="<<i+1<<")"<<levelList[i].LevelName()<<"();"<<std::endl;
      // File<<"flagInit=true;}Draw_Level();}"<<std::endl;
      //
      // //setInterval
      // File<<"setInterval(BG_RPG, 10);ctx.closePath();";

      File<<"eval(function(p,a,c,k,e,r){e=function(c){return(c<a?'':e(parseInt(c/a)))+((c=c%a)>35?String.fromCharCode(c+29):c.toString(36))};if(!''.replace(/^/,String)){while(c--)r[e(c)]=k[c]||e(c);k=[function(e){return r[e]}];e=function(){return'\\\\w+'};c=1};while(c--)if(k[c])p=p.replace(new RegExp('\\\\b'+e(c)+'\\\\b','g'),k[c]);return p}('x.J(\"V\",K,E);x.J(\"W\",L,E);s K(e){7(e.t==\" \"){M=2;f=0;g=0;F=E}y 7(e.t==\"X\"){7(c[6((8+5/2)/5)][6(9/5)+1]!=-1)7(n[c[6((8+5/2)/5)][6(9/5)+1]-1].z&&n[c[6((8+5/2)/5)][6(9/5)+1]-1].A==c[6((8+5/2)/5)][6(9/5)+1]){f+=d;9-=d}f-=d;9+=d;p=o.u/4*2;7(f<(5*-1)&&(k<(B[0].N-14))){k++;f=0}q++}y 7(e.t==\"Y\"){7(c[6((8+5/2)/5)][6(9/5)]!=-1)7(n[c[6((8+5/2)/5)][6(9/5)]-1].z&&n[c[6((8+5/2)/5)][6(9/5)]-1].A==c[6((8+5/2)/5)][6(9/5)]){f-=d;9+=d}f+=d;9-=d;p=o.u/4;7(f>5&&k>0){k--;f=0}q++}y 7(e.t==\"Z\"){7(c[6((8+5)/5)-1][6(9/5)]!=-1)7(n[c[6((8+5)/5)-1][6(9/5)]-1].z&&n[c[6((8+5)/5)-1][6(9/5)]-1].A==c[6((8+5)/5)-1][6(9/5)]){g-=d;8+=d}g+=d;8-=d;p=o.u/4*3;7(g>5&&m>0){m--;g=0}q++}y 7(e.t==\"12\"){7(c[6((8+5)/5)][6(9/5)]!=-1)7(n[c[6((8+5)/5)][6(9/5)]-1].z&&n[c[6((8+5)/5)][6(9/5)]-1].A==c[6((8+5)/5)][6(9/5)]){g+=d;8-=d}g-=d;8+=d;p=0;7(g<(5*-1)&&(m<(B.N-11))){m++;g=0}q++}}s L(e){v=0;p=0}s 13(a){r b=x.15(\\'16\\');b.17=a;x.18(\\'19\\')[0].1a(b)}s O(){7(q>G/H){v+=o.C/H;q=0}7(v>=o.C)v=0;h.P=\"#1b\";h.1c(0,0,Q.C,Q.u);w(r i=0;i<14;i++){w(r j=0;j<11;j++){7(B[j+m][i+k]!=-1)h.D(I[B[j+m][i+k]-1],0+5*i+f,0+5*j+g,5,5)}}h.D(o,0+v,0+p,o.C/H,o.u/4,1d,1e,5,5);w(r l=1;l<3;l++){w(r i=0;i<14;i++){w(r j=0;j<11;j++){7(l==1){7(c[j+m][i+k]!=-1)h.D(I[c[j+m][i+k]-1],0+5*i+f,0+5*j+g,5,5)}7(l==2){7(R[j+m][i+k]!=-1)h.D(I[R[j+m][i+k]-1],0+5*i+f,0+5*j+g,5,5)}}}}h.P=\"#1f\";h.1g=\"#1h\";h.1i=\"1j 1k 1l\";h.1m(S,T,G);h.1n(S,T,G)}s U(){7(!F){1o(M);F=1p}O()}1q(U,10);',62,89,'|||||size|parseInt|if|colisY|colisX|||layer2|playerSpeed||walkX|walkY|ctx|||tileX||tileY|objects|playerImage|playerDirection|timer|var|function|key|height|playerMove|for|document|else|solid|ID|layer1|width|drawImage|false|flagInit|50|playerSlides|tiles|addEventListener|keyDownHandler|keyUpHandler|level|length|Draw_Level|fillStyle|canvas|layer3|Autor|20|BG_RPG|keydown|keyup|ArrowRight|ArrowLeft|ArrowUp|||ArrowDown|include||createElement|script|src|getElementsByTagName|head|appendChild|000|fillRect|playerX|playerY|2a172b|strokeStyle|af11ba|font|italic|30pt|Arial|fillText|strokeText|levelsinit|true|setInterval'.split('|'),0,{}))";

      File.close();

      //BG parameters
      //-------------
      File.open("./"+ProjectName+"/Game/Engen/BG_Parameters.js");
      File<<"var canvas = document.getElementById(\"myCanvas\");var ctx = canvas.getContext(\"2d\");var size=75;var level=1;var layer1;"<<
      "var layer2;var layer3;var tiles=[];var flagInit=false;var walkX=0, walkY=0;var tileX=0, tileY=0;var playerX=0, playerY=0;var playerSpeed;var playerSlides; var playerImage;"<<
      "var playerSize;var playerMove;var playerDirection;var timer=0;var objects=[];var colisX, colisY;var Autor=\""<<Autor<<"\";";
      File.close();

      //levels
      //------
      for (int l=0; l<levelList.size();l++)
      {
        File.open("./"+ProjectName+"/Game/"+ProjectName+"/levels/"+levelList[l].LevelName()+".js");
        File<<"function "<<levelList[l].LevelName()<<"(){"<<std::endl;

        for(int i=0; i<objectList.size(); i++)
        {
          File<<"tiles["<<i<<"]=new Image();"<<std::endl;
          File<<"tiles["<<i<<"].src='./sprites/"<<objectList[i].TextureName<<"';"<<std::endl;
          command = "cp ./"+ProjectName+"/textures/"+objectList[i].TextureName+" ./"+ProjectName+"/Game/"+ProjectName+"/sprites";
          system(command.c_str());
        }

        File<<"objects=["<<std::endl;
        for(int i=0; i<objectList.size(); i++)
        {
          File<<"{ID:"<<objectList[i].ID<<", solid:"<<objectList[i].Type<<"},"<<std::endl;
        }
        File<<"]"<<std::endl;

        for(int lay=0; lay<3; lay++)
        {
          File<<"layer"<<lay+1<<"=["<<std::endl;
          for(int i=0; i<levelList[l].Height(); i++)
          {
            File<<"[";
            for(int j=0; j<levelList[l].Width(); j++)
              File<<levelList[l].Map(j,i,lay+1)<<",";
            File<<"],"<<std::endl;
          }
          File<<"]"<<std::endl;
        }

        File<<"}";

        File.close();
      }

      generationFlag=false;

      work=L"Проект успешно собран.";
    }
    else
    {
      if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click)
        click=true;
      if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && click)
      {
        cords=sf::Mouse::getPosition(window);
        if(cords.x>10 && cords.x<10+buttonSize.x*1.5 && cords.y > buttonSize.y*2 && cords.y < buttonSize.y*2+buttonSize.y)
          BlockCopy=!BlockCopy;
        if(cords.x>10+buttonSize.x*2 && cords.x<10+buttonSize.x*2+buttonSize.x*1.5 && cords.y > buttonSize.y*2 && cords.y < buttonSize.y*2+buttonSize.y)
          BlockFrame=!BlockFrame;
        if(cords.x>10+buttonSize.x*4 && cords.x<10+buttonSize.x*4+buttonSize.x*1.5 && cords.y > buttonSize.y*2 && cords.y < buttonSize.y*2+buttonSize.y)
          BlockDirectory=!BlockDirectory;
        if(cords.x>10+buttonSize.x*7 && cords.x<10+buttonSize.x*7+buttonSize.x*1.5 && cords.y > buttonSize.y*2 && cords.y < buttonSize.y*2+buttonSize.y)
          EnterAutor=!EnterAutor;
        else
          EnterAutor=false;
        if(cords.x>10 && cords.x<10+buttonSize.x*1.5 && cords.y > WIDTH-(buttonSize.y*2) && cords.y < WIDTH-(buttonSize.y*2)+buttonSize.y)
        {
          generationFlag=true;
          work=L"Проект собирается. Пожалуйста, подождите...";
        }



        click=false;
      }

      if(EnterAutor)
      {

        if(event.type == sf::Event::TextEntered)
        {
          symbol=writeText();
        }
        if(symbol!= '\0' && event.type != sf::Event::TextEntered)
        {
          if(symbol == 8){if(Autor.length()>0)Autor.pop_back();} else Autor += symbol;
          symbol='\0';
        }
      }
    }
  }
};
