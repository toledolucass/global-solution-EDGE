//Autor: Fábio Henrique Cabrini
//Resumo: Esse programa possibilita ligar e desligar o led onboard, além de mandar o status para o Broker MQTT possibilitando o Helix saber
//se o led está ligado ou desligado.
//Revisões:
//Rev1: 26-08-2023 Código portado para o ESP32 e para realizar a leitura de luminosidade e publicar o valor em um tópico aprorpiado do broker 
//Autor Rev1: Lucas Demetrius Augusto 
//Rev2: 28-08-2023 Ajustes para o funcionamento no FIWARE Descomplicado
//Autor Rev2: Fábio Henrique Cabrini
//Rev3: 1-11-2023 Refinamento do código e ajustes para o funcionamento no FIWARE Descomplicado
//Autor Rev3: Fábio Henrique Cabrini
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Configurações - variáveis editáveis
const char* default_SSID = "Wokwi-GUEST"; // Nome da rede Wi-Fi
const char* default_PASSWORD = ""; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "46.17.108.113"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/dreamGuard/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/dreamGuard/attrs"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_2 = "/TEF/dreamGuard/attrs/accelX"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_3 = "/TEF/dreamGuard/attrs/accelY";
const char* default_TOPICO_PUBLISH_4 = "/TEF/dreamGuard/attrs/accelZ";
const char* default_TOPICO_PUBLISH_5 = "/TEF/dreamGuard/attrs/gX";
const char* default_TOPICO_PUBLISH_6 = "/TEF/dreamGuard/attrs/gY";
const char* default_TOPICO_PUBLISH_7 = "/TEF/dreamGuard/attrs/gZ";
const char* default_ID_MQTT = "fiware_dreamGuard"; // ID MQTT

const int default_D4 = 2; // Pino do LED onboard

const char* topicPrefix = "dreamGuard";

// Declaração da variável para o prefixo do tópico
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
Adafruit_MPU6050 mpu;
 
// Defina uma variável global para armazenar os valores do acelerômetro
float accelX, accelY, accelZ;
float gX, gY, gZ;

// Variáveis para configurações editáveis
char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);
char* TOPICO_PUBLISH_3 = const_cast<char*>(default_TOPICO_PUBLISH_3);
char* TOPICO_PUBLISH_4 = const_cast<char*>(default_TOPICO_PUBLISH_4);
char* TOPICO_PUBLISH_5 = const_cast<char*>(default_TOPICO_PUBLISH_5);
char* TOPICO_PUBLISH_6 = const_cast<char*>(default_TOPICO_PUBLISH_6);
char* TOPICO_PUBLISH_7 = const_cast<char*>(default_TOPICO_PUBLISH_7);
char* ID_MQTT = const_cast<char*>(default_ID_MQTT);
int D4 = default_D4;

WiFiClient espClient;
PubSubClient MQTT(espClient);
char EstadoSaida = '0';

void initSerial() {
    Serial.begin(115200);
}

void initWiFi() {
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi();
}

void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);
}

void setup() {
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();
    delay(5000);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) // Don't proceed, loop forever
      delay(10);
    }
 
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE, 0);
    display.setCursor(35, 10);
    display.print("ESP32");
    display.setCursor(25, 40);
    display.print("MPU6050");
    display.display();
 
    // Try to initialize!
    if (!mpu.begin()) {
      Serial.println("Failed to find MPU6050 chip");
      while (1) {
        delay(10);
      }
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("");
    delay(1000);

    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
}

void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED)
        return;
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());

    // Garantir que o LED inicie desligado
    digitalWrite(D4, LOW);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (int i = 0; i < length; i++) {
        char c = (char)payload[i];
        msg += c;
    }
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);

    // Forma o padrão de tópico para comparação
    String onTopic = String(topicPrefix) + "@on|";
    String offTopic = String(topicPrefix) + "@off|";

    // Compara com o tópico recebido
    if (msg.equals(onTopic)) {
        digitalWrite(D4, HIGH);
        EstadoSaida = '1';
    }

    if (msg.equals(offTopic)) {
        digitalWrite(D4, LOW);
        EstadoSaida = '0';
    }
}

void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected())
        reconnectMQTT();
    reconectWiFi();
}

void EnviaEstadoOutputMQTT() {
    if (EstadoSaida == '1') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|on");
        Serial.println("- Led Ligado");
    }

    if (EstadoSaida == '0') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|off");
        Serial.println("- Led Desligado");
    }
    Serial.println("- Estado do LED onboard enviado ao broker!");
    delay(1000);
}

void InitOutput() {
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);
    boolean toggle = false;

    for (int i = 0; i <= 10; i++) {
        toggle = !toggle;
        digitalWrite(D4, toggle);
        delay(200);
    }
}

void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE);
        } else {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000);
        }
    }
}

void leituraMPU() {
    /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  accelX = a.acceleration.x;
  accelY = a.acceleration.y;
  accelZ = a.acceleration.z;
 
  gX = g.gyro.x;
  gY = g.gyro.y;
  gZ = g.gyro.z;
 
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE, 0);
  display.setCursor(0, 0);
  display.print("Acelerometro: ");
  display.setCursor(0, 12);
  display.print("X: ");
  display.print(accelX);
  display.print(" m/s^2");
 
  display.setCursor(0, 24); // Mova o cursor para a próxima linha
  display.print("Y: ");
  display.print(accelY);
  display.print(" m/s^2");
 
  display.setCursor(0, 36); // Mova o cursor para a próxima linha
  display.print("Z: ");
  display.print(accelZ);
  display.print(" m/s^2");
 
  display.setCursor(0, 48);
  display.println("Giroscopio:");
  display.print(gX);
  display.print(",");
  display.print(gY);
  display.print(",");
  display.print(gZ);
 
  display.display();

  String aceleracaoX = String(accelX);
  MQTT.publish(TOPICO_PUBLISH_2, aceleracaoX.c_str());

  String aceleracaoY = String(accelY);
  MQTT.publish(TOPICO_PUBLISH_3, aceleracaoY.c_str());

  String aceleracaoZ = String(accelZ);
  MQTT.publish(TOPICO_PUBLISH_4, aceleracaoZ.c_str());

  String giroscopioX = String(gX);
  MQTT.publish(TOPICO_PUBLISH_5, giroscopioX.c_str());

  String giroscopioY = String(gY);
  MQTT.publish(TOPICO_PUBLISH_6, giroscopioY.c_str());

  String giroscopioZ = String(gZ);
  MQTT.publish(TOPICO_PUBLISH_7, giroscopioZ.c_str());
 
  delay(10);
}

void loop() {
    VerificaConexoesWiFIEMQTT();
    EnviaEstadoOutputMQTT();
    leituraMPU();
    MQTT.loop();
}