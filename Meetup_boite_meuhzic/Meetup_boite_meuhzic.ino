#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(D5, D6); // RX, TX

DFPlayerMini_Fast myMP3;

const int switchPin = D1;
const unsigned long tempoButton = 250;

bool readyToPlay;
bool buttonStart;
bool buttonStartTempo;

int nbTrack = 0;
int nextTrack = 0;

unsigned long tempoStart;
unsigned long tempoEnd;

void setup()
{
  Serial.begin(115200);

  mySerial.begin(9600);
  myMP3.begin(mySerial);

  pinMode(switchPin, INPUT);
  digitalWrite(switchPin, HIGH);

  nbTrack = myMP3.numSdTracks();
  Serial.print("nb track: ");
  Serial.println(nbTrack);
}

void loop()
{
  //lissage detecteur de mouvement
  buttonStart = digitalRead(switchPin);//Read the value of pin2
  if (buttonStart) {
    tempoStart = millis();
    if (millis() - tempoEnd > tempoButton) {
      //la boite est penchée
      buttonStartTempo = true;
    }
  } else {
    tempoEnd = millis();
    if (millis() - tempoStart > tempoButton) {
      //la boite est droite
      buttonStartTempo = false;
    }
  }

  //lancer Mp3
  if (readyToPlay and buttonStartTempo) { //si la boite viens d'etre penché
    if (not(myMP3.isPlaying())) {//si le module MP3 n'est pas occupé
      myMP3.volume(25);
      delay(20);

      nextTrack = random(1, nbTrack + 1); //choisi un titre aléatoirement
      myMP3.play(nextTrack); //joue le titre choisi

      Serial.print("play track: "); //log port série
      Serial.println(nextTrack);
    }

    readyToPlay = false; //raz front montant

  } else if (!buttonStartTempo) {
    readyToPlay = true; //set front montant
  }
}
