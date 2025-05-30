// estado.h
#ifndef ESTADO_H
#define ESTADO_H

typedef struct EstadoPantalla {
    void (*Update)(void);
    void (*Draw)(void);
} EstadoPantalla;

#endif
