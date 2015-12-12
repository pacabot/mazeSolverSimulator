final int WIDTH_COTE=30;
final int NOMBER_OF_WALL=17;
final int HAUTEUR_LABYRINTHE = NOMBER_OF_WALL*WIDTH_COTE+4*(NOMBER_OF_WALL+1);

int [][][][] maze = new int[2][NOMBER_OF_WALL+2][NOMBER_OF_WALL+2][2]; //stoque le maze, dans la premiere case est stoquer les passages du robot et dans la 2ème case le maze
int number_of_the_maze=1;

import java.io.FilenameFilter;
import processing.serial.*;
Serial Zhonx;  // Create object from Serial class



String[] in_maze;
String file_path=null;
String files_name=null;
boolean file_select=false;
boolean Serial_actif=false;
boolean old_frame=false;
int position_x_zhonx=0;
int position_y_zhonx=NOMBER_OF_WALL-1;
int oriantation_zhonx=0;


void setup()
{
  files_name= year() + " " + month() + " " + day();
  noSmooth();
  noStroke();
  size(678, 718);
  poussoirs = new donnees_poussoir[0];
  new donnees_poussoir("exit", 10, 10, WIDTH_COTE*2.5, HAUTEUR_LABYRINTHE+70, false);
  new donnees_poussoir("screen\nshoot", 10, 10, WIDTH_COTE*5, HAUTEUR_LABYRINTHE+70, true);
  new donnees_poussoir("save\nmaze", 10, 10, WIDTH_COTE*7.5, HAUTEUR_LABYRINTHE+70, true);
  new donnees_poussoir("clear", 10, 10, WIDTH_COTE*10, HAUTEUR_LABYRINTHE+70, true);
  new donnees_poussoir("", 10, 10, WIDTH_COTE*10, HAUTEUR_LABYRINTHE+55, false);
  new donnees_poussoir("open\nmaze", 10, 10, WIDTH_COTE*12.5, HAUTEUR_LABYRINTHE+70, true);
  new donnees_poussoir("open\nserial", 10, 10, WIDTH_COTE*15, HAUTEUR_LABYRINTHE+70, false);
  frameRate(5);

  for (int a=0; a<NOMBER_OF_WALL; a++)
  {
    maze[1][a][NOMBER_OF_WALL][0]=2;
    maze[1][NOMBER_OF_WALL][a][1]=2;
    maze[1][a][0][0]=2;
    maze[1][0][a][1]=2;
  }
  maze[1][(NOMBER_OF_WALL/2)-1][(NOMBER_OF_WALL/2)-1][1]=2;
  maze[1][(NOMBER_OF_WALL/2)-1][(NOMBER_OF_WALL/2)-1][0]=2;
  maze[1][NOMBER_OF_WALL/2][(NOMBER_OF_WALL/2)-1][0]=2;
  maze[1][(NOMBER_OF_WALL/2)+1][(NOMBER_OF_WALL/2)-1][1]=2;
  maze[1][(NOMBER_OF_WALL/2)+1][NOMBER_OF_WALL/2][1]=2;
  maze[1][NOMBER_OF_WALL/2][(NOMBER_OF_WALL/2)+1][0]=2;
  maze[1][(NOMBER_OF_WALL/2)-1][(NOMBER_OF_WALL/2)+1][0]=2;
  maze[1][(NOMBER_OF_WALL/2)-1][NOMBER_OF_WALL/2][1]=2;
  maze[1][1][NOMBER_OF_WALL-1][1]=2;
  affiche();
}
void draw()
{
  old_frame=!old_frame;
  //  nombre_du_labirinthe=creat_maze(nombre_du_labirinthe);
  //  number_of_the_maze++;
  if (Serial_actif==true)
  {
    com_zhonx();
  }
  affiche();
  if (old_frame)
  {
    fill(255,0,0);
    ellipse(8,7,15,15);
  }
}
void mousePressed()
{
  //println(((mouseX-12)/25)+"\t"+(mouseY-12)/25+"\t");
  float x=((mouseX+float(12))/float(WIDTH_COTE+4))-1;
  float y=((mouseY+float(12))/float(WIDTH_COTE+4));
  if (x<=NOMBER_OF_WALL+1 && y<=NOMBER_OF_WALL+1 && x>=0 && y>=0)
  {
    //print("\n"+x+";"+y);
    //print("\t"+x%1+";"+y%1);
    if ((x%1>0.8 || x%1<0.2)&&(y%1<0.8 && y%1>0.2))
    {
      maze[1][int(x)+1][int(y)-1][1]=(maze[1][int(x)+1][int(y)-1][1]+2)%4;
      //print(((mouseX-12)/25)+"\t"+(mouseY-12)/25);
      //println(maze[1][int((mouseX-12)/25)][int((mouseY-12)/25)][0]);
    } else if ((x%1<0.8 && x%1>0.2)&&(y%1>0.8 || y%1<0.2))
    {
      maze[1][int(x)][int(y)][0]=(maze[1][int(x)][int(y)][0]+2)%4;
      //print(((mouseX-12)/25)+"\t"+(mouseY-12)/25);
      //println(maze[1][int((mouseX-12)/25)][int((mouseY-12)/25)][1]);
    }
    affiche();
  }
  action_bouton();
}
void mouseReleased()
{
  action_bouton();
  affiche();
}
void com_zhonx()
{
  if (Zhonx.available()>0)
  {
    //oriantation_zhonx+=4;
    int val=Zhonx.read();
    switch (val)
    {
    case 0:
      oriantation_zhonx=oriantation_zhonx%4;
      break;
    case 1:
      oriantation_zhonx=(4+oriantation_zhonx-1)%4;
      break;
    case 2:
      oriantation_zhonx=(oriantation_zhonx+1)%4;
      break;
    case 3:
      oriantation_zhonx=(oriantation_zhonx+2)%4;
    default:
      break;
    }
    affiche();
    switch (oriantation_zhonx)
    {
    case 0:
      position_y_zhonx--;
      Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 3));
      Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 2));
      Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 1));
      break;
    case 1:
      position_x_zhonx++;
      Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 0));
      Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 3));
      Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 2));
      break;
    case 2:
      position_y_zhonx++;
      Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 1));
      Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 0));
      Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 3));
      break;
    case 3:
      position_x_zhonx--;
      Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 0));
      Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 3));
      Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 2));
      break;
    }
  }
}