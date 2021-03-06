/*================================================================================================
Fichier:            main.ino
  Projet:             Résolution de problème et conception en génie

  Equipe:             Ingénierie Da Vinci / P14
  Auteurs:            
                      Simon St-Onge (Fonctions de mouvements)
                      Philippe B-L (Normes & Bluetooth)
                      Sébastien St-Denis (Fonctions de mouvements, normes & formes)
                      Samuel Croteau (Fonctions du crayon, optocoupleurs, écran lcd & avertisseur sonore)
                      Frédéric Gagnon (Formes)
                      William Caron (LEDs)

  Date:               04-10-2018
  Révision:           
  Compilateur:        Arduino IDE
  Description:        Code source du programme des robots LéonardUS.
                      Robots artistes, un robot contrôlé et un robot automatisé pour dessiner.

  Modification:      26-11-2018
=================================================================================================*/
#include <LibRobus.h> //Librairie de la platforme Robus (Robot)
#include <math.h>
/*===========================================================================
Defines globales & robots
===========================================================================*/
//DÉCOMMENTEZ le #define du robot que vous voulez utiliser SEULEMENT
#define ROBOTAUTONOME
//#define ROBOTMANUEL
#define DEBUG

//Si 2 robots définis, dé-defini les deux codes
#ifdef ROBOTAUTONOME
#ifdef ROBOTMANUEL
//#undef ROBOTAUTONOME
//#undef ROBOTMANUEL
#endif
#endif

#define GAUCHE 0
#define DROITE 1
#define DEVANT 1
#define DERRIERE -1

#define SOURIRE 0
#define TRISTE 1
#define BLAZE 2
//AUTRES ÉMOTIONS

#define speed0 0
#define speed1 0.20
#define vitesseTourne 0.15
#define speed3 0.20
#define speed4 0.20

#define POSITIF 1
#define NEGATIF -1

/*===========================================================================
Variables globales
===========================================================================*/
float vitesse;
int anglecrayon = 125;

int serIn;             // var that will hold the bytes-in read from the serialBuffer
char serInString[100]; // array that will hold the different bytes  100=100characters;
                       // -> you must state how long the array will be else it won't work.
int serInIndx = 0;     // index of serInString[] in which to insert the next incoming byte
int serOutIndx = 0;    // index of the outgoing serInString[] array;
char SerialRead[63];

int pinRougeDroite = 40;
int pinRougeGauche = 43;
int pinBlancheDroite = 41;
int pinBlancheGauche = 42;
/*===========================================================================
Appel des fonctions
===========================================================================*/
float PICalcul(float distanceGauche, float distanceDroite);
float PICalcultournercentre(float distanceGauche, float distanceDroite);
float distance_mm_pulse(float distance_mm);
void acceleration(float v, float vMax, float distance);
void MOTORS_reset();
double angle_degree_a_pulse(float angle);
void accel_avancer();
void accel_reculer();
void avancer(float distance_mm);
void reculer(float distance_mm);
void bruit();
void opto();
void tourner(int direction, float angle, int sens);
void tournerCentre(int direction, float angle);
void tournerCrayon(int direction, float angle);
void readSerialString();

void baisserCrayon();
void leverCrayon();

// ----- R O B O T  A U T O N O M E ------ //Appel des fonctions du robot autonome ici
//Formes
void polygone(int nbSommets, int lngrArete);
void polygoneEtoile(int nbSommets, int diffSommets, int lngrArete);
void croix(int lngrArete);
void arc(int rayon, float angle);
void ellipse(int longeur, int largeur, int t);
void spirale();
void parallelogramme(float base, float hauteur, float angle);
void emotion(int emotion, int rayon);
void electrique();
void informatique();

