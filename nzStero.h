#include "Arduino.h"
#include <EEPROM.h>

const int EEPROM_SIZE = 1024;

class nzStero {
public:
  nzStero();
  void initialize();
  int check(int);
  void out(int, int);
  int check_and(int, int);
  int check_or(int, int);
  int check_nand(int, int);
  int check_nor(int, int);
  int check_xor(int, int);
  void showInOut(bool, bool);  // inputs , outputs
  void setMarker(String name, int value);
  int getMarker(const char* name);
  void setCounter(String name, int value);
  int getCounter(const char* name);
  void setExtraVar(String name, char value);
  int getExtraVar(const char* name);
  int findVariableAddress(const char* name);
private:

  int input[4];
  int output[4];
  int outputVal[4];

  bool startsWith(const char* phrase, const char* letter);
  static const char* errors[];
  //void sErrors()
  int acAdres = 0;
  int countMax = 999; // zmieic format w set counter 
};

nzStero::nzStero() {
  // Konstruktor
}


void nzStero::initialize() {
  const char* errors[] = {
    "Bez błędu",
    "Zły input ",
    "Zły output",
    "Zła warotść",
    "Nie utworzono markra",
    "Marker już istnije",
    "Nie zdefionowano wartośći ",
    "Bład karty SD",
    // Dodaj inne komunikaty błędów w razie potrzeby
  };


  input[0] = 17;  // 6 zmiana kacpra wszystkie piny input;
  input[1] = 16;  // 3;
  input[2] = 15;  // 7;
  input[3] = 20;  // 5;
                  //

  output[0] = 7;
  output[1] = 6;
  output[2] = 5;
  output[3] = 3;  //zmiana kacpra

  outputVal[0] = 0;
  outputVal[1] = 0;
  outputVal[2] = 0;
  outputVal[3] = 0;

  pinMode(input[0], INPUT);
  pinMode(input[1], INPUT);
  pinMode(input[2], INPUT);
  pinMode(input[3], INPUT);

  pinMode(output[0], OUTPUT);
  pinMode(output[1], OUTPUT);
  pinMode(output[2], OUTPUT);
  pinMode(output[3], OUTPUT);

  // pinMode(7, OUTPUT);
  // pinMode(8, OUTPUT);
  // pinMode(9, OUTPUT);


  // if (!SD.begin(10)) {
  // Serial.println("SD initialization failed!");
  //  while (1)
  //  ;
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, NULL);  // Inicjalizacja wartością 0 na każdym adresie w pamięci EEPROM
  }
}


int nzStero::check(int index) {
  if (index >= 0 && index < 4) {
    if (analogRead(input[index]) > 900) {
      return 1;
    } else {
      return 0;
    }
    // return digitalRead(input[index]);
  }
  return -1;
  //sErrors()
}

void nzStero::out(int index, int val) {
  if (val == HIGH || val == LOW) {
    if (index >= 0 && index < 4) {
      if (val == 1) {
        analogWrite(output[index], 255);
      } else {
        analogWrite(output[index], 0);
      }
      outputVal[index] = val;
    } else {
      Serial.println("Error: Invalid output index");
    }
  } else {
    Serial.println("Error: Invalid value");
  }
}

int nzStero::check_and(int x, int y) {
  if (check(x) == HIGH && check(y) == HIGH) {
    return HIGH;
  }
  return LOW;
}
int nzStero::check_or(int x, int y) {
  if (check(x) == HIGH || check(y) == HIGH) {
    return HIGH;
  }
  return LOW;
}
int nzStero::check_nand(int x, int y) {
  if (check(x) == HIGH && check(y) == HIGH) {
    return LOW;
  }
  return HIGH;
}
int nzStero::check_nor(int x, int y) {
  if (check(x) == LOW || check(y) == LOW) {
    return HIGH;
  }
  return LOW;
}
int nzStero::check_xor(int x, int y) {
  if (check(x) == HIGH && check(y) == HIGH) {
    return LOW;
  }
  if (check(x) == HIGH || check(y) == HIGH) {
    return HIGH;
  }
  return LOW;
}

// void nzStero::addMarker(const char* name, int value = 0) {
//   if (startsWith(name, "M") != true) {
//     //error
//   } else {
//     addExtraVar(addExtraVar(name, value))
//   }

