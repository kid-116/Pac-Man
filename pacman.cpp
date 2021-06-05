#include<fstream>
#include<conio.h>
#include<dos.h>
int map[21][21][3];
int mapHeight=21;
int mapWidth=21;
int time;
void initMap()
 {
  ifstream mapVals("map.txt");
  for(int i=0;i<mapHeight;++i)
   for(int j=0;j<mapWidth;++j)
    {
     do
      {
       map[i][j][0]=mapVals.get();
      }while(map[i][j][0]==10);
     switch(map[i][j][0])
      {
       case 48:map[i][j][0]=0;
	       break;
       case 49:map[i][j][0]=1;
	       break;
       case 50:map[i][j][0]=2;
	       break;
       case 51:map[i][j][0]=3;
	       break;
      }
    }
  mapVals.close();
 }
void printMap()
 {
  gotoxy(25,2);
  cout<<"POINTS:"<<"     "<<"LIVES:";
  int k=3;
  for(int i=0;i<mapHeight;++i)
   {
    gotoxy(24,k);
    for(int j=0;j<mapWidth;++j)
     {
      delay(10);
      switch(map[i][j][0])
       {
	case 0:textcolor(1);
	       cprintf("%");
	       break;
	case 1:cout<<" ";
	       break;
	case 2:textcolor(7);
	       cprintf(".");
	       break;
	case 3:textcolor(7);
	       cprintf("?");
	       break;
       }
     }
    ++k;
   }
 }
int chkPts()
 {
  for(int i=0;i<mapHeight;++i)
   for(int j=0;j<mapWidth;++j)
    if(map[i][j][0]==2||map[i][j][0]==3)
     return 1;
  return 0;
 }
class ghost
 {
   int exi;
   int no;
   int c;
  protected:
   int x, y;
   int z;
   int dir;
   char ch;
   int color;
   void newCoor();
   void setCursor(int axe, int wai)
    {
     gotoxy(wai+24,axe+3);
    }
   void changeDir();
  public:
   void move();
   int getDir()
    {
     return dir;
    }
   int dead()
    {
     if(exi)
      return 0;
     else
      return 1;
    }
   void changeColor(char co)
    {
     if(co=='b')
      color=11;
     else
      color=c;
     setCursor(x,y);
     textcolor(color);
     cprintf("%c",ch);
    }
   void die();
   void run()
    {
     if(exi)
      {
       changeDir();
       move();
      }
    }
   void setToken(int axe, int wai, int d)
    {
     exi=1;
     x=axe;
     y=wai;
     dir=d;
     color=c;
     setCursor(x,y);
     map[x][y][z]=no;
     textcolor(color);
     cprintf("%c",ch);
    }
   ghost(int co, int n)
    {
     z=1;
     no=n;
     color=c=co;
     ch='#';
    }
 };
ghost red(4,5);
ghost green(2,6);
ghost pink(5,7);
ghost orange(12,8);
void ghost::die()
 {
  exi=0;
  map[x][y][z]=0;
 }
void ghost::newCoor()
 {
  if(x==9&&y==0&&dir==4)
   {
    y=20;
   }
  else
  if(x==9&&y==20&&dir==6)
   {
    y=0;
   }
  else
  switch(dir)
   {
    case 8:if(map[x-1][y][0])
	    {
	     x-=1;
	    }
	   break;
    case 4:if(map[x][y-1][0])
	    {
	     y-=1;
	    }
	   break;
    case 2:if(map[x+1][y][0])
	    {
	     x+=1;
	    }
	   break;
    case 6:if(map[x][y+1][0])
	    {
	     y+=1;
	    }
	   break;
   }
 }
