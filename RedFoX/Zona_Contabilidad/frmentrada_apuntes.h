#ifndef FRMENTRADA_APUNTES_H
#define FRMENTRADA_APUNTES_H

#include <QDialog>

#include "../mayamodule.h"

#include "apuntes.h"

namespace Ui {
class FrmEntrada_apuntes;
}

class FrmEntrada_apuntes : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmEntrada_apuntes(QWidget *parent = 0);
    ~FrmEntrada_apuntes();
    module_zone module_zone(){return Contabilidad;}
    QString module_name(){return "Entrada Apuntes";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
   // QPushButton* wantShortCut(bool& ok){ok = false; return 0;}
    
    QPushButton* wantShortCut(bool& ok) {ok = true; return push;}
private:
    QPushButton * push;
    Ui::FrmEntrada_apuntes *ui;
    QAction menuButton;
    apuntes *oApunte;
    void llenar_objeto();
    void clear();
    void llenar_tabla_apunte(int asiento);
    void totales_asiento(int asiento);

private slots:
    void on_btnnuevo_asiento_clicked();
    void controles(bool stat);
    void on_btncerrar_asiento_clicked();
    void on_btnanadir_linea_clicked();
    void on_txtnum_asiento_valueChanged(int arg1);
    void on_txtcuenta_debe_editingFinished();

    void on_btnbuscar_asiento_clicked();
};

#endif // FRMENTRADA_APUNTES_H
