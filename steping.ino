#include<Wire.h>
#include <Servo.h>

#define coilAp 5  // coil A + へ接続
#define coilAn 6  // coil A -
#define coilBp 7  // coil B +
#define coilBn 8  // coil B -

Servo servo1;

int t   = 2; // パルスレート設定　単位：ms
int deg = 0;
unsigned long pos   = 0x80000000;   // 回転位置記録用変数
unsigned int  micro = 128;          // マイクロステップ変化量(256に対する値を指定）
                                    // 256/micro = マイクロスッテップ分割数

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
                forwardRotate(); // 扉を開く
                Serial.println(0);
                break;

            case 'b':
                backwardRotate(); // 扉を閉める
                Serial.println(0);
                break;

            case 'o':
                Serial.println(getWeight(1)); // 重量の取得(外)
                break;

            case 'i':
                Serial.println(getWeight(2)); // 重量の取得(内)
                break;

            default:
                Serial.println(0);        
        }        
    }
}

void forwardRotate() {
    for (deg = 0;deg < 180; deg++) {
        servo1.write(deg);  // deg度まで回転
        delay(10);          // 20ms待機
    }

    for (int i = 0; i < (50 * 16); i++) {
        cwP(); // CW方向へ1パルス
    }
}

void backwardRotate() {
    for (int i = 0; i < (50 * 16); i++) {
        ccwP(); // CCW方向へ1パルス
    }
}

void cwP() {          // CW方向へ1パルス
    pos = pos + micro;
    microStep(pos);
}

void ccwP() {         // CCW方向へ1パルス
    pos = pos - micro;
    microStep(pos);
}

void microStep(unsigned long p) {      // マイクロステップでモーター駆動
    unsigned int pwm;
    unsigned int phase;
    pwm = p & 0xFF;             // 下位8ビットはPWMの値(micro step量）
    phase = (p >> 8) & 0x07;    // ビットの8,9,10で位相

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

// 重量センサーから値を取得する関数
unsigned long  getWeight(int mode) {
    unsigned long start, sum;
    int result, vol, cnt;

    // 初期値の代入
    start = millis();
    sum = result = vol = cnt = 0;

    // 値の取得
    while (millis() < start + 1000) {
        if (mode == 1) {
            sum += analogRead(A1);    
        } else {
            sum += analogRead(A2);
        }
        cnt += 1;
    }

    // 結果の出力
    result = sum / cnt;
    return result;
}