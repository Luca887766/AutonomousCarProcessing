import processing.serial.*;

Serial serial;

String temp="L0", temp1="R0",temp2="C0";

String[] l = {""};

PImage sensor;

void setup() {
  size(580, 720);                              // Dimensione schermo 1280 x 720
  sensor = loadImage("sensors.png");            // Carico l'immagine
  background(255);                              // Sfondo bianco
  fill(0);
  textSize(30);
  text("AUTONOMOUS CAR", 160,100);
  for (int i = 0; i < Serial.list().length; i++) {  // Stampo la lista delle porte disponibili
    print(i + " ");
    println(Serial.list()[i]);
  }
  String pname = Serial.list()[0];              // Scelgo dalla lista la porta con Arduino collegato
  serial = new Serial(this, pname, 9600);       // Imposto la porta da utilizzare e la velocità di trasmissione
}

void draw() {
  int len;
  String t,t1,t2;
  t = serial.readStringUntil('\n');
  t1 = serial.readStringUntil('\n');
  t2 = serial.readStringUntil('\n');
  
  if(t == null) t = temp;
  else temp = t;
  if(t1 == null) t1 = temp1;
  else temp1 = t1;
  if(t2 == null) t2 = temp2;
  else temp2 = t2;
  
  image(sensor, -25, 100);  // Stampo l'immagine a schermo
  
  

  if (t.charAt(0) == 'L') {
    l = match(t, "\\d+");
  } else if (t1.charAt(0) == 'L') {
    l = match(t1, "\\d+");
  } else if (t2.charAt(0) == 'L') {
    l = match(t2, "\\d+");
  }
  println(l[0]);
  len = (int) map(Long.parseLong(l[0]), 1, 80, 0, 350);
  strokeWeight(4);
  stroke(0);
  
  line(width/2-50, height/4*3-2, (width/2-50)+cos(radians(130))*len, (height/4*3-2)-sin(radians(130))*len);

  if (t.charAt(0) == 'R') {
    l = match(t, "\\d+");
  } else if (t1.charAt(0) == 'R') {
    l = match(t1, "\\d+");
  } else if (t2.charAt(0) == 'R') {
    l = match(t2, "\\d+");
  }
  len = (int) map(Long.parseLong(l[0]), 0, 80, 0, 350);
  line(width/2+48, height/4*3-2, (width/2+48)+cos(radians(60))*len, (height/4*3-2)-sin(radians(60))*len);

  if (t.charAt(0) == 'C') {
    l = match(t, "\\d+");
  } else if (t1.charAt(0) == 'C') {
    l = match(t1, "\\d+");
  } else if (t2.charAt(0) == 'C') {
    l = match(t2, "\\d+");
  }
  len = (int) map(Long.parseLong(l[0]), 0, 80, 0, 350);
  line(width/2, height/4*3-20, width/2, height/4*3-len-20);
  serial.clear();
}
