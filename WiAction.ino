//antes de hacer el ferdault debe quitar caca
// grabar como GENERIC Esp8266 module
String  VersionModulo = "2.0";
String  FechaVersion = "04/04/25";


// Envia eventos via PHP
// Soluciona fallo en armado parcial. (armaba parcialmente y desarmaba)
//&from=131ab0&to=5060141652&data= 131ab0,1234,1111,ubicacion de modulo,,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,00,00,-------,0,00,00,-------,0,00,00,-------,0,00,00,-------,0,00,00,-------,0,00,00,-------,0,00,00,-------,0,00,00,-------,0,

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>  //Local WebServer used to serve the configuration portal
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
//#include <Update.h>

#include <ArduinoJson.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <EEPROM.h>
HTTPClient http;
#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <ESP8266httpUpdate.h> // Para manejar la OTA

//_____________________________________________
//  IO PARA MODULO GONNER nuevo
//sacar este */
//HABILITAR ESTE BLOQUE SACANDO EL de abajo y el  (despues de in_bat)
//ESTO ES PARA LA PLACA GONNER!!!

int LedAmarillo = 2;   //led status 1
int IN_ARM_DESAR = 5;           //ENTRADA=DETECTA ARM/DESRAMA EXTERNO
int IN_SENS_SIR = 4;      //Sensado de sirena externa
int Out_PGM     = 12;        //Salida PGM1
int Out_CHG_AD  = 13;      //salida a eauipo externo para camviar estado
int IN_PANICO   = 14;       //sensado de panico de bornera
int LED_AZUL    = 15;     //Led status 2
int IN_BAT    = 16;         //SENSADO BATERI
int SwProgram   = 0;

//______________________________________________________
#define FIREBASE_HOST "control.wiaction.gonner"

//Posiciones eeprom de parametros a usar
const int E2_TelUs1 = 00;
const int E2_TelUs2 = 10;
const int E2_TelUs3 = 20;
const int E2_TelUs4 = 30;
const int E2_TelUs5 = 40;
const int E2_TelUs6 = 50;
const int E2_TelUs7 = 60;
const int E2_TelUs8 = 70;
//_____
const int E2_NomUs1 = 80;
const int E2_NomUs2 = 100;
const int E2_NomUs3 = 120;
const int E2_NomUs4 = 140;
const int E2_NomUs5 = 160;
const int E2_NomUs6 = 180;
const int E2_NomUs7 = 200;
const int E2_NomUs8 = 220;
//________
const int E2_UbicacionMod = 240;

//const int E2GETimHb = 268;

//_____
const int InicE2 = 269;

const int E2_Ruteo_ID1 = 270;
const int E2_Ruteo_ID2 = 271;
const int E2_Ruteo_ID3 = 272;
const int E2_Ruteo_ID4 = 273;
const int E2_Ruteo_ID5 = 274;
const int E2_Ruteo_ID6 = 275;
const int E2_Ruteo_ID7 = 276;
const int E2_Ruteo_ID8 = 277;

const int E2_NIV_PUL = 278;  //Modo de salida A/D pulso/nivel
const int E2TimPGM1 = 279;
const int E2TimFaltaPGM = 280;

const int E2TimPulAD = 281;
const int E2ModoPGM = 282;
//const int E2ModoPGM     = 283;

const int E2_TIPO_TIM1 = 284;
const int E2_PGM_PEND = 285;
const int E2_CNT_MINPGM = 286;
const int E2_CNT_HRPGM = 287;
const int E2_ST_ARMED = 288;

const int E2_HORA_FECHA = 290;

const int E2_VERSION = 310;

byte HayMQTT;
byte TotalLeds;
byte  DebeDemAP;    //04/10/24
//_________________________________________
//12577DLMNJVS
const int E2_TIMER1 = E2_ST_ARMED + 1;
// y= dia de la semana
// x es       01=on pgm1 /02=off pgm1
//            03=ON PGM2 /04=OFF PGM2
//            05=ARMED    /10=ARMED
//            06=ARMED    /10=DISARMED
const int E2_TIMER2 = E2_TIMER1 + 12;
const int E2_TIMER3 = E2_TIMER2 + 12;
const int E2_TIMER4 = E2_TIMER3 + 12;
const int E2_TIMER5 = E2_TIMER4 + 12;
const int E2_TIMER6 = E2_TIMER5 + 12;
const int E2_TIMER7 = E2_TIMER6 + 12;
const int E2_TIMER8 = E2_TIMER7 + 12;

const int E2_CON_INTERNET = E2_TIMER8 + 12;
const int E2_CLV_ADMIN = E2_CON_INTERNET + 1;
const int E2_BORRA_RED = E2_CLV_ADMIN + 4;
const int E2_NEXT = E2_BORRA_RED + 1;
byte Estado_Ota;
String  NuevaVersion;
String  ipPublica;
String incoming;
byte DebeEsperarProgAdm;
byte EsActualizadorOta;

//PERRO TEST
byte DebeEnviarTest;
int TimTest;
int TimSend;
byte EsTestTiempo;
unsigned long epochTime;
String MensajeEnviar;
String fecha;
String IDTelDestinatario;
//String VersionModulo, FechaVersion;
//String E2_NP_TEST = "1130116401";
String UsuarioGenerador;
String Titulo;
String Descripcion;
String TituloEvento = "Novedad desde dispositivo ";
String TextoNovedad = Descripcion;
int count;
int n = 0;

boolean  EnProcesoOTA;
String FechaResultado;
String HoraResultado;
String temporal;
String GeneradorPublicacion;
String HoraFechaActual;
String string_variable;
String ValorBuscado;
String IdModuloDestinatario;
String ImeiAutorizado;
String DatosGenerador;
String RamIdModRx;
String RamTelefonoUsuaRx;
String RamNombreUsua;
String HoraNotificacion;
String ModuloBorrar;
String Causa, TelefonoCausa;

byte HayCambioFlanco10;
byte HayAlarmaNueva, CntPulsosAlaSM300;
//byte posicion;
byte Val_NIV_PUL;
byte ValTimOutAD;
byte HayInternet;
byte HayDeteccionAux;
byte EstaContandoTiempoResetRed;
byte EstaContandoTiempoDefault;
byte SyncLED_AZUL;
byte cntFaltaInternet;
byte HayArmadoParcial;

byte ExisteUsuario;
byte StBateria;
byte HayDetBat;
byte FuePorTiempo;
byte DebeGenerarAvisoDef, DebeGenerarPulsoAD;
byte DebeBorrarModulo;
byte HayInicio;
byte HayTiempoFaltante;

byte DelStart;

const int TimPicoOn = 1;
const int TimPicoOff = 50;
byte DebeGenPicoON, DebeGenPicoOFF;
byte DelPicoOn, DelPicoOff;
byte EsPorPedidoUsua;
String formattedTime;
//String HoraMin;
//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

//______________________________
String ValTelUs1, ValTelUs2, ValTelUs3, ValTelUs4;
String ValTelUs5, ValTelUs6, ValTelUs7, ValTelUs8;

String ValNombUs1, ValNombUs2, ValNombUs3, ValNombUs4;
String ValNombUs5, ValNombUs6, ValNombUs7, ValNombUs8;

byte HayCambioUbicacion;

byte HayNombUsua1, HayNombUsua2, HayNombUsua3, HayNombUsua4;
byte HayNombUsua5, HayNombUsua6, HayNombUsua7, HayNombUsua8;

byte HayTelUsua1, HayTelUsua2, HayTelUsua3, HayTelUsua4;
byte HayTelUsua5, HayTelUsua6, HayTelUsua7, HayTelUsua8;

//_____________
byte ValRutId1, ValRutId2, ValRutId3, ValRutId4, ValRutId5, ValRutId6, ValRutId7, ValRutId8;

int tempo;
byte MuestraCuentaMinutos;
byte CoincideDiaSemanaActual;
byte CuentaTiempoDefault;
byte CntTimDefault;
byte DebeDesactivarSalida1;
byte DesactivaPorTiempoPGM1;
byte DebeDesactivarSalida2;
byte DesactivaPorTiempoPGM2;
byte AvisarCambioIDUsuarios;
byte PosGuardado;

byte ValTimPGM1;
byte ValTimRestaPGM;

byte ValModoPGM1;
byte ValModoPGM2;

byte ValModoCuentaPGM1;
byte ValModoCuentaPGM;

byte YaInicializoEEprom;
byte Respuesta;

byte EstaArmado;
byte StSir;

byte DebeResponderAOrigen, DebeReportar, Tim5s;
//byte DebeEnviarNotificacionesSegunRuteo;

int CntTimOut1;  // tiwmpo para control de PGM1 (sea luz o sirena)???
//int CntTimOut2;

byte LedTest;
byte HayDeteccionPanico;
byte DebeOscErrStatus;
byte HayAvisoFaltaInternet;
bool Start = false;

byte StPGM1;
byte StPGM2;
byte TimBlqNP;

String inputMessage;
String inputParam;
byte DebeMostrarTelefonoOrigen;

//________________________________________
String UsuarioEmisor;
String valCompara;
String DiaFiltrado;
String OrigenTel;

//String ValTim1, ValTim2, ValTim3, ValTim4, ValTim5, ValTim6, ValTim7, ValTim8;
//String RamHHTim1, RamMMTim1, RamSelDias1, RamModoAct1;
//String RamHHTim2, RamMMTim2, RamSelDias2, RamModoAct2;
//String RamHHTim3, RamMMTim3, RamSelDias3, RamModoAct3;
//String RamHHTim4, RamMMTim4, RamSelDias4, RamModoAct4;
//String RamHHTim5, RamMMTim5, RamSelDias5, RamModoAct5;
//String RamHHTim6, RamMMTim6, RamSelDias6, RamModoAct6;
//String RamHHTim7, RamMMTim7, RamSelDias7, RamModoAct7;
// String RamHHTim8, RamMMTim8, RamSelDias8, RamModoAct8;
String RamDiaActual, RamMesActual, RamAnoActual, RamHoraActual, RamMinuteActual, RamDiaSemanaActual;
String ArmaHF;

//int a;
//int b;
//int c;
//int d;
//unsigned long timeLastCheck = 0;
//unsigned long intervalCheck = 4000;

//_____
//String UbicacionIdUsuarioProgramador;
String IDOrigenEnvio;
String stringOne;
String ValUbicacionMod;
String DatoRecibido;
String resultado1;
String IDUsuarioNuevo;
String NombreUsuarioNuevo;
String TxtUbicaModulo;

String DireccionOrigen;

String ValClaveAdmin;
String ValClaveUsua;
String ModuloTest;
String ComandoActualizaValores;

String RamValClvAdmin;



//const int mqttPort = 1883;
//const char* mqttServer = "167.71.119.247";
//const char* mqttUser = "tecnosoftmicro";
// const char* mqttPass = "TecnoMicro54Mqtt";

WiFiClient espClient;
//pubSubClient MQTTClient(espClient);

String IdOrigen;
int value;
int pos;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 5000;

unsigned long startMillis1;
unsigned long currentMillis1;
const unsigned long period1 = 30000;

//___________________
int CntContMs;
int CntTotSeg60;  //cada cuenta hasta 60 es 1 NINUTO

//________
int EsCuentaSeg;  //Habilita cuenta en PGM segundos
int EsCuentaMin;  //idem minutos
int EsCuentaHr;   // idem horas
//____

int CtaSegPGM;  //permite contar tiempo en segundos
int CntSegMin;  //cuenta hasta 60 para contar 1 minuto

int CntSegHrPGM;
int CntMinHrPGM;
int CntHorasPGM1;

String ValIn1;
String resultado;

String variable;
String recibido;
String line;
String url1;
String NumSerialESP;

int YaDetectoEntradaSirena = 0;
int YaEnvioCambioPGM1 = 0;
byte EnvioDetArmado;
byte EsInicioReset;
byte HayAutorizaAdmin;
String url;
//String Origen= "" + IdOrigen + "";

String IdDestino;
byte estadetectandoNoConnect;

char packetBuffer[255];
unsigned int localPort;   //6000;
WiFiUDP Udp;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//unsigned int localPort = 8888;  // local port to listen for UDP packets

int address;
int posicionWr;
int DatoE2;
String CmdBasico = "&from=" + IdOrigen + "&to=" + IdDestino + "&data=";
String CmdAccion;
String TopicoDestino;



//&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//desde aqui? caca

#define MQTT_HOST "167.71.119.247"
#define MQTT_PORT 1883
#define MQTT_USERNAME "tecnosoftmicro"
#define MQTT_PASSWORD "TecnoMicro54Mqtt"

AsyncMqttClient mqttClient;

Ticker mqttReconnectTimer;
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  //WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  return;

  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

//_________________
void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  //Subscribe to topics
  // Subscribe to topic MQTT_SUB_DIGITAL when it connects to the broker
  String Patron1 = "PROGMODULO";
  uint16_t packetIdSub1 = mqttClient.subscribe(Patron1.c_str(), 0);

  String Patron2 = "PROGMODULO/" + String(NumSerialESP);
  uint16_t packetIdSub2 = mqttClient.subscribe(Patron2.c_str(), 0);

  uint16_t packetIdSub3 = mqttClient.subscribe( String(NumSerialESP).c_str(), 0);
  Serial.println(packetIdSub3);

  String BasicoOta = "WiAction/ota/send/" + String(NumSerialESP);
  uint16_t packetIdSub4 = mqttClient.subscribe(BasicoOta.c_str(), 0);
  Serial.println("*******");
}


void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

Ticker tickerSSID;
Ticker blinker;

