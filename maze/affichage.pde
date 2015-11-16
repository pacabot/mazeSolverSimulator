void affiche()
{
  affiche_maze();

  for (int i=0; i<poussoirs.length; i++)
  {
    poussoirs[i].affiche();
  }
  //verifie_maze(10);
}
void affiche_maze()
{
  //clearMazePoid ();
  //verifie_maze();
  pushMatrix();
  background(0);
  translate(15, 15);
  for (int y=0; y<=NOMBER_OF_WALL; y++)
  {
    pushMatrix();
    for (int x=0; x<=NOMBER_OF_WALL; x++)
    {
      //if (y<NOMBER_OF_WALL && x<NOMBER_OF_WALL)
      {
        switch(maze[1][x][y][0]) 
        {
        case 0: 
          fill(0);
          noStroke();//stroke(0);
          break;
        case 1: 
          fill(70);
          noStroke();//stroke(80);
          //          fill(100);
          //          stroke(100);
          break;
        case 2: 
          fill(255);
          noStroke();//stroke(255);
          break;
        case 3: 
          fill(30);
          noStroke();//stroke(30);
          //          fill(0);
          //          stroke(100);
          break;
        }
        rect(4, 0, WIDTH_COTE, 4);

        switch(maze[1][x][y][1]) 
        {
        case 0: 
          fill(0);
          noStroke();//stroke(0);
          break;
        case 1: 
          fill(30);
          noStroke();//stroke(30);
          //          fill(100);
          //          stroke(100);
          break;
        case 2: 
          fill(255);
          noStroke();//stroke(255);
          break;
        case 3: 
          fill(30);
          noStroke();//stroke(30);
          //          fill(0);
          //          stroke(100);
          break;
        }
        rect(0, 4, 4, WIDTH_COTE);
        fill(255);
        if (position_x_zhonx==x && position_y_zhonx==y && Serial_actif==true)
        {
          affiche_zhonx();
          println(x+";"+y);
        }
        pushMatrix();
        translate(0, -10);
        textAlign(LEFT, TOP);
        //text(maze[0][x][y][0], 5, 15);
        popMatrix();
      }
      fill(255, 0, 0);
      noStroke();//stroke(255, 0, 0);
      rect(0, 0, 4, 4);
      translate(WIDTH_COTE+4, 0);
    }
    popMatrix();
    translate(0, WIDTH_COTE+4);
  }
  popMatrix();
}
void affiche_zhonx()
{
  pushMatrix();
  translate(WIDTH_COTE/2+5,WIDTH_COTE/2+5);
  rotate(HALF_PI*oriantation_zhonx);
  translate(-WIDTH_COTE/2,-WIDTH_COTE/2);
  stroke(0);
  fill(255);
  triangle(0,WIDTH_COTE-2,WIDTH_COTE/2,0,WIDTH_COTE-2,WIDTH_COTE-2);
  popMatrix();
}
void action_bouton()
{
  if (poussoirs[1].etat==true)
  {
    saveFrame("maze####.png");
    print("save png");
  }
  if (poussoirs[6].etat==true&&Serial_actif==false)
  {
    Serial_actif=true;
    String portName = Serial.list()[0];
    Zhonx = new Serial(this, portName, 9600);
    delay(2000);
    Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 3));
    Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 2));
    Zhonx.write(un_mur(murs_alentours(position_x_zhonx, position_y_zhonx, false), 1));
  }
  if (poussoirs[5].etat==true)
  {
    open_file();
  }

  if (poussoirs[3].etat==true)
  {
    println("remplacement");
    int renplacement;
    if (poussoirs[4].etat==true)
    {
      renplacement=0;
    } else
    {
      renplacement=2;
    }
    for (int a=0; a<NOMBER_OF_WALL; a++)
    {
      for (int b=0; b<NOMBER_OF_WALL; b++)
      {
        for (int c=0; c<2; c++)
        {
          maze[1][a][b][c]=renplacement;
        }
      }
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
  }

  if (poussoirs[2].etat==true)
  {
    export_maze(100);
  }
  if (poussoirs[0].etat==true)
  {
    exit();
  }
}
void open_file()
{
  // cette fonction permet de selactioner un fichier dans l'arborécence de l'ordinateur puis de le charger en mémoire
  selectInput("Select a file to process:", "fileSelected"); // on ouvre la fenaitre de selection du fichier
  while (file_select==false)//on attend que la fenaitre soit refermer
  {
    delay(10);
  }
  file_select=false;
  if (fichier!=null /*&& fichier.endsWith(".zhonxIIImaze")*/)//on test si l'utilisateur n'a pas fermer la fenaitre de selection et si le fichier fini par ".zhonxIIImaze"
  {
    in_maze = loadStrings(fichier); // on charge dans la variable in maze le fichier
    if (in_maze.length>NOMBER_OF_WALL) // on verifie que le labyrinthe du fichier peut rentrer dans le tableau "maze"
    {
      background(255);
      fill(0);
      text("le labyrinthe sera rognier car il n'est pas de la bonne taille", 20, 20);
    }
    for (int a=0; a<NOMBER_OF_WALL; a++)
    {
      String[] ligne = split(in_maze[a], "w");
      for (int b=0; b<NOMBER_OF_WALL; b++)
      {
        if (ligne.length>NOMBER_OF_WALL)// on verifie que le labyrinthe du fichier peut rentrer dans le tableau "maze"  
        {
          background(255);
          fill(0);
          text("le labyrinthe sera rognier car il n'est pas de la bonne taille", 20, 20);
        }
        String[] cellule = split(ligne[b], "l");
        for (int c=0; c<cellule.length; c++)
        {
          println("x "+a+"\tz "+b+"\tc "+c+"\t lenght: x "+in_maze.length+",\ty "+ligne.length+",\tc "+cellule.length);
          maze[1][a][b][c]=int(cellule[c]);
        }
      }
    }
  }
}
void fileSelected(File selection) 
{
  file_select=true;
  if (selection == null) {
    println("Window was closed or the user hit cancel.");
    //exit();
  } else {
    fichier = selection.getAbsolutePath();
    println("User selected " + fichier);
  }
}
void export_maze(int nom)
{
  print("save maze");
  PrintWriter out_maze_user;
  out_maze_user = createWriter("user.txt");
  //for(int i=0; i<=
  for(int y=0; y<=NOMBER_OF_WALL;y++)
  {
    for(int x=0; x<=NOMBER_OF_WALL;x++)
    {
      out_maze_user.print("*");
      switch (maze[1][x][y][0])
      {
        case 0:
          out_maze_user.print("   ");
          break;
        case 2:
          out_maze_user.print("===");
          break;
      }      
    }
    out_maze_user.println();
    for(int x=0; x<=NOMBER_OF_WALL; x++)
    {
      switch (maze[1][x][y][1])
      {
        case 0:
          out_maze_user.print("    ");
          break;
        case 2:
          out_maze_user.print("|   ");
          break;
      } 
    }
    out_maze_user.println();
  }
  out_maze_user.flush(); // Write the remaining data
  out_maze_user.close(); // Finish the file
  
  PrintWriter out_maze_zhonx_c;
  out_maze_zhonx_c = createWriter("zhonx c.txt");
  out_maze_zhonx_c.print("{{");
  for (int a=0; a<NOMBER_OF_WALL; a++)
  {
    out_maze_zhonx_c.print("{");
    for (int b=0; b<NOMBER_OF_WALL; b++)
    {
      out_maze_zhonx_c.print("{");
      if (maze[1][a][b][0] == 0)
      {
        out_maze_zhonx_c.print("NO_WALL");
      }
      else
      {
        out_maze_zhonx_c.print("WALL_PRESENCE");
      }
      out_maze_zhonx_c.print(",");
      if (maze[1][a][b+1][0] == 0)
      {
        out_maze_zhonx_c.print("NO_WALL");
      }
      else
      {
        out_maze_zhonx_c.print("WALL_PRESENCE");
      }
      out_maze_zhonx_c.print(",");
      if (maze[1][a+1][b][1] == 0)
      {
        out_maze_zhonx_c.print("NO_WALL");
      }
      else
      {
        out_maze_zhonx_c.print("WALL_PRESENCE");
      }
      out_maze_zhonx_c.print(",");
      if (maze[1][a][b][1] == 0)
      {
        out_maze_zhonx_c.print("NO_WALL");
      }
      else
      {
        out_maze_zhonx_c.print("WALL_PRESENCE");
      }
      out_maze_zhonx_c.print(",2000},");
    }
    out_maze_zhonx_c.print("},");
  }
  out_maze_zhonx_c.print("}}");
  out_maze_zhonx_c.println();
  out_maze_zhonx_c.flush();
  out_maze_zhonx_c.close();


  PrintWriter out_maze_processing;
  int a,b;
  out_maze_processing = createWriter("processing_maze.zhonx"); //<>//
  for (a=0; a<NOMBER_OF_WALL; a++)
  {
    for (b=0; b<NOMBER_OF_WALL; b++)
    {
      out_maze_processing.print(maze[1][a][b][0]+"l");
      out_maze_processing.print(maze[1][a][b][1]);
      if (b<NOMBER_OF_WALL)
      {
        out_maze_processing.print("w");
      }
      //print("\t");
    }
    out_maze_processing.println(); //<>//
  }
  out_maze_processing.flush(); //<>//
  out_maze_processing.close();
}