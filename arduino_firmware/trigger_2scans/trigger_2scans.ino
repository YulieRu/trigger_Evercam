#define PARSE_AMOUNT 2         // число значений в массиве, который хотим получить
#define start_com '[' 
#define end_com ']' 
#define delCom '=' 
#define MOS_PIN1 3
unsigned long timing=0;

double commandsData[PARSE_AMOUNT];     // массив численных значений после парсинга
boolean recievedFlag;
boolean getStarted;
byte index;
String string_convert = "";
String commandName = "";
int ifGreeting = 0;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(100);
  pinMode(MOS_PIN1, OUTPUT);
  digitalWrite(MOS_PIN1, LOW);
}

void greeting() {
  Serial.println("Enter the command: [RECORD=numImpulses pause(ms)]");  
  ifGreeting = 1;
}

void parsing() {
  if (Serial.available() > 0) {
    char incomingByte = Serial.read();        // обязательно ЧИТАЕМ входящий символ
    if (getStarted) {                         // если приняли начальный символ (парсинг разрешён)
      if (incomingByte != ' ' && incomingByte != end_com) {   // если это не пробел И не конец
        string_convert += incomingByte;       // складываем в строку
      } else {                                // если это пробел или ; конец пакета
        commandsData[index] = string_convert.toDouble();  // преобразуем строку в int и кладём в массив
        string_convert = "";                  // очищаем строку
        index++;                              // переходим к парсингу следующего элемента массива
      }
    } else {
      commandName += incomingByte;
    }
    if (incomingByte == delCom) {             // если это '='
      getStarted = true;                      // поднимаем флаг, что можно парсить
      index = 0;                              // сбрасываем индекс
      string_convert = "";                    // очищаем строку 
    }

    if (incomingByte == end_com) {                // если таки приняли ] - конец парсинга
      getStarted = false;                     // сброс
      recievedFlag = true;                    // флаг на принятие
      commandName=commandName.substring(commandName.indexOf("[")+1, commandName.indexOf("="));
    }
  }
}

void record(int *Array){
  for (byte i = 0; i < PARSE_AMOUNT; i++) { // выводим элементы массива
      Serial.print(Array[i]); Serial.print(" ");
    } Serial.println();

  for (int i=0; i < Array[0]; i++){
    Serial.print(i);
    digitalWrite(MOS_PIN1, HIGH);
    delay(Array[1]);
    digitalWrite(MOS_PIN1, LOW);
  }
}

void loop() {
  if (ifGreeting == 0) {greeting();};
  parsing();       // функция парсинга
  if (recievedFlag) {                           // если получены данные
    recievedFlag = false;
  
  if (commandName.equalsIgnoreCase("RECORD")){
    //Serial.println("RECORD");
    //record(commandsData);
    commandsData[1] *= 1000;
    for (int i=0; i < commandsData[0]; i++){
      //Serial.print(i);
      digitalWrite(MOS_PIN1, HIGH);
      delayMicroseconds(commandsData[1]);
      digitalWrite(MOS_PIN1, LOW);
    }
  } else {
    Serial.println("No such command. Try RECORD");
  }
    ifGreeting=0;
    commandName = "";
  }
}