//_____________________
void setup() {
  Serial.begin(115200);
  pinMode(Out_PGM, OUTPUT);
  pinMode(Out_CHG_AD, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LedAmarillo, OUTPUT);  //

  digitalWrite(Out_PGM, LOW);
  digitalWrite(Out_CHG_AD, LOW);
  digitalWrite(LED_AZUL, HIGH);
  digitalWrite(LedAmarillo, HIGH);

  pinMode(IN_ARM_DESAR, INPUT_PULLUP);  //Armado/desarmado
  pinMode(IN_SENS_SIR, INPUT_PULLUP);   // Alarma/Sirena
  pinMode(IN_PANICO, INPUT_PULLUP);     //Panico externo
  pinMode(IN_BAT, INPUT_PULLUP);
  pinMode(SwProgram, INPUT_PULLUP);
  WiFiClient client;

  //  Serial.println("iniciando!!!");
  //  WiFiManager wifiManager;
  //  wifiManager.setTimeout(120);
  //  wifiManager.setBreakAfterConfig(true);

  //super caca
  //  wifiManager.resetSettings();
  //_________________
  NumSerialESP = String(ESP.getChipId(), HEX);
  Serial.println(NumSerialESP);
  String NumSerialESP1 = String(ESP.getChipId(), DEC);
  Serial.println(NumSerialESP1);
  if (NumSerialESP.length() < 6) {
    NumSerialESP = "0" + NumSerialESP;
  }

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onMessage(ArrivedMqtt);
  mqttClient.onPublish(onMqttPublish);

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCredentials(MQTT_USERNAME, MQTT_PASSWORD);

  tickerSSID.attach(0.6, tick);
  blinker.attach(0.1, Timers);
  digitalWrite(LedAmarillo, HIGH);

  WiFiManager wifiManager;
  wifiManager.setTimeout(120);
  wifiManager.setBreakAfterConfig(true);

  //super caca
  //wifiManager.resetSettings();

  digitalWrite(LedAmarillo, HIGH);

  wifiManager.autoConnect();
  //Serial.println("__________________");
  if (!wifiManager.autoConnect()) {
    Serial.println("Fallo conexion, debera resetear para ver si conecta");
    tickerSSID.detach();
    delay(3000);
    Serial.println("Va a resetear por falta de red!");
    ESP.reset();
    delay(2000);
  }

  //Serial.println("__________________");
  //if you get here you have connected to the WiFi
  Serial.println("Conectado a la red :)");
  tickerSSID.detach();
  //keep LED on
  digitalWrite(LED_AZUL, LOW);  //OFF LED azul
  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.SSID());
  printWiFiState();
  //uint32_t flashSize = ESP.getFlashChipSize();
  //Serial.printf("Tama//o de la memoria flash: %u bytes (%.2f MB)\n", flashSize, flashSize / 1024.0 / 1024.0);
  //Serial.printf("=======");
  EEPROM.begin(2048);

  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  //7200+ 3600
  timeClient.setTimeOffset(-10800);
  //delay(1000);
  HayInternet = 1;
  HayInicio = 1;
  TimBlqNP = 5;
  //Serial.println("BLOQUEA LECTURAS DE NOTIFICACIONES");
  DelStart = 5;

  //________
  address = InicE2;  //RdE2();
  YaInicializoEEprom = EEPROM.read(address);

  if (YaInicializoEEprom != 52) {
    CargaDatosDef();
    DatoE2 = 52;
    address = InicE2;
    WrE2();
    Serial.println("Grabacion completa");

    address = InicE2;
    YaInicializoEEprom = EEPROM.read(address);
    Serial.println("VALOR LEIDO DE GUARDADO MEMORIA EEPROM:");
    Serial.println(String(YaInicializoEEprom));
    Serial.println("****************");
  }
  EnProcesoOTA = false;
  digitalWrite(LED_AZUL, HIGH);  //ON LED VERDE
  EsInicioReset = 1;

  //___________
  if (digitalRead(IN_ARM_DESAR) == 0) {
    delay(300);
    if (digitalRead(IN_ARM_DESAR) == 0) {
      Serial.println("La entrada esta en 0 armado -> debe ver estado previo al reset");
      LeeModoSalidaRemoto();
      if (Val_NIV_PUL == 1) {
        Serial.println("ES MODO NIVEL-> LEE ENTRADA PARA AJUSTARSE!");
        if (digitalRead(IN_ARM_DESAR) == 0) {
          Serial.println("A0");
          EstaArmado = 1;  //ARMADO
          digitalWrite(Out_CHG_AD, LOW);

        } else {
          Serial.println("D1");
          EstaArmado = 0;  //ESTA DESARMADO DE LECTURTA DE ARRANQUE!
          digitalWrite(Out_CHG_AD, HIGH);
        }
      } else {
        Serial.println("ES MODO PULSO");
        digitalWrite(Out_CHG_AD, LOW);  //ARANCA PARA EL PULSO EN 0
        Serial.println(digitalRead(IN_ARM_DESAR));
        EstaArmado = 1;
      }
    }
    //00=Modo nivel 01=modo pulso de determinada duracion
  } else {
    if (digitalRead(IN_ARM_DESAR) == 1) {
      delay(300);
      if (digitalRead(IN_ARM_DESAR) == 1) {
        //Serial.println("La entrada esta en 1 -> desarmado, No hace nada");
        digitalWrite(Out_CHG_AD, LOW);  //ARANCA PARA EL PULSO EN 0
        Serial.println(digitalRead(IN_ARM_DESAR));
        EstaArmado = 0;
      }
    }
  }
  DebeGenPicoON = 1;
  DelPicoOn = TimPicoOn;
  digitalWrite(LED_AZUL, LOW);

  //Recupera tiempos de PGM si existiesen previamnet
  address = E2_PGM_PEND;
  HayTiempoFaltante = EEPROM.read(address);
  Serial.println(HayTiempoFaltante);

  address = E2ModoPGM;  //
  ValModoCuentaPGM1 = EEPROM.read(address);
  //Serial.println(ValModoCuentaPGM1);
  DefineTipoConteoTimPGM();
  if (HayTiempoFaltante != 0) {
    LeeRestoTimPgm();
    Serial.println("Activa PGM Y CARGA EL ULTIMO TIEMPO que estaba contando");
    ValTimPGM1 = ValTimRestaPGM;
    Serial.println(ValTimPGM1);
    Serial.println("ACTIVA PGM1 por RECUPERACION DE TIEMPOS");
    digitalWrite(Out_PGM, HIGH);
    StPGM1 = 1;
    CntTimOut1 = ValTimPGM1;

    if (ValModoCuentaPGM1 == 3) {
      Serial.println("RECUPERA MINUTOS Y HORAS ACTUALES DE LA MEMORIA E2PROM y copia a RAM");
      address = E2_CNT_MINPGM;
      CntMinHrPGM = EEPROM.read(address);
      Serial.println("TIEMPO RECUPERADO DE MINUTOS: " + String(CntMinHrPGM));
      address = E2_CNT_HRPGM;
      CntTimOut1 = EEPROM.read(address);
      Serial.println("TIEMPO RECUPERADO DE HORAS: " + String(CntTimOut1));
    }
  }
  LeeDatosMemoria();
}
//time_t prevDisplay = 0;  // when the digital clock was displayed

//
void tick() {
  //toggle state
  int state = digitalRead(LED_AZUL);  // get the current state of GPIO1 pin
  digitalWrite(LED_AZUL, !state);     // set pin to the opposite state
}

//________________________________________________________________________
void loop() {

  LeeBateria();
  LeeEntradaArmar();  // lee nivel entrada de armado/desarmado
  LeeStSirena();  //lee estado de sirena
  LeePanico();    // lee boton Panico
  SensaProg();
  if (DebeBorrarModulo == 1) {
    DebeBorrarModulo = 0;
    Serial.println("VA A GENERAR DEFAULT POR COMANDO RECIBIDO");
    CargaDatosDef();
    LeeDatosMemoria();
  }

  if (DebeGenerarAvisoDef == 1) {
    DebeGenerarAvisoDef = 0;
    TotalLeds = 5;
    GeneraBeepsLed1();
    //GENERA 5 BEEPS DE QUE YA LLEGO A DEFAULT!
    Serial.println("CARGA DEFAULT PORQUE TIENE BYTE/FLAG DE PEDIDO!");
    CargaDatosDef();
    LeeDatosMemoria();

    //2/4/22 importante
    //ACTIVA LED AZUL Y AMARILLO FIJO ARA QUE INDIQUE QUE ESTA LIMPIA LA MEMORIA
    //OJO QUE NINGUN EQUIPO ESTE CONECTADO GENERANDO PEDIDO DE DATOS!
    digitalWrite(LedAmarillo, LOW);
    digitalWrite(LED_AZUL, LOW);
    //           DebeGenerarAvisoDef=1;

    //04/04/22
    CmdAccion = "borradef ";
    CmdAccion = CmdAccion + NumSerialESP;
    Serial.println(CmdAccion);
    //DebeBorrarModulo = 1;
    //Serial.println("**************");
    Causa = "Accion interna";
    TelefonoCausa = ValUbicacionMod;
    DatosGenerador = NumSerialESP + "," + ValUbicacionMod + ", " + "accion interna";
    Serial.println(DatosGenerador);
    EnviaNotificacionPush(NumSerialESP);  //envia a tods los usuarios que debe borra su datos de la base + estadisticas
    variable = "";
    DatoRecibido = "";
    ESP.reset();
    Serial.println("GENERA RESET MODULO");
  }
  //return;
  //=========================

  ChequeaEnvioPaquete();

  HayInternet = 1;
  if (HayInternet == 1) {
    AnalizaStringRecibido();
  }

  //___________
  if (MuestraCuentaMinutos == 1) {
    MuestraCuentaMinutos = 0;
    Serial.println("CUENTA UN MINUTO");
    LeeReloj();
  }

  ////_________________________________
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period) {
    startMillis = currentMillis;
    printWiFiState();
  }
}

//____________________________________________________________________________
void LeeEntradaArmar() {
  if (digitalRead(IN_ARM_DESAR) == 0) {
    //Lee IN_ARM_DESAR=0-> armado
    if (EstaArmado == 0) {
      //SI PREVIO =1 -> ESTABA FLAG=DESARMADO-> TIEMPO CONFIRMACION
      delay(300);
      if (digitalRead(IN_ARM_DESAR) == 0) {  //20/1/22
        //entrada en 0
        EstaArmado = 1;
        Serial.println("GENERA NP ->POR LECTURA DE BORNERA -> A0");
        // determina si es repuesta a un pedido de usuario o lectura por remoto locaL!
        CmdAccion = "Sistema armado";
        Generar_NP_ACK_AD();
        DebeMostrarTelefonoOrigen = 0;
      }
    }

    //___________________________________
  } else {
    //ENTRADA =1 (DESARMADO)
    if (EstaArmado == 1) {
      //si previamente estaba ARMADO-> VUELVE A LEER ENTRADA CONFIRMA
      if (digitalRead(IN_ARM_DESAR) == 1) {
        delay(200);
        if (digitalRead(IN_ARM_DESAR) == 1) {
          //teoricamnete esta sensando DESARMADO
          //28/6/22  SUPERA
          //if (HayAlarmaNueva == 0) {

          Serial.println("CONFIRMADO LECTURA DE  D1 SIN ALARMA");
          EstaArmado = 0;                                  //desarmado
          Serial.println("POR LECTURA DE BORNERA -> D1");  // determina si es repuesta a un pedido de usuario o lectura por remoto locaL!
          //_______
          //if (EsPorPedidoUsua == 0) {
          CmdAccion = "Sistema desarmado";
          EstaArmado = 0;  //CONFIMADO ESTA DESARMADO
          HayAlarmaNueva = 0;
          //04/10/24
          DebeDemAP = 0;
          address = E2_ST_ARMED;
          DatoE2 = 0;
          WrE2();
          Generar_NP_ACK_AD();
          DebeMostrarTelefonoOrigen = 0;

        } else {
          delay(300);
          if (digitalRead(IN_ARM_DESAR) == 1) {
            //Serial.println("DESARMADO AUTORIZADO");
            EstaArmado = 0;
            DebeDemAP = 0;
            CmdAccion = "Sistema desarmado";
            address = E2_ST_ARMED;
            DatoE2 = 0;
            WrE2();
            Generar_NP_ACK_AD();
          } else {
            Serial.println("DESPUES DE 300 MS LEYO 0 Y DEBERIA HABER COBNFIRMADO CON 1");
          }
        }
      }
    }
  }
}

//_____________________________________
void LeeStSirena() {
  if (digitalRead(IN_SENS_SIR) == 0) {
    // Detecta entra a masa (ojo que la SIRENA puede ser una MASA o un ALTO
    if (YaDetectoEntradaSirena == 0) {
      delay(2500);
      if (digitalRead(IN_SENS_SIR) == 0) {
        //confirmado= ESTA SIRENA ACTIVA
        YaDetectoEntradaSirena = 1;
        StSir = 1;
        Serial.println("SIRENA ACTIVADA");
        //aqui esta armado???
        if (EstaArmado == 1) {
          // se detecta sirena + esta armado-> EVENTO!
          if (ValUbicacionMod != "ubicacion de modulo") {
            CmdAccion = "alarma nueva";
            UsuarioGenerador = "Desde su alarma";
            HayAlarmaNueva = 1;  //Avisa que en la lectura de entrada ARMADO-> viene con un alarma generada
            TelefonoCausa = ValUbicacionMod;
            Causa = UsuarioGenerador;
            DatosGenerador = RamIdModRx + "," + ValUbicacionMod + "," + UsuarioGenerador;
            Serial.println(DatosGenerador);
            EnviaNotificacionPush(NumSerialESP);
          } else {
            Serial.println("NO ENVIA NOTIFICACION PORQUE EL MODULO ESTA BORRADO!");
          }
        } else {
          Serial.println("Activa sirena localmente sin envio de NP");
        }
      }
    }

  } else {
    //puede o no estar armado
    if (YaDetectoEntradaSirena == 1) {
      delay(1500);
      if (digitalRead(IN_SENS_SIR) == 1) {
        StSir = 0;  //sirena esta apaga
        YaDetectoEntradaSirena = 0;
        Serial.println("SIRENA APAGADA (POR ACCION POR ACCION PREVIA O ALARMA PREVIA");
        CmdAccion = "Sirena normalizada";
      }
    }
  }
}

//________________
void LeeBateria() {
  if (digitalRead(IN_BAT) == 0) {
    if (HayDetBat == 0) {
      delay(2000);
      if (digitalRead(IN_BAT) == 0) {
        Serial.println("ATENCION! BATERIA BAJA");
        HayDetBat = 1;
        StBateria = 0;
        LeeUbicacionMod();
        if (ValUbicacionMod != "ubicacion de modulo") {
          CmdAccion = "BATERIA BAJA";
          Serial.println("#######################");  //12/1/22
          Causa = "Accion interna";
          TelefonoCausa = ValUbicacionMod;

          DatosGenerador = NumSerialESP + "," + ValUbicacionMod + ", " + "accion interna";
          Serial.println(DatosGenerador);
          EnviaNotificacionPush(NumSerialESP);
        }
      }
    }
  }
  if (digitalRead(IN_BAT) == 1) {
    if (HayDetBat == 1) {
      delay(2000);
      if (digitalRead(IN_BAT) == 1) {
        //Serial.println("LEYO BATERIA BAJA ? ");
        Serial.println("Normalizo bateria");
        StBateria = 1;  //BATERIA NORMAL
        HayDetBat = 0;
        LeeUbicacionMod();
        if (ValUbicacionMod != "ubicacion de modulo") {
          CmdAccion = "BATERIA NORMAL";
          Serial.println("#######################");
          Causa = "Accion interna";
          TelefonoCausa = ValUbicacionMod;
          DatosGenerador = NumSerialESP + ", " + ValUbicacionMod + ", " + "accion interna";
          //Serial.println(DatosGenerador);
          EnviaNotificacionPush(NumSerialESP);
        }
      }
    }
  }
}

//________________
void LeePanico() {
  // por quie no tengo el hareare correcto!
  if (digitalRead(IN_PANICO) == 0) {
    if (HayDeteccionPanico == 0) {
      delay(500);
      if (digitalRead(IN_PANICO) == 0) {
        if (ValUbicacionMod != "ubicacion de modulo") {
          Serial.println("ATENCION! PANICO");
          HayDeteccionPanico = 1;
          CmdAccion = "PANICO EQUIPO";
          Serial.println("Por pulsar boton de PANICO desde bornera");
          //IDOrigenEnvio = "dispositivo";
          RamTelefonoUsuaRx = "Accion manual";
          RamNombreUsua = "Desde su alarma";
          // 23/4/22
          Causa = RamNombreUsua;
          TelefonoCausa = RamTelefonoUsuaRx;
          DatosGenerador = RamIdModRx + "," + RamTelefonoUsuaRx + "," + RamNombreUsua;
          //Serial.println(DatosGenerador);
          EnviaNotificacionPush(NumSerialESP);
        }
      }
    }
  } else {
    //hay nivel 1 -> solto boton!
    if (HayDeteccionPanico == 1) {
      delay(500);
      if (digitalRead(IN_PANICO) == 1) {
        Serial.println("Normalizo boton de panico");
        HayDeteccionPanico = 0;
      }
    }
  }
}

