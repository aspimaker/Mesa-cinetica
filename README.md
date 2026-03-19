# MESA CINÉTICA 🪄✨

![Estado del proyecto](https://img.shields.io/badge/estado-en%20desarrollo-yellow)  
![Licencia](https://img.shields.io/badge/licencia-CC%20BY--NC--SA%204.0-blue)

**MESA CINÉTICA** es un proyecto de mobiliario interactivo que integra el arte y la tecnología combinando electrónica, programación para microcontroladores, diseño e impresión 3D y desarrollo de una app Android.

Se trata de una mesa que incorpora un mecanismo de movimiento polar. Mientras el suave desplazamiento de la bola de acero sobre la arena traza patrones hipnóticos, la iluminación LED y la música acompañan cada movimiento transformando la mesa en un lienzo vivo donde arte, luz y sonido se funden.

Empecé a programar hace poco más de 30 años con Visual Basic 3.0 y ya vamos por la 2026... pero este proyecto será un gran reto porque la aplicación de Android se va a desarrollar en C# con MAUI ([.NET MAUI](https://dotnet.microsoft.com/es-es/apps/maui))

Quiero agradecer a [@LaboratorioGluon](https://github.com/LaboratorioGluon) y [@STMicroelectronics ](https://github.com/STMicroelectronics)su ayuda por proporcionar la placa de desarrollo ST NUCLEO-G071RB.

## 📁 Estructura del proyecto

```txt
MESA CINETICA/
├── ANDROID/ # Aplicación para Android desarrollada en C# con MAUI (Visual Studio 2026)
├── CODIGO/ # Firmware desarrollado en C++ con PlatformIO en VS Code
├── ELECTRONICA/ # Esquemas y diseño de PCB con KiCad 10
├── HARDWARE/ # Modelos 3D para impresión (STL/STEP) con onshape
└── README.md # !! Este fichero !!
```

---

## 🧠 Descripción general

Este proyecto nace con la idea de transformar una mesa convencional en una superficie viva, donde el movimiento generado por algoritmos crea experiencias visuales únicas.

### Objetivos:

- Desarrollar un sistema de control de motores paso a paso preciso y silencioso
- Diseñar una PCB compacta que integre microcontrolador y drivers
- Modelar piezas 3D funcionales y estéticas
- Crear patrones de movimiento modulables por software

---

## ⚙️ Componentes principales

| Carpeta        | Tecnología       | Descripción                                                                              |
| -------------- | ---------------- | ---------------------------------------------------------------------------------------- |
| `ANDROID/`     | C# MAUI          | App para gestionar:- el movimiento de la mesa- leds RGB- enviar diseños- reproductor MP3 |
| `CODIGO/`      | C++ / PlatformIO | Lógica de movimiento, control de motores y comunicación con la aplicación de Android     |
| `ELECTRONICA/` | KiCad 10         | Esquemático, diseño de PCB, gerbers, etc...                                              |
| `HARDWARE/`    | onshape          | Piezas estructurales, soportes, etc...                                                   |

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

## 📄 Licencia

Este proyecto está bajo licencia **Creative Commons BY-NC-SA 4.0**

---

## 👤 Autor

aspimaker - [@aspimaker](https://github.com/aspimaker)
