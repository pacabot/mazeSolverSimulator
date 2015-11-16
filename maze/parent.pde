public class parent
{
  private coordonne_point []coordonnee = new coordonne_point [0];
  private int coup=0;
  parent(int prix)
  {
    coup=prix;
  }
  void nouveau_point ( int x, int y)
  {
    coordonnee=(coordonne_point [])append(coordonnee, new coordonne_point(x, y));
  }
  int nombre_point_restant()
  {
    return coordonnee.length;
  }
  coordonne_point test_point ()
  {
    coordonne_point retour= coordonnee[coordonnee.length-1];
    coordonnee = (coordonne_point[]) shorten(coordonnee);
    return retour;
  }
  int coup_deplacement ()
  {
    return coup;
  }
};
class coordonne_point
{
  private int [] coordonne= new int [2];
  coordonne_point(int x, int y)
  {
    //coordonne_point=(coordonnee[]) append(coordonnee, this);
    coordonne[0]=x;
    coordonne[1]=y;
  }
  int get_position_x ()
  {
    return coordonne[0];
  }
  int get_position_y ()
  {
    return coordonne[1];
  }
};

