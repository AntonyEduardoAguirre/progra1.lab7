#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_Menu.h"
#include "ArchivoContactos.h"

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QWidget* parent = nullptr);

private:
    void cargarLista();
    void editarSeleccionado();
    void eliminarSeleccionado();
    bool pedirDatos(Contacto& c, const QString& titulo);

    Ui::MenuClass ui;
    ArchivoContactos archivo;
    QList<Contacto> contactos;  // copia en memoria; la fila de la lista = índice aquí
};