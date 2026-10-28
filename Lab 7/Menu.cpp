#include "Menu.h"
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QTableWidget>
#include <QHeaderView>

Menu::Menu(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.tablaContactos->setColumnCount(3);
    ui.tablaContactos->setHorizontalHeaderLabels({ "Nombre", "Teléfono", "Correo" });
    ui.tablaContactos->setSelectionBehavior(QAbstractItemView::SelectRows);   // selecciona la fila completa
    ui.tablaContactos->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.tablaContactos->setEditTriggers(QAbstractItemView::NoEditTriggers);    // la edición va por el diálogo
    ui.tablaContactos->horizontalHeader()->setStretchLastSection(true);
    ui.tablaContactos->verticalHeader()->setVisible(false);

    ui.stackedWidget->setCurrentWidget(ui.paginaMenu);

    connect(ui.btnVer, &QPushButton::clicked, this, [this]() {
        cargarLista();
        ui.stackedWidget->setCurrentWidget(ui.paginaLista);
        });
    connect(ui.btnVolver, &QPushButton::clicked, this, [this]() {
        ui.stackedWidget->setCurrentWidget(ui.paginaMenu);
        });
    connect(ui.btnSalir, &QPushButton::clicked, this, &QWidget::close);

    connect(ui.btnEditar, &QPushButton::clicked, this, &Menu::editarSeleccionado);
    connect(ui.btnEliminar, &QPushButton::clicked, this, &Menu::eliminarSeleccionado);
    connect(ui.tablaContactos, &QTableWidget::cellDoubleClicked,
        this, [this]() { editarSeleccionado(); });

}

void Menu::cargarLista()
{
    contactos = archivo.leer();

    ui.tablaContactos->clearContents();
    ui.tablaContactos->setRowCount(contactos.size());

    for (int i = 0; i < contactos.size(); ++i) {
        ui.tablaContactos->setItem(i, 0, new QTableWidgetItem(contactos[i].nombre));
        ui.tablaContactos->setItem(i, 1, new QTableWidgetItem(contactos[i].telefono));
        ui.tablaContactos->setItem(i, 2, new QTableWidgetItem(contactos[i].correo));
    }

    ui.tablaContactos->resizeColumnsToContents();
    ui.tablaContactos->horizontalHeader()->setStretchLastSection(true);
}

bool Menu::pedirDatos(Contacto& c, const QString& titulo)
{
    QDialog dlg(this);
    dlg.setWindowTitle(titulo);

    auto* form = new QFormLayout(&dlg);
    auto* nombre = new QLineEdit(c.nombre);
    auto* telefono = new QLineEdit(c.telefono);
    auto* correo = new QLineEdit(c.correo);
    auto* botones = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    form->addRow("Nombre:", nombre);
    form->addRow("Teléfono:", telefono);
    form->addRow("Correo:", correo);
    form->addRow(botones);

    connect(botones, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(botones, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return false;

    Contacto nuevo{ nombre->text().trimmed(),
                    telefono->text().trimmed(),
                    correo->text().trimmed() };

    // ';' es el separador del archivo, no puede estar dentro de un campo
    if (nuevo.nombre.isEmpty()
        || nuevo.nombre.contains(';') || nuevo.telefono.contains(';')
        || nuevo.correo.contains(';')) {
        QMessageBox::warning(this, "Datos inválidos",
            "El nombre es obligatorio y ningún campo puede contener ';'.");
        return false;
    }

    c = nuevo;
    return true;
}

void Menu::editarSeleccionado()
{
    const int fila = ui.tablaContactos->currentRow();
    if (contactos.isEmpty() || fila < 0 || fila >= contactos.size()) {
        QMessageBox::information(this, "Editar", "Selecciona un contacto primero.");
        return;
    }

    Contacto c = contactos[fila];
    if (!pedirDatos(c, "Editar contacto"))
        return;

    contactos[fila] = c;
    if (!archivo.guardarTodos(contactos))
        QMessageBox::critical(this, "Error", "No se pudo guardar el archivo.");

    cargarLista();
    ui.tablaContactos->selectRow(fila);
}

void Menu::eliminarSeleccionado()
{
    const int fila = ui.tablaContactos->currentRow();
    if (contactos.isEmpty() || fila < 0 || fila >= contactos.size()) {
        QMessageBox::information(this, "Eliminar", "Selecciona un contacto primero.");
        return;
    }

    const auto resp = QMessageBox::question(this, "Eliminar",
        QString("¿Eliminar a \"%1\"?").arg(contactos[fila].nombre));
    if (resp != QMessageBox::Yes)
        return;

    contactos.removeAt(fila);
    if (!archivo.guardarTodos(contactos))
        QMessageBox::critical(this, "Error", "No se pudo guardar el archivo.");

    cargarLista();
}