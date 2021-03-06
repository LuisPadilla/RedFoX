#ifndef FRMPRESUPUESTOSCLI_H
#define FRMPRESUPUESTOSCLI_H

#include "Auxiliares/Globlal_Include.h"
#include "presupuesto.h"
#include "../Zona_Pacientes/cliente.h"


#include <QTimer>
#include "../mayamodule.h"
namespace Ui {
class FrmPresupuestosCli;
}

class FrmPresupuestosCli : public MayaModule
{
    Q_OBJECT

signals:

public:
    explicit FrmPresupuestosCli(QWidget *parent = 0);
    ~FrmPresupuestosCli();
    void LLenarCampos();
    void LLenarCamposCliente();
    void LLenarPresupuesto();
    void VaciarCampos();
    void BloquearCampos(bool state);
    Presupuesto *oPres;
    Cliente *oClientePres;

    module_zone module_zone(){return Ventas;}
    QString module_name(){return "Presupestos Ventas";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}
    void setConvertirMenu();
    void init_querys();

private slots:
    void init();

    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();

    void on_btnAnadir_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_btnBuscar_clicked();

    void on_botBuscarCliente_clicked();

    void convertir_epedido();
    void convertir_ealbaran();
    void convertir_enFactura();
    void on_btnBorrar_clicked();

    void on_btndeshacer_clicked();

    void on_txtcodigo_cliente_editingFinished();

    void on_btnImprimir_clicked();

    void on_tabla_doubleClicked(const QModelIndex &index);

    void on_btnDesbloquear_clicked();

    void mostrarBusqueda();
    void ocultarBusqueda();
    void filter_table(QString texto, QString orden, QString modo);
    void calcular_presupuesto();
    void on_btnAnadirLinea_clicked();
    void refrescar_modelo();

    void on_Lineas_doubleClicked(const QModelIndex &index);

    void on_btnPrforma_clicked();

    void on_SpinGastoDist1_valueChanged(double arg1);
    void on_SpinGastoDist2_valueChanged(double arg1);
    void on_SpinGastoDist3_valueChanged(double arg1);

    void on_cboporc_iva_gasto1_currentIndexChanged(int index);
    void on_cboporc_iva_gasto2_currentIndexChanged(int index);
    void on_cboporc_iva_gasto3_currentIndexChanged(int index);
    void on_spinPorc_dto_pp_valueChanged(double arg1);

    void on_spin_porc_dto_especial_valueChanged(double arg1);

    void on_spinPorc_irpf_valueChanged(double arg1);

    void on_chkrecargo_equivalencia_toggled(bool checked);

    void on_chklAprovado_toggled(bool checked);

    void listados();
private:
    Ui::FrmPresupuestosCli *ui;
    QAction * aPedido_action;
    QAction * aAlbaran_action;
    QAction * aFactura_action;
    QMenu * convertir_menu;

    bool editando;
    QString moneda;

    QAction menuButton;
    QPushButton* push;
    QSqlQueryModel *model_busqueda;
    QSqlQueryModel *modelLineas;
    bool eventFilter(QObject *obj, QEvent *event);

    void formato_tablas();

    BarraBusqueda* m_busqueda;
    void setUpBusqueda();

    bool __init;
};

#endif // FRMPRESUPUESTOSCLI_H
