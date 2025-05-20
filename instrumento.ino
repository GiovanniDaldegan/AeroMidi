#include "SimpleSwitch.h"

const int TEXT_FORMAT = 1;
const int BINARY_FORMAT = 2;
const int NUMERO_DE_BOTOES = 8;                                     // número de botões do instrumento
const int botoes[NUMERO_DE_BOTOES] = {2, 3, 4, 6, 7, A4, A3, A2};   // pinos dos botões
byte alturas[NUMERO_DE_BOTOES] = {72, 71, 69, 67, 65, 64, 62, 60};         // notas MIDI (G3 até G4 - escala de Sol Maior)
bool notaAtiva[NUMERO_DE_BOTOES] = {false, false, false, false,
                                    false, false, false, false};
const int micPin = A1;                                              // pino analógico do microfone
const int micDigitalPin = A0;                                       // pino digital do sensor de som
SimpleSwitch botoesSwitch[NUMERO_DE_BOTOES];

const int numLeituras = 14;     // número de leituras para calcular média
int leiturasMic[numLeituras];   // array para armazenar as leituras do microfone
int indiceLeitura = 0;
int somaLeituras = 0;
const int valorSilencio = 288;  // valor médio do microfone sem sopro

void sendMidi(byte statusByte, byte dataByte1, byte dataByte2, int format);

void setup() {
    Serial.begin(115200);

    for (int i = 0; i < NUMERO_DE_BOTOES; i++) {
        botoesSwitch[i].Define(botoes[i], MOMENTARY_ACTION, DEFAULT_OPEN_PULL_UP, 20, 1);
    }
    
    pinMode(micDigitalPin, INPUT);
    
    for (int i = 0; i < numLeituras; i++) {
        leiturasMic[i] = 0;
    }
}

void loop() {
    somaLeituras -= leiturasMic[indiceLeitura];
    leiturasMic[indiceLeitura] = analogRead(micPin);
    somaLeituras += leiturasMic[indiceLeitura];
    indiceLeitura = (indiceLeitura + 1) % numLeituras;
    int mediaMic = somaLeituras / numLeituras;
        
    int intensidadeSopro = abs(mediaMic - valorSilencio);
    // int velocity = map(intensidadeSopro, 0, 600, 0, 127);

    bool somDetectado = digitalRead(micDigitalPin) == HIGH;

    // sendMidi(0xB0, 0x02, velocity, BINARY_FORMAT);

    for (int i = 0; i < NUMERO_DE_BOTOES; i++) {
        botoesSwitch[i].debounce();

        if (botoesSwitch[i].changedState()) {
            if (botoesSwitch[i].on()) {
                sendMidi(0x90, alturas[i], 60, BINARY_FORMAT);
                // if (intensidadeSopro > 40) sendMidi(0x90, alturas[i], 60, BINARY_FORMAT);
                // else if (intensidadeSopro > 20) sendMidi(0x90, alturas[i], 30, BINARY_FORMAT);
                // else sendMidi(0x90, alturas[i], 0, BINARY_FORMAT);
                notaAtiva[i] = true;
            }
            else {
                sendMidi(0x80, alturas[i], 0, BINARY_FORMAT);
                notaAtiva[i] = false;
            }
        }
    }
}

void sendMidi(byte statusByte, byte dataByte1, byte dataByte2, int format) {
    switch(format) {
        case BINARY_FORMAT:
            byte serialBuff[3];
            serialBuff[0] = statusByte;
            serialBuff[1] = dataByte1;
            serialBuff[2] = dataByte2;
            Serial.write(serialBuff, 3);
            break;
        case TEXT_FORMAT:
            String message = String(statusByte,HEX);
            message += " ";
            if(dataByte1 < 0x10) message += "0";
            message += String(dataByte1,HEX);
            message += " ";
            if(dataByte2 < 0x10) message += "0";
            message += String(dataByte2,HEX);
            Serial.println(message);
            break;
    }
}