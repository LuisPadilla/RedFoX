#ifndef FRMFACTURARALABARANES_H
#define FRMFACTURARALABARANES_H

#include <QDialog>
#include "../mayamodule.h"
namespace Ui {
class FrmFacturarAlabaranes;
}

class FrmFacturarAlabaranes : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmFacturarAlabaranes(QWidget *parent = 0);
    ~FrmFacturarAlabaranes();
    
    module_zone module_zone(){return Ventas;}
    QString module_name(){return "Facturar Albaranes Ventas";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}QPushButton* wantShortCut(bool& ok){ok = false; return 0;}
private:
    Ui::FrmFacturarAlabaranes *ui;

    ToolBarButton toolButton;
    QAction menuButton;
};

#endif // FRMFACTURARALABARANES_H
