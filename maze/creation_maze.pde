int creat_maze(int nombre)
{
  while (verifie_maze (nombre)==false)
  {
    nombre++;
    boolean modification=true;
    int x=0, y=0, z=0;
    while (modification==true)
    {
      if (maze[1][x][y][z]==0)
      {
        maze[1][x][y][z]=2;
        modification=false;
      } else
      {
        maze[1][x][y][z]=0;
        if (z>0)
        {
          z=0;
          if (y>=NOMBER_OF_WALL)
          {
            y=0;
            if (x>=NOMBER_OF_WALL)
            {
              noLoop();
            } else
            {
              x++;
            }
          } else
          {
            y++;
          }
        } else
        {
          z=1;
        }
      }
    }
  }
  return nombre;
}
boolean verifie_maze(int n_labyrinthe)
{
  if (n_labyrinthe==-2147483647||n_labyrinthe==0)
  {
    export_maze(0);
    println("save"+n_labyrinthe);
  }
  for (int x=0; x<NOMBER_OF_WALL-1; x++)
  {
    if (maze[1][x][0][0]!=2 || maze[1][x][NOMBER_OF_WALL-1][0]!=2 || maze[1][0][x][1]!=2 || maze[1][NOMBER_OF_WALL-1][x][1]!=2)
    {
      return false;
    }
  }

  if (verifie_arrivee(7, 7)==true)
  {
      println("arrivee trouvee!");
    } else
  {
    println("pas bon maze (accune arrivee trouvee)");
    delay(200);
    return false;
  }
  parent point_a_verifier= new parent(1);
  point_a_verifier.nouveau_point(7, 7);
  while (point_a_verifier.nombre_point_restant ()!=0)
  {
    point_a_verifier=verifie_chemin(point_a_verifier);
    if (maze[0][0][NOMBER_OF_WALL-1][0]!=0)
    {
      break;
    }
  }
  if (maze[0][0][NOMBER_OF_WALL-1][0]==0)
  {
    print("pas bon maze (pas de chemain vers l'arrivée)\nmaze[0][0][");
    print(NOMBER_OF_WALL-1+"][0]="+maze[0][0][NOMBER_OF_WALL-1][0]);
    return false;
  }
  println("bon maze!!");
  delay(200);
  return true;
}
boolean verifie_maze()
{
  for (int x=0; x<NOMBER_OF_WALL-1; x++)
  {
    if (maze[1][x][0][0]!=2 || maze[1][x][NOMBER_OF_WALL][0]!=2)
    {
      println ("verifier contour maze 0 :"+x);
      return false;
    }
    if (maze[1][0][x][1]!=2 || maze[1][NOMBER_OF_WALL][x][1]!=2)
    {
      println ("verifier contour maze 1 : "+x);
      return false;
    }
    
  }

  if (verifie_arrivee(7, 7)==true)
  {
      println("arrivee trouvee!");
    } else
  {
    println("pas bon maze (accune arrivee trouvee)");
    delay(200);
    return false;
  }
  println("verifie maze");
  parent point_a_verifier= new parent(1);
  point_a_verifier.nouveau_point(7, 7);
  while (point_a_verifier.nombre_point_restant ()!=0)
  {
    point_a_verifier=verifie_chemin(point_a_verifier);
    if (maze[0][0][NOMBER_OF_WALL-1][0]!=0)
    {
      break;
    }
  }
  if (maze[0][0][NOMBER_OF_WALL-1][0]==0)
  {
    print("pas bon maze (pas de chemain vers l'arrivée)\nmaze[0][0][");
    print(NOMBER_OF_WALL-1+"][0]="+maze[0][0][NOMBER_OF_WALL-1][0]);
    return false;
  }
  println("bon maze!!");
  delay(200);
  return true;
}
boolean verifie_arrivee(int x, int y)
{
  int x_y=murs_alentours(x, y, false);
  int x1_y=murs_alentours(x+1, y, false);
  int x_y1=murs_alentours(x, y+1, false);
  int x1_y1=murs_alentours(x+1, y+1, false);

  delay(10);
  if (un_mur(x_y, 3)+un_mur(x_y, 2)+un_mur(x1_y, 2)+un_mur(x1_y, 1)+un_mur(x1_y1, 1)+un_mur(x1_y1, 0)+un_mur(x_y1, 3)+un_mur(x_y1, 0)==7)
  {
    if (un_mur(x_y, 1)+un_mur(x_y, 0)+un_mur(x1_y, 0)+un_mur(x1_y, 3)+un_mur(x1_y1, 3)+un_mur(x1_y1, 2)+un_mur(x_y1, 1)+un_mur(x_y1, 2)==0)
    {
      return true;
    }
  }
  return false;
}

