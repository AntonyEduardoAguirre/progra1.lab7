#include "ArchivoContactos.h"
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QTextStream>
#include <QSaveFile>


QString ArchivoContactos::ruta() const
{
    return QDir::homePath() + "/Lab7Datos/contactos.txt";
}

bool ArchivoContactos::guardar(const QString& nombre,
    const QString& telefono,
    const QString& correo) const
{
    QString carpeta = QDir::homePath() + "/Lab7Datos";
    if (!QDir().mkpath(carpeta))
        return false;

    QFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Append))
        return false;

    QString linea = nombre + ";" + telefono + ";" + correo + "\n";
    QByteArray datos = linea.toUtf8();
    bool completo = archivo.write(datos) == datos.size();
    bool vaciado = archivo.flush();
    archivo.close();
    return completo && vaciado;
}

QList<Contacto> ArchivoContactos::leer() const
{
    QList<Contacto> lista;

    QFile archivo(ruta());
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text))
        return lista;               // si no existe aún, devuelve lista vacía

    QTextStream in(&archivo);       // en Qt 6 ya lee UTF-8 por defecto
    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        if (linea.isEmpty())
            continue;

        QStringList campos = linea.split(';');
        if (campos.size() < 3)
            continue;               // ignora líneas mal formadas

        lista.append({ campos[0], campos[1], campos[2] });
    }
    return lista;
}

bool ArchivoContactos::guardarTodos(const QList<Contacto>& lista) const
{
    if (!QDir().mkpath(QFileInfo(ruta()).absolutePath()))
        return false;

    QSaveFile archivo(ruta());   // escribe a un temporal y reemplaza al final
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    for (const Contacto& c : lista) {
        const QByteArray datos =
            (c.nombre + ";" + c.telefono + ";" + c.correo + "\n").toUtf8();
        if (archivo.write(datos) != datos.size())
            return false;
    }
    return archivo.commit();
}