//___________________________________________
void Timers() {
  //OscilaLedStatus();
  CntContMs = CntContMs + 1;
  if (DebeEsperarProgAdm == 0) {
    if (DebeGenPicoON == 1) {
      DelPicoOn = DelPicoOn - 1;
      //Serial.println("#");
      if (DelPicoOn == 0) {
        DebeGenPicoON = 0;
        if (EstaContandoTiempoResetRed == 1 || EstaContandoTiempoDefault == 1) {
          // Serial.println("esta en programacion");
        } else {
          digitalWrite(LED_AZUL, HIGH);
        }
        DebeGenPicoOFF = 1;
        DelPicoOff = TimPicoOff;
      }
    }

    if (DebeGenPicoOFF == 1) {
      DelPicoOff = DelPicoOff - 1;
      if (DelPicoOff == 0) {
        DebeGenPicoOFF = 0;
        if (EstaContandoTiempoResetRed == 1 || EstaContandoTiempoDefault == 1) {
          Serial.println("esta en programacion");
        } else {
          digitalWrite(LED_AZUL, LOW);
        }
        DebeGenPicoON = 1;
        DelPicoOn = TimPicoOn;
      }
    }
  }

  if (CntContMs >= 10) {
    //si pasaron 10 lazosn de 100 ms -> es un segundo
    CntContMs = 0;
    CntTotSeg60++;  //contado de segundos
    // Serial.println(CntTotSeg60);
    //Serial.print(".");

    if (DelStart != 0) {
      DelStart--;
    }

    if (TimBlqNP != 0) {
      TimBlqNP = TimBlqNP - 1;
      //Serial.println(TimBlqNP);
      if (TimBlqNP == 0) {
        HayInicio = 0;
        //Serial.println("DESBLOQUEA LECTURAS DE NOTIFICACIONES");
      }
    }

    if (CntTotSeg60 == 60) {
      CntTotSeg60 = 0;
      //Serial.println(" Conto 1 minuto");
      MuestraCuentaMinutos = 1;
      //LeeReloj();
    }

    if (DebeResponderAOrigen == 1) {
      DebeResponderAOrigen = 0;
      EnviaComandoADestino();
      DatoRecibido = "";
      //Serial.println("RESPONDIO AL TELEFONO");
    }

    //___
    if (EsCuentaSeg != 0) {
      //Serial.println("esta contando en segundos ? ");
      //Analiza si esta contando segundos de PGM ACTIVO");
      if (CntTimOut1 != 0) {
        CntTimOut1 = CntTimOut1 - 1;
        ActualizaTiempoFaltante();
        //Serial.println(CntTimOut1);
        if (CntTimOut1 == 0) {
          DebeDesactivarSalida1 = 1;
          DesactivaPorTiempoPGM1 = 1;
        }
      }
    }
    //____________
    if (CuentaTiempoDefault != 0) {
      //Serial.println("Debe contar tiempo de pulsador activo!");
      if (CntTimDefault != 0) {
        CntTimDefault = CntTimDefault + 1;  //
        //Serial.println("Tiempo de cuenta para generar default:");
        Serial.println(CntTimDefault);
        if (SyncLED_AZUL == 1) {
          SyncLED_AZUL = 0;
          digitalWrite(LED_AZUL, LOW);  //HIGH); //off led amarillo del pulso de conteo de 100 ms
        } else {
          SyncLED_AZUL = 1;
          digitalWrite(LED_AZUL, HIGH);  //  LOW); //ON led amarillo
        }

        if (CntTimDefault == 5) {
          EstaContandoTiempoResetRed = 1;
          Serial.println("YA CONTO TIEMPO VALIDO PARA CAMBIAR RED/PASSWORD");
          digitalWrite(LedAmarillo, LOW);  //ON LED

        } else if (CntTimDefault > 9) {
          EstaContandoTiempoDefault = 1;
          EstaContandoTiempoResetRed = 0;
          Serial.println("YA CONTO TIEMPO VALIDO PARA GENERAR DEFAULT!");
          DebeGenerarAvisoDef = 1;
          CuentaTiempoDefault = 0;
          CntTimDefault = 0;
          //02/04/22
          DebeEsperarProgAdm = 1;  //02/04/22

        } else {
          //Serial.println("Ya conto mas de 5 pero menos de 10");
        }
      }
    }

    //______________
    if (EsCuentaMin != 0) {
      //para cuenta en minutos
      if (CntTimOut1 != 0) {
        if (CntSegMin == 60) {
          CntSegMin = 0;
          //esta contando aun tiempo pgm1 en minutos...
          CntTimOut1 = CntTimOut1 - 1;
          stringOne = "Tiempo actual cuenta minutos OUT 1= ";
          Serial.println(stringOne + CntTimOut1);
          ActualizaTiempoFaltante();
          if (CntTimOut1 == 0) {
            //Serial.println("Fin tiempo de activacion out 1");
            //debe avisar el corte si esta habilitado a hacerlo mediante el ruteo y el telefono de destino
            DebeDesactivarSalida1 = 1;
            DesactivaPorTiempoPGM1 = 1;
          }
        }
        CntSegMin++;
        //Serial.println(CntSegMin);
      }
    }
    //_______________
    if (EsCuentaHr != 0) {
      //Serial.println("Contador de segundos en modo PGM1 HORAS:");
      if (CntTimOut1 != 0) {
        // si hay tiempoi en curso-> empieza a contar segundos de minutos

        CntSegHrPGM++;
        if (CntSegHrPGM == 60) {
          CntSegHrPGM = 0;
          CntMinHrPGM++;            //Incrementa contador minUtos para PGM 1
          address = E2_CNT_MINPGM;  //actualiza minutos
          DatoE2 = CntMinHrPGM;
          WrE2();

          //________________
          //analiza si llego a los 60 minutos para DECREMENTAR LAS HORAS QUE DEBE CONTAR!
          if (CntMinHrPGM == 60) {
            CntMinHrPGM = 0;
            Serial.println("termino de contar 1 hora");
            CntTimOut1 = CntTimOut1 - 1;
            Serial.println("HORAS ACTUALES:");
            Serial.println(CntTimOut1);
            if (CntTimOut1 == 0) {
              //debe avisar el corte si esta habilitado a hacerlo mediante el ruteo y el telefono de destino
              DebeDesactivarSalida1 = 1;
              DesactivaPorTiempoPGM1 = 1;
              Serial.println("Debe desactivar salida auxiliar POR HORAS");
            }
            address = E2_CNT_HRPGM;
            DatoE2 = CntTimOut1;
            WrE2();
            Serial.println("ACTUALIZA TOTAL DE HORAS RESTANTES PARA TERMINAR");
          }
        }
      }
    }
  }
}

//___________________________________________
void RdE2() {
  byte value;
  // read a byte from the current address of the EEPROM
  value = EEPROM.read(address);
  Respuesta = value;
  Serial.println(Respuesta);
  //Serial.println("____________");
}

//_____________________
void WrE2() {
  //EEPROM Write
  EEPROM.write(address, DatoE2);
  EEPROM.commit();
  //Serial.println( "Grabo dato en eeprom");
  //Serial.println(DatoE2);
}

//________________________________
void BorraE2() {
  //Serial.println("Borrando E2prom");
  String resultado;
  EEPROM.begin(512);
  //String stringOne = "Borrando posicion= ";
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 1500; i++) {
    EEPROM.write(i, 0);
    //resultado = stringOne + i;
    //Serial.println(resultado);
    delay(20);
  }
  // turn the LED on when we're done
  EEPROM.commit();
  //EEPROM.end();
  //Serial.println("Fin de BORRADO");
}

//_____________
void GrabaPaqueteE2() {
  int j = 0;
  for (int i = address; i < address + string_variable.length(); i++) {
    EEPROM.write(i, string_variable[j]);  //Write one by one with starting address of 0x0F
    //Serial.println(i);
    //Serial.println(string_variable[j]);
    j++;                                  //=j+1;
  }
  EEPROM.commit();  //Store data to EEPROM
}

//___________
void CargaDefUsuario1() {
  address = E2_TelUs1;
  string_variable = "?         ";
  GrabaPaqueteE2();
}

//_____
void CargaDefUsuario2() {
  address = E2_TelUs2;
  string_variable = "?         ";
  GrabaPaqueteE2();
}

//_______
void CargaDefUsuario3() {
  address = E2_TelUs3;
  string_variable = "?         ";
  GrabaPaqueteE2();
}

//______
void CargaDefUsuario4() {
  address = E2_TelUs4;
  string_variable = "?         ";
  GrabaPaqueteE2();
}

void CargaDefUsuario5() {
  address = E2_TelUs5;
  string_variable = "?         ";
  GrabaPaqueteE2();
}

void CargaDefUsuario6() {
  address = E2_TelUs6;
  string_variable = "?         ";
  GrabaPaqueteE2();
}

void CargaDefUsuario7() {
  address = E2_TelUs7;
  string_variable = "?         ";
  GrabaPaqueteE2();
}

void CargaDefUsuario8() {
  address = E2_TelUs8;
  string_variable = "?         ";
  GrabaPaqueteE2();
}

//________________________________
void CargaDeftNombre1() {
  string_variable = "?                   ";
  address = E2_NomUs1;
  GrabaPaqueteE2();
}

//___
void CargaDeftNombre2() {
  string_variable = "?                   ";
  address = E2_NomUs2;
  GrabaPaqueteE2();
}
//______
void CargaDeftNombre3() {
  string_variable = "?                   ";
  address = E2_NomUs3;
  GrabaPaqueteE2();
}
//_____
void CargaDeftNombre4() {
  string_variable = "?                   ";
  address = E2_NomUs4;
  GrabaPaqueteE2();
}
//_____
void CargaDeftNombre5() {
  string_variable = "?                   ";
  address = E2_NomUs5;
  GrabaPaqueteE2();
}
//_____
void CargaDeftNombre6() {
  string_variable = "?                   ";
  address = E2_NomUs6;
  GrabaPaqueteE2();
}
//___________
void CargaDeftNombre7() {
  string_variable = "?                   ";
  address = E2_NomUs7;
  GrabaPaqueteE2();
}
//____________
void CargaDeftNombre8() {
  string_variable = "?                   ";
  address = E2_NomUs8;
  GrabaPaqueteE2();
}

//____
void LeeUbicacionMod() {
  // lee datos de ubicacion
  address = E2_UbicacionMod;
  ValUbicacionMod = "";
  for (int i = address; i < address + 20; i++) {
    ValUbicacionMod = ValUbicacionMod + char(EEPROM.read(i));  //Read one by one with starting address of 0x0F
  }
  ValUbicacionMod.trim();
  Serial.println("Ubicacion del modulo= " + ValUbicacionMod);
}

//___________
void LeeGuardaValorRuteo() {
  Serial.println(variable);
  resultado = variable.substring(pos + 8, pos + 11);
  Serial.println(resultado);
  resultado.replace(',', ' ');
  resultado.replace('"', ' ');
  resultado.replace('}', ' ');
  Serial.println("Voy a guardar el dato de ruteo");
  DatoE2 = resultado.toInt();
  Serial.println(DatoE2);
  WrE2();
}

//____________________
void LeeValorUbicacionModulo() {
  address = E2_UbicacionMod;
  ValUbicacionMod = "";
  for (int i = address; i < address + 20; i++) {
    ValUbicacionMod = ValUbicacionMod + char(EEPROM.read(i));  //Read one by one with starting address of 0x0F
  }
  //GRAVE ERROR
  //analiza si tiene una coma dentro del texto
  ValorBuscado = ",";
  pos = ValUbicacionMod.indexOf(ValorBuscado);
  if (pos >= 0) {
    Serial.println("ERROR DE AGREGAR UNA COMMA DONDE NO CORRESPONDE");
    ValUbicacionMod.replace(',', ' ');
  }
  Serial.println("Ub.modulo = " + ValUbicacionMod);
}

//_____________
void LeeDatosMemoria() {
  Serial.println("Leyendo datos de usuarios!");
  LeeTelUsua1();
  LeeTelUsua2();
  LeeTelUsua3();
  LeeTelUsua4();
  LeeTelUsua5();
  LeeTelUsua6();
  LeeTelUsua7();
  LeeTelUsua8();

  Serial.println("Valor IdUsuario 1  actual : " + ValTelUs1);
  Serial.println("Valor IdUsuario 2  actual : " + ValTelUs2);
  Serial.println("Valor IdUsuario 3  actual : " + ValTelUs3);
  Serial.println("Valor IdUsuario 4  actual : " + ValTelUs4);
  Serial.println("Valor IdUsuario 5  actual : " + ValTelUs5);
  Serial.println("Valor IdUsuario 6  actual : " + ValTelUs6);
  Serial.println("Valor IdUsuario 7  actual : " + ValTelUs7);
  Serial.println("Valor IdUsuario 8  actual : " + ValTelUs8);

  //__________________
  LeeNomUsua1();
  LeeNomUsua2();
  LeeNomUsua3();
  LeeNomUsua4();
  LeeNomUsua5();
  LeeNomUsua6();
  LeeNomUsua7();
  LeeNomUsua8();

  Serial.println("Nombre usu.1 = " + ValNombUs1);
  Serial.println("Nombre usu.2 = " + ValNombUs2);
  Serial.println("Nombre usu.3 = " + ValNombUs3);
  Serial.println("Nombre usu.4 = " + ValNombUs4);
  Serial.println("Nombre usu.5 = " + ValNombUs5);
  Serial.println("Nombre usu.6 = " + ValNombUs6);
  Serial.println("Nombre usu.7 = " + ValNombUs7);
  Serial.println("Nombre usu.8 = " + ValNombUs8);

  //LeeValorClaveAdmin();
  //LeeValorClaveUsuario();
  LeeValorUbicacionModulo();
  LeeModoSalidaRemoto();

  LeeValoresRuteo();
  //LeeModoUsoPGM1();
  //LeeModoUsoPGM2();
  LeeTiempoPGM1();
  //LeeTiempoPGM2();
  LeeModoConteoPGM1();
  //LeeModoConteoPGM2();
  LeeTiempoPulsoAD();
  Serial.println("__________________________");
}

//___
void DefUbicacion() {
  address = E2_UbicacionMod;
  string_variable = "ubicacion de modulo ";
  GrabaPaqueteE2();
}

//_______________________________________________________________________
void ChequeaEnvioPaquete() {
  if (HayInternet == 1) {
    /*
      if (DebeResponderAOrigen == 1) {
          DebeResponderAOrigen = 0;
          EnviaComandoADestino();
          DatoRecibido = "";
          Serial.println("RESPONDIO AL TELEFONO");
          //  delay(1000);
    */
    //___________
    if (DebeDesactivarSalida1 == 1) {
      DebeDesactivarSalida1 = 0;
      CortaSalidaPGM1();
    }
  }
}

//_____________
void LeeValoresRuteo() {
  LeeRuteoID1();
  LeeRuteoID2();
  LeeRuteoID3();
  LeeRuteoID4();
  LeeRuteoID5();
  LeeRuteoID6();
  LeeRuteoID7();
  LeeRuteoID8();
}

//___
void LeeRuteoID1() {
  value = EEPROM.read(E2_Ruteo_ID1);
  stringOne = "Ruteo Id1 = ";
  ValRutId1 = value;
  Serial.println(stringOne + ValRutId1);
}

//___
void LeeRuteoID2() {
  value = EEPROM.read(E2_Ruteo_ID2);
  stringOne = "Ruteo Id2 = ";
  ValRutId2 = value;
  Serial.println(stringOne + ValRutId2);
}
//___
void LeeRuteoID3() {
  value = EEPROM.read(E2_Ruteo_ID3);
  stringOne = "Ruteo Id3 = ";
  ValRutId3 = value;
  Serial.println(stringOne + ValRutId3);
}
//___
void LeeRuteoID4() {
  value = EEPROM.read(E2_Ruteo_ID4);
  stringOne = "Ruteo Id4 = ";
  ValRutId4 = value;
  Serial.println(stringOne + ValRutId4);
}
//__________________
void LeeRuteoID5() {
  address = E2_Ruteo_ID5;
  value = EEPROM.read(address);
  stringOne = "Ruteo Id5 = ";
  ValRutId5 = value;
  Serial.println(stringOne + ValRutId5);
}
//__________________
void LeeRuteoID6() {
  address = E2_Ruteo_ID6;
  value = EEPROM.read(address);
  stringOne = "Ruteo Id6 = ";
  ValRutId6 = value;
  Serial.println(stringOne + ValRutId6);
}
//__________________
void LeeRuteoID7() {
  address = E2_Ruteo_ID7;
  value = EEPROM.read(address);
  stringOne = "Ruteo Id7 = ";
  ValRutId7 = value;
  Serial.println(stringOne + ValRutId7);
}
//__________________
void LeeRuteoID8() {
  address = E2_Ruteo_ID8;
  value = EEPROM.read(address);
  stringOne = "Ruteo Id8 = ";
  ValRutId8 = value;
  Serial.println(stringOne + ValRutId8);
}

