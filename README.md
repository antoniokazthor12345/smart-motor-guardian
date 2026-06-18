# 🛡️ Smart Motor Guardian

Sistema IoT para monitoreo industrial de motor/máquina en tiempo real usando **ESP32**, **MQTT Cloud**, **OLED** y un **dashboard web**.

## 🚀 Descripción

**Smart Motor Guardian** recibe datos por UART desde un módulo/sistema sensor y los publica en la nube mediante MQTT.
El ESP32 muestra la información en una pantalla OLED y también envía los datos a un dashboard web en tiempo real.

El sistema monitorea:

* 🌡️ Temperatura
* 📳 Vibración
* ⚙️ RPM
* ⚡ Corriente
* 🚨 Estado de la máquina

## 🧠 Arquitectura

```txt
Sensores / Módulo principal
        │
        │ UART
        ▼
      ESP32
 ┌───────────────┐
 │ OLED Display  │
 │ WiFi          │
 │ MQTT TLS      │
 └───────┬───────┘
         │
         ▼
   HiveMQ Cloud
         │
         ▼
 Dashboard Web
```

## 🧰 Tecnologías usadas

* ESP32
* Arduino IDE
* WiFi
* WiFiClientSecure
* PubSubClient
* OLED SSD1306
* MQTT
* HiveMQ Cloud
* HTML / CSS / JavaScript
* Chart.js
* MQTT.js

## 🔌 Conexiones ESP32

| Módulo         | Pin ESP32 |
| -------------- | --------- |
| OLED SDA       | GPIO 21   |
| OLED SCL       | GPIO 22   |
| UART RX        | GPIO 16   |
| UART TX        | GPIO 17   |
| Serial Monitor | 115200    |
| Serial2 UART   | 115200    |

## 📡 Topics MQTT

```txt
tony/factory/temp
tony/factory/vib
tony/factory/rpm
tony/factory/amp
tony/factory/status
```

## 📦 Formato de datos UART

El ESP32 espera recibir frames con este formato:

```txt
<temp,vib,rpm,amp,status>
```

Ejemplo:

```txt
<28.50,0,1450,1.25,0>
```

Significado:

| Dato   | Descripción                           |
| ------ | ------------------------------------- |
| temp   | Temperatura en °C                     |
| vib    | Vibración: 0 = OK, 1 = alerta         |
| rpm    | Revoluciones por minuto               |
| amp    | Corriente en amperes                  |
| status | 0 = Normal, 1 = Warning, 2 = Critical |

## 🖥️ Dashboard Web

El dashboard muestra los datos en tiempo real desde HiveMQ Cloud usando WebSockets MQTT.

Incluye:

* Tarjetas de temperatura, corriente, RPM y vibración
* Estado de la máquina
* Última actualización
* Gráfica en vivo con Chart.js

## 🚨 Estados de la máquina

| Código | Estado   |
| ------ | -------- |
| 0      | NORMAL   |
| 1      | WARNING  |
| 2      | CRITICAL |

## 🔐 Seguridad

Las credenciales WiFi y MQTT deben mantenerse privadas.

No subir al repositorio datos reales como:

```cpp
const char* ssid = "TU_WIFI";
const char* password = "TU_PASSWORD";
const char* mqtt_user = "TU_USER";
const char* mqtt_pass = "TU_PASS";
```

Usar valores ocultos o un archivo separado de configuración.

## 📁 Estructura recomendada

```txt
Smart-Motor-Guardian/
│
├── esp32/
│   └── smart_motor_guardian.ino
│
├── dashboard/
│   └── index.html
│
└── README.md
```

## ✅ Estado del proyecto

Proyecto funcional en versión inicial.

* ✅ ESP32 conectado a WiFi
* ✅ MQTT Cloud funcionando
* ✅ OLED mostrando datos
* ✅ Dashboard web conectado
* ✅ Publicación de temperatura, vibración, RPM, corriente y estado
* ✅ Gráfica en tiempo real

## 🔮 Futuras mejoras

* Agregar alerta sonora con buzzer
* Agregar LEDs físicos de estado
* Guardar historial de datos
* Crear carcasa del dispositivo
* Mejorar seguridad TLS con certificado real
* Agregar botón de reset o calibración
* Integrar sensores reales directamente al ESP32

## 👨‍💻 Autor

Proyecto desarrollado por **KAZTHOR LABS**
Creado como parte del aprendizaje en IoT, electrónica, monitoreo industrial y sistemas embebidos.

---