// ----- R O B O T  M A N U E L ----- //Appel des fonctions du robot manuel ici
void avancerLumiere();
void reculerLumiere();
void eteindreLumiere();
/*===========================================================================
Lancement
===========================================================================*/
void setup()
{
  BoardInit();

  Serial.begin(9600);
  Serial1.begin(57600);
  Serial1.setTimeout(25);
  while (!Serial);
  while (!Serial1);
  Serial1.setTimeout(25);

  pinMode(A10, INPUT);
  pinMode(A9, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode(A8, INPUT);
  pinMode(48, INPUT_PULLUP); // Pin bouton bras.

  //pinMode(45, INPUT); // Pin suiveur de ligne

  pinMode(pinRougeDroite, OUTPUT);
  pinMode(pinBlancheGauche, OUTPUT);
  pinMode(pinRougeDroite, OUTPUT);
  pinMode(pinRougeGauche, OUTPUT);

  pinMode(44, INPUT);
  
  delay(5000);
  #ifdef ROBOTMANUEL
  leverCrayon();
  #endif
  #ifdef ROBOTAUTONOME
  baisserCrayon();
  #endif
}
/*===========================================================================
Boucle infinie
===========================================================================*/
void loop()
{
  int typeForme = -1;
  int noForme = -1;

  opto();
  Serial1.readBytes(SerialRead, 64);
  //Serial.println("Lecture trame...");
  for (int i = 0; i <= 63; i++)
  {
    if (SerialRead[i] == '#')
    {
      #ifdef DEBUG
      Serial.println("# recue");
      Serial.print(SerialRead[i]);
      Serial.print(SerialRead[i + 1]);
      Serial.print(SerialRead[i + 2]);
      Serial.print(SerialRead[i + 3]);
      Serial.print(SerialRead[i + 4]);
      Serial.println(SerialRead[i + 5]);
      #endif

      for (int a = 0; a <= 63; a++)
      {
        // ----- R O B O T  A U T O N O M E ------
        #ifdef ROBOTAUTONOME
        // 0123
        // 1010
        #ifdef DEBUG
        Serial.println(" ");
        Serial.print("Lecture Trame: ");
        Serial.print(SerialRead[i + a]);
        Serial.print(SerialRead[i + a + 1]);
        Serial.print(SerialRead[i + a + 2]);
        Serial.print(SerialRead[i + a + 3]);
        Serial.println(SerialRead[i + a + 4]);
        #endif

        Serial.print("typeForme decod lue: ");

        char trameLue = SerialRead[i + a + 1];
        switch (trameLue)
        {
        case '0':
          typeForme = 00;
          break;
        case '1':
          typeForme = 10;
          break;
        case '2':
          typeForme = 20;
          break;
        case '3':
          typeForme = 30;
          break;
        case '4':
          typeForme = 40;
          break;
        case '5':
          typeForme = 50;
          break;
        case '6':
          typeForme = 60;
          break;
        case '7':
          typeForme = 70;
          break;
        case '8':
          typeForme = 80;
          break;
        case '9':
          typeForme = 90;
          break;
        }

        trameLue = SerialRead[i + a + 2];

        switch (trameLue)
        {
        case '0':
          typeForme += 0;
          break;
        case '1':
          typeForme += 1;
          break;
        case '2':
          typeForme += 2;
          break;
        case '3':
          typeForme += 3;
          break;
        case '4':
          typeForme += 4;
          break;
        case '5':
          typeForme += 5;
          break;
        case '6':
          typeForme += 6;
          break;
        case '7':
          typeForme += 7;
          break;
        case '8':
          typeForme += 8;
          break;
        case '9':
          typeForme += 9;
          break;
        }
        //typeForme = 1;
        Serial.println(typeForme);

        Serial.print("noForme decod lue: ");

        trameLue = SerialRead[i + a + 3];

        switch (trameLue)
        {
        case '0':
          noForme = 00;
          break;
        case '1':
          noForme = 10;
          break;
        case '2':
          noForme = 20;
          break;
        case '3':
          noForme = 30;
          break;
        case '4':
          noForme = 40;
          break;
        case '5':
          noForme = 50;
          break;
        case '6':
          noForme = 60;
          break;
        case '7':
          noForme = 70;
          break;
        case '8':
          noForme = 80;
          break;
        case '9':
          noForme = 90;
          break;
        }

        trameLue = SerialRead[i + a + 4];

        switch (trameLue)
        {
        case '0':
          noForme += 0;
          break;
        case '1':
          noForme += 1;
          break;
        case '2':
          noForme += 2;
          break;
        case '3':
          noForme += 3;
          break;
        case '4':
          noForme += 4;
          break;
        case '5':
          noForme += 5;
          break;
        case '6':
          noForme += 6;
          break;
        case '7':
          noForme += 7;
          break;
        case '8':
          noForme += 8;
          break;
        case '9':
          noForme += 9;
          break;
        }

        Serial.println(noForme);

        /*void polygone(int nbSommets, int lngrArete);
        void polygoneEtoile(int nbSommets,int diffSommets, int lngrArete);
        void croix(int lngrArete);
        void arc(int rayon, float angle, int t);
        void ellipse(int longeur, int largeur, int t);
        void spirale();
        void parallelogramme(float base, float hauteur, float angle);
        void emotion(int emotion, int rayon);
        void electrique();
        void informatique();*/

        switch (typeForme)
        {
        case 0:
          //Polygone de 2 à 12 arètes
          switch(noForme)
          {
            case 0:
              polygone(2, 100);
            break;
            case 1:
              polygone(3, 100);
            break;
            case 2:
              polygone(4, 100);
            break;
            case 3:
              polygone(5, 100);
            break;
            case 4:
              polygone(6, 100);
            break;
            case 5:
              polygone(7, 100);
            break;
            case 6:
              polygone(8, 100);
            break;
            case 7:
              polygone(9, 100);
            break;
            case 8:
              polygone(10, 100);
            break;
            case 9:
              polygone(11, 100);
            break;
            case 10:
              polygone(12, 100);
            break;
          }
          break;
        case 1:
          //Ellipse
          switch (noForme)
          {
          case 0:
            arc(80, 360);
            break;
          }
          break;
        case 2:
          //Spirale
          //spirale();
          break;
        case 3:
          //Parallelogramme
          switch (noForme)
          {
          case 0:
            parallelogramme(100, 100, 90);
            break;
          case 1:
            parallelogramme(100, (100 * 0.8), 90);
            break;
          case 2:
            parallelogramme(100, (100 * 0.6), 90);
            break;
          case 3:
            parallelogramme(100, (100 * 0.4), 90);
            break;
          case 4:
            parallelogramme(100, (100 * 0.2), 90);
            break;
          case 5:
            parallelogramme(100, (100 * 0.6), 120);
            break;
          case 6:
            parallelogramme(100, 100, 80);
            break;
          }
          break;
        case 4:
          //Emoji
          break;
        case 5:
          //Etoile
          switch (noForme)
          {
          case 0:
            polygoneEtoile(5, 2, 200);
            break;
          case 1:
            polygoneEtoile(7, 2, 200);
            break;
          case 2:
            polygoneEtoile(7, 3, 200);
            break;
          case 3:
            polygoneEtoile(8, 3, 200);
            break;
          }
          //polygoneEtoile(5 , 5, int lngrArete);
          break;
        case 6:
          //Autre
          switch (noForme)
          {
          case 0:
            break;
          case 1:
            break;
          case 2:
            croix(100);
            break;
            break;
          }
        }

        #endif
          // ----- R O B O T  M A N U E L -----
          #ifdef ROBOTMANUEL
          // 01234
          // G-0.2

          // Affichage de la tramme.
          #ifdef DEBUG
          //Serial.println(" ");
          //Serial.print("Lecture Trame: ");
          //Serial.print(SerialRead[i+a]);
          //Serial.print(SerialRead[i+a+1]);
          //Serial.print(SerialRead[i+a+2]);
          //Serial.print(SerialRead[i+a+3]);
          //Serial.println(SerialRead[i+a+4]);
          #endif

          eteindreLumiere();

          float vitLueConv = 0;
          int sensMoteur = 0;
          switch (SerialRead[i + a])
          {
          case 'G': // GAUCHE
            // sens:
            sensMoteur = 0;
            if (SerialRead[i + a + 1] == '+') // Détermination du signe.
              sensMoteur = POSITIF;           // Signe positif donc avance.
            else if (SerialRead[i + a + 1] == '-')
              sensMoteur = NEGATIF;

            #ifdef DEBUG
            Serial.println(" ");
            Serial.print("sensMoteur: ");
            Serial.println(sensMoteur);
            #endif

            // Valeur pour le moteur:
            if (SerialRead[i + a + 2] == '1')
            {
              vitLueConv = 0.4;
            }
            else if (SerialRead[i + a + 2] == '0')
            {
              char vitToConv = SerialRead[i + a + 4];
              switch (vitToConv)
              {
              case '0':
                vitLueConv = 0;
                break;
              case '1':
                vitLueConv = 0.04;
                break;
              case '2':
                vitLueConv = 0.08;
                break;
              case '3':
                vitLueConv = 0.12;
                break;
              case '4':
                vitLueConv = 0.16;
                break;
              case '5':
                vitLueConv = 0.20;
                break;
              case '6':
                vitLueConv = 0.24;
                break;
              case '7':
                vitLueConv = 0.28;
                break;
              case '8':
                vitLueConv = 0.32;
                break;
              case '9':
                vitLueConv = 0.36;
                break;
              }

              #ifdef DEBUG
              Serial.println(" ");
              Serial.print("Valeur moteur G: ");
              Serial.print(vitLueConv);
              #endif

              if (vitLueConv == 0.0f)
                eteindreLumiere();
              if (sensMoteur == POSITIF)
              {
                avancerLumiere();
                MOTOR_SetSpeed(GAUCHE, vitLueConv);
              }
              if (sensMoteur == NEGATIF)
              {
                reculerLumiere();
                MOTOR_SetSpeed(GAUCHE, (vitLueConv - vitLueConv - vitLueConv));
              }
            }
            break;
          case 'D': // DROIT

            // sens:
            sensMoteur = 0;
            if (SerialRead[i + a + 1] == '+') // Détermination du signe.
              sensMoteur = POSITIF;           // Signe positif donc avance.
            else if (SerialRead[i + a + 1] == '-')
              sensMoteur = NEGATIF;

            #ifdef DEBUG
            Serial.println(" ");
            Serial.print("sensMoteur: ");
            Serial.println(sensMoteur);
            #endif

            // Valeur pour le moteur:
            if (SerialRead[i + a + 2] == '1')
            {
              vitLueConv = 0.4;
            }
            else if (SerialRead[i + a + 2] == '0')
            {
              char vitToConv = SerialRead[i + a + 4];

              switch (vitToConv)
              {
              case '0':
                vitLueConv = 0;
                break;
              case '1':
                vitLueConv = 0.04;
                break;
              case '2':
                vitLueConv = 0.08;
                break;
              case '3':
                vitLueConv = 0.12;
                break;
              case '4':
                vitLueConv = 0.16;
                break;
              case '5':
                vitLueConv = 0.20;
                break;
              case '6':
                vitLueConv = 0.24;
                break;
              case '7':
                vitLueConv = 0.28;
                break;
              case '8':
                vitLueConv = 0.32;
                break;
              case '9':
                vitLueConv = 0.36;
                break;
              }

              #ifdef DEBUG
              Serial.println(" ");
              Serial.print("Valeur moteur D: ");
              Serial.print(vitLueConv);
              #endif

              if (vitLueConv == 0.0f)
                eteindreLumiere();
              if (sensMoteur == POSITIF)
              {
                avancerLumiere();
                MOTOR_SetSpeed(DROITE, vitLueConv);
              }
              if (sensMoteur == NEGATIF)
              {
                reculerLumiere();
                MOTOR_SetSpeed(DROITE, (vitLueConv - vitLueConv - vitLueConv));
              }
            }
            break;
          case 'H': // HAUT
            #ifdef DEBUG
            Serial.println(" ");
            Serial.print("Lever crayon");
            #endif

            if (!digitalRead(45))
              leverCrayon();
            break;
          case 'B': // BAS
            #ifdef DEBUG
            Serial.println(" ");
            Serial.print("baisser crayon");
            #endif

            if (!digitalRead(45))
              baisserCrayon();
            break;
          default:
            break;
          }
          #endif

          #ifdef DEBUG
          //Serial.print(SerialRead[i+a]);
          #endif
          SerialRead[i + a] = 0;
        }
      }
    }
    delay(10); // Delais pour décharger le CPU
  }
/*===========================================================================
Définition des fonctions
===========================================================================*/

// ----- 2  R O B O T S ------
float PICalcul(float distanceGauche, float distanceDroite)
{
  float erreur = 0;
  float proportionnel = 0;
  float integral = 0;
  float PIresultant = 0;

  erreur = distanceGauche - distanceDroite;       //Calcul de l'erreur
  proportionnel = erreur * 0.03f;                 //P
  integral = (integral + (erreur * 0.1f)) * 0.0f; //I

  //P=20, I=20    //PI=40 -> 40 tick de plus a faire
  PIresultant = (proportionnel + integral) / 100; //Calcul PI en pulse
  //Serial.print("PIOUT: ");
  //Serial.println(PIresultant);
  return (PIresultant);
}
float PICalcultournercentre(float distanceGauche, float distanceDroite)
{
  float erreur = 0;
  float proportionnel = 0;
  float integral = 0;
  float PIresultant = 0;

  erreur = distanceGauche - distanceDroite;//Calcul de l'erreur
  proportionnel = erreur * 0.01f;//P
  integral = (integral + (erreur * 0.1f)) * 0.01f;//I

  //P=20, I=20    //PI=40 -> 40 tick de plus a faire
  PIresultant = (proportionnel+integral)/2500;//Calcul PI en pulse
  //Serial.print("PIOUT: ");
  //Serial.println(PIresultant);
  return (PIresultant);
}
float distance_mm_pulse(float distance_mm)
{
  // déterminer la circonference d'une roue en mm et en pulse
  float diametre_roue_mm = 76;
  float circonference_roue_mm = 3.1416 * diametre_roue_mm;
  float circonference_roue_pulse = 3200;
  float distance_pulse = (distance_mm / circonference_roue_mm) * circonference_roue_pulse;
  return distance_pulse;
}
void acceleration(float *v, float vVoulue, float distance)
{
  float distanceAvantFrein = distance - ((-((*v) * (*v))) / (2 * (0.01f / 0.035f)));
  while (*v != vVoulue && ENCODER_Read(GAUCHE) < distance_mm_pulse(distanceAvantFrein))
  {
    if (*v < vVoulue)
      *v = *v + 0.01f;
    else if (*v > vVoulue)
      *v = *v - 0.01f;

    MOTOR_SetSpeed(GAUCHE, *v);
    MOTOR_SetSpeed(DROITE, *v);
    delay(35);
  }
  while (*v != 0)
  {
    if (*v < 0)
      *v = *v + 0.01f;
    else if (*v > 0)
      *v = *v - 0.01f;

    MOTOR_SetSpeed(GAUCHE, *v);
    MOTOR_SetSpeed(DROITE, *v);
    delay(35);
  }
}
void MOTORS_reset()
{
  MOTOR_SetSpeed(GAUCHE, speed0);
  MOTOR_SetSpeed(DROITE, speed0);
  ENCODER_Reset(GAUCHE);
  ENCODER_Reset(DROITE);
  delay(150);
}
double angle_degree_a_pulse(float angle)
{
  // déterminer la circonference d'une roue en mm et en pulse
  double diametre_roue_mm = 76;
  double circonference_roue_mm = 3.1416 * diametre_roue_mm;
  double circonference_roue_pulse = 3200;
  // déterminer la circonference des 2 roues en mm et en pulse
  double diametre_2_roues_mm = 187;
  double quart_circonference_2_roues_mm = (3.1416 * 2 * diametre_2_roues_mm) / 4;
  double quart_circonference_2_roues_pulse = (quart_circonference_2_roues_mm / circonference_roue_mm) * circonference_roue_pulse;
  // déterminer le rapport pulses par degré
  double pulses_par_degre_2_roues = quart_circonference_2_roues_pulse / 90;
  // déterminer le nombre de pulses requis pour arriver à l'angle demandé
  double pulses_pour_angle_x;

  pulses_pour_angle_x = angle * pulses_par_degre_2_roues;

  return pulses_pour_angle_x;
}
void accel_avancer()
{
  float accel = 0;
  while (accel <= speed1)
  {
    accel = accel + 0.01;
    MOTOR_SetSpeed(GAUCHE, accel);
    MOTOR_SetSpeed(DROITE, accel);
    delay(35);
  }
}
void accel_reculer()
{
  float accel = 0;
  while (accel <= speed1)
  {
    accel = accel + 0.01;
    MOTOR_SetSpeed(GAUCHE, -accel);
    MOTOR_SetSpeed(DROITE, -accel);
    delay(35);
  }
}
void avancer(float distance_mm)
{
  float distance_pulse, distgauche1, distdroite1;
  float k;
  float speed = speed1;
  int counter = 0;
  distance_pulse = distance_mm_pulse(distance_mm);

  while (ENCODER_Read(GAUCHE) <= distance_pulse)
  {
    //accel
    if (counter == 0)
    {
      accel_avancer();
      counter = 1;
    }
    //deccel
    if (ENCODER_Read(GAUCHE) >= (distance_pulse / 1.3))
    {
      distgauche1 = ENCODER_Read(GAUCHE);
      distdroite1 = ENCODER_Read(DROITE);
      k = PICalcul(distgauche1, distdroite1);
      speed = speed3 + k;

      MOTOR_SetSpeed(GAUCHE, speed3);
      MOTOR_SetSpeed(DROITE, speed);
      delay(100);
      counter = 2;
    }
    //vitesse intermédiaire
    if (counter == 1)
    {
      distgauche1 = ENCODER_Read(GAUCHE);
      distdroite1 = ENCODER_Read(DROITE);
      k = PICalcul(distgauche1, distdroite1);
      speed = speed1 + k;

      MOTOR_SetSpeed(GAUCHE, speed1);
      MOTOR_SetSpeed(DROITE, speed);
      delay(100);
    }
  }
  MOTORS_reset();
}
void reculer(float distance_mm)
{
  float distance_pulse = distance_mm_pulse(distance_mm);
  while (ENCODER_Read(GAUCHE) >= -distance_pulse)
  {
    MOTOR_SetSpeed(GAUCHE, -speed1);
    MOTOR_SetSpeed(DROITE, -speed1);
  }
  MOTORS_reset();
}
void bruit()
{
  int peizoPin = 44;
  tone(peizoPin, 3000, 500);
  delay(1000);
}
void opto()
{
  int compteur = 0;
  int surfaceblanche = 100;
  int optocoupleur = analogRead(A6);

  if (optocoupleur < surfaceblanche)
  {
    while (optocoupleur < surfaceblanche && compteur <= 200)
    {
      optocoupleur = analogRead(A6);
      compteur++;
      Serial.println(compteur);
    }
  }
  if (compteur > 200) // Hors du tableau
  {
    while (optocoupleur < surfaceblanche)
    {
      //leverCrayon();
      bruit();
      optocoupleur = analogRead(A6);
    }
  }
  noTone(44);
  compteur = 0;
}
void tourner(int direction, float angle, int sens)
{
  float angle_pulse;

  angle_pulse = angle_degree_a_pulse(angle); //détermine le nombre de pulse pour arriver à l'angle demandé

  if ((direction == GAUCHE && sens == DEVANT) || (direction == DROITE && sens == DERRIERE))
  {
    while (ENCODER_Read(DROITE) <= angle_pulse)
    {
      MOTOR_SetSpeed(GAUCHE, speed0);
      MOTOR_SetSpeed(DROITE, sens * speed4);
    }
  }
  if ((direction == DROITE && sens == DEVANT) || (direction == GAUCHE && sens == DERRIERE))
  {
    while (ENCODER_Read(GAUCHE) <= angle_pulse)
    {
      MOTOR_SetSpeed(GAUCHE, sens * speed4);
      MOTOR_SetSpeed(DROITE, speed0);
    }
  }
  MOTORS_reset();
}
void tournerCentre(int direction, float angle)
{
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  float anglePulse = angle_degree_a_pulse(angle);//Variable en pulse selon l'angle
  float distgauche1;
  float distdroite1;
  float k;
  float speed;

  if(direction == GAUCHE)
  {
    while(distdroite1 <= (anglePulse/2))
    {
      distgauche1 = ENCODER_Read(GAUCHE);
      distdroite1 = ENCODER_Read(DROITE);
      k = PICalcultournercentre(distgauche1,distdroite1);
      speed = vitesseTourne+k;
      MOTOR_SetSpeed(GAUCHE,-speed);
      MOTOR_SetSpeed(DROITE,vitesseTourne);
    }
  }

  else if(direction == DROITE)
  {
    while(distgauche1 <= anglePulse/2)
    {
      distgauche1 = ENCODER_Read(GAUCHE);
      distdroite1 = ENCODER_Read(DROITE);
      k = PICalcultournercentre(distgauche1,distdroite1);
      speed = vitesseTourne+k;
      MOTOR_SetSpeed(GAUCHE,vitesseTourne);
      MOTOR_SetSpeed(DROITE,-speed);
    }
  }
  MOTORS_reset();
}
/*void tournerCentre(int direction, float angle)
{
  double anglePulse = angle_degree_a_pulse(angle); //Variable en pulse selon l'angle

  if (direction == GAUCHE)
  {
    while (ENCODER_Read(DROITE) <= anglePulse / 2)
    {
      MOTOR_SetSpeed(GAUCHE, -vitesseTourne);
      MOTOR_SetSpeed(DROITE, vitesseTourne);
    }
  }
  else if (direction == DROITE)
  {
    while (ENCODER_Read(GAUCHE) <= anglePulse / 2)
    {
      MOTOR_SetSpeed(GAUCHE, vitesseTourne);
      MOTOR_SetSpeed(DROITE, -vitesseTourne);
    }
  }
  MOTORS_reset();
}*/
void tournerCrayon(int direction, float angle)
{
  leverCrayon();
  avancer(168);
  tournerCentre(direction, angle);
  reculer(165);
  baisserCrayon();
}
void readSerialString()
{
  int sb = 0;
  if (Serial.available())
  {
    while (Serial.available())
    {
      sb = Serial.read();
      serInString[serInIndx] = sb;
      serInIndx++;
    }
  }
}

// ----- R O B O T  A U T O N O M E ------ //Définitions des fonctions du robot autonome ici
#ifdef ROBOTAUTONOME
void baisserCrayon()
{
  int actif = 1;
  anglecrayon = 125;
  SERVO_Enable(0);
  SERVO_SetAngle(0, anglecrayon);
  delay(100);

  while (actif == 1)
  {
    //Serial.println(anglecrayon);
    SERVO_SetAngle(0, (anglecrayon));
    delay(20);
    //ROBUS_IsBumper(1)
    if (!digitalRead(48))
    {
      actif = 0;
      SERVO_Disable(0);
      Serial.println(anglecrayon);
    }
    anglecrayon--;
    if (anglecrayon == 27)
    {
      anglecrayon = 27;
      SERVO_Disable(0);
      actif = 0; //POUR L'INSTANT
    }
  }
  SERVO_Disable(0);
}
void leverCrayon()
{
  SERVO_Enable(0);

  delay(100);
  while (anglecrayon < 125)
  {
    anglecrayon++;
    SERVO_SetAngle(0, anglecrayon);
    delay(20);
  }
  delay(20);
  //SERVO_Disable(0);
}
//Formes
void polygone(int nbSommets, int lngrArete)
{
  double sommeAngles = (nbSommets - 2) * 180;
  double angle = 180 - (sommeAngles / nbSommets);
  for (int tournant = 0; tournant < nbSommets; tournant++)
  {
    avancer(lngrArete);
    tournerCrayon(DROITE, angle);
  }
}
void parallelogramme(int base, int hauteur, float angle)
{
  for (int diagonale = 0; diagonale < 2; diagonale++)
  {
    avancer(base);
    tournerCrayon(GAUCHE, 180 - angle);
    avancer(hauteur / cos(angle - 90));
    tournerCrayon(GAUCHE, angle);
  }
  leverCrayon();
  reculer(167);
}
void polygoneEtoile(int nbSommets, int diffSommets, int lngrArete)
{
  float angle = 180 * (nbSommets - 2 * diffSommets) / nbSommets;
  float angleExterne = 180 - angle;
  float angleInterne = 360 * (diffSommets - 1) / nbSommets;

  tournerCrayon(DROITE, 90);
  for(int i = 0; i < nbSommets ; i++)
  {
    avancer(lngrArete);
    tournerCrayon(GAUCHE, angleExterne);
  }
  leverCrayon();
  avancer(167);
  tournerCentre(GAUCHE, 90);
  reculer(192);
}
void croix(int lngrArete)
{
  avancer(125);
  for (int i = 0; i < 4; i++)
  {
    avancer(lngrArete);
    tournerCrayon(DROITE, 90);
    avancer(lngrArete);
    tournerCrayon(GAUCHE, 90);
    avancer(lngrArete);
    tournerCrayon(DROITE, 90);
  }
}
void arc(int rayon, float angle)
{
  float distanceG = 2*PI*(rayon+93)*angle/360;
  float vG = 2*PI*(rayon+93)/10000;
  float vD = 2*PI*(rayon-93)/10000;
  avancer(125);

  while(ENCODER_Read(GAUCHE) <= distanceG)
  {
    MOTOR_SetSpeed(GAUCHE, vG);
    MOTOR_SetSpeed(DROITE, vD);
  }
  reculer(125);
}
/*void arc(int rayon, float angle)
{
  float anglePulse = angle_degree_a_pulse(angle); //Variable en pulse selon l'angle

  float dV = (rayon - 92.5f) / (rayon + 92.5f);

  float vG = 0.4f;
  float vD = 0.4f * dV;

  while (ENCODER_Read(DROITE) <= anglePulse)
  {
    MOTOR_SetSpeed(GAUCHE, vG);
    MOTOR_SetSpeed(DROITE, vD);
  }
}*/
void ellipse(int longeur, int largeur, int t)
{
  float vG = 2 * PI * (largeur / 2) / t;
  float vD = 2 * PI * (largeur / 2) / t;

  for (int i = 0; i < t / 4; i++)
  {
    //vG = vG + ???;
    //r = sqrt(pow(largeur/2,2) + pow(lngrArete/2,2));
    MOTOR_SetSpeed(GAUCHE, vG);
    MOTOR_SetSpeed(DROITE, vD);
  }
  for (int i = 0; i < t / 4; i++)
  {
    //vG = vG - ???;
    MOTOR_SetSpeed(GAUCHE, vG);
    MOTOR_SetSpeed(DROITE, vD);
  }
  for (int i = 0; i < t / 4; i++)
  {
    //vG = vG + ???;
    MOTOR_SetSpeed(GAUCHE, vG);
    MOTOR_SetSpeed(DROITE, vD);
  }
  for (int i = 0; i < t / 4; i++)
  {
    //vG = vG - ???;
    MOTOR_SetSpeed(GAUCHE, vG);
    MOTOR_SetSpeed(DROITE, vD);
  }
}
void spirale()
{
  float vMoins, dV;
  for (vMoins = 0.4f, dV = 0.0f; vMoins > -0.4f; vMoins = vMoins - dV, dV = 0.0005f /*dV+0.00025f*/)
  {
    MOTOR_SetSpeed(GAUCHE, vMoins);
    MOTOR_SetSpeed(DROITE, 0.4f);
    delay(50);
  }
  leverCrayon();
  avancer(300);
  delay(3000);
  MOTORS_reset();
}
/*void parallelogramme(float base, float hauteur, float angle)
{
  for (int diagonale = 0; diagonale < 2; diagonale++)
  {
    float angle1 = 180 - angle;
    float angle2 = angle - 90;
    float angle3 = 90 - angle;
    avancer(base);
    Serial.print("AVANCE de ");
    Serial.println(base);
    tournerCrayon(GAUCHE, angle1);
    Serial.print("TOURNE de ");
    Serial.println(angle1);
    if (angle >= 90)
    {
      float distance = hauteur / cos(angle2);
      avancer(distance);
      Serial.print("AVANCE de ");
      Serial.println(distance);
    }
    else
    {
      float distance = hauteur / cos(angle3);
      avancer(distance);
      Serial.print("AVANCE de ");
      Serial.println(distance);
    }
    tournerCrayon(GAUCHE, angle);
    Serial.print("TOURNE de ");
    Serial.println(angle);
  }
}*/
void emotion(int emotion, int rayon)
{
  //Contour
  arc(rayon, 360);           //1
  tournerCrayon(GAUCHE, 90); //2
  switch (emotion)
  {
  case SOURIRE:
    //Transition
    avancer(rayon / 3);        //3
    tournerCrayon(DROITE, 90); //4
    avancer(rayon / 3);        //5
    tournerCrayon(GAUCHE, 90); //6
    avancer(rayon / 3);        //7
    //Yeux
    arc(rayon / 6, 360);    //8
    avancer(rayon * 2 / 3); //9
    arc(rayon / 6, 360);    //10
    //Transition
    avancer(rayon / 3);        //11
    tournerCrayon(GAUCHE, 90); //12
    avancer(rayon / 3);        //13
    //Bouche
    arc(rayon * 2 / 3, 180); //14
    //Transition
    tournerCrayon(DROITE, 90); //15
    avancer(rayon / 3);        //16
    tournerCrayon(GAUCHE, 90); //17
    break;
  case TRISTE:
    //Transition
    avancer(rayon / 3);        //3
    tournerCrayon(DROITE, 90); //4
    avancer(rayon / 3);        //5
    tournerCrayon(GAUCHE, 90); //6
    avancer(rayon / 3);        //7
    //Yeux
    arc(rayon / 6, 360);    //8
    avancer(rayon * 2 / 3); //9
    arc(rayon / 6, 360);    //10
    //Transition
    avancer(rayon / 3);        //11
    tournerCrayon(GAUCHE, 90); //12
    avancer(rayon);            //13
    tournerCrayon(GAUCHE, 90); //14
    avancer(rayon * 4 / 3);    //15
    tournerCrayon(GAUCHE, 90); //16
    //Bouche
    arc(rayon * 2 / 3, 180); //17
    //Transition
    tournerCrayon(GAUCHE, 90); //18
    avancer(rayon * 4 / 3);    //19
    tournerCrayon(GAUCHE, 90); //20
    avancer(rayon * 2 / 3);    //21
    tournerCrayon(DROITE, 90); //22
    avancer(rayon / 3);        //23
    tournerCrayon(GAUCHE, 90); //24
    break;
  case BLAZE:
    //Transition
    avancer(rayon / 3);        //3
    tournerCrayon(DROITE, 90); //4
    avancer(rayon / 3);        //5
    tournerCrayon(GAUCHE, 90); //6
    avancer(rayon / 3);        //7
    //Yeux
    arc(rayon / 6, 360);    //8
    avancer(rayon * 2 / 3); //9
    arc(rayon / 6, 360);    //10
    //Transition
    avancer(rayon / 3);        //11
    tournerCrayon(GAUCHE, 90); //12
    avancer(rayon / 3);        //13
    //Bouche
    arc(rayon * 2 / 3, 180); //14
    //Transition
    tournerCrayon(DROITE, 90); //15
    avancer(rayon / 3);        //16
    tournerCrayon(GAUCHE, 90); //17
    break;
  default:
    //ERREUR
    break;
  }
}
void electrique()
{
  float P = 40; //distance en mm des petits côtés
  tournerCrayon(GAUCHE, 55);
  avancer(85.81);
  tournerCrayon(DROITE, 90);
  avancer(P);
  tournerCrayon(GAUCHE, 90);
  avancer(66.65);
  tournerCrayon(DROITE, 90);
  avancer(P);
  tournerCrayon(GAUCHE, 95);
  avancer(60);
  tournerCrayon(GAUCHE, 52);
  avancer(70);
  tournerCrayon(GAUCHE, 125);
  avancer(50);
  tournerCrayon(DROITE, 90);
  avancer(P);
  tournerCrayon(GAUCHE, 90);
  avancer(62.72);
  tournerCrayon(DROITE, 90);
  avancer(P);
  tournerCrayon(GAUCHE, 115);
  avancer(152.36 /*+distance entre crayon et roues*/);
  //permet de reset le robot à la position ini.
  tournerCentre(DROITE, 149);
  reculer(/*distance entre crayon et roues*/ 100);
}
void informatique()
{
  //avancer(/*distance entre crayon et roues*//100);
  tournerCentre(GAUCHE, 70);
  //reculer(/*distance entre crayon et roues*/);
  //Descendre crayon
  arc(67.7, 134.79);
  tournerCrayon(DROITE, 20);
  avancer(130);
  tournerCrayon(DROITE, 61.44);
  //reculer(/*Distance entre crayon et roues*/);
  arc(130.23, 57.36);
  tournerCrayon(DROITE, 61.1);
  avancer(130);
  leverCrayon();
  tournerCentre(GAUCHE, 180);
  //DESCENDRE CRAYON
  //avancer(90.43-2*(/*distance entre crayon et roues*/));
  tournerCrayon(GAUCHE, 90);
  avancer(125);
  leverCrayon();
  tournerCentre(GAUCHE, 180);
  //DESCENDRE CRAYON
  avancer(125 / 2);
  tournerCrayon(GAUCHE, 90);
  avancer(54.33);
  arc(62.75, 89.77);
  //Retour à la position ini.
  leverCrayon();
  //avancer(/*Distance entre crayon et roues*//100);
  tournerCentre(DROITE, 90);
  avancer(207.75);
  tournerCentre(DROITE, 90);
  //reculer(/*Distance entre crayon et roues*//100);
}
#endif

// ----- R O B O T  M A N U E L ----- //Définitions des fonctions du robot manuel ici
#ifdef ROBOTMANUEL
void baisserCrayon()
{
  int actif = 1;
  anglecrayon = 145;
  SERVO_Enable(0);
  SERVO_SetAngle(0, anglecrayon);
  delay(100);

  while (actif == 1)
  {
    //Serial.println(anglecrayon);
    SERVO_SetAngle(0, (anglecrayon));
    delay(20);
    //ROBUS_IsBumper(1)
    if (!digitalRead(48))
    {
      actif = 0;
      SERVO_Disable(0);
      Serial.println(anglecrayon);
    }
    anglecrayon--;
    if (anglecrayon == 102)
    {
      anglecrayon = 102;
      SERVO_Disable(0);
      actif = 0; //POUR L'INSTANT
    }
  }
  SERVO_Disable(0);
}
void leverCrayon()
{
  SERVO_Enable(0);

  delay(100);
  while (anglecrayon < 145)
  {
    anglecrayon++;
    SERVO_SetAngle(0, anglecrayon);
    delay(20);
  }
  delay(20);
  //SERVO_Disable(0);
}

void avancerLumiere()
{
  digitalWrite(pinBlancheGauche, HIGH);
  digitalWrite(pinBlancheDroite, HIGH);
  digitalWrite(pinRougeGauche, LOW);
  digitalWrite(pinRougeDroite, LOW);
}
void reculerLumiere()
{
  digitalWrite(pinBlancheGauche, LOW);
  digitalWrite(pinBlancheDroite, LOW);
  digitalWrite(pinRougeGauche, HIGH);
  digitalWrite(pinRougeDroite, HIGH);
}

void eteindreLumiere()
{
  digitalWrite(pinBlancheGauche, LOW);
  digitalWrite(pinBlancheDroite, LOW);
  digitalWrite(pinRougeGauche, LOW);
  digitalWrite(pinRougeDroite, LOW);
}
#endif
