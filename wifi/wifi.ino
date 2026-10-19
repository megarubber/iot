#include <WiFi.h>
#include <ESP_Google_Sheet_Client.h>
#include <ESP32Servo.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>

Adafruit_ADS1115 ads;
Servo servo;

#define MOTOR_PIN 18
#define PROJECT_ID "projeto-do-dragao-guerreiro"
#define CLIENT_EMAIL "esp32iot@projeto-do-dragao-guerreiro.iam.gserviceaccount.com"
#define SPREADSHEET_ID "1dnOn1AiQJhA3T02qwPbDCbBpHlYDftc1H0ZB63vDMWQ"
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDQJzllNtELX+JQ\n09FQjN1B41374tYiC+svSWiHDW0st0AkzvSxStTbPN+iDYl7uMr5bUkGCfsw7p8e\nugbeEbz9CIE52A5d/km47KtjOV7gXBWf65fntq4aoFVkCosBb1sfIsOYfehaBsJL\nBwQYw66u+XS7q/6BE8MhezVysBAZPvYpf84pi+r27NResxE6CTfecpJ+ZER/2sBU\nECS5rSx8TyXPRCguClhsP/AE0DdZT5MmmuQdCodVjb1U/iuVcxh0Fz4J8tlXWGnO\nybxYZ91A7oZPp1Mhtn70LwGJRc4wITA+96UGk2hqLrjMN/RZ07DpXSsk1Ur7ai66\nkK5bePTtAgMBAAECggEALiSD1KwSKzNWOBS8kd6oLbL5Yp0mWuXJfPII+edADGmT\nDpXSKHsuNGuKJ6JYl9SfKivPxEbyKCa9n1Ids5y7CFyaSPhcNvnn3daZJ6XQImxH\nXuwbAIK+lah1RXgQ2ICW8pACTAudNlfJOliTz+mRD7i85914zaRT3VgywPnGl9G2\nn9jcUJ0C56+sGJZgVOHGOUvrB3VH1YGbSC5USjHwHL1MfSuhp7wJu4oGRBVqPThP\nRTPYRdWB+yekd53899ol9XGGgXmm9lTVLQpWdXRefkWHjlgNaniZp1gDedD0QuOJ\nvMKRtKJ/oud/9ZyYT0OwpNdxFQMCRFmfG5ZseMYtAQKBgQD4kgegJOYZieBCkd7p\npLY+y/fSTPg7gBjVPhOD0ZJVI5frV3zvF3sGvgxLEuIu+gIP8nzSjb25omhl3Sg4\ngHy6Iqrqtx0u9Vkz7HA9Btk2h7EUS+v9m3PmffnA8kkolM02h1FFnTIVg8MR9bEn\ns+M/aH/RFRD71a0NXpG18JiH+wKBgQDWX+/KC4gnDW01LcWeXOY7IfLHvvMYmPcw\n1YkYlIqWzlRjKhoiMRpJ70Nmq4V2kFCs+WtdOwog3COoHZJ8SG6DQlk5nUN5Czvo\nCUQCJEGczykLCfD6Ici5u0DnLt+kIRx+OR/qR8FjuJ7p/X6o6j0h2K/a+Es6t9r0\ncHrhmDjaNwKBgQD4YXtLM/PViydTfc2ALH2LbGdbdlwJoKuv0ZLQgoa1mpaArPCD\nPTRK/XbtQZZQGLQc2VKPVuFBcJ7fQsmhBAseN+wQFCAOP03+i4FwCbrdOWPiAydy\nSHbc1/7MJY6jBWW12iuE41obk4lGFfsK0D8AqlwmQeWxr8d3d4skgaD5JwKBgBqs\n526xzqhlYFB8uJmDH9b923MFjMRjzGaUO7IGUdSAKjoItCXLDGVk03NEJqBhRtiv\nVK/ON+1s1zcFLwQuEypcKDQmII5AWpz+AituDYgQsx+/he/qIksSRPo/d+65fA0s\n2I7TjPn2sDeXppooQ6EhkIhj2yCcXi+Ki4/k3DYzAoGBAOTkdNsNuqvC8U6hpz3a\naAyJnCiO+B9NvRUeLOq/SR5K2xFFpULDZUOtXuPDyltHn+fsFdD65yh3ARXVjqHJ\nKO1FowP5bbbPsHV/4w1zmNx5G7qsxi10uGuwzIRal5T53lr89KulEPyR++rBc6R+\nF46uBrKeUdEbNH0Dm5qPxLvn\n-----END PRIVATE KEY-----\n";

bool leituraConcluida = false;
bool gsheetInitialized = false;

unsigned long ultimoEnvio = 0;
const unsigned long intervaloEnvio = 5000; // Intervalo de 5 segundos entre envios