//______

void LeeModoUsoPGM1() {
  address = E2ModoPGM;
  ValModoPGM1 = EEPROM.read(address);
  Serial.println("Modo PGM1= " + String(ValModoPGM1));
}

//_________________
void LeeModoConteoPGM1() {
  address = E2_TIPO_TIM1;
  ValModoCuentaPGM1 = EEPROM.read(address);
  Serial.println("Modo HMS 1= " + String(ValModoCuentaPGM1));
}
//____________
void LeeRestoTimPgm() {
  address = E2TimFaltaPGM;
  ValTimRestaPGM = EEPROM.read(address);
  Serial.println("Tiempo faltante PGM= " + String(ValTimRestaPGM));
}

//__________
void LeeTiempoPGM1() {
  address = E2TimPGM1;
  ValTimPGM1 = EEPROM.read(address);
  Serial.println("Tiempo PGM1= " + String(ValTimPGM1));
}
//_________
void LeeTiempoPulsoAD() {
  address = E2TimPulAD;
  ValTimOutAD = EEPROM.read(address);
  //Serial.println("Tiempo pulso A/D = " + String(ValTimOutAD));
}
//__________________________________________________
void LeeModoSalidaRemoto() {
  address = E2_NIV_PUL;
  Val_NIV_PUL = EEPROM.read(address);
  //00=Modo nivel 01=modo pulso de determinada duracion
  Serial.println("Modo Remoto = " + String(Val_NIV_PUL));
}