bool nzStero::startsWith(const char* phrase, const char* letter) {
  return (phrase[0] == letter);
}
// void nzStero::addExtraVar(String name, int value) {
//   String fullName = name + String(value);
//   int length = fullName.length();
//   for (int i = 0; i < length; i++) {
//     EEPROM.write(acAdres + i, fullName.charAt(i));
//   }
//   acAdres = acAdres + length;
// }
//{if (address == -1) {
// Zmienna nie istnieje, szukamy dostępnego miejsca w pamięci EEPROM

// address = 0;
// while (address < EEPROM_SIZE) {
//   int length = strlen(name);
//   bool found = true;
//   for (int i = 0; i < length; i++) {
//     if (EEPROM.read(address + i) != name[i]) {
//       found = false;
//       break;
//     }
//   }
//   if (found) {
//     // Zmienna o takiej nazwie już istnieje, przechodzimy do kolejnego adresu
//     address += length + 1; // Przesuń adres poza nazwę zmiennej i separator
//   } else {
//     // Znaleźliśmy dostępne miejsce, zapisujemy nazwę i wartość
//     for (int i = 0; i < length; i++) {
//       EEPROM.write(address + i, name[i]);
//     }
//     EEPROM.write(address + length, ':'); // Separator między nazwą a wartością
//     EEPROM.write(address + length + 1, value); // Zapisz wartość
//     Serial.print("Dodano zmienną: ");
//     Serial.print(name);
//     Serial.print(" na adresie: ");
//     Serial.println(address);
//     return; // Przerwij pętlę, gdy znaleziono miejsce i zapisano zmienną
//   }
// }
// Jeśli przeszukano całą pamięć i nie znaleziono miejsca, wypisz błąd
//   Serial.println("Błąd: Brak dostępnego miejsca w pamięci EEPROM!");
// } else {
//   // Jeśli zmienna istnieje, zaktualizuj jej wartość
//   address = address + strlen(name) + 1; // Przesuń adres poza nazwę zmiennej i separator
//   EEPROM.write(address, ':'); // Separator między nazwą a wartością
//   EEPROM.write(address + 1, value); // Zapisz wartość
//   Serial.print("Aktualizowano zmienną: ");
//   Serial.print(name);
//   Serial.print(" na adresie: ");
//   Serial.println(address);
// }}
void nzStero::setMarker(String name, int value) {
  if (name.length() != 4 || name[0] != 'M' || !isdigit(name[1]) || !isdigit(name[2]) || !isdigit(name[3])) {
    Serial.println("Błędna nazwa zmiennej. Oczekiwano formatu MXYZ, gdzie XYZ to trzy cyfry.");
    return;
  }
  if (value != 1 && value != 0) {
    Serial.println("Błędna wartość. Oczekiwana 0/1");
    return;
  }
  setExtraVar(name, value);
}
int nzStero::getMarker(const char* name) {

  return getExtraVar(name);
}

void nzStero::setCounter(String name, int value) {
  if (name.length() != 4 || name[0] != 'C' || !isdigit(name[1]) || !isdigit(name[2]) || !isdigit(name[3])) {
    Serial.println("Błędna nazwa zmiennej. Oczekiwano formatu CXYZ, gdzie XYZ to trzy cyfry.");
    return;
  }
  if (value >= 0  && value <= countMax) {
    Serial.print("Błędna wartość. Oczekiwany zakres 0- ");
    Serial.println(countMax);
    return;
  }
  String countFormat;
  if (value < 10) {
    countFormat = "00" + String(value);
  } else if (value < 100) {
    countFormat = "0" + String(value);
  } else {
    countFormat = String(value);
  }
   const char* countValue = countFormat.c_str();
  setExtraVar(name, countValue);
}
int nzStero::getCounter(const char* name) {

  return getExtraVar(name);
}

void nzStero::setExtraVar(String name, char value) {
  int address = findVariableAddress(name.c_str());
  if (address != -1) {
    char valueChar = value;
    // char valueChar = '0' + value;
    EEPROM.write(address, valueChar);
    Serial.print("Wartość nadpisana w adresie ");
    Serial.print(address);
    Serial.print(" na wartość ");
    Serial.println(value);
  } else {
    // Jeśli zmienna nie istnieje, dodaj ją do EEPROM
    String fullName = name + String(value);
    int length = fullName.length();
    for (int i = 0; i < length; i++) {
      EEPROM.write(acAdres + i, fullName.charAt(i));
    }
    Serial.print("Nowa wartość ");
    Serial.print(name);
    Serial.print(" dodana od adresu zaczynajacego się ");
    Serial.print(acAdres);
    Serial.print(" o wartości ");
    Serial.println(value);
    acAdres = acAdres + length;
  }
  return 0;  // Zwróć sukces
}