// 3 variáveis com valores 1, 2 e 3
int dado1 = 1;
int dado2 = 2;
int dado3 = 3;

String ssid = "";
String password = "";

String readSerialLine() {
  while (Serial.available() == 0) {
    delay(50);
  }
  String input = Serial.readStringUntil('\n');
  input.trim();
  return input;
}

void conectarWifiPelaSerial() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("\n--- Internet Scan ---");
    int n = WiFi.scanNetworks();
    
    if (n == 0) {
      Serial.println("Nenhuma rede detectada.");
      delay(3000);
      continue;
    }

    Serial.print("Foram encontradas ");
    Serial.print(n);
    Serial.println(" redes:");
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(" dBm)");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " [Open]" : " [Locked]");
      delay(10);
    }

    // Limpa caracteres residuais no buffer da serial
    while (Serial.available() > 0) {
      Serial.read();
    }

    Serial.println("\nQual o nome (SSID) da rede que pretende conectar?");
    ssid = readSerialLine();
    Serial.println("Rede selecionada: " + ssid);

    Serial.println("Qual a senha dessa rede? Se for pública digite 0:");
    password = readSerialLine();

    Serial.print("A conectar a " + ssid);
    if (password == "0" || password.length() == 0) {
      WiFi.begin(ssid.c_str());
    } else {
      WiFi.begin(ssid.c_str(), password.c_str());
    }

    int t = 0;
    while (WiFi.status() != WL_CONNECTED && t < 20) {
      Serial.print(".");
      delay(500);
      t++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConectado ao Wi-Fi com sucesso!");
      Serial.print("IP ESP32: ");
      Serial.println(WiFi.localIP());
      Serial.print("RSSI: ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");
    } else {
      Serial.println("\nFalha ao conectar. Tente novamente.");
      WiFi.disconnect();
      delay(2000);
    }
  }
}

void adicionarLinha(int v1, int v2) {
  FirebaseJson response;
  FirebaseJson valueRange;

  // 1. Cria a linha
  FirebaseJsonArray row;
  row.add(v1);
  row.add(v2);

  // 2. Cria a matriz de valores e adiciona a linha dentro dela
  FirebaseJsonArray values;
  values.add(row);

  // 3. Atribui a matriz ao campo "values" do objeto principal
  valueRange.set("values", values);

  //Serial.println("\nEnviando dados para a planilha...");

  // Envia com append para a primeira aba
  bool success = GSheet.values.append(&response, SPREADSHEET_ID, "Página1!A1", &valueRange);
  /*
  if (success) {
    Serial.println("Dados gravados com sucesso!");
    response.toString(Serial, true);
    Serial.println();
  } else {
    Serial.print("Erro ao gravar: ");
    Serial.println(GSheet.errorReason());
  }
  */
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.disconnect();
  WiFi.setAutoReconnect(true);
  // Executa o scan e a conexão interativa via Serial
  conectarWifiPelaSerial();
  // Inicia o processo de autenticação do Google Sheets
  Serial.println("\nA iniciar autenticação com o Google Sheets...");
  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);
  gsheetInitialized = true;
  servo.attach(MOTOR_PIN);
  Wire.begin();
  while(!ads.begin(0x48)) Serial.println("Failed to initialize.");
  Serial.println("Start ADS1115.");
}

void loop() {
  // Se perder conexão com a rede, reconecta
  if (WiFi.status() != WL_CONNECTED) {
    conectarWifiPelaSerial();
    return;
  }

  // Verifica continuamente a autenticação do token do Google
  bool ready = GSheet.ready();

  if (ready) {
    // 1. Lê os 3 primeiros dados da tabela
    if (!leituraConcluida) {
      FirebaseJson response;

      Serial.println("\nA obter valores da folha de cálculo...");
      Serial.println("---------------------------------------------------------------");

      // Lê as células A1:A3 da aba Página1
      bool success = GSheet.values.get(&response, SPREADSHEET_ID, "Página1!A1:A3");

      if (success) {
        Serial.println("Dados obtidos com sucesso:");
        response.toString(Serial, true);
        Serial.println();
        leituraConcluida = true;
      } else {
        Serial.print("Erro ao ler planilha: ");
        Serial.println(GSheet.errorReason());
      }
    } 
    // 2. Depois de ler, fica enviando continuamente a cada intervalo (5s)
    else {
      if (millis() - ultimoEnvio >= intervaloEnvio) {
        ultimoEnvio = millis();
        int16_t adc0;
        adc0 = ads.readADC_SingleEnded(0);
        Serial.print("ADC0 Raw: ");
        Serial.println(adc0);
        int angle = map(adc0, 13000, 17200, 0, 180);
        servo.write(angle);
        Serial.print("Servo Angle: "); 
        Serial.println(angle);
        
        adicionarLinha((int)adc0, angle);
      }
    }
  }
}