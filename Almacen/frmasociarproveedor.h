#ifndef FRMASOCIARPROVEEDOR_H
#define FRMASOCIARPROVEEDOR_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"
namespace Ui {
class FrmAsociarProveedor;
}

class FrmAsociarProveedor : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmAsociarProveedor(QWidget *parent = 0);
    ~FrmAsociarProveedor();
    QString codigo;
    double pvd;
    double pvdreal;
    QString DescOferta;
    QString Oferta;
    int id_proveedor ;
    int id_divisa;
    void setAnadir();
    void seteditar();
    
private:
    Ui::FrmAsociarProveedor *ui;
    QSqlQueryModel *modelProveedor;
    Configuracion configuracion;
private slots:
    void filtrar_proveedor();
    void setcodigo();
    void setpvd();
    void setDescOferta();
    void setOferta();
    void seleccionarPro(QModelIndex indicde);
    void setpvdReal();
    void setIdDivisa();
public slots:
    void Aceptar();
};

#endif // FRMASOCIARPROVEEDOR_H
