#include "BarraProgreso.h"

BarraProgreso::BarraProgreso(Adafruit_ST7735 &pantalla,
                             int x, int y, int ancho, int alto,
                             uint16_t colorRelleno, uint16_t colorFondo,
                             uint16_t colorBorde, String texto,
                             bool mostrarPorcentaje)
{
    _pantalla = &pantalla;
    _x = x;
    _y = y;
    _ancho = ancho;
    _alto = alto;
    _colorRelleno = colorRelleno;
    _colorFondo = colorFondo;
    _colorBorde = colorBorde;
    _colorTexto = ST7735_WHITE;
    _texto = texto;
    _mostrarPorcentaje = mostrarPorcentaje;
    _progresoActual = -1;
    _progresoObjetivo = 0;
    _visible = false;
    _inicializada = false;
    _ultimoPorcentaje = -1;
    _ultimoTextX = 0;
    _ultimoTextY = 0;

    for (int i = 0; i < 4; i++) {
        _ultimosDigitos[i] = -1;
        _ultimasPosX[i] = 0;
    }
}

// Método begin
void BarraProgreso::begin()
{
    if (!_pantalla)
    {
        Serial.println("BarraProgreso::begin() - ERROR: _pantalla es NULL");
        return;
    }

    _inicializada = true;
    _progresoActual = -1;
    _visible = false;

    // Configurar tamaño de texto
    _pantalla->setTextSize(1);
    _pantalla->setTextWrap(false);

    setProgresoYActualizar(0);
}

// Calcular ancho de la barra según porcentaje
int BarraProgreso::_calcularAnchoProgreso(int porcentaje)
{
    porcentaje = constrain(porcentaje, 0, 100);
    return (porcentaje * _ancho) / 100; // Más preciso que map()
}

// Dibujar el texto de porcentaje centrado
void BarraProgreso::_dibujarPorcentaje(int porcentaje)
{
    if (!_inicializada || !_mostrarPorcentaje || !_pantalla)
    {
        return;
    }

    static const int ANCHO_DIGITO = 6;
    static const int ANCHO_PORCENTAJE = 6;
    static const int ANCHO_TOTAL = 24;
    
    _pantalla->setTextSize(1);
    
    int baseX = _x + (_ancho / 2) - (ANCHO_TOTAL / 2);
    int textY = _y + (_alto / 2) - 3;
    
    int anchoProgreso = _calcularAnchoProgreso(porcentaje);
    
    // Caso especial: 100% -> 99%
    if (_ultimoPorcentaje == 100 && porcentaje == 99)
    {
        // Limpiar centena (posición del "1")
        int posCentena = baseX;
        int centroXCentena = posCentena + (ANCHO_DIGITO / 2);
        bool dentroBarraCentena = (centroXCentena < _x + anchoProgreso);
        _pantalla->fillRect(posCentena - 1, textY - 1, ANCHO_DIGITO + 2, 12, 
                           dentroBarraCentena ? _colorRelleno : _colorFondo);
        
        // Limpiar unidad (posición del segundo "0" que se convierte en "9")
        int posUnidad = baseX + ANCHO_DIGITO * 2;  // posición del tercer dígito (unidades)
        int centroXUnidad = posUnidad + (ANCHO_DIGITO / 2);
        bool dentroBarraUnidad = (centroXUnidad < _x + anchoProgreso);
        _pantalla->fillRect(posUnidad - 1, textY - 1, ANCHO_DIGITO + 1, 10, 
                           dentroBarraUnidad ? _colorRelleno : _colorFondo);
        
        // Forzar redibujo de todos los dígitos
        for (int i = 0; i < 4; i++) {
            _ultimosDigitos[i] = -1;
        }
    }
    
    // Separar dígitos
    int digitos[3];
    int numDigitos;
    
    if (porcentaje >= 100) {
        digitos[0] = 1;
        digitos[1] = (porcentaje / 10) % 10;
        digitos[2] = porcentaje % 10;
        numDigitos = 3;
    } else if (porcentaje >= 10) {
        digitos[0] = porcentaje / 10;
        digitos[1] = porcentaje % 10;
        numDigitos = 2;
    } else {
        digitos[0] = porcentaje;
        numDigitos = 1;
    }
    
    // Dibujar dígitos (alineados a la derecha)
    int startX = baseX + (3 - numDigitos) * ANCHO_DIGITO;
    
    for (int i = 0; i < numDigitos; i++)
    {
        int charX = startX + (i * ANCHO_DIGITO);
        int charCenterX = charX + (ANCHO_DIGITO / 2);
        
        bool dentroBarra = (charCenterX < _x + anchoProgreso);
        uint16_t colorTexto = dentroBarra ? ST7735_WHITE : ST7735_BLACK;
        uint16_t fondoTexto = dentroBarra ? _colorRelleno : _colorFondo;
        
        // Limpiar y dibujar dígito
        if (_ultimosDigitos[i] != digitos[i] || _ultimasPosX[i] != charX)
        {
            _pantalla->fillRect(charX - 1, textY - 1, ANCHO_DIGITO + 1, 10, fondoTexto);
        }
        
        char digitoStr[2] = {char('0' + digitos[i]), '\0'};
        _pantalla->setCursor(charX, textY);
        _pantalla->setTextColor(colorTexto);
        _pantalla->print(digitoStr);
        
        _ultimosDigitos[i] = digitos[i];
        _ultimasPosX[i] = charX;
    }
    
    // Dibujar símbolo %
    int percentX = baseX + 3 * ANCHO_DIGITO;
    int percentCenterX = percentX + (ANCHO_PORCENTAJE / 2);
    
    bool dentroBarra = (percentCenterX < _x + anchoProgreso);
    uint16_t colorTexto = dentroBarra ? ST7735_WHITE : ST7735_BLACK;
    uint16_t fondoTexto = dentroBarra ? _colorRelleno : _colorFondo;
    
    if (_ultimosDigitos[3] != '%' || _ultimasPosX[3] != percentX)
    {
        _pantalla->fillRect(percentX - 1, textY - 1, ANCHO_PORCENTAJE + 1, 10, fondoTexto);
    }
    
    _pantalla->setCursor(percentX, textY);
    _pantalla->setTextColor(colorTexto);
    _pantalla->print("%");
    
    _ultimosDigitos[3] = '%';
    _ultimasPosX[3] = percentX;
    
    // Limpiar dígitos sobrantes
    for (int i = numDigitos; i < 3; i++) {
        if (_ultimosDigitos[i] != -1) {
            int charX = baseX + i * ANCHO_DIGITO;
            _pantalla->fillRect(charX - 1, textY - 1, ANCHO_DIGITO + 1, 10, _colorFondo);
            _ultimosDigitos[i] = -1;
            _ultimasPosX[i] = 0;
        }
    }
    
    _ultimoPorcentaje = porcentaje;
}

