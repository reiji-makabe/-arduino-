
const int buttonPin = 2;      //ボタンの状態を検知するピン
const int ledPin = 8;
int buttonState = 0;          //導通（ボタン押下）していれば1
int counter = 0;              //確認用なのでここでは無視

void setup() {
  pinMode(ledPin, OUTPUT);    //LEDピンをアウトプット用に設定
  pinMode(buttonPin, INPUT_PULLUP);  //ボタンの状態を検知するピン
  Serial.begin(9600);         //確認用なのでここでは無視
}

void loop() {
  buttonState = digitalRead(buttonPin);   //ボタンの状態を検知する

  counter++;                              //確認用なのでこのブロックは無視
  if (counter >= 10000) {
    Serial.println(buttonState);
    counter = 0;
  }

  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
