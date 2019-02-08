#include<Wire.h>
#include <Servo.h>

#define coilAp 5  // coil A + �֐ڑ�
#define coilAn 6  // coil A -
#define coilBp 7  // coil B +
#define coilBn 8  // coil B -

Servo servo1;

int t   = 2; // �p���X���[�g�ݒ�@�P�ʁFms
int deg = 0;
unsigned long pos   = 0x80000000;   // ��]�ʒu�L�^�p�ϐ�
unsigned int  micro = 128;          // �}�C�N���X�e�b�v�ω���(256�ɑ΂���l���w��j
                                    // 256/micro = �}�C�N���X�b�e�b�v������

void setup() {
    servo1.attach(9);
    Serial.begin(9600);
    pinMode(coilAp, OUTPUT);
    pinMode(coilAn, OUTPUT);
    pinMode(coilBp, OUTPUT);
    pinMode(coilBn, OUTPUT);
    }

void loop() {
    if (Serial.available() > 0) {
        char mode = Serial.read();
        switch (mode) {
            case 'f':
                forwardRotate(); // �����J��
                Serial.println(0);
                break;

            case 'b':
                backwardRotate(); // ����߂�
                Serial.println(0);
                break;

            case 'o':
                Serial.println(getWeight(1)); // �d�ʂ̎擾(�O)
                break;

            case 'i':
                Serial.println(getWeight(2)); // �d�ʂ̎擾(��)
                break;

            default:
                Serial.println(0);        
        }        
    }
}

void forwardRotate() {
    for (deg = 0;deg < 180; deg++) {
        servo1.write(deg);  // deg�x�܂ŉ�]
        delay(10);          // 20ms�ҋ@
    }

    for (int i = 0; i < (50 * 16); i++) {
        cwP(); // CW������1�p���X
    }
}

void backwardRotate() {
    for (int i = 0; i < (50 * 16); i++) {
        ccwP(); // CCW������1�p���X
    }
}

void cwP() {          // CW������1�p���X
    pos = pos + micro;
    microStep(pos);
}

void ccwP() {         // CCW������1�p���X
    pos = pos - micro;
    microStep(pos);
}

void microStep(unsigned long p) {      // �}�C�N���X�e�b�v�Ń��[�^�[�쓮
    unsigned int pwm;
    unsigned int phase;
    pwm = p & 0xFF;             // ����8�r�b�g��PWM�̒l(micro step�ʁj
    phase = (p >> 8) & 0x07;    // �r�b�g��8,9,10�ňʑ�

    switch (phase) {
        case 0:
            analogWrite(coilAp, 255);
            analogWrite(coilAn,  0);
            analogWrite(coilBp, pwm);
            analogWrite(coilBn,  0);
            break;

        case 1:
            analogWrite(coilAp, 255 - pwm);
            analogWrite(coilAn,  0);
            analogWrite(coilBp, 255);
            analogWrite(coilBn,  0);
            break;

        case 2:
            analogWrite(coilAp, 0);
            analogWrite(coilAn, pwm);
            analogWrite(coilBp, 255);
            analogWrite(coilBn,  0);
            break;

        case 3:
            analogWrite(coilAp, 0);
            analogWrite(coilAn, 255);
            analogWrite(coilBp, 255 - pwm);
            analogWrite(coilBn,  0);
            break;

        case 4:
            analogWrite(coilAp, 0);
            analogWrite(coilAn, 255);
            analogWrite(coilBp, 0);
            analogWrite(coilBn, pwm);
            break;

        case 5:
            analogWrite(coilAp, 0);
            analogWrite(coilAn, 255 - pwm);
            analogWrite(coilBp, 0);
            analogWrite(coilBn, 255);
            break;

        case 6:
            analogWrite(coilAp, pwm);
            analogWrite(coilAn, 0);
            analogWrite(coilBp, 0);
            analogWrite(coilBn, 255);
            break;

        case 7:
            analogWrite(coilAp, 255);
            analogWrite(coilAn, 0);
            analogWrite(coilBp, 0);
            analogWrite(coilBn, 255 - pwm);
            break;

        default:
    }
    delay(t);
}

// �d�ʃZ���T�[����l���擾����֐�
unsigned long  getWeight(int mode) {
    unsigned long start, sum;
    int result, vol, cnt;

    // �����l�̑��
    start = millis();
    sum = result = vol = cnt = 0;

    // �l�̎擾
    while (millis() < start + 1000) {
        if (mode == 1) {
            sum += analogRead(A1);    
        } else {
            sum += analogRead(A2);
        }
        cnt += 1;
    }

    // ���ʂ̏o��
    result = sum / cnt;
    return result;
}