# Estacion-Monitoreo-Ambiental-Inteligente
Estación de monitoreo ambiental autónoma para Laboratorio I - UBP. Mide T/H, Luz, Gas y Sonido, con registro SD y transmisión Bluetooth.

El presente proyecto consiste en el desarrollo de una estación de monitoreo ambiental autónoma, diseñada para medir y registrar variables del entorno como temperatura, humedad, luminosidad, concentración de gas y nivel de sonido. El sistema se basa en una placa Arduino Uno que integra varios sensores (DHT11, LDR, MQ5 y KY037) junto con un reloj en tiempo real DS1302 que asegura una sincronización de horario precisa. Los datos son almacenados automáticamente cada minuto en una microSD en formato CSV. Para la visualización local, se usa una pantalla LCD I²C  que muestra las variables en rotación, mientras que mediante el módulo bluetooth HC-05 se posibilita la transmisión de datos hacia un dispositivo móvil.

Integrantes: Juan Mazzeo, Candela Coria, Santino Marchesini
Materia: Laboratorio I
Año: 2025

Lista de componentes:
  Unidad de control:
    - Arduino Uno
    
  Sensado primario:
    - DHT11
    - LDR
    - RTC DS1302

  Sensado adicional:
    - MQ-5 (gas)
    - KY-037 (sonido)

  Almacenamiento:
    - Módulo Adaptador MicroSD (SPI)

  Interfaz local:
    - LCD I²C 16x2

  Comunicación:
    - Bluetooth HC-05

  Energía:
    - Fuente USB 5 V 1 A o Power-bank 10 000 mAh

Conexiones:

DHT11 --> Pin 8
LDR --> A1
MQ5 --> A0
KY-037 --> A2
RTC DS1302 -->
            IO --> 3
            SCLK --> 2
            CE --> 4
LCD I2C 16x2 -->
            SDA --> A4
            SCL --> A5
SD -->
  CS --> 10
  MOSI --> 11
  MISO --> 12
  SCK --> 13
HC-05 --> 
    RX --> 6
    TX --> 5

Instrucciones de uso

- Insertar microSD (FAT32). El sistema no registrará datos sin esta. Verificar que este todo bien conectado.
- Conectar la alimentación vía USB 5V/1A o Powerbank.
- Esperar el mensaje de inicio de la LCD: "SD: OK".
- El sistema lee, procesa y guarda en un archivo CSV cada minuto.

- La pantalla LCD muestra las 5 variables en un ciclo de rotación (cada 4 segundos):
    1. Fecha y hora.
    2. Temperatura y humedad.
    3. Luminosidad.
    4. Concentración de gas.
    5. Nivel de sonido.
 
- La app permite el monitoreo remoto:
    - Emparejar el módulo Bluetooth HC-O5 desde el celular (contraseña típica: 1234/0000).
    - Abrir la aplicación y establecer la conexión con el HC-05.
    - La aplicación recibirá el nuevo conjunto de datos cada minuto, mostrando las variables en tiempo real.