// Dibujar el texto principal arriba de la barra
void BarraProgreso::_dibujarTextoPrincipal()
{
    if (!_inicializada || !_pantalla)
    {
        return;
    }

    if (_texto.length() > 0)
    {
        _pantalla->setTextSize(1);
        _pantalla->setCursor(_x, _y - 10);
        _pantalla->setTextColor(_colorTexto);
        _pantalla->print(_texto);
    }
}

// Establecer nuevo progreso (solo guarda el valor)
void BarraProgreso::setProgreso(int nuevoProgreso)
{
    _progresoObjetivo = constrain(nuevoProgreso, 0, 100);
}

// Obtener progreso actual
int BarraProgreso::getProgreso()
{
    return _progresoActual;
}

// Redibujar la barra completa
void BarraProgreso::redibujarCompleto()
{
    if (!_inicializada || !_pantalla)
    {
        return;
    }

    // 1. Dibujar fondo completo
    _pantalla->fillRect(_x, _y, _ancho, _alto, _colorFondo);

    // 2. Dibujar barra según progreso actual
    if (_progresoActual > 0)
    {
        int anchoProgreso = _calcularAnchoProgreso(_progresoActual);
        if (anchoProgreso > 0) {
            _pantalla->fillRect(_x, _y, anchoProgreso, _alto, _colorRelleno);
        }
    }

    // 3. Dibujar texto principal (arriba de la barra)
    _dibujarTextoPrincipal();
    
    // 4. Dibujar borde (encima de todo)
    _pantalla->drawRect(_x, _y, _ancho, _alto, _colorBorde);
    
    // 5. Dibujar porcentaje (encima del borde, con su propio fondo)
    if (_mostrarPorcentaje)
    {
        _dibujarPorcentaje(_progresoActual);
    }

    _visible = true;
}

// Actualizar solo la parte que cambió
// ------------------------------------------------------------
void BarraProgreso::actualizar()
{
    if (!_inicializada || !_pantalla)
    {
        return;
    }

    // Si es la primera vez o estaba oculta, redibujar completo
    if (_progresoActual == -1 || !_visible)
    {
        _progresoActual = _progresoObjetivo;
        redibujarCompleto();
        return;
    }

    // Si no hay cambio, no hacer nada
    if (_progresoActual == _progresoObjetivo)
        return;

    // Guardar valores anteriores
    int progresoAnterior = _progresoActual;
    
    // Calcular anchos
    int anchoAnterior = _calcularAnchoProgreso(progresoAnterior);
    int anchoNuevo = _calcularAnchoProgreso(_progresoObjetivo);

    // Determinar si aumentó o disminuyó
    if (anchoNuevo > anchoAnterior)
    {
        // Aumentó: dibujar solo la parte nueva
        _pantalla->fillRect(_x + anchoAnterior, _y,
                            anchoNuevo - anchoAnterior, _alto,
                            _colorRelleno);
    }
    else if (anchoNuevo < anchoAnterior)
    {
        // Disminuyó: restaurar la parte que sobra con el color de fondo
        _pantalla->fillRect(_x + anchoNuevo, _y,
                            anchoAnterior - anchoNuevo, _alto,
                            _colorFondo);
    }

    // Redibujar el borde
    _pantalla->drawRect(_x, _y, _ancho, _alto, _colorBorde);
    
    // Redibujar el porcentaje (limpia y dibuja de nuevo)
    if (_mostrarPorcentaje)
    {
        _dibujarPorcentaje(_progresoObjetivo);
    }

    // Actualizar el valor actual
    _progresoActual = _progresoObjetivo;
}

