# Sistema embebido de monitoreo de proximidad y alerta multimodal

Prototipo basado en **Arduino Uno** que detecta si un espacio está ocupado midiendo la distancia con un sensor ultrasónico y señaliza el resultado con luces y sonido. Fue diseñado, simulado en Tinkercad y montado sobre una **maqueta a escala 1:10** de un cubículo de estudio de la Facultad de Ingeniería de la Universidad de San Buenaventura, sede Bogotá.

Proyecto del curso de Circuitos Digitales.

![Simulación en Tinkercad](docs/simulacion_tinkercad.png)

## Problemática

En los cubículos de estudio y las zonas de laboratorio de la Facultad de Ingeniería hay problemas de saturación y de gestión del espacio en las franjas de mayor afluencia: los usuarios no cuentan con una señalización que les indique en tiempo real si un puesto está disponible u ocupado. Los carteles estáticos o las reservas en papel no se adaptan a la dinámica del campus.

Este sistema mide la ocupación de forma local y continua, y la comunica de inmediato con indicadores luminosos y acústicos, a bajo costo y sin depender de infraestructura de red.

## Cómo funciona

1. El sensor **PING)))** mide la distancia al objeto o persona más cercana.
2. Un **potenciómetro** fija el umbral de detección, de 0 a 100 cm.
3. Si la distancia es menor o igual al umbral, se enciende el **LED rojo** (ocupado); si no, el **LED verde** (libre).
4. Cada vez que el estado **cambia** (entra o sale del umbral) suena el **zumbador** durante menos de un segundo.
5. Un **pulsador** silencia o reactiva el zumbador; los LED siguen funcionando.
6. En cada ciclo se envía por **UART** (9600 bps) una línea de telemetría al monitor serial.

## Lista de componentes

| Cantidad | Componente | Función |
|:--:|---|---|
| 1 | Arduino Uno (ATmega328P) y cable USB | Microcontrolador y alimentación |
| 1 | Sensor ultrasónico PING))) | Medición de distancia (tiempo de vuelo) |
| 1 | Potenciómetro | Ajuste del umbral (entrada analógica) |
| 1 | Pulsador | Silenciar / reactivar el zumbador |
| 1 | Resistencia de 10 kΩ | *Pull-down* externo del pulsador |
| 1 | LED verde | Espacio libre |
| 1 | LED rojo | Espacio ocupado / umbral superado |
| 2 | Resistencias de 220 Ω | Limitación de corriente de los LED |
| 1 | Zumbador piezoeléctrico | Alerta acústica |
| 1 | Protoboard y cables | Conexiones |
| 1 | Maqueta a escala 1:10 | Soporte e integración del sistema |

## Diagrama de conexiones

![Esquemático](docs/esquematico.png)

| Pin | Componente | Tipo |
|:--:|---|---|
| D2 | Sensor PING))) (pin SIG) | Entrada / salida (disparo y eco) |
| D3 | Pulsador con *pull-down* de 10 kΩ | Entrada digital |
| D4 | LED verde (con 220 Ω) | Salida digital |
| D5 | LED rojo (con 220 Ω) | Salida digital |
| D6 | Zumbador piezoeléctrico | Salida (`tone()`) |
| A0 | Cursor del potenciómetro | Entrada analógica (ADC de 10 bits) |
| 5 V / GND | Alimentación del sensor y del potenciómetro; tierra común | — |

El pulsador conecta la entrada D3 a 5 V al presionarse; la resistencia de 10 kΩ a tierra mantiene la entrada en nivel bajo en reposo. No se usa `INPUT_PULLUP`.

## Instrucciones de uso

### Cargar el código

1. Instala el [Arduino IDE](https://www.arduino.cc/en/software).
2. Abre `monitoreo_proximidad/monitoreo_proximidad.ino`.
3. Selecciona la placa **Arduino Uno** y el puerto correspondiente, y pulsa **Subir**.

### Probar el sistema

1. Abre el **Monitor Serie** a **9600 baudios**. Verás líneas como:

   ```
   Distance: 31 cm | Threshold: 40 cm | Buzzer: ON -> INSIDE (RED)
   ```

2. Gira el **potenciómetro** para ajustar el umbral (`Threshold`).
3. Acerca un objeto al sensor: dentro del umbral se enciende el LED rojo y suena una vez el zumbador; al alejarlo vuelve el verde y suena de nuevo.
4. Presiona el **pulsador** para silenciar el zumbador (`MUTED`) y de nuevo para reactivarlo (`ON`).
5. Si el sensor no recibe eco, `Distance` vale `999` y el LED queda en verde.

### Probar en simulación

El circuito se puede simular en Tinkercad Circuits con los mismos componentes y el mismo código.

<!-- Agregar aquí el enlace al proyecto de Tinkercad -->

## Estructura del repositorio

```
.
├── README.md
├── monitoreo_proximidad/
│   └── monitoreo_proximidad.ino   # Código fuente comentado
└── docs/
    ├── esquematico.png            # Diagrama de conexiones
    └── simulacion_tinkercad.png   # Captura de la simulación
```

## Limitaciones y mejoras

- El lazo usa `delay()` (50 ms por ciclo y 300 ms tras cada alerta), por lo que durante ese tiempo no se lee el sensor. Se propone reemplazarlos por control temporal con `millis()`.
- Superficies que absorben el sonido o con ángulos de incidencia grandes dispersan el eco.
- La conversión a centímetros supone una velocidad del sonido constante (29 µs/cm).
- Mejoras propuestas: compensación por temperatura, arreglo de sensores, pantalla OLED y envío de datos a la nube (por ejemplo con ESP32).

## Autores

- Juan Sebastian Rios Socha
- Gustavo Pacheco
- Ivan Romero
- Santiago Cabrera

Universidad de San Buenaventura, sede Bogotá. Facultad de Ingeniería.