//_______________________________________
void AnalizaStringRecibido() {
  if (DatoRecibido != "") {
    DatoRecibido.replace("** DONATIONWARE **", "");
    Serial.println("Rx: " + DatoRecibido);
    variable = DatoRecibido;
    DatoRecibido = "";
    TotalLeds = 2;
    GeneraBeepsLed();

    //1- Determina datos a procesar y quien lo envió!(IDOrigenEnvio)
    //$$$$$$$$$$$$$%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //1- Determina datos a procesar y quien lo envió!(IdOrigenEnvio)
    //AGREGADO PARA DETECTAR COMANDO DESDE ACTUALIZADOR
    //19/3/25
    if (EsActualizadorOta == 1) {
      Serial.println("ES PROCESO DE OTA");

      EsActualizadorOta == 0;
      pos = variable.indexOf("datosOTA");
      if (pos >= 0) {
        //Serial.println("*))==");
        String Accion = "datosOTA";
        Estado_Ota = 0;
        MensajeEnviar = Accion + "," + NumSerialESP + "," + VersionModulo + "," + FechaVersion + "," + Estado_Ota;    //+ "," + Fecha_Ota;
        //NUEVO OTA
        Serial.println("📢 Datos enviados al telefono: " + MensajeEnviar);
        Serial.println("RESPONDE AL TELEFONO GENERADOR!");
        IDTelDestinatario = "WiAction/ota/rx/" + String(NumSerialESP);
        Serial.println(IDTelDestinatario);

        mqttClient.publish(String(IDTelDestinatario).c_str(), 0, false, String(MensajeEnviar).c_str());
        Serial.println(" ");
        // return;
      }

      pos = variable.indexOf("https://");
      if (pos >= 0) {
        Serial.println("VA A PROCESAR EL PEDIDO GRABACION DE OTA");
        variable.replace("** DONATIONWARE **", "");

        // Busca "data=" y extrae la URL completa
        ValorBuscado = "data=";
        pos = variable.indexOf(ValorBuscado);
        if (pos >= 0) {
          int start = pos + 5; // Longitud de "data="
          int end = variable.indexOf(' ', start); // Busca el primer espacio después de "data="
          if (end == -1) end = variable.length(); // Si no hay espacio, usa el final del string

          url = variable.substring(start, end); // Extrae desde "data=" hasta el espacio/fin
          url.trim(); // Elimina espacios/tabs/saltos de línea residuales

          Serial.print("URL extraída: ");
          Serial.println(url);
          Serial.println(url.length());
          Serial.println("===&&&%%%");
          //caca  5/4/25
          actualizarFirmware(url); // 🔹 Envía la URL a la función de OTA

          //actualizarFirmwareSegura(url);
          //actualizarFirmwareSegura("https://raw.githubusercontent.com/softmicrosystems/firmware-esp/main/WiAction.ino.generic.bin");

        } else {
          Serial.println("NO ENCONTRO PALABRA");
        }
      }
      DatoRecibido = "";
      return;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    ValorBuscado = "&to=";
    pos = variable.indexOf(ValorBuscado);
    if (pos >= 0) {
      IdModuloDestinatario = variable.substring(pos + 4, pos + 10);
      Serial.println("Modulo buscado: " + IdModuloDestinatario);
    }

    //Serial.println("Busqueda de valor 'data'...");
    ValorBuscado = "data";
    pos = variable.indexOf(ValorBuscado);
    if (pos >= 0) {
      recibido = variable.substring(pos + 5, pos + 17);
      Serial.println(recibido);
      //Serial.println("___");
      ValorBuscado = "from";
      pos = variable.indexOf(ValorBuscado);
      if (pos >= 0) {
        IdDestino = variable.substring(pos + 5, pos + 15);
        UsuarioEmisor = IdDestino;
        Serial.println("Enviado desde : " + String(IdDestino));
        //Serial.println("Moulo destinatario : " + OrigenTopicoEnvio);
        Serial.println("________________");
        //Serial.println("DECOFIFICA EL COMANDO RECIBIDO POR MQTT Y RESPONDE CON MQTT O NOTIFICACION PUSH");
      }
    }

    //_________________________________________________________
    if (IdModuloDestinatario == NumSerialESP) {
      //COINCIDE COD MODULO CON CODIGO SOLICITADO
      Serial.print(" ");

      ValorBuscado = "solicitadatos";
      pos = variable.indexOf(ValorBuscado);
      if (pos >= 0) {
        EnviaMensajeEnviarAUsuario();
      }

      //ValorBuscado = "Confirm";
      //pos = variable.indexOf(ValorBuscado);
      //if (pos >= 0) {
      // CmdAccion = "Conectado-OK";
      // DebeResponderAOrigen = 1;

      //_____________________________________
      pos = variable.indexOf("newmod");
      if (pos >= 0) {
        resultado = variable.substring(pos + 7, pos + 13);
        Serial.println("Id modulo : " + resultado);

        string_variable = variable.substring(pos + 14, pos + 34);  // texto ubicacion modulo
        Serial.println("Ubicacion modulo : " + string_variable);
        address = E2_UbicacionMod;
        GrabaPaqueteE2();
        LeeUbicacionMod();

        NombreUsuarioNuevo = variable.substring(pos + 35, pos + 55);  //Texto NOMBRE /ID de usuario a crear
        Serial.println("Nombre usuario : " + NombreUsuarioNuevo);
        address = E2_NomUs1;
        borraBloqueE2prom20();
        resultado = ValNombUs1;
        GrabaE2_Buffer();
        LeeNomUsua1();
        IDUsuarioNuevo = variable.substring(pos + 56, pos + 66);  //  IMEI de nuevo usuario
        Serial.println("El imei que sera su publicador= " + IDUsuarioNuevo);
        resultado = variable.substring(pos + 67, pos + 68);  //ubicacion donde guardar
        DatoE2 = resultado.toInt();

        UbicaPosicionNewUsu();
        CmdAccion = "newmod " + NumSerialESP + "," + ValUbicacionMod + "," + NombreUsuarioNuevo + "," + IDUsuarioNuevo;
        variable = "";
        DatoRecibido = "";
        DebeResponderAOrigen = 1;
        LeeDatosMemoria();
        //CACA
        DebeEsperarProgAdm = 0;  //02/04/22
      }

      //____________________________
      pos = variable.indexOf("modpgm1");
      if (pos >= 0) {
        Serial.println("Detecto comando MODPGM1!");
        // procesar comando modo de uso pgm1
        Serial.println(variable);
        Serial.println("__________________________");
        //modpgm1 2,timpgm1 50,outremoto 0,timpulso 1,1310b1,1165666851,Alejandro

        resultado = variable.substring(pos + 8, pos + 9);
        Serial.println("Modo de conteo para pgm 1");
        DatoE2 = resultado.toInt();
        address = E2_TIPO_TIM1;
        WrE2();
        LeeModoConteoPGM1();

        pos = variable.indexOf("timpgm1");
        resultado = variable.substring(pos + 8, pos + 10);
        DatoE2 = resultado.toInt();
        address = E2TimPGM1;
        WrE2();
        LeeTiempoPGM1();

        // DECODIFICA datos DE MODO DE USO SALIDA remota PARA armar/desarmar equipo externo
        pos = variable.indexOf("outremoto");
        resultado = variable.substring(pos + 10, pos + 11);
        Serial.println("Modo salida AD");
        Serial.println(resultado);
        DatoE2 = resultado.toInt();
        address = E2_NIV_PUL;  //tiempo del pulso de armado/desarmado
        WrE2();
        LeeModoSalidaRemoto();


        //__________
        //Ubica texto timpulso
        pos = variable.indexOf("timpulso");
        resultado = variable.substring(pos + 8, pos + 10);
        Serial.println("Tim.pulso Remoto");
        resultado.replace(", ", " ");
        //resultado.replace('"', " ");
        resultado.replace("}", " ");
        Serial.println(resultado);
        DatoE2 = resultado.toInt();
        address = E2TimPulAD;  //tiempo del pulso de armado/desarmado
        WrE2();
        LeeTiempoPulsoAD();
        CmdAccion = "prgpgm-OK";
        DebeResponderAOrigen = 1;
        //AvisarCambioParametros = 1;
        // CortaSalidaPGM1();              //ABORTA PROCESO PARA CARGAR LOS NUEVOS VALORES
      }

      //___________________
      pos = variable.indexOf("nombusua ");
      if (pos >= 0) {
        Serial.println("Hay cambio/edicion nombre usuario");
        // nombusua " & DestinoPush & "," & NombreNewUsua
        resultado1 = variable.substring(pos + 9, pos + 19);
        Serial.println(resultado1);
        resultado1.trim();

        resultado = variable.substring(pos + 20, pos + 40);
        //Serial.println("NOMBRE DEL USUARIO EDITADO");
        //Serial.println(resultado);
        CmdAccion = "nombusua ";
        CmdAccion = CmdAccion + resultado;
        DebeResponderAOrigen = 1;

        if (resultado1 == ValTelUs1) {
          ValNombUs1 = resultado;
          //Serial.println("ACTUALIZA NOMBRE 1");
          ActualizaNombreUs1();

        } else if (resultado1 == ValTelUs2) {
          ValNombUs2 = resultado;
          //Serial.println("ACTUALIZA NOMBRE 2");
          ActualizaNombreUs2();

        } else if (resultado1 == ValTelUs3) {
          ValNombUs3 = resultado;
          //Serial.println("ACTUALIZA NOMBRE 3");
          ActualizaNombreUs3();

        } else if (resultado1 == ValTelUs4) {
          ValNombUs4 = resultado;
          ActualizaNombreUs4();

        } else if (resultado1 == ValTelUs5) {
          ValNombUs5 = resultado;
          ActualizaNombreUs5();

        } else if (resultado1 == ValTelUs6) {
          ValNombUs6 = resultado;
          ActualizaNombreUs6();

        } else if (resultado1 == ValTelUs7) {
          ValNombUs7 = resultado;
          ActualizaNombreUs7();

        } else if (resultado1 == ValTelUs8) {
          ValNombUs8 = resultado;
          ActualizaNombreUs8();
        }
      }

      //_____________________________________
      pos = variable.indexOf("txtubica");
      if (pos >= 0) {
        Serial.println("DETECTO CAMBIO UBICACION");
        resultado = variable.substring(pos + 9, pos + 15);
        Serial.println("Id modulo : " + resultado);
        string_variable = variable.substring(pos + 16, pos + 35);
        Serial.println("Ubicacion : " + string_variable);
        address = E2_UbicacionMod;
        GrabaPaqueteE2();
        LeeUbicacionMod();
        CmdAccion = "txtubica," + string_variable;
        variable = "";
        DatoRecibido = "";
        DebeResponderAOrigen = 1;
        //Tim5s = 5;
      }

      //___________
      pos = variable.indexOf("agregausua");
      if (pos >= 0) {
        Serial.println(pos);
        Serial.println("===");

        resultado = variable.substring(pos + 11, pos + 17);
        Serial.println("Id modulo : " + resultado);

        IDUsuarioNuevo = variable.substring(pos + 18, pos + 28);  //  Telefono de nuevo usuario
        Serial.println("Guarda telefono nuevo usuario agregado= " + IDUsuarioNuevo);

        NombreUsuarioNuevo = variable.substring(pos + 29, pos + 49);  //Texto NOMBRE /ID de usuario a crear
        Serial.println("Nombre usuario : " + NombreUsuarioNuevo);

        resultado = variable.substring(pos + 50, pos + 51);  //ubicacion donde guardar
        DatoE2 = resultado.toInt();
        Serial.println(DatoE2);
        UbicaPosicionNewUsu();

        CmdAccion = "agregausua " + NumSerialESP + "," + IDUsuarioNuevo + "," + NombreUsuarioNuevo + "," + resultado;
        variable = "";
        DatoRecibido = "";
        DebeResponderAOrigen = 1;
        LeeDatosMemoria();
      }


      //___________________
      pos = variable.indexOf("ruteo 1 ");
      if (pos >= 0) {
        Serial.println("Hay prog.RUTEO 1!");
        address = E2_Ruteo_ID1;
        resultado = variable.substring(pos + 8, pos + 11);
        DatoE2 = resultado.toInt();
        WrE2();
        LeeRuteoID1();  //Lee valor de ruteo telefono ID
        CmdAccion = "cambio ruteo 1 ";
        CmdAccion = CmdAccion + DatoE2;
        DebeResponderAOrigen = 1;
      }

      //______________
      pos = variable.indexOf("ruteo 2 ");
      if (pos >= 0) {
        Serial.println("Hay prog.RUTEO 2!");
        address = E2_Ruteo_ID2;
        resultado = variable.substring(pos + 8, pos + 11);
        DatoE2 = resultado.toInt();
        WrE2();
        LeeRuteoID2();  //Lee valor de ruteo telefono ID
        CmdAccion = "cambio ruteo 2 ";
        CmdAccion = CmdAccion + DatoE2;
        DebeResponderAOrigen = 1;
      }

      //______________
      pos = variable.indexOf("ruteo 3 ");
      if (pos >= 0) {
        Serial.println("Hay prog.RUTEO 3!");
        address = E2_Ruteo_ID3;
        resultado = variable.substring(pos + 8, pos + 11);
        DatoE2 = resultado.toInt();
        WrE2();
        LeeRuteoID3();  //Lee valor de ruteo telefono ID
        CmdAccion = "cambio ruteo 3 ";
        CmdAccion = CmdAccion + DatoE2;
        DebeResponderAOrigen = 1;
      }

      //______________
      pos = variable.indexOf("ruteo 4 ");
      if (pos >= 0) {
        Serial.println("Hay prog.RUTEO 4!");
        resultado = variable.substring(pos + 8, pos + 11);
        address = E2_Ruteo_ID4;
        DatoE2 = resultado.toInt();
        WrE2();
        LeeRuteoID4();  //Lee valor de ruteo telefono ID
        CmdAccion = "cambio ruteo 4 ";
        CmdAccion = CmdAccion + DatoE2;
        DebeResponderAOrigen = 1;
      }

      //______________
      pos = variable.indexOf("ruteo 5 ");
      if (pos >= 0) {
        Serial.println("Hay prog.RUTEO 5!");
        address = E2_Ruteo_ID5;
        resultado = variable.substring(pos + 8, pos + 11);
        DatoE2 = resultado.toInt();
        WrE2();
        LeeRuteoID5();  //Lee valor de ruteo telefono ID
        CmdAccion = "cambio ruteo 5 ";
        CmdAccion = CmdAccion + DatoE2;
        DebeResponderAOrigen = 1;
      }

      //______________
      pos = variable.indexOf("ruteo 6 ");
      if (pos >= 0) {
        Serial.println("Hay prog.RUTEO 6!");
        address = E2_Ruteo_ID6;
        resultado = variable.substring(pos + 8, pos + 11);
        DatoE2 = resultado.toInt();
        WrE2();
        LeeRuteoID6();  //Lee valor de ruteo telefono ID
        CmdAccion = "cambio ruteo 6 ";
        CmdAccion = CmdAccion + DatoE2;
        DebeResponderAOrigen = 1;
      }

      //______________
      pos = variable.indexOf("ruteo 7 ");
      if (pos >= 0) {
        Serial.println("Hay prog.RUTEO 7!");
        address = E2_Ruteo_ID7;
        resultado = variable.substring(pos + 8, pos + 11);
        DatoE2 = resultado.toInt();
        WrE2();
        LeeRuteoID7();  //Lee valor de ruteo telefono ID
        CmdAccion = "cambio ruteo 7 ";
        CmdAccion = CmdAccion + DatoE2;
        DebeResponderAOrigen = 1;
      }

      //______________
      pos = variable.indexOf("ruteo 8 ");
      if (pos >= 0) {
        Serial.println("Hay prog.RUTEO 8!");
        address = E2_Ruteo_ID8;
        resultado = variable.substring(pos + 8, pos + 11);
        DatoE2 = resultado.toInt();
        WrE2();
        LeeRuteoID8();  //Lee valor de ruteo telefono ID
        CmdAccion = "cambio ruteo 8 ";
        CmdAccion = CmdAccion + DatoE2;
        DebeResponderAOrigen = 1;
      }

      //___________________
      pos = variable.indexOf("borrausuario");
      if (pos >= 0) {
        byte posicion;
        Serial.println("Detecto BORRAR USUARIO!");
        Serial.println(variable);
        GeneradorPublicacion = IDOrigenEnvio;
        resultado = variable.substring(pos + 13, pos + 14);
        posicion = resultado.toInt();
        if (posicion == 1) {
          CmdAccion = "borrausua 1";
          DebeResponderAOrigen = 1;
          CargaDefUsuario1();  //Limpia todos los datos del usuario 1 en memoria
          CargaDeftNombre1();  //default de nomnbre del usuario

        } else if (posicion == 2) {
          CmdAccion = "borrausua 2";
          DebeResponderAOrigen = 1;
          Serial.println("Debe inicializar los datos de usuario y ubicacion 2");
          CargaDefUsuario2();
          CargaDeftNombre2();

        } else if (posicion == 3) {
          CmdAccion = "borrausua 3";
          DebeResponderAOrigen = 1;
          CargaDefUsuario3();
          CargaDeftNombre3();

        } else if (posicion == 4) {
          CmdAccion = "borrausua 4";
          DebeResponderAOrigen = 1;
          CargaDefUsuario4();
          CargaDeftNombre4();

        } else if (posicion == 5) {
          //Serial.println("debe enviar comando BORRA USUARIO 5");
          CmdAccion = "borrausua 5";
          DebeResponderAOrigen = 1;
          CargaDefUsuario5();
          CargaDeftNombre5();

        } else if (posicion == 6) {
          //Serial.println("debe enviar comando BORRA USUARIO 6");
          CmdAccion = "borrausua 6";
          DebeResponderAOrigen = 1;
          CargaDefUsuario6();
          CargaDeftNombre6();

        } else if (posicion == 7) {
          //Serial.println("debe enviar comando BORRA USUARIO 7");
          CmdAccion = "borrausua 7";
          DebeResponderAOrigen = 1;
          CargaDefUsuario7();
          CargaDeftNombre7();

        } else if (posicion == 8) {
          //Serial.println("debe enviar comando BORRA USUARIO 8");
          CmdAccion = "borrausua 8";
          DebeResponderAOrigen = 1;
          CargaDefUsuario8();
          CargaDeftNombre8();
        } else {
          Serial.println("NO ENTIENDIO EL NUMERO!!");
        }
      }

      //___________________
      pos = variable.indexOf("borramod");
      if (pos >= 0) {
        //borramodulo,131ab0,131ab0,1165666851,Alejandro
        Serial.println("Detecto BORRAR PERFIL!");
        Serial.println(variable);
        resultado = variable.substring(pos + 9, pos + 15);
        resultado.replace(',', ' ');
        resultado.replace('"', ' ');
        resultado.replace('}', ' ');
        Serial.println(resultado);
        CmdAccion = "borramod ";
        CmdAccion = CmdAccion + resultado;
        Serial.println(CmdAccion);
        ModuloBorrar = resultado;
        DebeResponderAOrigen = 1;
        DebeBorrarModulo = 1;
        variable = "";
        DatoRecibido = "";
      }

      //__________
      pos = variable.indexOf("parcial");
      if (pos >= 0) {
        Serial.println("Detecto comando ACTIVAR PARCIAL!");
        LeeModoSalidaRemoto();
        if (Val_NIV_PUL == 0) {
          if (DebeDemAP == 0) {    //4/10/24
            digitalWrite(Out_CHG_AD, LOW);  //Off led de salida ARM/DESARM
            Serial.println("Es modo NIVEL y pasa a LOW cuando es ACTIVAR");
            HayArmadoParcial = 1;
            GeneraPulsoParcial();
            delay(1500);
            if (digitalRead(IN_ARM_DESAR) == 0) {  //AQUI CAMBIA POLARIDAD  Detecta entra a masa (ojo que la SIRENA puede ser una MASA o un ALTO
              delay(100);                          //200
              if (digitalRead(IN_ARM_DESAR) == 0) {
                EstaArmado = 1;  //ESTA ARMADO CONFIRMADO
                CmdAccion = "OK_PARCIAL";
                EnviaComandoADestino();
                CmdAccion = "Sistema armado";
                Generar_NP_ACK_AD();
                DebeMostrarTelefonoOrigen = 0;
                DebeDemAP = 1; //4/10/24
                //ComunSensadoArmado();

              } else {
                CmdAccion = "Accion no confirmada";
                //Serial.println("NO CAMBIO  EL NIVEL A 0!");
                Generar_NP_ACK_AD();
                DebeMostrarTelefonoOrigen = 0;
                EsPorPedidoUsua = 0;
              }
            }
          }
        }
      }

      //__________
      pos = variable.indexOf("activar");
      if (pos >= 0) {
        HayArmadoParcial = 0;
        Serial.println("Detecto comando ACTIVAR!");
        LeeModoSalidaRemoto();
        if (Val_NIV_PUL == 1) {
          digitalWrite(Out_CHG_AD, LOW);  //Off led de salida ARM/DESARM
          Serial.println("Es modo NIVEL y pasa a LOW cuando es ACTIVAR");
        } else {
          GeneraPulsoAD();
        }
        EsPorPedidoUsua = 1;
        DebeMostrarTelefonoOrigen = 1;
        delay(1500);

        //SUPER 20/9/24
        // EstaArmado = 1;  //ESTA ARMADO CONFIRMADO
        // CmdAccion = "OK_ARMAR";
        // EnviaComandoADestino();
        // CmdAccion = "Sistema armado";
        // Generar_NP_ACK_AD();
        // DebeMostrarTelefonoOrigen = 0;
        // return;
        if (digitalRead(IN_ARM_DESAR) == 0) {  //AQUI CAMBIA POLARIDAD  Detecta entra a masa (ojo que la SIRENA puede ser una MASA o un ALTO
          delay(100);                          //200
          if (digitalRead(IN_ARM_DESAR) == 0) {
            EstaArmado = 1;  //ESTA ARMADO CONFIRMADO
            CmdAccion = "OK_ARMAR";
            EnviaComandoADestino();
            CmdAccion = "Sistema armado";
            Generar_NP_ACK_AD();
            DebeMostrarTelefonoOrigen = 0;
          }
        } else {
          CmdAccion = "Accion no confirmada";
          //Serial.println("NO CAMBIO  EL NIVEL A 0!");
          Generar_NP_ACK_AD();
          DebeMostrarTelefonoOrigen = 0;
          EsPorPedidoUsua = 0;
        }
      }

      //________________
      pos = variable.indexOf("desarmar");
      if (pos >= 0) {
        HayArmadoParcial = 0;
        DebeDemAP = 0;        //4/10/24
        if (Val_NIV_PUL == 1) {
          digitalWrite(Out_CHG_AD, HIGH);  //ON SALIDA ARM/DESARM
          Serial.println("Es modo NIVEL y pasa a HIGH cuando es DESACTIVAR");
        } else {
          GeneraPulsoAD();
        }
        EsPorPedidoUsua = 1;
        //Serial.println("EMPIEZA A LEER ENTRADA PARA CONFIRMAR");
        delay(1500);
        if (digitalRead(IN_ARM_DESAR) == 1) {  //AQUI CAMBIA POLARIDAD  Detecta entra a masa (ojo que la SIRENA puede ser una MASA o un ALTO
          delay(100);                          //200
          if (digitalRead(IN_ARM_DESAR) == 1) {
            EstaArmado = 0;  //CONFIMADO ESTA DESARMADO
            HayAlarmaNueva = 0;
            DebeMostrarTelefonoOrigen = 1;
            CmdAccion = "OK_DESARMAR";
            EnviaComandoADestino();
            CmdAccion = "Sistema desarmado";
            Generar_NP_ACK_AD();
            DebeMostrarTelefonoOrigen = 0;
          }
        } else {
          Serial.println("NO CAMBIO  EL NIVEL A 1!");
          CmdAccion = "Accion no confirmada";
          Generar_NP_ACK_AD();
          DebeMostrarTelefonoOrigen = 0;
          EsPorPedidoUsua = 0;
        }
      }

      //_____________________________________
      pos = variable.indexOf("panrem");
      if (pos >= 0) {
        Serial.println("Detecto PANREM");
        //procesar datos de activar PANICO REMOTO
        if (ValUbicacionMod != "ubicacion de modulo") {
          CmdAccion = "PANICO TELEFONICO";
          IDOrigenEnvio = "dispositivo";
          ArmaRespuestaDatosUsu();
          DatosGenerador = RamIdModRx + "," + RamTelefonoUsuaRx + "," + RamNombreUsua;
          EnviaNotificacionPush(NumSerialESP);
        }
      }
      //_________________________________
      pos = variable.indexOf("on_out1");
      if (pos >= 0) {
        CmdAccion = "OK_ON_PGM";
        EnviaComandoADestino();
        ActivaPGM();
      }
      //________
      pos = variable.indexOf("off_out1");
      if (pos >= 0) {
        CmdAccion = "OK_OFF_PGM";
        EnviaComandoADestino();
        CortaSalidaPGM1();
      }
    }
  }
}

void ComunSensadoArmado() {
  EsPorPedidoUsua = 1;
  DebeMostrarTelefonoOrigen = 1;
  delay(1500);
  if (digitalRead(IN_ARM_DESAR) == 0) {
    delay(100);
    if (digitalRead(IN_ARM_DESAR) == 0) {
      EstaArmado = 1;                    //ESTA ARMADO CONFIRMADO
      CmdAccion = "Sistema armado";
      Generar_NP_ACK_AD();
      DebeMostrarTelefonoOrigen = 0;
    }

  } else if (HayArmadoParcial == 1) {
    CmdAccion = "Armado parcial";       //nuevo
    Generar_NP_ACK_AD;

  } else {
    CmdAccion = "Accion no confirmada";
    Generar_NP_ACK_AD();
    DebeMostrarTelefonoOrigen = 0;
    EsPorPedidoUsua = 0;
  }
}

//_______
void LeeTelUsua1() {
  address = E2_TelUs1;
  ValTelUs1 = LeeComunIDUsuario();
  ValTelUs1.trim();
  //Serial.println("Valor IdUsuario 1  actual : ");
  //Serial.println(ValTelUs1);
}

//_____
void LeeTelUsua2() {
  address = E2_TelUs2;
  ValTelUs2 = LeeComunIDUsuario();
  ValTelUs2.trim();
  //Serial.println("Valor IdUsuario 2  actual : ");
  //Serial.println(ValTelUs2);
}

//_____
void LeeTelUsua3() {
  address = E2_TelUs3;
  ValTelUs3 = LeeComunIDUsuario();
  ValTelUs3.trim();
  //Serial.println("Valor IdUsuario 3  actual : ");
  //Serial.println(ValTelUs3);
}
//_____
void LeeTelUsua4() {
  address = E2_TelUs4;
  ValTelUs4 = LeeComunIDUsuario();
  ValTelUs4.trim();
  //Serial.println("Valor IdUsuario 4  actual : ");
  //Serial.println(ValTelUs4);
}
//_____
void LeeTelUsua5() {
  address = E2_TelUs5;
  ValTelUs5 = LeeComunIDUsuario();
  ValTelUs5.trim();
  //Serial.println("Valor IdUsuario 5  actual : ");
  //Serial.println(ValTelUs5);
}
//_____
void LeeTelUsua6() {
  address = E2_TelUs6;
  ValTelUs6 = LeeComunIDUsuario();
  ValTelUs6.trim();
  //Serial.println("Valor IdUsuario 6  actual : ");
  //Serial.println(ValTelUs6);
}
//_____
void LeeTelUsua7() {
  address = E2_TelUs7;
  ValTelUs7 = LeeComunIDUsuario();
  ValTelUs7.trim();
  // Serial.println("Valor IdUsuario 7  actual : ");
  // Serial.println(ValTelUs7);
}

//_____
void LeeTelUsua8() {
  address = E2_TelUs8;
  ValTelUs8 = LeeComunIDUsuario();
  ValTelUs8.trim();
  //Serial.println("Valor IdUsuario 8  actual : ");
  //Serial.println(ValTelUs8);
}

//_______________________________________________________
String LeeComunIDUsuario() {
  String ValIdUsuario = "";
  for (int i = address; i < address + 10; i++) {
    ValIdUsuario = ValIdUsuario + char(EEPROM.read(i));
  }
  ValIdUsuario = ValIdUsuario.substring(0, 10);
  return ValIdUsuario;
}

//_________________________________________________________________________________________
void LeeNomUsua1() {
  address = E2_NomUs1;
  ValNombUs1 = LeeComunUbicaUsua();
  ValNombUs1.trim();
}

//_______
void LeeNomUsua2() {
  address = E2_NomUs2;
  ValNombUs2 = LeeComunUbicaUsua();
  ValNombUs2.trim();
}

//_______
void LeeNomUsua3() {
  address = E2_NomUs3;
  ValNombUs3 = LeeComunUbicaUsua();
  ValNombUs3.trim();
}

//_______
void LeeNomUsua4() {
  address = E2_NomUs4;
  ValNombUs4 = LeeComunUbicaUsua();
  ValNombUs4.trim();
}
//_______

void LeeNomUsua5() {
  address = E2_NomUs5;
  ValNombUs5 = LeeComunUbicaUsua();
  ValNombUs5.trim();
}
//_______
void LeeNomUsua6() {
  address = E2_NomUs6;
  ValNombUs6 = LeeComunUbicaUsua();
}
//_______
void LeeNomUsua7() {
  address = E2_NomUs7;
  ValNombUs7 = LeeComunUbicaUsua();
  ValNombUs7.trim();
}
//_______
void LeeNomUsua8() {
  address = E2_NomUs8;
  ValNombUs8 = LeeComunUbicaUsua();
  ValNombUs8.trim();
}

//________________________________
String LeeComunUbicaUsua() {
  String ValUbicaUsua = "";
  for (int i = address; i < address + 20; i++) {
    ValUbicaUsua = ValUbicaUsua + char(EEPROM.read(i));
  }
  ValUbicaUsua = ValUbicaUsua.substring(0, 20);
  return ValUbicaUsua;
}
//_____________
void borraBloqueE2prom() {
  resultado1 = string_variable;
  string_variable = "                    ";
  GrabaPaqueteE2();
  string_variable = resultado1;
}

//________________________________________________________________
//lectura de reloj para agregar al envio de novedad
//void digitalClockDisplay() {
//  int digits;
//  //Serial.println("?????????");
//
//  HoraFechaActual = "";
//  printDigitsN2p(hour());
//  HoraFechaActual = HoraFechaActual + ":";
//
//  printDigits(minute());
//  HoraFechaActual = HoraFechaActual + ":";
//
//  printDigits(second());
//  HoraResultado = HoraFechaActual;
//  //Serial.println(HoraResultado);
//  //Serial.println("=====");
//  HoraFechaActual = "";
//  printfecha(day());
//  HoraFechaActual = HoraFechaActual + "/";
//  printfecha(month());
//  HoraFechaActual = HoraFechaActual + "/";
//  printfecha(year());
//
//  FechaResultado = HoraFechaActual;
//  //Serial.println("Hora a agregar a la notificacion : ");
//  Serial.println(HoraResultado + "-" + FechaResultado);
//  HoraNotificacion = HoraResultado + "-" + FechaResultado;
//}
//

////_____________
//void printDigitsN2p(int digits) {
//  if (digits < 10)
//    HoraFechaActual = HoraFechaActual + '0';
//  HoraFechaActual = HoraFechaActual + digits;
//}
////_____________
//void printDigits(int digits) {
//  if (digits < 10)
//    HoraFechaActual = HoraFechaActual + '0';
//  HoraFechaActual = HoraFechaActual + digits;
//  //Serial.println(HoraFechaActual);
//}
//
////_____________
//void printfecha(int digits) {
//  if (digits < 10)
//    HoraFechaActual = HoraFechaActual + '0';
//  HoraFechaActual = HoraFechaActual + digits;
//}
//_________________________________________

//const int NTP_PACKET_SIZE = 48;      // NTP time is in the first 48 bytes of
//byte packetBuffer[NTP_PACKET_SIZE];  //buffer to hold incoming & outgoing packets
//
//time_t getNtpTime() {
//  IPAddress ntpServerIP;  // NTP server's ip address
//  while (Udp.parsePacket() > 0)
//    ;  // discard any previously received packets
//  //Serial.println("Transmit NTP Request");
//  // get a random server from the pool
//  WiFi.hostByName(ntpServerName, ntpServerIP);
//
//
//  sendNTPpacket(ntpServerIP);
//  uint32_t beginWait = millis();
//  while (millis() - beginWait < 1500) {
//    int size = Udp.parsePacket();
//    if (size >= NTP_PACKET_SIZE) {
//      //Serial.println("Receive NTP Response");
//      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
//      unsigned long secsSince1900;
//      // convert four bytes starting at location 40 to a long integer
//      secsSince1900 = (unsigned long)packetBuffer[40] << 24;
//      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
//      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
//      secsSince1900 |= (unsigned long)packetBuffer[43];
//      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
//    }
//  }
//  return 0;  // return 0 if unable to get the time
//}
//
//// send an NTP request to the time server at the given address
//void sendNTPpacket(IPAddress & address) {
//  // set all bytes in the buffer to 0
//  memset(packetBuffer, 0, NTP_PACKET_SIZE);
//  // Initialize values needed to form NTP request
//  // (see URL above for details on the packets)
//  packetBuffer[0] = 0b11100011;  // LI, Version, Mode
//  packetBuffer[1] = 0;           // Stratum, or type of clock
//  packetBuffer[2] = 6;           // Polling Interval
//  packetBuffer[3] = 0xEC;        // Peer Clock Precision
//  // 8 bytes of zero for Root Delay & Root Dispersion
//  packetBuffer[12] = 49;
//  packetBuffer[13] = 0x4E;
//  packetBuffer[14] = 49;
//  packetBuffer[15] = 52;
//  // all NTP fields have been given values, now
//  // you can send a packet requesting a timestamp:
//  Udp.beginPacket(address, 123);  //NTP requests are to port 123
//  Udp.write(packetBuffer, NTP_PACKET_SIZE);
//  Udp.endPacket();
//}

//____________
void SensaProg() {
  if (digitalRead(SwProgram) == 0) {
    if (HayDeteccionAux == 0) {
      Serial.println("Deteccion entrada AUXILIAR");
      HayDeteccionAux = 1;
      CuentaTiempoDefault = 1;
      //empieza a contar tiempo para borrar red o default!
      CntTimDefault = 1;
      EstaContandoTiempoResetRed = 0;
      EstaContandoTiempoDefault = 0;

      digitalWrite(LED_AZUL, HIGH);  //LOW); //on led amarillo
      SyncLED_AZUL = 1;
      delay(1000);  //minimo 1 segun de led on para empezar la cuenta
      //Serial.println("Debe empezar a contar 1 segundo y alternar led ON/OFF");
    }

    //____________
  } else {
    // no esta pulsado boton de programacion
    if (HayDeteccionAux == 1) {
      //Serial.println("Normalizo pulsador AUXILIAR");
      HayDeteccionAux = 0;
      // segun si estuvo pulsado anytes-> cuenta totqal de pulsos si nçlos hubos
      if (CuentaTiempoDefault == 1) {
        if (EstaContandoTiempoResetRed == 1) {
          Serial.println("esta contando mas de 5 pulsos -> directo a reset valor red WIFI!");

          EstaContandoTiempoResetRed == 0;  //ACEPTA QUE CONTO MAS DE 3 PERO MENOS DE 6 SEGUNDOS
          //se genera borrado de red por pulsacion constante pero desdpues de dos reset
          Serial.println("Pide nuevos datos de red!");

          //DatoE2 = 1;
          //address = E2_BORRA_RED;
          //WrE2();
          delay(100);
          WiFiManager wifiManager;
          wifiManager.resetSettings();
          CuentaTiempoDefault = 0;
          CntTimDefault = 0;
          EstaContandoTiempoDefault = 0;
          EstaContandoTiempoResetRed = 0;
          Serial.println("GENERO BORRADO DE RED/PASSWORD!");
          ESP.reset();
          Serial.println("GENERA RESET MODULO");
        }

        else if (EstaContandoTiempoDefault == 1) {
          EstaContandoTiempoDefault == 0;  //Acepta que conto mas de 6 pulsos
          Serial.println("esta contando mas de 6 pulsos -> directo a DEFAULT!");
          CargaDatosDef();
          //queda encendido led amarillo hasta que hay algun usuario !!!!!!!!!!!!!!!!!!!!
          CuentaTiempoDefault = 0;
          CntTimDefault = 0;
          EstaContandoTiempoDefault = 0;
          EstaContandoTiempoResetRed = 0;
          // SI CONTO MAS DE 6 SEGUNDO ES BAJA DEFAULT
          //AVISO QUE ENTRO EN DEFAULT
          for (int i = 0; i < 6; i++) {
            Serial.println(variable);
            digitalWrite(LED_AZUL, HIGH);  //LOW);
            delay(100);
            digitalWrite(LED_AZUL, LOW);  //HIGH);
            delay(1000);
            Serial.println("PASA POR ACA ALGUNA VEZ?");
          }

        } else {
          //Serial.println("Conto menos de 3 pulsos -> NO HACE NADA");
          digitalWrite(LED_AZUL, LOW);
          SyncLED_AZUL = 0;
          //Serial.println("Deja de contar tiempo previo");
          CuentaTiempoDefault = 0;
          CntTimDefault = 0;
          EstaContandoTiempoDefault = 0;
          EstaContandoTiempoResetRed = 0;
        }
      } else {
        //Serial.println("Deja de contar tiempo previo");
        CuentaTiempoDefault = 0;
        CntTimDefault = 0;
      }
    } else {
      //Serial.println("No hubo pulsacion previa");
    }
  }
}


//________________
void ArmaEstadoInOutActual() {
  resultado = "MUESTRA ESTADO ACTUAL ";
  LeeStSirena();
  if (digitalRead(IN_SENS_SIR) == 1) {  // 1=desactivada !
    resultado = resultado + "SIR_0,";
  } else {
    resultado = resultado + "SIR1,";
  }
  //________________
  if (digitalRead(IN_ARM_DESAR) == 1) {
    delay(200);
    if (digitalRead(IN_ARM_DESAR) == 1) {
      //genera 200 MSos de demora para confirmar si no esta oscilando
      resultado = resultado + "DISARM,";
      address = E2_ST_ARMED;
      DatoE2 = 0;
      WrE2();
    }
  } else {
    resultado = resultado + "ARMED,";
    address = E2_ST_ARMED;
    DatoE2 = 1;
    WrE2();
  }
  //________
  if (digitalRead(Out_PGM) == 0) {
    delay(50);
    if (digitalRead(Out_PGM) == 0) {
      StPGM1 = 0;
      resultado = resultado + "PGM1_0,";
    }
  } else {
    delay(100);
    if (digitalRead(Out_PGM) == 1) {
      StPGM1 = 1;
      resultado = resultado + "PGM1_1,";
    }
  }
  //_______________
  if (digitalRead(IN_BAT) == 0) {
    StBateria = 0;
    resultado = resultado + "BAT_0,";

  } else {
    resultado = resultado + "BAT_1,";
    StBateria = 1;
  }

  resultado = resultado + "\r\n" + VersionModulo + "\r\n";
  resultado = resultado + FechaVersion + "\r\n";
  Serial.println(resultado);
}

void CortaSalidaPGM1() {
  DatoE2 = 00;
  address = E2TimFaltaPGM;
  WrE2();
  address = E2_PGM_PEND;
  WrE2();
  address = E2_CNT_MINPGM;
  WrE2();
  address = E2_CNT_HRPGM;
  WrE2();

  //Serial.println("BORRA TIEMPO PEDIENTE DE EEPROM DE BACKUP PGM");
  Serial.println("DESACTIVA PGM1");
  digitalWrite(Out_PGM, LOW);
  CntTotSeg60 = 0;

  EsCuentaMin = 0;
  EsCuentaHr = 0;
  EsCuentaSeg = 0;

  HayTiempoFaltante = 0;
  CntSegMin = 0;

  CntSegHrPGM = 0;
  CntMinHrPGM = 0;

  StPGM1 = 0;
  CntTimOut1 = 0;  //Anula tiempo de pgm 1

  if (DesactivaPorTiempoPGM1 == 1) {
    Serial.println("El corte de la salida auxiliar fue por tiempo");
    FuePorTiempo = 1;
  } else {
    Serial.println("El corte de la salida auxiliar fue por accion de usuario");
    FuePorTiempo = 0;
  }

  CmdAccion = "Desactivacion salida auxiliar";
  Descripcion = CmdAccion;
  DesactivaPorTiempoPGM1 = 0;
  ArmaRespuestaDatosUsu();
  //mal por envio por tiempo de test!
  DatosGenerador = RamIdModRx + "," + RamTelefonoUsuaRx + "," + RamNombreUsua;
  Causa = RamNombreUsua;
  TelefonoCausa = RamTelefonoUsuaRx;

  //____________________
  //ojo aqui que es por tienpo
  if (FuePorTiempo == 1) {
    FuePorTiempo = 0;
    RamTelefonoUsuaRx = "Por tiempo";
    RamIdModRx = NumSerialESP;
    RamNombreUsua = "accion interna";
    DatosGenerador = RamIdModRx + "," + RamTelefonoUsuaRx + "," + RamNombreUsua;
    Causa = RamNombreUsua;
    TelefonoCausa = RamTelefonoUsuaRx;
  }
  if (ValUbicacionMod != "ubicacion de modulo") {
    EnviaNotificacionPush(NumSerialESP);
  }
}

void EnviaMensajeEnviarAUsuario() {
  LeeDatosMemoria();
  Serial.println("Reporta a usuario : " + IdDestino);
  IDTelDestinatario = IdDestino;

  ArmaEstadoInOutActual();
  String CmdAccion = "datosmodulo " + NumSerialESP + "," + ValUbicacionMod + "," + ValTelUs1 + "," + ValTelUs2 + ","
                     + ValTelUs3 + "," + ValTelUs4 + "," + ValTelUs5 + "," + ValTelUs6 + "," + ValTelUs7 + "," + ValTelUs8 + ","
                     + ValNombUs1 + "," + ValNombUs2 + "," + ValNombUs3 + "," + ValNombUs4 + "," + ValNombUs5 + "," + ValNombUs6 + ","
                     + ValNombUs7 + "," + ValNombUs8 + "," + ValRutId1 + "," + ValRutId2 + "," + ValRutId3 + "," + ValRutId4 + "," + ValRutId5 + "," + ValRutId6 + ","
                     + ValRutId7 + "," + ValRutId8 + "," + ValModoPGM1 + "," + ValModoPGM2 + "," + ValTimPGM1 + "," + ValTimRestaPGM + "," + ValModoCuentaPGM1 + ","
                     + ValModoCuentaPGM + "," + Val_NIV_PUL + "," + ValTimOutAD + ",";
  CmdAccion = CmdAccion + resultado;
  LeeReloj();
  Serial.println("***");
  //Serial.println("PAQUETE POR MQTT A ENVIAR A TELEFONO SOLICITANTE");

  CmdBasico = "&from=" + NumSerialESP + "&to=" + IdDestino + "&data=";
  CmdBasico = CmdBasico + CmdAccion + HoraNotificacion;
  //CmdBasico=CmdBasico + HoraNotificacion;
  MensajeEnviar = CmdBasico;
  Serial.println(MensajeEnviar);
  mqttClient.publish(String(IDTelDestinatario).c_str(), 0, false, String(MensajeEnviar).c_str());
  variable = "";
  DatoRecibido = "";
}

void EnviaComandoADestino() {
  if (HayInternet == 1) {
    IDTelDestinatario = IdDestino;
    CmdBasico = "&from=" + NumSerialESP + "&to=" + IdDestino + "&data=";
    CmdBasico = CmdBasico + CmdAccion;
    MensajeEnviar = "message" + CmdBasico;
    Serial.println("Texto enviar : " + MensajeEnviar);
    //18/3/25
    Serial.println("Publica como: " + GeneradorPublicacion);
    mqttClient.publish(String(IDTelDestinatario).c_str(), 0, false, String(MensajeEnviar).c_str());

  } else {
    Serial.println("No puede enviar comando porque no hay internet");
  }
  Serial.println("________________");
}

void BuscaCoincideIdUsua() {
  //Determina di el destino corresponde a un usuario habilitado
  ExisteUsuario = 0;
  if (IdDestino == ValTelUs1) {
    ExisteUsuario = 1;
  } else if (IdDestino == ValTelUs2) {
    ExisteUsuario = 1;
  } else if (IdDestino == ValTelUs3) {
    ExisteUsuario = 1;
  } else if (IdDestino == ValTelUs4) {
    ExisteUsuario = 1;
  } else if (IdDestino == ValTelUs5) {
    ExisteUsuario = 1;
  } else if (IdDestino == ValTelUs6) {
    ExisteUsuario = 1;
  } else if (IdDestino == ValTelUs7) {
    ExisteUsuario = 1;
  } else if (IdDestino == ValTelUs8) {
    ExisteUsuario = 1;
  }
}

//&from=131ab0&to=5060141652&data= 131ab0,1234,1111,ubicacion de modulo,,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,00,00,-------,0,00,00,-------,0,00,00,-------,0,00,00,-------,0,00,00,-------,0,00,00,-------,0,00,00,-------,0,00,00,-------,0,

//_____________
void borraBloqueE2prom10() {
  resultado1 = string_variable;
  string_variable = "          ";
  GrabaPaqueteE2();
  string_variable = resultado1;
}
//_____________
void borraBloqueE2prom20() {
  resultado1 = string_variable;
  string_variable = "                    ";
  GrabaPaqueteE2();
  string_variable = resultado1;
}

void ClearDatosIDNombre() {
  CargaDefUsuario1();
  CargaDeftNombre1();
  //___
  CargaDefUsuario2();
  CargaDeftNombre2();
  //___
  CargaDefUsuario3();
  CargaDeftNombre3();
  //___
  CargaDefUsuario4();
  CargaDeftNombre4();
  //___
  CargaDefUsuario5();
  CargaDeftNombre5();
  //___
  CargaDefUsuario6();
  CargaDeftNombre6();
  //___
  CargaDefUsuario7();
  CargaDeftNombre7();
  //___
  CargaDefUsuario8();
  CargaDeftNombre8();
}
//______________
void GrabaE2_TelUsProgramado() {
  //resultado = recibido.substring(pos + 5, pos + 15);
  Serial.println("Telefono a guardar : " + resultado);
  int j = 0;
  for (int i = address; i < address + resultado.length(); i++) {
    EEPROM.write(i, resultado[j]);  //Write one by one with starting address of 0x0F
    j = j + 1;
  }
  EEPROM.commit();  //Store data to EEPROM
}

void GrabaE2_Buffer() {
  //Serial.println("DATO A GRABAR: " + resultado);
  int j = 0;
  for (int i = address; i < address + resultado.length(); i++) {
    EEPROM.write(i, resultado[j]);  //Write one by one with starting address of 0x0F
    j = j + 1;
  }
  EEPROM.commit();  //Store data to EEPROM
}

void ArmaRespuestaDatosUsu() {
  if (DebeDesactivarSalida1 == 1) {
    RamIdModRx = NumSerialESP;
    RamTelefonoUsuaRx = "interna";
    RamNombreUsua = ValUbicacionMod;

  } else {
    ValorBuscado = ",";
    pos = variable.indexOf(ValorBuscado);
    RamIdModRx = variable.substring(pos + 1, pos + 7);
    RamTelefonoUsuaRx = variable.substring(pos + 8, pos + 18);
    RamNombreUsua = variable.substring(pos + 19, pos + 39);
  }
  Serial.println(RamIdModRx);
  Serial.println(RamTelefonoUsuaRx);
  Serial.println(RamNombreUsua);
  Causa = RamTelefonoUsuaRx;
  TelefonoCausa = RamNombreUsua;
  //Serial.println("****************");
  //Serial.println(DatosGenerador);
  //Serial.println(Causa);
  //Serial.println(TelefonoCausa);
}

//___________________________________________
//void ReporteMod() {
//  String Destino;
//  ArmaRespuestaDatosUsu();
//  LeeTelUsua1();
//  LeeNomUsua1();
//  Serial.println(NumSerialESP);
//  Serial.println(ValTelUs1);
//  Serial.println(ValNombUs1);
//  DatosGenerador = NumSerialESP + "," + ValTelUs1 + "," + ValNombUs1;
//  CmdAccion = "prueba";
//  Serial.println(DatosGenerador);
//  EnviaNotificacionPush(E2_NP_TEST);
//}


void ActualizaNombreUs1() {
  Serial.println("Nombre usuario 1: " + ValNombUs1);
  address = E2_NomUs1;
  borraBloqueE2prom20();
  resultado = ValNombUs1;
  GrabaE2_Buffer();
  LeeNomUsua1();
}

void ActualizaNombreUs2() {
  Serial.println("Nombre usuario 2: " + ValNombUs2);
  address = E2_NomUs2;
  borraBloqueE2prom20();
  resultado = ValNombUs2;
  GrabaE2_Buffer();
  LeeNomUsua2();
}

void ActualizaNombreUs3() {
  Serial.println("Nombre usuario 3: " + ValNombUs3);
  address = E2_NomUs3;
  borraBloqueE2prom20();
  resultado = ValNombUs3;
  GrabaE2_Buffer();
  LeeNomUsua3();
}

void ActualizaNombreUs4() {
  Serial.println("Nombre usuario 4: " + ValNombUs4);
  address = E2_NomUs4;
  borraBloqueE2prom20();
  resultado = ValNombUs4;
  GrabaE2_Buffer();
  LeeNomUsua4();
}

void ActualizaNombreUs5() {
  Serial.println("Nombre usuario 5: " + ValNombUs5);
  address = E2_NomUs5;
  borraBloqueE2prom20();
  resultado = ValNombUs5;
  GrabaE2_Buffer();
  LeeNomUsua5();
}

void ActualizaNombreUs6() {
  Serial.println("Nombre usuario 6: " + ValNombUs6);
  address = E2_NomUs6;
  borraBloqueE2prom20();
  resultado = ValNombUs6;
  GrabaE2_Buffer();
  LeeNomUsua6();
}

void ActualizaNombreUs7() {
  Serial.println("Nombre usuario 7: " + ValNombUs7);
  address = E2_NomUs7;
  borraBloqueE2prom20();
  resultado = ValNombUs7;
  GrabaE2_Buffer();
  LeeNomUsua7();
}

void ActualizaNombreUs8() {
  Serial.println("Nombre usuario 8: " + ValNombUs8);
  address = E2_NomUs8;
  borraBloqueE2prom20();
  resultado = ValNombUs8;
  GrabaE2_Buffer();
  LeeNomUsua8();
}

/*
   if (topic == "home/office/esp1/desk") {
     Serial.print("Changing Desk Light to ");
     if (incoming == "1") {
       // Sends binary code to turn on Desk Light
       // BINARY CODE EXAMPLE. REPLACE WITH YOUR BINARY CODE
       mySwitch.send("000101010101000101010101");
       Serial.print("On");
     }
     else if (incoming == "0") {
       // Sends binary code to turn off Desk Light
       // BINARY CODE EXAMPLE. REPLACE WITH YOUR BINARY CODE
       mySwitch.send("000101010101000101010100");
       Serial.print("Off");
     }
   } 45684743
*/
/*
  <form action="/get" target="hidden-form">
  inputString (current value %inputString%): <input type="text" name="inputString">
  <input type="submit" value="Submit" onclick="submitMessage()">
  </form>
*/

void UbicaPosicionNewUsu() {
  if (DatoE2 == 1) {
    //es para telefono administrador
    string_variable = IDUsuarioNuevo;
    address = E2_TelUs1;
    GrabaPaqueteE2();
    LeeTelUsua1();

    string_variable = NombreUsuarioNuevo;
    address = E2_NomUs1;
    GrabaPaqueteE2();
    LeeNomUsua1();
    Serial.println("___________");

  } else if (DatoE2 == 2) {
    string_variable = IDUsuarioNuevo;
    address = E2_TelUs2;
    GrabaPaqueteE2();
    LeeTelUsua2();

    string_variable = NombreUsuarioNuevo;
    address = E2_NomUs2;
    GrabaPaqueteE2();
    LeeNomUsua2();
    Serial.println("___________");

  } else if (DatoE2 == 3) {
    string_variable = NombreUsuarioNuevo;
    address = E2_NomUs3;
    GrabaPaqueteE2();
    LeeNomUsua3();

    string_variable = IDUsuarioNuevo;
    address = E2_TelUs3;
    GrabaPaqueteE2();
    LeeTelUsua3();
    Serial.println("___________");


  } else if (DatoE2 == 4) {
    address = E2_TelUs4;
    string_variable = IDUsuarioNuevo;
    GrabaPaqueteE2();
    LeeTelUsua4();

    string_variable = NombreUsuarioNuevo;
    address = E2_NomUs4;
    GrabaPaqueteE2();
    LeeNomUsua4();
    Serial.println("___________");

  } else if (DatoE2 == 5) {
    string_variable = NombreUsuarioNuevo;
    address = E2_NomUs5;
    GrabaPaqueteE2();
    LeeNomUsua5();

    string_variable = IDUsuarioNuevo;
    address = E2_TelUs5;
    GrabaPaqueteE2();
    LeeTelUsua5();

    Serial.println("___________");

  } else if (DatoE2 == 6) {
    string_variable = IDUsuarioNuevo;
    address = E2_TelUs6;
    GrabaPaqueteE2();
    LeeTelUsua6();

    string_variable = NombreUsuarioNuevo;
    address = E2_NomUs6;
    GrabaPaqueteE2();
    LeeNomUsua6();
    Serial.println("___________");

  } else if (DatoE2 == 7) {
    string_variable = IDUsuarioNuevo;
    address = E2_TelUs7;
    GrabaPaqueteE2();
    LeeTelUsua7();

    string_variable = NombreUsuarioNuevo;
    address = E2_NomUs7;
    GrabaPaqueteE2();
    LeeNomUsua7();
    Serial.println("___________");

  } else if (DatoE2 == 8) {
    string_variable = IDUsuarioNuevo;
    address = E2_TelUs8;
    GrabaPaqueteE2();
    LeeTelUsua8();

    string_variable = NombreUsuarioNuevo;
    address = E2_NomUs8;
    GrabaPaqueteE2();
    LeeNomUsua8();
    Serial.println("___________");
  }
}

//_____
void GeneraPulsoParcial() {
  LeeTiempoPulsoAD();
  Serial.println(ValTimOutAD);
  Serial.println("segundos");
  ValTimOutAD = ValTimOutAD * 3;  //cambiar para multiplicar al tiempo programado
  ComunGeneraPulsoArmado();
}

//_________
void ComunGeneraPulsoArmado() {
  digitalWrite(Out_CHG_AD, HIGH);  //ON led de salida ARM/DESARM
  delay(ValTimOutAD * 1000);
  digitalWrite(Out_CHG_AD, LOW);
}

//_____
void GeneraPulsoAD() {
  LeeTiempoPulsoAD();
  //Serial.println(ValTimOutAD);
  //Serial.println("segundos");
  digitalWrite(Out_CHG_AD, HIGH);  //ON led de salida ARM/DESARM
  //Serial.println("PULSO A 1");
  delay(ValTimOutAD * 1000);
  digitalWrite(Out_CHG_AD, LOW);
  //Serial.println("PULSO A 0");
}
//________
void Generar_NP_ACK_AD() {
  ArmaRespuestaDatosUsu();
  RamIdModRx = NumSerialESP;
  if (DebeMostrarTelefonoOrigen == 1) {
    DatosGenerador = RamIdModRx + "," + RamTelefonoUsuaRx + "," + RamNombreUsua;
  } else {
    DatosGenerador = RamIdModRx + "," + "deteccion local" + "," + "accion interna";
  }

  if (DebeMostrarTelefonoOrigen == 1) {
    Causa = RamIdModRx;
    TelefonoCausa = RamTelefonoUsuaRx;
  } else {
    //29/4/22
    Causa = "Desde su alarma";
    TelefonoCausa = RamIdModRx;
  }
  LeeUbicacionMod();

  ValUbicacionMod.trim();
  if (ValUbicacionMod != "ubicacion de modulo") {
    //Serial.println("ENVIA NOTIFICACION!");
    EnviaNotificacionPush(NumSerialESP);
  } else {
    DebeMostrarTelefonoOrigen = 0;
    Serial.println("NO ENVIA NOTIFICACION PORQUE EL MODULO ESTA BORRADO!");
  }
}

void ActualizaTiempoFaltante() {
  DatoE2 = CntTimOut1;
  address = E2TimFaltaPGM;
  WrE2();
}

//_____
void DefineTipoConteoTimPGM() {
  if (ValModoCuentaPGM1 == 1) {
    Serial.println("Cuenta tiempo PGM 1 EN SEGUNDOS");
    EsCuentaMin = 0;  //habilita cuenta en SEGUNDOS para PGM 1
    EsCuentaSeg = 1;  //habilita cuenta segundos
    EsCuentaHr = 0;

  } else if (ValModoCuentaPGM1 == 2) {
    //Serial.println("Cuenta tiempo PGM1 en MINUTOS");
    EsCuentaMin = 1;  // habilita cuenta en MINUTOS para PGM 1
    EsCuentaSeg = 0;
    EsCuentaHr = 0;
    CntSegMin = 0;

  } else if (ValModoCuentaPGM1 == 3) {
    EsCuentaHr = 1;  // habilita cuenta en HORAS para PGM 1
    EsCuentaSeg = 0;
    EsCuentaMin = 0;
    //Serial.println("Cuenta tiempo PGM1 en HORAS");

  } else {
    //Serial.println("ESTA EN TOGGLE?");
    EsCuentaMin = 0;
    EsCuentaSeg = 0;
    EsCuentaHr = 0;
  }
}

//_________________
//Recibe un texto MMQTT

void ArrivedMqtt(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  // Save topic in a String variable
  String receivedTopic = String(topic);
  Serial.print("Received Topic: ");
  Serial.println(receivedTopic);
  String incoming = "";
  for (int i = 0; i < len; i++)  {
    incoming += (char)payload[i];
  }
  incoming.trim();

  Serial.println("DATO RECIBIDO POR MQTT!");
  Serial.println(incoming);
  DatoRecibido = incoming;

  ValorBuscado = "datosOTA";
  pos = DatoRecibido.indexOf(ValorBuscado);
  if (pos >= 0) {
    EsActualizadorOta = 1;
    Serial.println("HAY COMANDO DESDE ACTUALIZADOR de solicitar datos");
  }

  ValorBuscado = "https://";
  pos = DatoRecibido.indexOf(ValorBuscado);
  if (pos >= 0) {
    EsActualizadorOta = 1;
    Serial.println("Hay comando actualizador programa OTA");
  }

  //  if (DelStart == 0) {
  if (HayInicio == 0) {
    DatoRecibido = incoming;
    String str_topic(topic);
    if (String(topic) == "PROGMODULO" || String(topic) == String(NumSerialESP).c_str()) {
      ValorBuscado = "&to=";
      pos = DatoRecibido.indexOf(ValorBuscado);
      if (pos >= 0) {
        resultado = DatoRecibido.substring(pos + 4, pos + 10);
        Serial.println("modulo buscado");
        Serial.println(resultado);
      }
      if (resultado == NumSerialESP) {
        Serial.println("Se va a procesar este mensaje -> " + DatoRecibido);

      } else {
        Serial.println("No es modulo solicitado");
        DatoRecibido = "";
      }
    }
  }
}

void GeneraBeepsLed() {
  for (int i = 0; i < TotalLeds; i++) {
    digitalWrite(LedAmarillo, LOW);
    delay(120);
    digitalWrite(LedAmarillo, HIGH);  //OFF
    delay(120);
  }
}

void GeneraBeepsLed1() {
  for (int i = 0; i < TotalLeds; i++) {
    digitalWrite(LedAmarillo, LOW);
    delay(70);
    digitalWrite(LedAmarillo, HIGH);  //OFF
    delay(70);
  }
}

void ActivaPGM() {
  Serial.println("ACTIVA PGM1");
  digitalWrite(Out_PGM, HIGH);
  StPGM1 = 1;
  LeeModoConteoPGM1();
  LeeTiempoPGM1();
  CntTimOut1 = ValTimPGM1;

  DatoE2 = CntTimOut1;
  address = E2TimFaltaPGM;
  WrE2();

  DatoE2 = 1;           //esta contando tiempo de backup
  address = E2_PGM_PEND;
  WrE2();
  HayTiempoFaltante = EEPROM.read(address);

  address = E2ModoPGM;  //tipo de cuenta
  DatoE2 = ValModoCuentaPGM1;
  WrE2();

  address = E2ModoPGM;
  ValModoCuentaPGM1 = EEPROM.read(address);
  DefineTipoConteoTimPGM();
  if (ValModoCuentaPGM1 == 3) {
    DatoE2 = 0;
    address = E2_CNT_MINPGM;
    WrE2();
    address = E2_CNT_MINPGM;
    Serial.println("Inicio MINUTOS en: " + String(CntMinHrPGM));
    //____________
    DatoE2 = CntTimOut1;
    address = E2_CNT_HRPGM;
    WrE2();
    Serial.println("Inicia ciclo  PGM en HORAS: " + String(CntTimOut1));
  }
  if (ValUbicacionMod != "ubicacion de modulo") {
    CmdAccion = "Activacion salida auxiliar";
    if (EsTestTiempo == 0) {
      ArmaRespuestaDatosUsu();
    } else {
      EsTestTiempo = 0;
    }
    DatosGenerador = RamIdModRx + ", " + RamTelefonoUsuaRx + ", " + RamNombreUsua;  //
    Serial.println("DATOS GENERADOR: " + DatosGenerador);
    Causa = RamNombreUsua;
    TelefonoCausa = RamTelefonoUsuaRx;
    EnviaNotificacionPush(NumSerialESP);
  }
}

//modificado 19/9/24
//******************************************************************************************
//Esta función ahora envia los datos al rest, que se encarga de la comunicación con firebase para enviar la notificación
void EnviaNotificacionPush(String pTopic) {
  LeeReloj();
  //digitalClockDisplay();
  String strBody = "";
  Serial.println("ENVIA NOTIFICACION!");
  // ATENCION -> Envia el evento, esta en el TITULO DE LA NOTIFICACION y en ANDROID dentro del body
  // delay(1000);
  Serial.println("[HttpPost]" + pTopic);

  // Cambiar el host y URL para la nueva API
  String host = "gonner.com.ar";
  String url = "/wfactionrest/enviarNotificacion.php";
  String key = "AAAA-kdeDmM:APA91bFlFacmzrO21jZZP-x8svI174gDLqXqCOFbCFLzy0rmVnzmJaEdqOD9Ry6Ewbda4mzUCpqlbGZMaL9iZYtgVpIs5X8aOzRWdrEC2si6qovdOVAwdZLTy1GvJ5zZViHSzbjf0zMl";

  // Crear el cuerpo de la notificación (title, body, topic)
  StaticJsonDocument<512> doc;
  JsonObject root = doc.to<JsonObject>();

  Serial.println(DatosGenerador);
  Serial.println(CmdAccion);
  strBody = "";
  strBody = strBody + CmdAccion + "\r\n";
  strBody = strBody + ValUbicacionMod + "\r\n";
  strBody = strBody + HoraNotificacion + "\r\n";
  strBody = strBody + DatosGenerador + "\r\n";
  strBody = strBody + NumSerialESP + "\r\n";
  root["title"] = "Evento en " + ValUbicacionMod;  // Campo title
  root["body"] = strBody;                          // Campo body
  root["topic"] = pTopic;                          // Campo topic

  String data = "";
  serializeJson(root, data);

  //-----------------------------------------------------------------
  WiFiClientSecure client2;
  client2.setInsecure();

  if (client2.connect(host.c_str(), 443)) {  // Puerto 443 para HTTPS
    client2.println("POST " + url + " HTTP/1.1");
    client2.println("Host: " + (String)host);
    client2.println("User-Agent: ESP8266/1.0");
    client2.println("Connection: close");
    client2.println("Content-Type: application/json");
    client2.println("X-Authorization: key=" + key);  // El header X-Authorization que usa el rest
    client2.print("Content-Length: ");
    client2.println(data.length());
    client2.println();
    client2.println(data);
    delay(20);
    //delay(2000);
    TotalLeds = 3;
    GeneraBeepsLed1();
    EsPorPedidoUsua = 0;
    Serial.println("***");
    Serial.println(strBody);
  } else {
    //Se puede imprimir en caso de error de conexión
    //Serial.println("Conexión fallida");
  }
  //sendDataIOSFirebase();
}


//_______________________________
String leerVersionDesdeEEPROM() {
  address = E2_VERSION;
  VersionModulo = "";
  for (int i = address; i < address + 3; i++) {
    VersionModulo = VersionModulo + char(EEPROM.read(i));
  }
  //VersionModulo = VersionModulo.substring(0, 4);
  Serial.println("Version recuperada: " + String (VersionModulo));
  return VersionModulo;
}

//______
// Función para leer la fecha de última actualización desde EEPROM
String leerFechaDesdeEEPROM() {
  address = E2_HORA_FECHA;
  FechaVersion = "";
  for (int i = address; i < address + 17; i++) {
    FechaVersion = FechaVersion + char(EEPROM.read(i));
  }
  Serial.println("Fecha recuperada: " + String(FechaVersion));
  return FechaVersion;
}

// Función para guardar la versión en EEPROM
void guardarVersionEnEEPROM(String version) {
  address = E2_VERSION;
  string_variable = version;
  Serial.println(string_variable);
  GrabaPaqueteE2();
  Serial.println("Guardo version en eeprom");
}

//___________
// Función para guardar la fecha y hora en EEPROM
void guardarFechaHora() {
  address = E2_HORA_FECHA;
  string_variable = HoraNotificacion;
  GrabaPaqueteE2();
  Serial.println("Guardo fecha actualizada");
  Serial.println(" ");
}
//___________
void incrementarVersion() {
  int major, minor;
  sscanf(VersionModulo.c_str(), "%d.%d", &major, &minor);
  minor++;
  if (major > 9) {
    major = 0;
    minor = 1;
  }
  if (minor > 9) {
    minor = 0;
    major++;
  }
  VersionModulo = String(major) + "." + String(minor);
  //Serial.println(VersionModulo);
  guardarVersionEnEEPROM(VersionModulo);
  Serial.println("VERSION incrementada ACTUALIZADA: " + VersionModulo);
}

//void LeeIpPublica() {
//WiFiClient client;
//HTTPClient http;
//
//  Serial.println("[HTTP] Intentando obtener la IP pública...");
//  http.begin(client, "http://api.ipify.org/");
//
//  int httpCode = http.GET();
//  if (httpCode > 0) {
//    Serial.printf("[HTTP] Código HTTP recibido: %d\n", httpCode);
//    if (httpCode == HTTP_CODE_OK) {
//      ipPublica = http.getString();
//      ipPublica.trim(); // Eliminar espacios en blanco
//      Serial.print("IP pública encontrada: ");
//      Serial.println(ipPublica);
//    }
//  } else {
//    Serial.printf("[HTTP] Error en GET: %s\n", http.errorToString(httpCode).c_str());
//  }
//
//  http.end();
//}

void ArmaVersionFechaOTA() {
  //digitalClockDisplay();
  //Serial.println(HoraNotificacion);
  //guardarFechaHora();
  // leerVersionDesdeEEPROM();
  //Serial.println(" ");
  //incrementarVersion();
  //guardarVersionEnEEPROM(VersionModulo);
  //Serial.println(" ");
  //leerVersionDesdeEEPROM();
}


//_____________________
void CargaDatosDef() {
  Serial.println("_______________________");
  Serial.println("Grabando default de eeprom");
  DefUbicacion();
  //LeeUbicacionMod();
  // guarda default de datos de USUARIO (ID y UBICACION)
  ClearDatosIDNombre();

  //____________
  DatoE2 = 2;
  address = E2ModoPGM;
  WrE2();
  //____________
  DatoE2 = 2;
  address = E2TimPGM1;
  WrE2();

  //__
  DatoE2 = 1;
  address = E2TimPulAD;  //tiempo del pulso de armado/desarmado
  WrE2();

  //____________
  DatoE2 = 2;  //minutos
  address = E2_TIPO_TIM1;
  WrE2();

  //____________
  DatoE2 = 0;

  address = E2TimFaltaPGM;
  WrE2();

  address = E2_PGM_PEND;
  WrE2();

  //address = E2ModoPGM;
  //WrE2();


  //__________
  DatoE2 = 0;            //MODO PULSO
  address = E2_NIV_PUL;  //1=Modo nivel /00=modo pulso de determinada duracion
  WrE2();

  //____________
  DatoE2 = 15;
  address = E2_Ruteo_ID1;
  WrE2();
  //____________
  DatoE2 = 15;
  address = E2_Ruteo_ID2;
  WrE2();
  //____________
  DatoE2 = 15;
  address = E2_Ruteo_ID3;
  WrE2();
  //____________
  DatoE2 = 15;
  address = E2_Ruteo_ID4;
  WrE2();
  //____________
  DatoE2 = 15;
  address = E2_Ruteo_ID5;
  WrE2();
  //____________
  DatoE2 = 15;
  address = E2_Ruteo_ID6;
  WrE2();
  //____________
  DatoE2 = 15;
  address = E2_Ruteo_ID7;
  WrE2();

  //____________
  DatoE2 = 15;
  address = E2_Ruteo_ID8;
  WrE2();
  EstaContandoTiempoResetRed = 0;
  EstaContandoTiempoDefault = 0;
}

void LeeReloj() {
  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();
  // Serial.print("Epoch Time: ");
  //Serial.println(epochTime);

  String formattedTime = timeClient.getFormattedTime();
  //Serial.print("Formatted Time: ");
  //Serial.println(formattedTime);

  int currentHour = timeClient.getHours();
  //Serial.print("Hour: ");
  //Serial.println(currentHour);

  int currentMinute = timeClient.getMinutes();
  //Serial.print("Minutes: ");
  //Serial.println(currentMinute);

  int currentSecond = timeClient.getSeconds();
  //Serial.print("Seconds: ");
  //Serial.println(currentSecond);

  String weekDay = weekDays[timeClient.getDay()];
  //Serial.print("Week Day: ");
  // Serial.println(weekDay);

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime);

  int monthDay = ptm->tm_mday;
  //Serial.print("Month day: ");
  //Serial.println(monthDay);

  int currentMonth = ptm->tm_mon + 1;
  //  Serial.print("Month: ");
  // Serial.println(currentMonth);

  String currentMonthName = months[currentMonth - 1];
  // Serial.print("Month name: ");
  // Serial.println(currentMonthName);

  int currentYear = ptm->tm_year + 1900;
  // Serial.print("Year: ");
  // Serial.println(currentYear);

  //Print complete date:
  String currentDate =  String(monthDay) + "/" + String(currentMonth) + "/" + String(currentYear);
  Serial.print(currentDate);
  HoraNotificacion = formattedTime + "-" + currentDate;
  Serial.println(HoraNotificacion);
}