// Establecer nuevo progreso Y actualizar en un solo paso
void BarraProgreso::setProgresoYActualizar(int nuevoProgreso)
{
    setProgreso(nuevoProgreso);
    actualizar();
}

// Ocultar la barra (borrarla de la pantalla)
void BarraProgreso::ocultar()
{
    if (!_inicializada || !_pantalla || !_visible)
    {
        return;
    }

    // Borrar toda el área de la barra (incluyendo texto)
    _pantalla->fillRect(_x, _y - 12, _ancho, _alto + 18, ST7735_BLACK);
    _visible = false;
    _progresoActual = -1;
}

// Cambiar el color de relleno dinámicamente
void BarraProgreso::setColorRelleno(uint16_t nuevoColor)
{
    _colorRelleno = nuevoColor;
    if (_visible)
    {
        redibujarCompleto();
    }
}

// Cambiar el color de fondo dinámicamente
void BarraProgreso::setColorFondo(uint16_t nuevoColor)
{
    _colorFondo = nuevoColor;
    if (_visible)
    {
        redibujarCompleto();
    }
}

// Cambiar el color del borde dinámicamente
void BarraProgreso::setColorBorde(uint16_t nuevoColor)
{
    _colorBorde = nuevoColor;
    if (_visible)
    {
        redibujarCompleto();
    }
}

// Cambiar el texto principal
void BarraProgreso::setTexto(String nuevoTexto)
{
    if (!_inicializada)
    {
        return;
    }

    _texto = nuevoTexto;
    if (_visible)
    {
        // Borrar texto antiguo
        _pantalla->fillRect(_x, _y - 12, _ancho, 12, ST7735_BLACK);
        // Dibujar nuevo texto
        _dibujarTextoPrincipal();
        // Redibujar borde por si acaso
        _pantalla->drawRect(_x, _y, _ancho, _alto, _colorBorde);
    }
}

// Cambiar si mostrar o no el porcentaje
void BarraProgreso::setMostrarPorcentaje(bool mostrar)
{
    if (!_inicializada)
    {
        return;
    }

    _mostrarPorcentaje = mostrar;
    if (_visible)
    {
        if (mostrar)
        {
            _dibujarPorcentaje(_progresoActual);
        }
        else
        {
            // Borrar el porcentaje si estaba visible
            int anchoTexto = 30; // Aproximado para "100%"
            int altoTexto = 8;
            int textX = _x + (_ancho / 2) - (anchoTexto / 2);
            int textY = _y + (_alto / 2) + (altoTexto / 2) - 2;
            _pantalla->fillRect(textX - 1, textY - altoTexto + 1, anchoTexto + 2, altoTexto, _colorFondo);
        }
        // Redibujar borde
        _pantalla->drawRect(_x, _y, _ancho, _alto, _colorBorde);
    }
}

// Obtener si la barra está visible
bool BarraProgreso::isVisible()
{
    return _visible;
}

// Cambiar la posición de la barra
void BarraProgreso::setPosicion(int nuevaX, int nuevaY)
{
    if (_visible)
    {
        ocultar();
    }
    _x = nuevaX;
    _y = nuevaY;
    if (_visible)
    {
        redibujarCompleto();
    }
}

// Cambiar el tamaño de la barra
void BarraProgreso::setTamanio(int nuevoAncho, int nuevoAlto)
{
    if (_visible)
    {
        ocultar();
    }
    _ancho = nuevoAncho;
    _alto = nuevoAlto;
    if (_visible)
    {
        redibujarCompleto();
    }
}

// Método para animar suavemente el progreso
void BarraProgreso::animarHasta(int progresoFinal, int pasoDelayMs)
{
    int paso = (progresoFinal > _progresoActual) ? 1 : -1;

    for (int p = _progresoActual + paso;
         (paso > 0) ? p <= progresoFinal : p >= progresoFinal;
         p += paso)
    {
        setProgresoYActualizar(p);
        delay(pasoDelayMs);
    }
}