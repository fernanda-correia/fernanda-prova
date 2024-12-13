#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 2 // Pino utilizado para controle do led verde
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led amarelo

long tempoAnterior = 0;
long tempo_amarelo = 1000;

const int botaoPin = 18;  // Pino utilizado para o controle do botão
int botaoEstado = 0;  // Variável para leitura do estado do botão

const int ldrPin = 4;  // Pino utilizado para o controle do LDR
int limite=600;

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);

  // Inicialização das entradas
  pinMode(botaoPin, INPUT); // Inicializa o botão como input do ESP32

  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  /*while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)
*/
  // Verifica estado do botão
  botaoEstado = digitalRead(botaoPin);
  if (botaoEstado == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("HTTP Código de Resposta: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Código de Erro: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Desconectado");
  }
}

void loop() {
  int ldrEstado=analogRead(ldrPin);

  if(ldrEstado<=limite){
    if (millis() >= tempoAnterior + tempo_amarelo) {
    tempoAnterior = millis(); 
    digitalWrite(led_amarelo, HIGH);
    }
    else{digitalWrite(led_amarelo, LOW);} 

    Serial.print("Está escuro, ligue a luz");
    Serial.println(ldrEstado);
  }

  else{
    Serial.print("Está claro, desligue a luz");
    Serial.println(ldrEstado);
  }
}