#ifndef FRMEMPRESAS_H
#define FRMEMPRESAS_H

#include "Auxiliares/Globlal_Include.h"
#include "auxmodule.h"
#include "../empresa.h"
#include "../mayamodule.h"
#include "copy_db_progressfrm.h"

#include <QtConcurrent/QtConcurrent>
class QListWidgetItem;
namespace Ui {
class FrmEmpresas;
}

class FrmEmpresas : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmEmpresas(QWidget *parent = 0);
    ~FrmEmpresas();

    void CargarCamposEnEmpresa();

    module_zone module_zone(){return AdminZone;}
    QString module_name(){return "Control de empresas";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    QPushButton* wantShortCut(bool& ok){ok = false; return 0;}

    void hide_salir();
signals:
    void endGroup(bool);
    void _groupError(QString);

private slots:
    void groupError(QString s);

    void on_btn_ruta_db_clicked();

    void on_txtcDriver_currentIndexChanged(int index);

    void on_btn_migrar_clicked();

    void on_btnConfigTerminal_clicked();

    void on_btn_crear_nuevo_clicked();

    void on_btn_add_nuevo_clicked();

    void on_btn_inicio_clicked();

    void on_btn_guardar_nuevo_clicked();

    void on_btn_crearGrupo_clicked();

    void on_btn_add_empresaGrupo_clicked();

    void on_btn_add_empresa_clicked();
    void on_btn_edit_empresa_clicked();

    void on_listWidgetGrupos_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_btnEditaEmpresa_clicked();

    void on_btn_guardar_edit_clicked();

    void on_btnCambiarLogo_create_clicked();

    void on_btnCambiarLogo_edit_clicked();

    void on_txtcp_editingFinished();

    void on_txtcp_3_editingFinished();

    void on_txtdireccion1_3_editingFinished();

    void on_txtdireccion1_editingFinished();

    void on_btnNewGroupImgPath_clicked();

private:
    Ui::FrmEmpresas *ui;
    Empresa oEmpresa;

    QAction menuButton;

    struct empresa
    {
        QString name;
        QSqlRecord record;
    };

    QHash<QString, QPair<QList<empresa> , QSqlRecord> > _empresas;
    void getEmpresas();
    void _addEmpresa();
    void _llenarCampos(QSqlRecord r);

    bool _createTables(QSqlDatabase db);
    bool _insertMonedas(QSqlDatabase db, QString &error);
    bool _insertIVA(QSqlDatabase db, QString &error);

    bool _insertPaises(QSqlDatabase db, QString &error);
    bool _insertReports(QSqlDatabase db, QString empresa);
    void _insertNewGroup(QString grupo);
    void createGroup();
    QString _targetGroup;
    QSqlDatabase _targetGroupDb;
    int _idEmpresa;
    void limpiar_campos();

    QSqlQueryModel * tarifaEditModel;
    QSqlQueryModel * monedaEditModel;
    QSqlQueryModel * seriesEditModel;
    QSqlQueryModel * paisesEditModel;

    QCompleter * pob_completer;
    QSqlTableModel * pob_completer_model;

    QCompleter * calle_completer;
    QSqlTableModel * calle_completer_model;

    QString _newGroupIMGPath;
};

#endif // FRMEMPRESAS_H
