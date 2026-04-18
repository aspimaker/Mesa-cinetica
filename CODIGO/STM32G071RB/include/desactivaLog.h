#pragma once

// desactivaLog.h — Elimina t

#ifdef RELEASE

// Silencia todas las llamadas a Serial.*
// do { } while(0) es el idioma estándar para macros multi-statement
// seguras dentro de if/else sin llaves.

#define Serial_begin(baud)      do { } while(0)

// Reemplaza el objeto Serial completo con un struct vacío inline.
// Al ser 'static const', cada .cpp obtiene su propia copia pero
// el compilador la elimina completamente (zero overhead).
namespace _log_sink {
    struct Sink {
        void begin(unsigned long)           const {}
        template<typename T>
        void print(const T &)               const {}
        template<typename T>
        void println(const T &)             const {}
        void println()                      const {}
        void printf(const char *, ...)      const {}
        void write(const char *)            const {}
        void write(int)                     const {}
        void flush()                        const {}
        int  available()                    const { return 0; }
        int  read()                         const { return -1; }
        explicit operator bool()            const { return false; }
    };
    static const Sink sink;
}

// Redirige Serial al sink vacío sin tocar el objeto real
#define Serial (_log_sink::sink)

#endif // RELEASE