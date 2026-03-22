# MESA CINÉTICA 🪄✨

Estado y licencia  
![version](https://img.shields.io/badge/version-1.0.0-blue?style=for-the-badge) ![estado](https://img.shields.io/badge/estado-activo-brightgreen?style=for-the-badge) ![Estado](https://img.shields.io/badge/estado-en%20desarrollo-yellow?style=for-the-badge) ![Licencia](https://img.shields.io/badge/Creative%20Commons-CC%20BY--NC--SA%204.0-blue?style=for-the-badge)

Microcontrolador  
![STM32](https://img.shields.io/badge/STM32-G071RB-blue?style=for-the-badge&logo=stmicroelectronics&logoColor=white)

Firmware  
![VS Code](https://img.shields.io/badge/Editor-VS%20Code-007ACC?style=for-the-badge&logo=visualstudiocode&logoColor=white) ![C++](https://img.shields.io/badge/C++-17-blue?style=for-the-badge&logo=cplusplus&logoColor=white) ![PlatformIO](https://img.shields.io/badge/PlatformIO-IDE-orange?style=for-the-badge&logo=platformio&logoColor=white) ![Arduino](https://img.shields.io/badge/Framework-Arduino-00878F?style=for-the-badge&logo=arduino&logoColor=white)

Electrónica  
![KiCad](https://img.shields.io/badge/KiCad_10-PCB-blue?style=for-the-badge&logo=kicad&logoColor=white)

App Android  
![.NET C#](https://img.shields.io/badge/.NET%20C%23-512BD4?style=for-the-badge&logo=dotnet&logoColor=white) ![MAUI](https://img.shields.io/badge/MAUI-Framework-512BD4?style=for-the-badge&logo=dotnet&logoColor=white) ![Android](https://img.shields.io/badge/Android-3DDC84?style=for-the-badge&logo=android&logoColor=white)

Diseño 3D  
![Onshape](https://img.shields.io/badge/Onshape-Diseño%203D-0066CC?style=for-the-badge) ![3D Printing](https://img.shields.io/badge/3D%20Printing-STL-orange?style=for-the-badge)

Comunicaciones  
![Bluetooth](https://img.shields.io/badge/Bluetooth_classic-JDY--31-blue?style=for-the-badge)

Componentes  
![TMC2209](https://img.shields.io/badge/Driver%20motor-TMC2209-brightgreen?style=for-the-badge) ![DFPlayer](https://img.shields.io/badge/Audio%20MP3-DFPlayer%20Mini-red?style=for-the-badge) ![WS2812B](https://img.shields.io/badge/LEDs_RGB-WS2812B-brightgreen?style=for-the-badge)


**MESA CINÉTICA** es un proyecto de mobiliario interactivo que integra el arte y la tecnología combinando electrónica, programación para microcontroladores, diseño e impresión 3D y desarrollo de una app Android.

Se trata de una mesa que incorpora un mecanismo de movimiento polar. Mientras el suave desplazamiento de la bola de acero sobre la arena traza patrones hipnóticos, la iluminación LED y la música acompañan cada movimiento transformando la mesa en un lienzo vivo donde arte, luz y sonido se funden.

Empecé a programar hace poco más de 30 años con Visual Basic 3.0 y ya vamos por la 2026... pero este proyecto será un gran reto porque la aplicación de Android se va a desarrollar en C# con MAUI ([.NET MAUI](https://dotnet.microsoft.com/es-es/apps/maui))

Quiero agradecer a [@LaboratorioGluon](https://github.com/LaboratorioGluon) y [@STMicroelectronics ](https://github.com/STMicroelectronics)su ayuda por proporcionar la placa de desarrollo ST NUCLEO-G071RB.

## 📁 Estructura del proyecto

```txt
MESA CINETICA
├── ANDROID # Aplicación para Android desarrollada en C# con MAUI (Visual Studio 2026)
├── CODIGO # Firmware desarrollado en C++ con PlatformIO en VS Code
├── ELECTRONICA # Esquemas y diseño de PCB con KiCad 10
├── PIEZAS # Modelos 3D para impresión (STL/STEP) con onshape
└── README.md # !! Este fichero !!
```

---

## 🧠 Descripción general

Este proyecto nace con la idea de transformar una mesa convencional en una superficie viva, donde el movimiento generado por algoritmos crea experiencias visuales únicas.

### Objetivos:

- desarrollar un sistema de control de motores paso a paso preciso y silencioso
- diseñar una PCB que integre la placa de desarrollo ST NUCLEO G071RB junto con el resto de componentes (drivers de motores, control de leds, reproductor MP3 (), amplificador, fuente alimentación...)
- modelar piezas 3D funcionales y estéticas
- crear patrones de movimiento modulables por software

() Con la finalidad hacer accesible el proyecto a todos los makers, el reproductor MP3 será un módulo independiente que tiene su propia tarjeta SD. En futuras versiones el decodificador será el VS1053 y todo lo gestionará el microcontrolador.

---

## ⚙️ Componentes principales


| Carpeta       | Tecnología       | Descripción                                                                             |
| ------------- | ---------------- | --------------------------------------------------------------------------------------- |
| `ANDROID`     | C# MAUI          | App para gestionar el movimiento de la mesa, leds RGB, enviar diseños, reproduir MP3... |
| `CODIGO`      | C++ / PlatformIO | Lógica de movimiento, control de motores y comunicación con la aplicación de Android    |
| `ELECTRONICA` | KiCad 10         | Esquemático, diseño de PCB, gerbers...                                                  |
| `HARDWARE`    | onshape          | Piezas estructurales, soportes...                                                       |


---

## 🚀 Cómo empezar

### Prerrequisitos

- [PlatformIO](https://platformio.org/) (extensión de VS Code para compilar el código)
- [KiCad 10](https://www.kicad.org/) (para editar la pcb) o visor ([https://kicanvas.org/?repo=https://github.com/aspimaker/Mesa-cinetica/tree/main/ELECTRONICA/PCB](https://kicanvas.org/?repo=https://github.com/aspimaker/Mesa-cinetica/tree/main/ELECTRONICA/PCB))
- [onshape](https://www.freecad.org/) (para modificar las piezas impresas en 3d)
- [Visual Studio 2026 - community](https://www.freecad.org/) (para modificar la aplicación de Android)

### Clonar el repositorio

```bash
git clone https://github.com/aspimaker/mesa-cinetica.git
cd mesa-cinetica
```

---

## ⚡Electrónica


| Componente                 | Modelo / Especificación | Cantidad | Función                                               |
| -------------------------- | ----------------------- | -------- | ----------------------------------------------------- |
| **Microcontrolador**       | STM32 Nucleo-G071RB     | 1        | Gestión del sistema: control de motores, LEDs y audio |
| **Driver de motor**        | TMC2209                 | 2        | Movimiento silencioso y suave de los ejes R/θ         |
| **Motores paso a paso**    | NEMA17                  | 2        | Movimiento polar de la bola de acero                  |
| **Módulo bluetooth 3.0**   | JDY-31                  | 1        | Módulo bluetooth 3.0 SPP                              |
| **Reproductor de audio**   | DFPlayer Mini MP3 V3.0  | 1        | Reproducción de música y efectos de sonido            |
| **Amplificador de audio**  | TPA3110 XH-A232         | 1        | Amplificación estéreo para altavoces                  |
| **Altavoces**              | 4Ω - 8Ω, 10W-15W        | 2        | Salida de sonido estéreo                              |
| **Fuente de alimentación** | 24V / 5A                | 1        | Alimentación para motores y amplificador              |
| **Conversor DC-DC**        | LM2596                  | 1        | Conversión 24V → 5V para DFPlayer, leds y otros       |
| **Iluminación LED**        | Tira LED RGB (WS2812B)  | 1        | Efectos de luz sincronizados con movimiento y música  |


---

### 🧩 Conexiones principales

- **Motores**: STM32 → TMC2209 (modo standalone con StealthChop) → NEMA17 (alimentación a 24V)
- **Audio**: STM32 → DFPlayer Mini → TPA3110 → Altavoces
- **LEDs**: STM32 → Tira WS2812B
- **Alimentación**: Fuente 24V → TMC2209, TPA3110, y LM2596 → 5V → DFPlayer, STM32, leds

---

## 📄 Licencia

Este proyecto está bajo licencia **Creative Commons Atribución-NoComercial-CompartirIgual 4.0 Internacional (CC BY-NC-SA 4.0)**

Puedes:

- Compartir, copiar y redistribuir el material
- Adaptar, remezclar y transformar el material

siempre que:

- **Atribuyas** el crédito al autor original (@aspimaker / MESA CINÉTICA)
- **No uses** el material con fines comerciales
- **Compartas** tus modificaciones bajo la misma licencia

[Ver texto completo de la licencia](https://creativecommons.org/licenses/by-nc-sa/4.0/deed.es)

---

## 👤 Autor

aspimaker - [@aspimaker](https://github.com/aspimaker)