void ghost::changeDir()
 {
  if(!((x==9)&&(y==0||y==20)))
  {
  switch(dir)
   {
    case 8:if(map[x-1][y][0]&&!map[x-1][y][z])
	    dir=8;
	   else
	   if(map[x][y-1][0]&&!map[x][y-1][z])
	    dir=4;
	   else
	   if(map[x][y+1][0]&&!map[x][y+1][z])
	    dir=6;
	   else
	   if(map[x+1][y][0]&&!map[x+1][y][z])
	    dir=2;
	   break;
    case 4:if(map[x+1][y][0]&&!map[x+1][y][z])
	    dir=2;
	   else
	   if(map[x-1][y][0]&&!map[x-1][y][z])
	    dir=8;
	   else
	   if(map[x][y-1][0]&&!map[x][y-1][z])
	    dir=4;
	   else
	   if(map[x][y+1][0]&&!map[x][y+1][z])
	    dir=6;
	   break;
    case 2:if(map[x][y-1][0]&&!map[x][y-1][z])
	    dir=4;
	   else
	   if(map[x+1][y][0]&&!map[x+1][y][z])
	    dir=2;
	   else
	   if(map[x][y+1][0]&&!map[x][y+1][z])
	    dir=6;
	   else
	   if(map[x-1][y][0]&&!map[x-1][y][z])
	    dir=8;
	   break;
    case 6:if(map[x][y+1][0]&&!map[x][y+1][z])
	    dir=6;
	   else
	   if(map[x+1][y][0]&&!map[x+1][y][z])
	    dir=2;
	   else
	   if(map[x-1][y][0]&&!map[x-1][y][z])
	    dir=8;
	   else
	   if(map[x][y-1][0]&&!map[x][y-1][z])
	    dir=4;
	   break;
   }
  switch(dir)
   {
    case 8:if(x!=1)
	    if(map[x-1][y][z]||map[x-2][y][z])
	     dir=2;
	   break;
    case 4:if(map[x][y-1][z]||map[x][y-2][z])
	    dir=6;
	   break;
    case 2:if(x!=19)
	    if(map[x+1][y][z]||map[x+2][y][z])
	     dir=8;
	   break;
    case 6:if(map[x][y+1][z])
	    dir=4;
	   break;
   }
  if(x!=1)
   {
    if(map[x-1][y][2]||map[x-2][y][2])
     dir=8;
   }
  else
  if(map[x][y-1][2]||map[x][y-2][2])
   dir=8;
  else
  if(x!=19)
   {
    if(map[x+1][y][2]||map[x+2][1][2])
     dir=2;
   }
  else
  if(map[x][y+1][2]||map[x][y+2][2])
   dir=6;
  }
 }
void ghost::move()
 {
  setCursor(x,y);
  map[x][y][z]=0;
  switch(map[x][y][0])
   {
    case 1:cout<<" ";
	   break;
    case 2:textcolor(7);
	   cprintf(".");
	   break;
    case 3:textcolor(7);
	   cprintf("?");
	   break;
   }
  newCoor();
  setCursor(x,y);
  map[x][y][z]=no;
  textcolor(color);
  cprintf("%c",ch);
 }
class PAC_MAN:public ghost
 {
   int di;
   int lives;
   int power;
   int points;
   int st;
   void getDir(char d);
   void chk();
  protected:
   void changeDir();
   void move();
  public:
   int getLives()
    {
     return lives;
    }
   void printPoints()
    {
     setCursor(-1,8);
     cout<<points;
    }
   void printLives()
    {
     setCursor(-1,19);
     cout<<lives;
    }
   void giveLives()
    {
     ++lives;
    }
   void run()
    {
     if(!map[x][y][1])
      {
       if(kbhit())
	getDir(getch());
       changeDir();
       move();
      }
     chk();
    }
   void setToken()
    {
     x=15;
     y=10;
     power=0;
     setCursor(x,y);
     map[x][y][z]=1;
     textcolor(color);
     cprintf("%c",ch);
    }
   PAC_MAN():ghost(14,0)
    {
     z=2;
     lives=1;
     power=0;
     points=0;
     ch='@';
    }
 };
void PAC_MAN::chk()
 {
  if(map[x][y][1])
   if(power)
    {
     switch(map[x][y][1])
      {
       case 5:red.die();
	      break;
       case 6:green.die();
	      break;
       case 7:pink.die();
	      break;
       case 8:orange.die();
	      break;
      }
     points+=100;
    }
   else
    --lives;
 }
void PAC_MAN::getDir(char d)
 {
  switch(d)
   {
    case 'w':di=8;
	     break;
    case 'a':di=4;
	     break;
    case 's':di=2;
	     break;
    case 'd':di=6;
	     break;
    case 'q':lives=0;
	     break;
   }
 }
void PAC_MAN::changeDir()
 {
  switch(di)
   {
    case 8:if(map[x-1][y][0])
	    dir=di;
	   break;
    case 4:if(map[x][y-1][0])
	    dir=di;
	   break;
    case 2:if(map[x+1][y][0])
	    dir=di;
	   break;
    case 6:if(map[x][y+1][0])
	    dir=di;
	   break;
   }
 }
