# Estacion-Monitoreo-Ambiental-Inteligente
Estación de monitoreo ambiental autónoma para Laboratorio I - UBP. Mide T/H, Luz, Gas y Sonido, con registro SD y transmisión Bluetooth.

[cite_start]El presente proyecto consiste en el desarrollo de una estación de monitoreo ambiental autónoma, diseñada para medir y registrar variables del entorno como temperatura, humedad, luminosidad, concentración de gas y nivel de sonido[cite: 86]. [cite_start]El sistema se basa en una placa Arduino Uno que integra varios sensores (DHT11, LDR, MQ5 y KY037) junto con un reloj en tiempo real DS1302 que asegura una sincronización de horario precisa. [cite_start]Los datos son almacenados automáticamente cada minuto en una microSD en formato CSV[cite: 88]. [cite_start]Para la visualización local, se usa una pantalla LCD I²C que muestra las variables en rotación, mientras que mediante el módulo bluetooth HC-05 se posibilita la transmisión de datos hacia un dispositivo móvil[cite: 89].

[cite_start]Integrantes: Juan Mazzeo, Candela Coria, Santino Marchesini [cite: 82]
[cite_start]Materia: Laboratorio I [cite: 83]
[cite_start]Año: 2025 [cite: 84]

Lista de componentes:
  Unidad de control:
[cite_start]    - **Arduino Uno** 
    
  Sensado primario:
[cite_start]    - DHT11 (Temperatura/Humedad) 
    - [cite_start]LDR (Luminosidad) 
    - [cite_start]RTC DS1302 (Reloj) 

  Sensado adicional:
[cite_start]    - MQ-5 (Sensor de Gas) 
    - [cite_start]KY-037 (Sensor de Sonido) 

  Almacenamiento:
[cite_start]    - **Módulo Lector/Adaptador MicroSD (SPI)** [cite: 88, 130]

  Interfaz local:
[cite_start]    - LCD I²C 16x2 [cite: 89]

  Comunicación:
[cite_start]    - Bluetooth HC-05 [cite: 89]

  Energía:
[cite_start]    - Fuente USB 5 V 1 A o Power-bank 10 000 mAh [cite: 142, 143]

Conexiones:

[cite_start]DHT11 --> Pin 8 [cite: 179]
[cite_start]LDR --> A1 [cite: 181]
[cite_start]MQ5 --> A0 [cite: 182]
[cite_start]KY-037 --> A2 [cite: 182]
RTC DS1302 -->
[cite_start]            IO --> 3 [cite: 182]
            [cite_start]SCLK --> 2 [cite: 182]
            [cite_start]CE --> 4 [cite: 182]
LCD I2C 16x2 -->
[cite_start]            SDA --> A4 [cite: 182]
            [cite_start]SCL --> A5 [cite: 182]
SD -->
[cite_start]  CS --> 10 [cite: 182]
  [cite_start]MOSI --> 11 [cite: 182]
  [cite_start]MISO --> 12 [cite: 182]
  [cite_start]SCK --> 13 [cite: 182]
HC-05 --> 
[cite_start]    RX --> 6 [cite: 182]
    [cite_start]TX --> 5 [cite: 182]

Instrucciones de uso

- Insertar microSD (FAT32). El sistema no **registrará** datos sin esta. Verificar que este todo bien conectado.
- Conectar la alimentación vía USB 5V/1A o Powerbank.
- Esperar el mensaje de inicio de la LCD: "SD: OK".
- [cite_start]El sistema lee, procesa y guarda en un archivo CSV cada minuto. [cite: 133]

- [cite_start]La pantalla LCD muestra las 5 variables en un ciclo de rotación (cada 4 segundos [cite: 242]):
    1. Fecha y Hora
    2. Temperatura y Humedad
    3. Luminosidad (LDR)
    4. Concentración de Gas (MQ5)
    5. Nivel de Sonido (KY-037)
 
- La app permite el monitoreo remoto:
    - Emparejar el módulo Bluetooth HC-05 desde el celular (contraseña típica: 1234/0000).
    - Abrir la aplicación y establecer la conexión con el HC-05.
    - La aplicación recibirá el nuevo conjunto de datos cada minuto, mostrando las variables en tiempo real.
