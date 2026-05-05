#include "menu.h"
#include "configuracion.h"
//#include "DFRobotDFPlayerMini.h"

extern Configuracion config;
//extern DFRobotDFPlayerMini myDFPlayer;

// Constructor
Menu::Menu(Adafruit_ST7735 &pantalla) : tft(pantalla), iconoActivo(0)
{
}

// Inicialización
void Menu::begin()
{
    tft.setRotation(1);
    tft.fillScreen(COLOR_FONDO);
    dibujarBarraEstado("MENU PRINCIPAL");
    dibujarIconos();
    dibujarSeleccion(iconoActivo);
}

// dibujar barra de estado superior
void Menu::dibujarBarraEstado(const char *texto)
{
    tft.fillRect(0, 0, ANCHO_PANTALLA, ALTO_BARRA, COLOR_BARRA);
    tft.setTextColor(COLOR_TEXTO_BARRA, COLOR_BARRA);
    tft.setTextSize(1);
    tft.setCursor(5, 4);
    tft.print(texto);
}

// obtener coordenadas X según columna
int Menu::getXPorColumna(int columna)
{
    switch (columna)
    {
    case 0:
        return POS_X_COL0;
    case 1:
        return POS_X_COL1;
    case 2:
        return POS_X_COL2;
    default:
        return 0;
    }
}

// obtener coordenadas Y según fila
int Menu::getYPorFila(int fila)
{
    switch (fila)
    {
    case 0:
        return POS_Y_FILA0;
    case 1:
        return POS_Y_FILA1;
    default:
        return 0;
    }
}

// obtener coordenadas de un icono específico
void Menu::obtenerCoordenadasIcono(int index, int &x, int &y)
{
    int fila = index / 3;    // 0 o 1
    int columna = index % 3; // 0, 1 o 2

    x = getXPorColumna(columna);
    y = getYPorFila(fila);
}

// dibujar rectángulo de selección
void Menu::dibujarRectanguloSeleccion(int x, int y, bool dibujar)
{
    uint16_t color = dibujar ? COLOR_SELECCION : COLOR_FONDO;
    tft.drawRect(x, y, ANCHO_SELECCION, ALTO_SELECCION, color);
}

// dibujar un icono individual
void Menu::dibujarIcono(int index, bool seleccionado)
{
    int x, y;
    obtenerCoordenadasIcono(index, x, y);

    // color del icono según si está seleccionado
    uint16_t colorIcono = seleccionado ? COLOR_ICONO_SELEC : COLOR_ICONO_NORMAL;
    uint16_t colorFondo = seleccionado ? COLOR_SELECCION : COLOR_FONDO;

    // Área del icono (limpia el área primero)
    tft.fillRect(x, y, ANCHO_ICONO - MARGEN_SELECCION * 2,
                 ALTO_ICONO - MARGEN_SELECCION * 2, colorFondo);

    // dibujar icono
    tft.setTextColor(colorIcono, colorFondo);
    tft.setTextSize(2);

    switch (index)
    {
    case 0:
        tft.setCursor(x + 15, y + 20);
        tft.print("VOL");
        break;
    case 1:
        tft.setCursor(x + 15, y + 20);
        tft.print("EQ");
        break;
    case 2:
        tft.setCursor(x + 15, y + 20);
        tft.print("LED");
        break;
    case 3:
        tft.setCursor(x + 10, y + 20);
        tft.print("MODO");
        break;
    case 4:
        tft.setCursor(x + 10, y + 20);
        tft.print("INFO");
        break;
    case 5:
        tft.setCursor(x + 15, y + 20);
        tft.print("SET");
        break;
    }
}

// dibujar todos los iconos
void Menu::dibujarIconos()
{
    for (int i = 0; i < 6; i++)
    {
        dibujarIcono(i, false);
    }
}

// dibujar selección en un icono
void Menu::dibujarSeleccion(int iconoIndex)
{
    int x, y;
    obtenerCoordenadasIcono(iconoIndex, x, y);

    // primero redibujar el icono como seleccionado
    dibujarIcono(iconoIndex, true);

    // dibujar el rectángulo alrededor
    dibujarRectanguloSeleccion(x, y, true);
}

// borrar selección de un icono
void Menu::borrarSeleccion(int iconoIndex)
{
    int x, y;
    obtenerCoordenadasIcono(iconoIndex, x, y);

    // Borrar el rectángulo
    dibujarRectanguloSeleccion(x, y, false);

    // Redibujar el icono como no seleccionado
    dibujarIcono(iconoIndex, false);
}

// mover selección: 0=arriba, 1=abajo, 2=izquierda, 3=derecha
void Menu::moverSeleccion(int direccion)
{
    int nuevoIcono = iconoActivo;

    switch (direccion)
    {
    case 0: // Arriba
        if (nuevoIcono >= 3)
            nuevoIcono -= 3;
        break;
    case 1: // Abajo
        if (nuevoIcono < 3)
            nuevoIcono += 3;
        break;
    case 2: // Izquierda
        if (nuevoIcono % 3 != 0)
            nuevoIcono--;
        break;
    case 3: // Derecha
        if (nuevoIcono % 3 != 2)
            nuevoIcono++;
        break;
    }

    if (nuevoIcono != iconoActivo)
    {
        borrarSeleccion(iconoActivo);
        iconoActivo = nuevoIcono;
        dibujarSeleccion(iconoActivo);
    }
}

// obtener icono activo
int Menu::getIconoActivo()
{
    return iconoActivo;
}

// ejecutar acción según icono seleccionado
void Menu::ejecutarAccion()
{
    switch (iconoActivo)
    {
    case 0:
    // barraVolumen.dibujar(config.get().volumen);
    dibujarBarraEstado("Volumen");
                break;
    case 1:
    // acción ecualizador
    dibujarBarraEstado("Ecualizador");
        break;
    case 2:
    // acción LEDs WS2812B
    dibujarBarraEstado("Brillo RGB");
        break;
    case 3:
    // cambiar modo (USB/SD/AUX/BT)
    dibujarBarraEstado("Modo reproduccion");
        break;
    case 4:
    // mostrar info del sistema
    dibujarBarraEstado("Informacion");
        break;
    case 5:
    // menú de configuración
    dibujarBarraEstado("Configuracion");
        break;
    }
    delay(1000);
    dibujarBarraEstado("MENU PRINCIPAL");
}