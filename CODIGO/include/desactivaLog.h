#pragma once

#ifdef RELEASE
#undef Serial
    class DummySerial
{
public:
    void begin(unsigned long) {}
    template <typename T>
    void print(const T &) {}
    template <typename T>
    void println(const T &) {}
    void println() {}
    void printf(const char *, ...) {}
    void write(const char *) {}
    void write(int) {}
    void flush() {}
    int available() { return 0; }
    int read() { return -1; }
    operator bool() { return false; }
};
DummySerial Serial;
#endif