parent verifie_chemin(parent point_a_verifier)
{
  int coup = point_a_verifier.coup_deplacement();
  parent nouveaux_point_a_verifier= new parent(coup+1);
  while (point_a_verifier.nombre_point_restant ()>0)
  {
    coordonne_point point=point_a_verifier.test_point();

    int x=point.get_position_x();
    int y=point.get_position_y();


    int murs = murs_alentours(x, y, true);

    int direction=0;//maze[0][x][y][1];
    //maze[0][x][y][0]=0;
    if (un_mur(murs, 3)==0 && direction!=1 && maze[0][x][y][0]==0)
    {
      nouveaux_point_a_verifier.nouveau_point(x-1, y);
      maze[0][x-1][y][1]=-1;
    }
    if (un_mur(murs, 2)==0 && direction!=-2 && maze[0][x][y][0]==0)
    {
      nouveaux_point_a_verifier.nouveau_point(x, y-1);
      maze[0][x][y-1][1]=-2;
    }
    if (un_mur(murs, 1)==0 && direction!=-1 && maze[0][x][y][0]==0)
    {
      nouveaux_point_a_verifier.nouveau_point(x+1, y);
      maze[0][x+1][y][1]=1;
    }  
    if (un_mur(murs, 0)==0  && direction!=2 && maze[0][x][y][0]==0)
    {
      nouveaux_point_a_verifier.nouveau_point(x, y+1);
      maze[0][x][y+1][1]=2;
    }
    maze[0][x][y][0]=coup;
  }
  return nouveaux_point_a_verifier;
}

int murs_alentours(int x, int y, boolean niveau_de_conaissance)
{
  //println("a la localité x="+x+" y="+y+" les murs present sont :\n "+maze[1][x][y][0]+"\n"+maze[1][x][y][1]+" "+maze[1][x+1][y][1]+"\n "+maze[1][x][y+1][0]/*+
  // "\n"+maze[1][x][y][1]+""+maze[1][x][y][0]+""+maze[1][x+1][y][1]+""+maze[1][x][y+1][0]*/);
  int retour=0;
  if (maze[1][x][y][1]==2 ||(niveau_de_conaissance==true && maze[1][x][y][1]==1))
  {
    retour=1000;
  }
  if (maze[1][x][y][0]==2 ||(niveau_de_conaissance==true && maze[1][x][y][0]==1))
  {
    retour=retour+100;
  }
  if (maze[1][x+1][y][1]==2 ||(niveau_de_conaissance==true && maze[1][x+1][y][1]==1))
  {
    retour=retour+10;
  }
  if (maze[1][x][y+1][0]==2 ||(niveau_de_conaissance==true && maze[1][x][y+1][0]==1))
  {
    retour=retour+1;
  }
  return retour;
}
int un_mur(int murs, int mur_a_acceder)
{
  return (murs/int(pow(10, mur_a_acceder)))%10;
}
void clearMazePoid ()
{
  for(int y =0; y<NOMBER_OF_WALL; y++)
  {
    for(int x =0; x<NOMBER_OF_WALL; x++)
    {
      maze[0][x][y][0]=0;
    }
  }
}
 void move_maze(int x, int y)
 {
   int xi = 0, xmax = NOMBER_OF_WALL, xmin = 0, xdepart = 0, xsigne = 1; //<>//
   int yi = 0, ymax = NOMBER_OF_WALL, ymin = 0, ydepart = 0, ysigne = 1;
   if (x == -1)
   {
     xdepart = NOMBER_OF_WALL;
     xsigne = -1;
     xmin = 1;
     xmax = NOMBER_OF_WALL + 1;
   }
   else if (x == 0)
   {
     xdepart = 0;
     xmin = -1;
     xmax = NOMBER_OF_WALL + 1;
   }
   else
   {
     xdepart=0;
     xmin = -1;
     xmax = NOMBER_OF_WALL;
   }
   if (y == -1)
   {
     ysigne = -1;
     ydepart= NOMBER_OF_WALL;
     ymin = 0;
     ymax = NOMBER_OF_WALL + 1;
   }
   else if (y == 0)
   {
     ydepart=0;
     ymin = -1;
     ymax = NOMBER_OF_WALL + 1;
   }
   else
   {
     ydepart=0;
     ymin = -1;
     ymax = NOMBER_OF_WALL;
   }
   println("x " + x + " y " + y + " xi " + xi+ " yi " + yi + " xmax " + xmax + " ymax " + ymax + " xmin " + xmin + " ymin " + ymin + " signe x " + xsigne + " signe y " + ysigne); //<>//
   for (xi = xdepart; xi> xmin && xi < xmax; xi+= xsigne)
   {
     for (yi =ydepart; yi > ymin &&  yi < ymax; yi+= ysigne) //<>//
     {
       println("x " + x + " y " + y + " xi " + xi+ " yi " + yi + " xmax " + xmax + " ymax " + ymax);
       maze [1][xi][yi][0]=maze [1][xi + x][yi + y][0];
       maze [1][xi][yi][1]=maze [1][xi + x][yi + y][1];
     } //<>//
   }
 }