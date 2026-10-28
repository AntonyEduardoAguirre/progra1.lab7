#pragma once
#include <QList>
#include <QString>

struct Contacto {
    QString nombre;
    QString telefono;
    QString correo;
};


class ArchivoContactos
{
public:
    QString ruta() const;
    bool guardarTodos(const QList<Contacto>& lista) const;
    bool guardar(const QString& nombre,
        const QString& telefono,
        const QString& correo) const;
    QList<Contacto> leer() const;
};