void printWiFiState() {
  if (EnProcesoOTA == false) {
    return;
  }
  static boolean printed = false;
  if (WiFi.status() == WL_CONNECTED) {
    if (printed)
      return;
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
    Serial.println();
    printed = true;
  } else {  // if WiFi is not connected
    if (!printed)
      return;
    Serial.println("WiFi disconnected!\n");
    printed = false;
  }
}

/*
  void actualizarFirmwareSegura(String url) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ No hay conexión WiFi. OTA cancelada.");
    IDTelDestinatario = "WiAction/ota/rx/" + String(NumSerialESP);
    MensajeEnviar = "FALTA_WIFI";
    mqttClient.publish(IDTelDestinatario.c_str(), 0, false, MensajeEnviar.c_str());
    return;
  }

  Serial.println("📥 Iniciando OTA desde: " + url);
  IDTelDestinatario = "WiAction/ota/rx/" + String(NumSerialESP);
  MensajeEnviar = "INICIA_OTA";
  mqttClient.publish(IDTelDestinatario.c_str(), 0, false, MensajeEnviar.c_str());

  WiFiClientSecure client;
  client.setInsecure();

  t_httpUpdate_return resultado = ESPhttpUpdate.update(client, url);

  switch (resultado) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("❌ OTA falló. Código: %d, %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      MensajeEnviar = "FALLO_OTA_" + String(ESPhttpUpdate.getLastError());
      mqttClient.publish(IDTelDestinatario.c_str(), 0, false, MensajeEnviar.c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("ℹ️ No hay actualizaciones.");
      MensajeEnviar = "SIN_ACTUALIZACION";
      mqttClient.publish(IDTelDestinatario.c_str(), 0, false, MensajeEnviar.c_str());
      break;

    case HTTP_UPDATE_OK:
      Serial.println("✅ OTA completada exitosamente. Reiniciando...");

      ArmaVersionFechaOTA(); // tu función interna

      MensajeEnviar = "OK_OTA";
      mqttClient.publish(IDTelDestinatario.c_str(), 0, false, MensajeEnviar.c_str());
      Serial.println("📢 Confirmación enviada a: " + IDTelDestinatario);
      delay(2000);
          ESP.restart();
      break;
  }
  }
*/

