# Sistema de Monitoreo de Proximidad Embebido y Alerta Multimodal

Un prototipo basado en **Arduino Uno** que detecta si un espacio está ocupado mediante la medición de distancia con un sensor ultrasónico, señalando el resultado con luces y sonido. Fue diseñado, simulado en Tinkercad y montado en un **modelo a escala 1:10** de un cubículo de estudio en la Facultad de Ingeniería de la Universidad de San Buenaventura, sede Bogotá.

Proyecto para el curso de Circuitos Digitales.

![Simulación en Tinkercad](docs/simulacion_tinkercad.png)

## Planteamiento del Problema

En los cubículos de estudio y las áreas de laboratorio de la Facultad de Ingeniería, el espacio está sobresaturado y mal gestionado durante las horas pico: los usuarios no cuentan con señalización que les indique en tiempo real si un lugar está disponible u ocupado. Los carteles estáticos y las reservas en papel no se adaptan a la dinámica cambiante del campus.

Este sistema mide la ocupación de forma local y continua, y la comunica inmediatamente con indicadores visuales y acústicos, a bajo costo y sin depender de una infraestructura de red.

## Cómo Funciona

1. El sensor **HC-SR04** mide la distancia al objeto o persona más cercana.
2. Si la distancia es menor o igual al umbral (**5 cm** en el modelo a escala), se enciende el **LED rojo** (ocupado); de lo contrario, se enciende el **LED verde** (libre).
3. Cada vez que el estado **cambia** (entra o sale del umbral), el **zumbador (buzzer)** suena a 1 kHz. Un **potenciómetro** establece cuánto tiempo suena, de 0.3 a 3 s, y el zumbador se apaga sin bloquear el bucle principal (`millis()`).
4. Un **pulsador** silencia o vuelve a habilitar el zumbador; los LEDs siguen funcionando.
5. En cada ciclo, se envía una línea de telemetría a través de **UART** (9600 bps) al monitor serie.

## Lista de Componentes

| Cant. | Componente | Función |
|:--:|---|---|
| 1 | Arduino Uno (ATmega328P) y cable USB | Microcontrolador y fuente de alimentación |
| 1 | Sensor ultrasónico HC-SR04 | Medición de distancia (tiempo de vuelo) |
| 1 | Potenciómetro | Ajuste de la duración de la alerta (entrada analógica) |
| 1 | Pulsador | Silenciar / rehabilitar el zumbador |
| 1 | Resistencia de 10 kΩ | *Pull-down* externo para el pulsador |
| 1 | LED verde | Espacio libre |
| 1 | LED rojo | Espacio ocupado |
| 2 | Resistencias de 220 Ω | Limitación de corriente para los LEDs |
| 1 | Zumbador piezoeléctrico | Alerta acústica |
| 1 | Protoboard y cables | Conexiones |
| 1 | Modelo a escala 1:10 | Soporte para el sensor y los LEDs |

## Diagrama de Cableado

![Esquemático](docs/esquematico.png)

| Pin | Componente | Tipo |
|:--:|---|---|
| A1 | Sensor HC-SR04 (Pin Trig) | Salida digital (disparo) |
| A2 | Sensor HC-SR04 (Pin Echo) | Entrada digital (eco) |
| D2 | LED verde (con 220 Ω) | Salida digital |
| D3 | LED rojo (con 220 Ω) | Salida digital |
| D4 | Zumbador piezoeléctrico | Salida (`tone()`) |
| D5 | Pulsador con *pull-down* de 10 kΩ | Entrada digital |
| A0 | Cursor del potenciómetro | Entrada analógica (ADC de 10 bits) |
| 5 V / GND | Alimentación para el sensor y potenciómetro; tierra común | — |

El pulsador conecta la entrada D5 a 5 V cuando se presiona; la resistencia de 10 kΩ a tierra mantiene la entrada en un nivel bajo en reposo. No se utiliza `INPUT_PULLUP`.

## Instrucciones de Uso

### Subir el Código

1. Instala el [Arduino IDE](https://www.arduino.cc/en/software).
2. Abre `monitoreo_proximidad/monitoreo_proximidad.ino`.
3. Selecciona la placa **Arduino Uno** y el puerto correspondiente, luego haz clic en **Subir**.

### Probar el Sistema

1. Abre el **Monitor Serie** a **9600 baudios**. Verás líneas como:

   ```text
   Distance: 4 cm | Set Duration: 1.7 s | Buzzer: ENABLED -> RED LED
   ```

2. Gira el **potenciómetro** para ajustar la duración de la alerta (`Set Duration`).
3. Coloca un objeto a menos de 5 cm del sensor: el LED rojo se enciende y el zumbador suena una vez; al alejarlo, vuelve el LED verde y el zumbador vuelve a sonar.
4. Presiona el **pulsador** para silenciar el zumbador (`DISABLED`) y presiónalo de nuevo para rehabilitarlo (`ENABLED`).
5. Si el sensor no recibe eco, `Distance` será `999` y el LED permanecerá verde.

### Probar en Simulación

El circuito se puede simular en Tinkercad Circuits. La simulación se realizó con una versión anterior del firmware: un sensor PING))) en D2, un umbral ajustable de 0 a 100 cm con el potenciómetro y una asignación de pines diferente a la versión final construida.

<!-- Agrega el enlace del proyecto de Tinkercad aquí -->

## Estructura del Repositorio

```text
.
├── README.md
├── monitoreo_proximidad/
│   └── monitoreo_proximidad.ino   # Código fuente comentado
└── docs/
    ├── esquematico.png            # Diagrama de cableado
    └── simulacion_tinkercad.png   # Captura de pantalla de la simulación
```

## Limitaciones y Mejoras

- El umbral está fijo en 5 cm en el firmware, y el bucle mantiene un `delay(50)` por ciclo. Se propone hacer que el umbral sea ajustable y reemplazar el `delay()` restante por `millis()`.
- Las superficies que absorben el sonido o aquellas con grandes ángulos de incidencia dispersan el eco.
- La conversión a centímetros asume una velocidad del sonido constante (≈29 µs/cm por dirección).
- El Arduino y la protoboard se encuentran fuera de la caja del modelo a escala; se propone alojarlos en su interior.
- Mejoras propuestas: compensación por temperatura, matriz de sensores, pantalla OLED y envío de datos a la nube (por ejemplo, con un ESP32).

## Autores

- Juan Sebastian Rios Socha
- Gustavo Pacheco
- Ivan Romero
- Santiago Cabrera

Universidad de San Buenaventura, sede Bogotá. Facultad de Ingeniería.
