#include "menu.h"
#include "configuracion.h"       // Si necesitas acceso a configuración
#include "DFRobotDFPlayerMini.h" // Si necesitas el reproductor

// Referencias externas (si las necesitas en las acciones)
extern Configuracion config;
extern DFRobotDFPlayerMini myDFPlayer;

// Constructor
Menu::Menu(Adafruit_ST7735 &pantalla) : tft(pantalla), iconoActivo(0)
{
}

// Inicialización
void Menu::begin()
{
    // No llamamos a tft.init() porque ya se hace en iniciarPantalla()
    // Solo aseguramos la orientación correcta
    tft.setRotation(1);
    tft.fillScreen(COLOR_FONDO);
    dibujarBarraEstado("MENU PRINCIPAL");
    dibujarIconos();
    dibujarSeleccion(iconoActivo);
}

// Dibujar barra de estado superior
void Menu::dibujarBarraEstado(const char *texto)
{
    tft.fillRect(0, 0, ANCHO_PANTALLA, ALTO_BARRA, COLOR_BARRA);
    tft.setTextColor(COLOR_TEXTO_BARRA, COLOR_BARRA);
    tft.setTextSize(1);
    tft.setCursor(5, 4);
    tft.print(texto);
}

// Obtener coordenadas X según columna
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

// Obtener coordenadas Y según fila
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

// Obtener coordenadas de un icono específico
void Menu::obtenerCoordenadasIcono(int index, int &x, int &y)
{
    int fila = index / 3;    // 0 o 1
    int columna = index % 3; // 0, 1 o 2

    x = getXPorColumna(columna);
    y = getYPorFila(fila);
}

// Dibujar rectángulo de selección
void Menu::dibujarRectanguloSeleccion(int x, int y, bool dibujar)
{
    uint16_t color = dibujar ? COLOR_SELECCION : COLOR_FONDO;
    tft.drawRect(x, y, ANCHO_SELECCION, ALTO_SELECCION, color);
}

// Dibujar un icono individual
void Menu::dibujarIcono(int index, bool seleccionado)
{
    int x, y;
    obtenerCoordenadasIcono(index, x, y);

    // Color del icono según si está seleccionado
    uint16_t colorIcono = seleccionado ? COLOR_ICONO_SELEC : COLOR_ICONO_NORMAL;
    uint16_t colorFondo = seleccionado ? COLOR_SELECCION : COLOR_FONDO;

    // Área del icono (limpia el área primero)
    tft.fillRect(x, y, ANCHO_ICONO - MARGEN_SELECCION * 2,
                 ALTO_ICONO - MARGEN_SELECCION * 2, colorFondo);

    // Aquí dibujas tus iconos personalizados
    // EJEMPLO TEMPORAL con texto - REEMPLAZAR CON TUS ICONOS REALES
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

// Dibujar todos los iconos
void Menu::dibujarIconos()
{
    for (int i = 0; i < 6; i++)
    {
        dibujarIcono(i, false);
    }
}

// Dibujar selección en un icono
void Menu::dibujarSeleccion(int iconoIndex)
{
    int x, y;
    obtenerCoordenadasIcono(iconoIndex, x, y);

    // Primero redibujar el icono como seleccionado
    dibujarIcono(iconoIndex, true);

    // Dibujar el rectángulo alrededor
    dibujarRectanguloSeleccion(x, y, true);
}

// Borrar selección de un icono
void Menu::borrarSeleccion(int iconoIndex)
{
    int x, y;
    obtenerCoordenadasIcono(iconoIndex, x, y);

    // Borrar el rectángulo
    dibujarRectanguloSeleccion(x, y, false);

    // Redibujar el icono como no seleccionado
    dibujarIcono(iconoIndex, false);
}

// Mover selección: 0=arriba, 1=abajo, 2=izquierda, 3=derecha
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

// Obtener icono activo
int Menu::getIconoActivo()
{
    return iconoActivo;
}

// Ejecutar acción según icono seleccionado
void Menu::ejecutarAccion()
{
    switch (iconoActivo)
    {
    case 0:
        dibujarBarraEstado("Volumen");
        // Aquí mostrarías la barra de volumen
        // barraVolumen.dibujar(config.get().volumen);
        break;
    case 1:
        dibujarBarraEstado("Ecualizador");
        // Acción ecualizador
        break;
    case 2:
        dibujarBarraEstado("Brillo RGB");
        // Acción LEDs WS2812B
        break;
    case 3:
        dibujarBarraEstado("Modo reproduccion");
        // Cambiar modo (USB/SD/AUX/BT)
        break;
    case 4:
        dibujarBarraEstado("Informacion");
        // Mostrar info del sistema
        break;
    case 5:
        dibujarBarraEstado("Configuracion");
        // Menú de configuración
        break;
    }
    delay(1000);
    dibujarBarraEstado("MENU PRINCIPAL");
}