void actualizarFirmware(String url) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ No hay conexión WiFi. OTA cancelada.");
    return;
  }

  Serial.println("📥 Iniciando OTA desde: " + url);
  ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

  WiFiClientSecure client;
  client.setInsecure();  // ⚠️ Permite conexiones HTTPS sin validar el certificado
  HTTPClient http;
  if (http.begin(client, url)) { // Inicia conexión HTTPS
    Serial.println("🔍 Conexión HTTPS establecida con éxito.");

    int httpCode = http.GET();
    Serial.printf("🔍 Código de respuesta HTTP: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      int contentLength = http.getSize();
      Serial.printf("📏 Tama//o del archivo: %d bytes\n", contentLength);

      if (Update.begin(contentLength)) {
        Serial.println("📤 Espacio adecuado para iniciar actualización.");

        WiFiClient* stream = http.getStreamPtr();
        size_t totalWritten = 0;
        size_t written;
        Serial.println("📡 Descargando el firmware...");


        //
        //  HTTPClient http;
        //  if (http.begin(client, url)) {  // Comienza la conexión HTTPS
        //    int httpCode = http.GET();   // Realiza la solicitud GET
        //
        //    if (httpCode == HTTP_CODE_OK) {  // Comprueba si la respuesta es exitosa
        //      int contentLength = http.getSize();
        //      Serial.printf("🔍 Tama//o del firmware: %d bytes\n", contentLength);
        //
        //      if (Update.begin(contentLength)) {  // Inicia la actualización
        //        WiFiClient* stream = http.getStreamPtr();
        //        size_t written = Update.writeStream(*stream);

        if (written == contentLength) {
          if (Update.end()) {
            Serial.println("✅ OTA completada. Reiniciando...");
            ArmaVersionFechaOTA(); // tu función interna
            MensajeEnviar = "OK_OTA";
            mqttClient.publish(IDTelDestinatario.c_str(), 0, false, MensajeEnviar.c_str());
            Serial.println("📢 Confirmación enviada a: " + IDTelDestinatario);
            delay(2000);
            ESP.restart();

          } else {
            Serial.printf("❌ Error aplicando la actualización: %d\n", Update.getError());
            MensajeEnviar = "ERR_APLICA_OTA";
            mqttClient.publish(IDTelDestinatario.c_str(), 0, false, MensajeEnviar.c_str());
            Serial.println("📢 Confirmación enviada a: " + IDTelDestinatario);
          }
        } else {
          Serial.printf("❌ Error escribiendo la actualización: %d/%d bytes\n", written, contentLength);
          MensajeEnviar = "MAL_WR_OTA";
          mqttClient.publish(IDTelDestinatario.c_str(), 0, false, MensajeEnviar.c_str());
          Serial.println("📢 Confirmación enviada a: " + IDTelDestinatario);
        }

      } else {
        Serial.printf("❌ No hay suficiente espacio: %d\n", Update.getError());
        MensajeEnviar = "NO_HAY_SPACE_OTA";
        mqttClient.publish(IDTelDestinatario.c_str(), 0, false, MensajeEnviar.c_str());
        Serial.println("📢 Confirmación enviada a: " + IDTelDestinatario);
      }
    } else {
      Serial.printf("❌ Error al descargar el firmware: %d\n", httpCode);
      MensajeEnviar = "ERR_DESCARGA_OTA";
      mqttClient.publish(IDTelDestinatario.c_str(), 0, false, MensajeEnviar.c_str());
      Serial.println("📢 Confirmación enviada a: " + IDTelDestinatario);
    }

    DatoRecibido = "";

    http.end();
  } else {
    Serial.println("❌ Falló la conexión HTTPS.");
    MensajeEnviar = "ERR_COMUNIC_OTA";
    mqttClient.publish(IDTelDestinatario.c_str(), 0, false, MensajeEnviar.c_str());
    Serial.println("📢 Confirmación enviada a: " + IDTelDestinatario);
  }
}
