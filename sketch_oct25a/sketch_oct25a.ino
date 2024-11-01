
const int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  digitalWrite(ledPin, LOW);
  Serial.println("1を入力するとLEDがON、0を入力するとLEDがOFFになります");
}

void loop() {
  if (Serial.available() > 0) {         //シリアル通信が可能か確認
    char input = Serial.read();         //シリアルからデータを読み取る
    if (input == '\n' || input == '\r') //制御文字をカット
      return;

    if (input == '1') {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED ON");
    }else if (input == '0') {
      digitalWrite(ledPin, LOW);
      Serial.println("LED OFF");
    }else {
      Serial.println("無効な入力");
      Serial.println("1または0を入力してください");
    }
  }
}