// int address = findVariableAddress(name.c_str());
// if (address != -1) {
//   char valueChar = '0' + value;
//   EEPROM.write(address, valueChar);
//   Serial.print("Wartość nadpisana w adresie ");
//   Serial.print(address);
//   Serial.print(" na wartość ");
//   Serial.println(value);
// } else {
//   // Jeśli zmienna nie istnieje, dodaj ją do EEPROM
//   String fullName = name + String(value);
//   int length = fullName.length();
//   for (int i = 0; i < length; i++) {
//     EEPROM.write(acAdres + i, fullName.charAt(i));
//   }
//   Serial.print("Nowa wartość dodana od adresu ");
//   Serial.print(acAdres);
//   Serial.print(" o wartości ");
//   Serial.println(value);
//   acAdres = acAdres + length;
// }
// }
// void nzStero::setExtraVar(const char* name, int value) {
//   int address = findVariableAddress(name);
//   if (address != -1) {
//     EEPROM.write(address, charAt(value));
//   }
// }

int nzStero::getExtraVar(const char* name) {
  int address = findVariableAddress(name);
  if (address != -1) {
    char charValue = EEPROM.read(address);
    return atoi(&charValue);
  }
  return -1;  // Zmienna nie znaleziona
}

int nzStero::findVariableAddress(const char* name) {
  for (int address = 0; address < EEPROM_SIZE; address++) {
    int length = strlen(name);
    bool found = true;
    for (int i = 0; i < length; i++) {
      if (EEPROM.read(address + i) != name[i]) {
        found = false;
        break;
      }
    }
    if (found) {
      return address + length;
    }
  }
  return -1;  // Zmienna nie znaleziona
}

// void nzStero::addExtraVar(const char* name, int value = 0) {
//   if (checkVarInSD(name) == false) {
//     File file = SD.open(filename, FILE_WRITE);  // Otwarcie pliku w trybie zapisu

//     if (file) {
//       char line[50];
//       sprintf(line, "%s=%d", name, value);  // Sformatowanie linii tekstu
//       file.println(line);                   // Dopisanie nowej linii do pliku
//       file.close();                         // Zamknięcie pliku
//     }
//   }
// }
// void nzStero::setExtraVar(const char* name, int value) {
//   if (checkVarInSD(name)) {
//     File originalFile = SD.open(filename, FILE_READ);
//     File tempFile = SD.open("temp.txt", FILE_WRITE);

//     if (originalFile && tempFile) {
//       while (originalFile.available()) {
//         String line = originalFile.readStringUntil('\n');
//         if (!line.startsWith(name)) {
//           tempFile.println(line);
//         } else {
//           char newline[50];
//           sprintf(newline, "%s=%d", name, value);
//           tempFile.println(newline);
//         }
//       }

//       originalFile.close();
//       tempFile.close();

//       SD.remove(filename);
//       SD.rename("temp.txt", filename);
//     }
//   }
// }

// int nzStero::getExtraVar(const char* name) {
//   File file = SD.open(filename, FILE_READ);
//   int value = -1;  // Wartość domyślna, jeśli zmienna nie zostanie znaleziona

//   if (file) {
//     while (file.available()) {
//       String line = file.readStringUntil('\n');
//       if (line.startsWith(name)) {
//         int equalsPos = line.indexOf('=');
//         if (equalsPos != -1) {
//           value = line.substring(equalsPos + 1).toInt();
//         }
//         break;  // Przerywamy pętlę, bo znaleźliśmy wartość
//       }
//     }
//     file.close();
//   }

//   return value;
// }



void nzStero::showInOut(bool input, bool output) {
  if (input == 1) {
    Serial.print("Inputs: ");
    Serial.print(check(0));
    Serial.print(" ");
    Serial.print(check(1));
    Serial.print(" ");
    Serial.print(check(2));
    Serial.print(" ");
    Serial.print(check(3));
    Serial.print(" ");
  }
  if (output == 1) {
    Serial.print("Outputs: ");
    Serial.print(outputVal[0]);
    Serial.print(" ");
    Serial.print(outputVal[1]);
    Serial.print(" ");
    Serial.print(outputVal[2]);
    Serial.print(" ");
    Serial.print(outputVal[3]);
    Serial.print(" ");
  }
  Serial.println(" ");
}