void PAC_MAN::move()
 {
  switch(map[x][y][0])
   {
    case 2:++points;
	   break;
    case 3:points+=5;
	   st=time;
	   power=1;
	   if(!red.dead())
	    red.changeColor('b');
	   if(!green.dead())
	    green.changeColor('b');
	   if(!pink.dead())
	    pink.changeColor('b');
	   if(!orange.dead())
	    orange.changeColor('b');
	   break;
   }
  setCursor(x,y);
  map[x][y][0]=1;
  cout<<" ";
  map[x][y][z]=0;
  newCoor();
  setCursor(x,y);
  map[x][y][z]=1;
  textcolor(color);
  cprintf("%c",ch);
  if(power&&st+30<time)
   {
    power=0;
    if(red.dead())
     red.setToken(7,10,6);
    if(green.dead())
     green.setToken(9,9,6);
    if(pink.dead())
     pink.setToken(9,10,8);
    if(orange.dead())
     orange.setToken(9,11,4);
    red.changeColor('r');
    green.changeColor('g');
    pink.changeColor('p');
    orange.changeColor('o');
   }
 }
void game()
 {
  PAC_MAN yellow;
  int k=0;
  do
   {
    clrscr();
    if(yellow.getLives()>k)
     initMap();
    for(int i=0;i<mapHeight;++i)
     for(int j=0;j<mapWidth;++j)
      map[i][j][1]=map[i][j][2]=0;
    printMap();
    yellow.setToken();
    yellow.printLives();
    red.setToken(7,10,6);
    green.setToken(9,9,6);
    pink.setToken(9,10,8);
    orange.setToken(9,11,4);
    k=yellow.getLives();
    for(time=0;(chkPts())&&(yellow.getLives()==k);++time)
     {
      yellow.printPoints();
      delay(200);
      red.run();
      gotoxy(1,80);
      if(time>20)
       pink.run();
      if(time>30)
       green.run();
      if(time>40)
       orange.run();
      yellow.run();
     }
    if(!chkPts())
     yellow.giveLives();
    yellow.printLives();
   }while(yellow.getLives());
  gotoxy(21,13);
  cout<<"Oops! Better luck next time:)";
  gotoxy(23,14);
  cout<<"Press any key to continue";
  getch();
 }
void intro(int t)
 {
  int x;
  if(t==0)
   x=150;
  else
   x=0;
  delay(x);
  for(int i=0;i<mapHeight;++i)
   for(int j=0;j<mapWidth;++j)
    map[i][j][0]=1;
  red.setToken(7,21,4);
  for(i=0;i<13;++i)
   {
    delay(x);
    red.move();
   }
  delay(x);
  gotoxy(33,12);
  cprintf("Shadow \"Blinky\"");
  pink.setToken(7,21,4);
  for(i=0;i<12;++i)
   {
    delay(x);
    pink.move();
   }
  delay(x);
  gotoxy(33,13);
  cprintf("Speedy \"Pinky\"");
  green.setToken(7,21,4);
  for(i=0;i<11;++i)
   {
    delay(x);
    green.move();
   }
  delay(x);
  gotoxy(33,14);
  cprintf("Bashful \"Inky\"");
  orange.setToken(7,21,4);
  for(i=0;i<10;++i)
   {
    delay(x);
    orange.move();
   }
  delay(x);
  gotoxy(33,15);
  cprintf("Pokey \"Clyde\"");
  gotoxy(30,10);
  textcolor(14);
  cprintf("@");
  textcolor(7);
 }
int main()
 {
  for(int i=0;;++i)
   {
    clrscr();
    cout<<"\n\n\n"<<"\t\t";
    textcolor(7);
    textbackground(10);
    cprintf("//PAC_MAN//");
    textbackground(8);
    cout<<"\n"<<"\t\t";
    cprintf("1.Play");
    cout<<"\n"<<"\t\t";
    cprintf("2.Exit");
    intro(i);
    int x=16, y=5;
    gotoxy(x+1,20);
    cprintf("Created by ");
    textbackground(1);
    cprintf("MEHUL");
    textbackground(8);
    char ch;
    gotoxy(x,y);
    for(;;)
     {
      if(kbhit())
       {
	ch=getch();
	switch(ch)
	 {
	  case 'w':if(y>5)
		    {
		     --y;
		     gotoxy(x,y);
		    }
		   break;
	  case 's':if(y<6)
		    {
		     ++y;
		     gotoxy(x,y);
		    }
		   break;
	  case 13:goto a;
	 }
       }
     }
    a:
    switch(y)
     {
      case 5:game();
	     break;
      case 6:return 0;
     }
   }
 }


