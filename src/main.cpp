#include <Keypad.h>
#include "ansi.h"

#define LINE_COUNT 4
#define COLUMN_COUNT 4

byte pinosLinhas[] = { 30, 28, 26, 24 };
byte pinosColunas[] = { 38, 36, 34, 32 };

char matriz_teclas[LINE_COUNT][COLUMN_COUNT] = {
  { 1, 2, 3, 4 },
  { 5, 6, 7, 8 },
  { 9, 10, 11, 12 },
  { 13, 14, 15, 16 }
};

int keys[LINE_COUNT*COLUMN_COUNT][2] = {
  {0, 0},
  {0, 1},
  {0, 2},
  {0, 3},
  {1, 0},
  {1, 1},
  {1, 2},
  {1, 3},
  {2, 0},
  {2, 1},
  {2, 2},
  {2, 3},
  {3, 0},
  {3, 1},
  {3, 2},
  {3, 3}
};


int resposta[4][4] = {
  {false, false, true, false},
  {true, false, false, false},
  {false, false, false, true},
  {false, true, false, false}
};

bool ganhou = false;

bool tentativa[4][4] = {
  {false,false,false,false},
  {false,false,false,false},
  {false,false,false,false},
  {false,false,false,false},
};

int tentativa_64[8][8] = {
  {false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false},
  {false,false,false,false,false,false,false,false},
};

// B em 8 bits
int asnwer[8][8] = {
  {1,1,1,1,1,1,1,0},
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,0},
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,0},
};

uint8_t colors[8][8] = {
  {ANSI::red, ANSI::red, ANSI::red, ANSI::red, ANSI::blue, ANSI::blue, ANSI::blue, ANSI::blue},
  {ANSI::red, ANSI::red, ANSI::red, ANSI::red, ANSI::blue, ANSI::blue, ANSI::blue, ANSI::blue},
  {ANSI::red, ANSI::red, ANSI::red, ANSI::red, ANSI::blue, ANSI::blue, ANSI::yellow, ANSI::yellow},
  {ANSI::red, ANSI::red, ANSI::red, ANSI::red, ANSI::blue, ANSI::blue, ANSI::yellow, ANSI::yellow},
  {ANSI::red, ANSI::red, ANSI::magenta, ANSI::magenta, ANSI::yellow, ANSI::yellow, ANSI::yellow, ANSI::yellow},
  {ANSI::red, ANSI::red, ANSI::magenta, ANSI::magenta, ANSI::yellow, ANSI::yellow, ANSI::yellow, ANSI::yellow},
  {ANSI::magenta, ANSI::magenta, ANSI::magenta, ANSI::magenta, ANSI::yellow, ANSI::yellow, ANSI::yellow, ANSI::yellow},
  {ANSI::magenta, ANSI::magenta, ANSI::magenta, ANSI::magenta, ANSI::yellow, ANSI::yellow, ANSI::yellow, ANSI::yellow},
}; 



ANSI ansi = ANSI(&Serial);
Keypad teclado = Keypad(makeKeymap(matriz_teclas), pinosLinhas, pinosColunas, LINE_COUNT, COLUMN_COUNT);
void to_64(int i, int j);
void print_64();
void check_response();
void print_answer();

void setup() {
  Serial.begin(9600);   
  print_64();
}

void loop() {
  int tecla_pressionada = (int)teclado.getKey();  //VERIFICA SE ALGUMA DAS TECLAS FOI PRESSIONADA

  if (tecla_pressionada) {                 //SE ALGUMA TECLA FOR PRESSIONADA, FAZ
    int i = keys[tecla_pressionada-1][0];
    int j = keys[tecla_pressionada-1][1];
    tentativa[i][j] = !tentativa[i][j];
    to_64(i*2, j*2);
    print_64();
    Serial.println();
    check_response();
    if(ganhou){
      print_answer();
    }
  }
}

void check_response(){
  ganhou = true;
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      if(tentativa[i][j] != resposta[i][j]){
        ganhou = false;
        break;
      }
    }
  }
}

void to_64(int i, int j){
  tentativa_64[i][j] = !tentativa_64[i][j];
  tentativa_64[i][j+1] = !tentativa_64[i][j+1];
  tentativa_64[i+1][j] = !tentativa_64[i+1][j];
  tentativa_64[i+1][j+1] = !tentativa_64[i+1][j+1];
}

void print_64(){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(tentativa_64[i][j]){
        ansi.background(ANSI::black);
        ansi.foreground(ANSI::black);
      } else {
        ansi.background(colors[i][j]);
        ansi.foreground(colors[i][j]);
      }
      Serial.print(tentativa_64[i][j]);
      ansi.background(ANSI::black);
      ansi.foreground(ANSI::white | ANSI::bright);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void print_answer() {
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(asnwer[i][j]){
        ansi.background(ANSI::white);
        ansi.foreground(ANSI::white | ANSI::bright);
      } else {
        ansi.background(ANSI::black);
        ansi.foreground(ANSI::black);
      }
      Serial.print(asnwer[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  ansi.foreground(ANSI::white | ANSI::bright);
}