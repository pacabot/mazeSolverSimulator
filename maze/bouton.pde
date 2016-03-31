donnees_poussoir[] poussoirs;

class donnees_poussoir
{
  String nom;
  int taille_x;
  int taille_y;
  int pos_x;
  int pos_y;
  boolean etat;
  boolean maintien; // metttre false si il faut rester appuier pour le maintenir a true
  boolean a_mouse_state; // il sera stoquer dans cette variable si la sourie été enfoner lors de la dernière demande
  donnees_poussoir(String a, int b, int c, float d, int e, boolean f)
  {
    poussoirs=(donnees_poussoir[]) append(poussoirs, this);
    nom=a;
    taille_x=b;
    taille_y=c;
    pos_x=int(d);
    pos_y=e;
    maintien=f;
    etat = false;
    if (!maintien)
    {
      a_mouse_state=false;
    }
    textAlign(CENTER, TOP);
    fill(0);
    affiche();
  }
  void affiche()
  {
    if (mousePressed && mouseX>pos_x && mouseX<pos_x+taille_x && mouseY>pos_y && mouseY<pos_y+taille_y)
    {
      if (maintien)
      {
        etat=true;
      } else if (!a_mouse_state)
      {
        etat=!etat;
        a_mouse_state=true;
      }
    } else
    {
      if (maintien)
      {
        etat=false;
      } else
      {
        a_mouse_state=false;
      }
    }

    if (etat)
    {
      fill (0);
      stroke(255);
    } else
    {
      fill (255);
      stroke(0);
    }

    rect(pos_x, pos_y, taille_x, taille_y);
    fill (255);
    stroke(255);
    text(nom, pos_x+taille_x/2, pos_y+taille_y);
  }
}

