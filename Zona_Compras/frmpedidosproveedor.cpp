#include "frmpedidosproveedor.h"
#include "ui_frmpedidosproveedor.h"
#include "../Busquedas/db_consulta_view.h"
#include "../Auxiliares/frmdialogoimprimir.h"
#include "../Auxiliares/frmaddentregascuenta.h"
#include "../Auxiliares/entregascuenta.h"
#include <QPrintDialog>


FrmPedidosProveedor::FrmPedidosProveedor(QWidget *parent, bool showCerrar) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmPedidosProveedor),
    helper(this),
    prov(this),
    toolButton(tr("Pedidos \nProv."),":/Icons/PNG/pedido_pro.png",this),
    menuButton(QIcon(":/Icons/PNG/pedido_pro.png"),tr("Pedidos Prov."),this),
    shortCut(new QPushButton(QIcon(":/Icons/PNG/pedido_pro.png"),"",this))
{
    ui->setupUi(this);

    oPedido_proveedor = new PedidoProveedor(this);
    bloquearcampos(true);
    ui->lbimpreso->setVisible(false);
    ui->lblnombreProveedor->clear();
    ui->lblnumero_pedido->clear();
    shortCut->setToolTip(tr("Gestión de pedidos a proveedor"));
    shortCut->setStyleSheet("background-color: rgb(133, 170, 142)");

    //oProveedor = new Proveedor(this);
    ui->combo_pais->setModel(Configuracion_global->paises_model);
    ui->combo_pais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    helper.set_Tipo(true);
    helper.help_table(ui->Lineas);
    helper.set_tarifa(1);
    helper.setDatabase("empresa","lin_ped_pro");
    helper.setidHeader(1);
    helper.blockTable(true);

    connect(&helper,SIGNAL(lineaReady(lineaDetalle*)),this,SLOT(lineaReady(lineaDetalle*)));
    connect(&helper,SIGNAL(lineaDeleted(int)),this,SLOT(lieaDeleted(int)));


    connect(ui->btnAnadirLinea,SIGNAL(clicked()),&helper,SLOT(addRow()));
    connect(ui->btn_borrarLinea,SIGNAL(clicked()),&helper,SLOT(removeRow()));
    connect(ui->chklporc_rec,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));
    connect(ui->btnAnadir,SIGNAL(clicked()),this,SLOT(anadir_pedido()));
    connect(ui->btnEditar,SIGNAL(clicked()),this,SLOT(editar_pedido()));
    connect(ui->btnGuardar,SIGNAL(clicked()),this,SLOT(guardar_pedido()));
    connect(ui->btnDeshacer,SIGNAL(clicked()),this,SLOT(deshacer()));
    connect(ui->btnSiguiente,SIGNAL(clicked()),this,SLOT(siguiente()));
    connect(ui->btnAnterior,SIGNAL(clicked()),this,SLOT(anterior()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(imprimir()));
    connect(ui->btn_borrar,SIGNAL(clicked()),this,SLOT(borrar_pedido()));

    connect(ui->tabWidget_2,SIGNAL(currentChanged(int)),this,SLOT(resizeTable(int)));

    connect(&helper,SIGNAL(totalChanged(double,double,double,double,double,double,QString)),
            this,SLOT(totalChanged(double,double,double,double,double,double,QString)));

    connect(&helper,SIGNAL(desglose1Changed(double,double,double,double)),
            this,SLOT(desglose1Changed(double,double,double,double)));

    connect(&helper,SIGNAL(desglose2Changed(double,double,double,double)),
            this,SLOT(desglose2Changed(double,double,double,double)));

    connect(&helper,SIGNAL(desglose3Changed(double,double,double,double)),
            this,SLOT(desglose3Changed(double,double,double,double)));

    connect(&helper,SIGNAL(desglose4Changed(double,double,double,double)),
            this,SLOT(desglose4Changed(double,double,double,double)));

    ui->txtiva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtiva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtiva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtiva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));


    ui->btn_cerrar->setVisible(showCerrar);

    pedido.get(0);

}

FrmPedidosProveedor::~FrmPedidosProveedor()
{
    delete ui;
}

void FrmPedidosProveedor::llenarProveedor(int id, bool isNew)
{
    if(isNew)
    {
        ui->btnAnadir->clicked();
        ui->tabWidget_2->setCurrentIndex(1);
        ui->btnAnadirLinea->clicked();
    }
    prov.Recuperar("Select * from proveedores where id="+QString::number(id),1);
    ui->txtcodigoProveedor->setText(prov.codigo);
    ui->txtproveedor->setText(prov.proveedor);
    ui->txtdireccion1->setText(prov.direccion1);
    ui->txtdireccion2->setText(prov.direccion2);
    ui->txtpoblacion->setText(prov.poblacion);
    ui->txtprovincia->setText(prov.provincia);
    ui->txtcp->setText(prov.cp);
    ui->txtcif->setText(prov.cif);
    ui->combo_pais->setCurrentText(Configuracion::Devolver_pais(prov.id_pais));
    ui->lblnombreProveedor->setText(prov.proveedor);
    ui->chklporc_rec->setChecked(prov.recargo_equivalencia);
}

void FrmPedidosProveedor::lineaReady(lineaDetalle * ld)
{
    //-----------------------------------------------------
    // Insertamos línea de pedido y controlamos acumulados
    //-----------------------------------------------------

    bool ok_empresa,ok_Maya;
    ok_empresa = true;
    ok_Maya = true;
    QSqlDatabase::database("empresa").transaction();
    QSqlDatabase::database("Maya").transaction();
    if (ld->idLinea == -1)
    {
        //qDebug()<< ld->idLinea;
        QSqlQuery queryArticulos(QSqlDatabase::database("Maya"));
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;

        QSqlQuery query_lin_ped_pro(QSqlDatabase::database("empresa"));
        query_lin_ped_pro.prepare("INSERT INTO lin_ped_pro (id_cab,id_articulo,codigo_articulo_proveedor,"
                                  "descripcion, cantidad, coste_bruto,subtotal_coste,porc_dto,dto,porc_iva,"
                                  "iva,total,cantidad_pendiente) VALUES (:id_cab,:id_articulo,:codigo_articulo_proveedor,"
                                  ":descripcion,:cantidad,:coste_bruto,:subtotal_coste,:porc_dto,:dto,"
                                  ":porc_iva,:iva,:total,:cantidad_pendiente);");
        query_lin_ped_pro.bindValue(":id_cab", oPedido_proveedor->id);
        query_lin_ped_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_ped_pro.bindValue(":codigo_articulo_proveedor",ld->codigo);
        query_lin_ped_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_ped_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_ped_pro.bindValue(":cantidad_pendiente",ld->cantidad);
        query_lin_ped_pro.bindValue(":coste_bruto",ld->importe);
        query_lin_ped_pro.bindValue(":subtotal_coste",ld->subtotal);
        query_lin_ped_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_ped_pro.bindValue(":dto",ld->dto);
        query_lin_ped_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_ped_pro.bindValue(":iva",0); // importe iva hay que calcularlo
        query_lin_ped_pro.bindValue(":total",ld->total);
        if (!query_lin_ped_pro.exec()){
            ok_empresa = false;
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Ocurrió un error al insertar la nueva línea: %1").arg(query_lin_ped_pro.lastError().text()),
                                 tr("Aceptar"));
        }
        // ---------------------------------
        // Actualización stock y acumulados
        //----------------------------------

        queryArticulos.prepare("update articulos set cantidad_pendiente_recibir = cantidad_pendiente_recibir+:cant_recibir, "
                               " stock_real = stock_real + :cant_recibir2 where codigo=:codigo");
        queryArticulos.bindValue(":cant_recibir",ld->cantidad);
        queryArticulos.bindValue(":cant_recibir2",ld->cantidad);
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec() && ok_empresa){
            QSqlDatabase::database("empresa").commit();
            QSqlDatabase::database("Maya").commit();
        } else
        {
            QSqlDatabase::database("empresa").rollback();
            QSqlDatabase::database("Maya").rollback();
        }

        ld->idLinea = query_lin_ped_pro.lastInsertId().toInt();

    } else
    {
        // --------------------------
        // Descuento unidades pedidas
        //---------------------------
        QSqlQuery queryArticulos(QSqlDatabase::database("Maya"));
        queryArticulos.prepare("update articulos set cantidad_pendiente_recibir = cantidad_pendiente_recibir-:cant_recibir,"
                               "stock_real = stock_real - :cant_recibir2 where codigo=:codigo");
        queryArticulos.bindValue(":cant_recibir",ld->cantidad_old);
        queryArticulos.bindValue(":cant_recibir2",ld->cantidad_old);
        queryArticulos.bindValue(":codigo",ld->codigo);
        if (!queryArticulos.exec())
        {
            ok_Maya = false;
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Se produjo un error al actualizar stock : %1").arg(queryArticulos.lastError().text()));

        }

        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;
        QSqlQuery query_lin_ped_pro(QSqlDatabase::database("empresa"));
        query_lin_ped_pro.prepare("UPDATE lin_ped_pro SET "
                                  "id_articulo =:id_articulo,"
                                  "codigo_articulo_proveedor =:codigo_articulo_proveedor,"
                                  "descripcion =:descripcion,"
                                  "cantidad =:cantidad,"
                                  "cantidad_pendiente =:cantidad_pendiente,"
                                  "coste_bruto =:coste_bruto,"
                                  "subtotal_coste =:subtotal_coste,"
                                  "porc_dto =:porc_dto,"
                                  "dto =:dto,"
                                  "porc_iva =:porc_iva,"
                                  "iva =:iva,"
                                  "total =:total "
                                  "WHERE id = :id;");

        query_lin_ped_pro.bindValue(":id_cab", oPedido_proveedor->id);
        query_lin_ped_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_ped_pro.bindValue(":codigo_articulo_proveedor",ld->codigo);
        query_lin_ped_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_ped_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_ped_pro.bindValue(":cantidad_pendiente",ld->cantidad);
        query_lin_ped_pro.bindValue(":coste_bruto",ld->importe);
        query_lin_ped_pro.bindValue(":subtotal_coste",ld->subtotal);
        query_lin_ped_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_ped_pro.bindValue(":dto",ld->dto);
        query_lin_ped_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_ped_pro.bindValue(":iva",0); // importe iva hay que calcularlo
        query_lin_ped_pro.bindValue(":total",ld->total);
        query_lin_ped_pro.bindValue(":id",ld->idLinea);

        if (!query_lin_ped_pro.exec()) {
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Ocurrió un error al guardar la línea: %1").arg(query_lin_ped_pro.lastError().text()),
                                 tr("Aceptar"));
            ok_empresa = false;
        }
        // ---------------------------------
        // Actualización stock y acumulados
        //----------------------------------
        queryArticulos.prepare("update articulos set cantidad_pendiente_recibir = cantidad_pendiente_recibir + :cant_recibir, "
                               "stock_real = stock_real + :cant_recibir2 where codigo=:codigo");
        queryArticulos.bindValue(":cant_recibir",ld->cantidad);
        queryArticulos.bindValue(":cant_recibir2",ld->cantidad);
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec() && ok_empresa && ok_Maya){
            QSqlDatabase::database("empresa").commit();
            QSqlDatabase::database("Maya").commit();
        } else
        {
            QSqlDatabase::database("empresa").rollback();
            QSqlDatabase::database("Maya").rollback();
        }
    }
    ld->cantidad_old = ld->cantidad;
}

void FrmPedidosProveedor::lineaDeleted(lineaDetalle * ld)
{
    //todo borrar de la bd y stock y demas
    //if id = -1 pasando olimpicamente
    bool ok_empresa,ok_Maya;
    ok_empresa = true;
    ok_Maya = true;
    QSqlDatabase::database("empresa").transaction();
    QSqlDatabase::database("Maya").transaction();
    if(ld->idLinea >-1)
    {
        // --------------------------
        // Descuento unidades pedidas
        //---------------------------
        QSqlQuery queryArticulos(QSqlDatabase::database("Maya"));
        queryArticulos.prepare("update articulos set cantidad_pendiente_recibir = cantidad_pendiente_recibir-:cant_recibir,"
                               "stock_real = stock_real + :cant_recibir2 where codigo=:codigo");
        queryArticulos.bindValue(":cant_recibir",ld->cantidad_old);
        queryArticulos.bindValue(":cant_recibir2",ld->cantidad_old);
        queryArticulos.bindValue(":codigo",ld->codigo);
        if (!queryArticulos.exec())
        {
            ok_Maya = false;
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Se produjo un error al actualizar stock : %1").arg(queryArticulos.lastError().text()));

        }
        //----------------------
        // Borrar línea pedido
        //----------------------
        QSqlQuery querylin_ped_pro(QSqlDatabase::database("empresa"));
        querylin_ped_pro.prepare("delete from lin_ped_pro where id =:id");
        querylin_ped_pro.bindValue(":id",id);
        if(querylin_ped_pro.exec() && ok_Maya)
        {
            QSqlDatabase::database("empresa").commit();
            QSqlDatabase::database("Maya").commit();
        } else
        {
            QSqlDatabase::database("empresa").rollback();
            QSqlDatabase::database("Maya").rollback();
        }
    }
    //qDebug() << "borra" << id;
    delete ld;
}

void FrmPedidosProveedor::totalChanged(double base, double dto, double subtotal, double iva, double re, double total, QString moneda)
{
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)+moneda));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',2)+moneda));
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',2)+moneda));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)+moneda));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)+moneda));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)+moneda));
    ui->lbl_total->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)+moneda));

    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)+moneda));
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)+moneda));
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)+moneda));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)+moneda));
    this->moneda = moneda;
}

void FrmPedidosProveedor::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmPedidosProveedor::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmPedidosProveedor::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda( QString::number(re,'f',2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmPedidosProveedor::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}


void FrmPedidosProveedor::bloquearcampos(bool estado)
{

    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(estado);
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(!estado);
        //qDebug() << lineEdit->objectName();
    }
    // SpinBox
//    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
//    QSpinBox *SpinBox;
//    foreach (SpinBox, SpinBoxList) {
//        SpinBox->setReadOnly(!estado);
//        //qDebug() << lineEdit->objectName();
//   }
//DoubleSpinBox
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(estado);
        //qDebug() << lineEdit->objectName();
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(!estado);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(estado);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(!estado);
        //qDebug() << lineEdit->objectName();
    }

    ui->btnAnadir->setEnabled(estado);
    ui->btnAnterior->setEnabled(estado);
    ui->btn_borrar->setEnabled(estado);
    ui->btnDeshacer->setEnabled(!estado);
    ui->btnEditar->setEnabled(estado);
    ui->btnGuardar->setEnabled(!estado);
    ui->btnSiguiente->setEnabled(estado);
    ui->btnAnadirLinea->setEnabled(!estado);

    ui->btn_borrarLinea->setEnabled(!estado);
    ui->botBuscarCliente->setEnabled(!estado);
    helper.blockTable(estado);
    // activo controles que deben estar activos.


}

void FrmPedidosProveedor::buscar_proveeedor()
{
//    FrmBuscarProveedor buscar(this);
//    if(buscar.exec() == QDialog::Accepted)
    db_consulta_view consulta;
    QStringList campos;
    campos << "proveedor";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Proveedores");
    consulta.set_db("Maya");
    consulta.set_SQL("select id, codigo,proveedor,cif,poblacion from proveedores");
    QStringList cabecera;
    QVariantList tamanos;
    cabecera  << tr("Código") << tr("Proveedor") << tr("DNI/CIF/NIE") << tr("Población");
    tamanos <<"0" << "100" << "300" << "180" << "300";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de proveedores");
    if(consulta.exec())
    {
        int id_proveedor = consulta.get_id();
        oPedido_proveedor->id_proveedor = id_proveedor;
        llenarProveedor(id_proveedor);
    }
}

void FrmPedidosProveedor::anadir_pedido()
{
    int id = oPedido_proveedor->nuevo_pedido_proveedor();
    clear();
    oPedido_proveedor->recuperar(id);
    this->id = id;
    emit block();
    llenar_campos();
    bloquearcampos(false);
    }

void FrmPedidosProveedor::guardar_pedido()
{
    guardar_campos_en_objeto();
    oPedido_proveedor->id =this->id;
    oPedido_proveedor->guardar();

    int regs = ui->Lineas->rowCount();
   // helper.saveTable(oPedido_proveedor->id,"empresa","lin_ped_pro");
    oPedido_proveedor->recuperar(oPedido_proveedor->id);
    llenar_campos();
    bloquearcampos(true);
    emit unblock();
}

void FrmPedidosProveedor::editar_pedido()
{
    bloquearcampos(false);
    emit block();
    ui->txtcodigoProveedor->setFocus();
}
void FrmPedidosProveedor::deshacer()
{
    llenar_campos();
    bloquearcampos(true);
    emit unblock();
}

void FrmPedidosProveedor::siguiente()
{
    oPedido_proveedor->recuperar("select * from ped_pro where pedido > "+
                                 QString::number(oPedido_proveedor->pedido)+ " order by pedido limit 0,1",1);
    llenar_campos();
}

void FrmPedidosProveedor::anterior()
{
    oPedido_proveedor->recuperar("select * from ped_pro where pedido < "
                                 +QString::number(oPedido_proveedor->pedido)+ " order by pedido desc  limit 0,1 ",2);
    llenar_campos();
}

void FrmPedidosProveedor::imprimir()
{
    oPedido_proveedor->imprimir(oPedido_proveedor->id);
}

void FrmPedidosProveedor::borrar_pedido()
{
    if(QMessageBox::question(this,tr("Gestión de Pedidos a proveedores"),
                             tr("¿Desea eliminar este pedido?\nEsta opción no se podrá deshacer"),
                             tr("Cancelar"),tr("Aceptar")) == QMessageBox::Accepted)
    {
        bool exito = oPedido_proveedor->borrar(oPedido_proveedor->id);
        if (exito == true)
            clear();


    }
}

void FrmPedidosProveedor::llenar_campos()
{
    this->id =oPedido_proveedor->id;
  //  helper.set_id_cabecera(oPedido_proveedor->id);
    ui->txtpedido->setText(QString::number(oPedido_proveedor->pedido));
    ui->lblSerie->setText(QString::number(oPedido_proveedor->ejercicio));
    ui->lblnumero_pedido->setText(QString::number(oPedido_proveedor->pedido));
    ui->lblnombreProveedor->setText(oPedido_proveedor->proveedor);
    ui->txtfecha->setDate(oPedido_proveedor->fecha);
    ui->txtFechaLimite->setDate(oPedido_proveedor->recepcion);
    ui->txtcodigoProveedor->setText(oPedido_proveedor->codigoProveedor);
    ui->txtproveedor->setText(oPedido_proveedor->proveedor);
    ui->txtdireccion1->setText(oPedido_proveedor->direccion1);
    ui->txtdireccion2->setText(oPedido_proveedor->direccion2);
    ui->txtcp->setText(oPedido_proveedor->cp);
    ui->txtpoblacion->setText(oPedido_proveedor->poblacion);
    ui->txtprovincia->setText(oPedido_proveedor->provincia);
    ui->txtcif->setText(oPedido_proveedor->cif_nif);
    ui->txtbase->setText(QString::number(oPedido_proveedor->base_total));
    ui->txtsubtotal->setText(QString::number(oPedido_proveedor->subtotal));
    ui->txtimporte_descuento->setText(QString::number(oPedido_proveedor->dto));
    ui->txtiva->setText(QString::number(oPedido_proveedor->iva));
    ui->txtporc_rec1->setText(QString::number(oPedido_proveedor->recTotal));
    ui->chklporc_rec->setChecked(oPedido_proveedor->recargo_equivalencia);
    ui->txttotal->setText(QString::number(oPedido_proveedor->total));
    ui->chklEnviado->setChecked(oPedido_proveedor->lEnviado);
    ui->chkRecibido->setChecked(oPedido_proveedor->recibido);
    ui->chklCompleto->setChecked(oPedido_proveedor->recibido_completo);

    ui->txtcomentario->setText(oPedido_proveedor->comentario);
    ui->txtfechaRecepcion->setDate(oPedido_proveedor->fechaEntrega);
    ui->txtdireccion1Entrega->setText(oPedido_proveedor->direccion1Entrega);
    ui->txtdireccion1Entrega_2->setText(oPedido_proveedor->direccion2Entrega);
    ui->txtcpEntrega->setText(oPedido_proveedor->cpEntrega);
    ui->txtpoblacionEntrega->setText(oPedido_proveedor->poblacionEntrega);
    ui->txtprovinciaEntrega->setText(oPedido_proveedor->provinciaEntrega);
    ui->txtHorarioEntrega->setText(oPedido_proveedor->horarioActivo);
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->base1,'f',2)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->base2,'f',2)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->base3,'f',2)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->base4,'f',2)));
    ui->txtiva1->setText(QString::number(oPedido_proveedor->iva1));
    ui->txtiva2->setText(QString::number(oPedido_proveedor->iva2));
    ui->txtiva3->setText(QString::number(oPedido_proveedor->iva3));
    ui->txtiva4->setText(QString::number(oPedido_proveedor->iva4));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->iva1,'f',2)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->iva2,'f',2)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->iva3,'f',2)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->iva4,'f',2)));
    ui->txtporc_rec1->setText(QString::number(oPedido_proveedor->porc_rec1));
    ui->txtporc_rec2->setText(QString::number(oPedido_proveedor->porc_rec2));
    ui->txtporc_rec3->setText(QString::number(oPedido_proveedor->porc_rec3));
    ui->txtporc_rec4->setText(QString::number(oPedido_proveedor->porc_rec4));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->rec1,'f',2)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->rec2,'f',2)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->rec3,'f',2)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->rec4,'f',2)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->total1,'f',2)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->total2,'f',2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->total3,'f',2)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->total4,'f',2)));
    QString filter = QString("id_cab = '%1'").arg(oPedido_proveedor->id);
    helper.fillTable("empresa","lin_ped_pro",filter);
    helper.resizeTable();
    cargar_tabla_entregas();

}

void FrmPedidosProveedor::guardar_campos_en_objeto()
{
    oPedido_proveedor->id = this->id;
    oPedido_proveedor->pedido = ui->lblnumero_pedido->text().toInt();
    oPedido_proveedor->ejercicio = ui->lblSerie->text().toInt();
    oPedido_proveedor->proveedor = ui->txtproveedor->text();
    oPedido_proveedor->fecha = ui->txtfecha->date();
    oPedido_proveedor->recepcion =ui->txtfechaRecepcion->date();
    oPedido_proveedor->codigoProveedor = ui->txtcodigoProveedor->text();
    oPedido_proveedor->direccion1 = ui->txtdireccion1->text();
    oPedido_proveedor->direccion2 = ui->txtdireccion2->text();
    oPedido_proveedor->cp = ui->txtcp->text();
    oPedido_proveedor->poblacion = ui->txtpoblacion->text();
    oPedido_proveedor->provincia = ui->txtprovincia->text();
    oPedido_proveedor->cif_nif = ui->txtcif->text();
    oPedido_proveedor->base_total = ui->txtbase_total_2->text().toDouble();
    oPedido_proveedor->subtotal = ui->txtsubtotal->text().toDouble();
    oPedido_proveedor->dto = ui->txtimporte_descuento->text().toDouble();
    oPedido_proveedor->iva = ui->txtiva->text().toDouble();
    oPedido_proveedor->recTotal= ui->txttotal_recargo->text().toDouble();
    oPedido_proveedor->total = ui->txttotal->text().toDouble();
    oPedido_proveedor->lEnviado = ui->chklEnviado->isChecked();
    oPedido_proveedor->recibido = ui->chkRecibido->isChecked();
    oPedido_proveedor->recibido_completo = ui->chklCompleto->isChecked();
    oPedido_proveedor->recargo_equivalencia = ui->chklporc_rec->isChecked();
    oPedido_proveedor->comentario = ui->txtcomentario->toPlainText();
    oPedido_proveedor->fechaEntrega = ui->txtfechaRecepcion->date();
    oPedido_proveedor->direccion1Entrega = ui->txtdireccion1Entrega->text();
    oPedido_proveedor->direccion2Entrega = ui->txtdireccion1Entrega_2->text();
    oPedido_proveedor->cpEntrega = ui->txtcpEntrega->text();
    oPedido_proveedor->poblacionEntrega = ui->txtpoblacionEntrega->text();
    oPedido_proveedor->provinciaEntrega = ui->txtprovinciaEntrega->text();
    oPedido_proveedor->horarioActivo = ui->txtHorarioEntrega->text();
    oPedido_proveedor->base1 = ui->txtbase1->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->base2 = ui->txtbase2->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->base3 = ui->txtbase3->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->base4 = ui->txtbase4->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->iva1 = ui->txtiva1->text().toInt();
    oPedido_proveedor->iva2 = ui->txtiva2->text().toInt();
    oPedido_proveedor->iva3 = ui->txtiva3->text().toInt();
    oPedido_proveedor->iva4 = ui->txtiva4->text().toInt();
    oPedido_proveedor->iva1 = ui->txtiva1->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->iva2 = ui->txtiva2->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->iva3 = ui->txtiva3->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->iva4 = ui->txtiva4->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->porc_rec1 = ui->txtporc_rec1->text().toInt();
    oPedido_proveedor->porc_rec2 = ui->txtporc_rec2->text().toInt();
    oPedido_proveedor->porc_rec3 = ui->txtporc_rec3->text().toInt();
    oPedido_proveedor->porc_rec4 = ui->txtporc_rec4->text().toInt();
    oPedido_proveedor->rec1 = ui->txtporc_rec1->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->rec2 = ui->txtporc_rec2->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->rec3 = ui->txtporc_rec3->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->rec4 = ui->txtporc_rec4->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->total1 = ui->txttotal1->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->total2 = ui->txttotal2->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->total3 = ui->txttotal3->text().replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->total4 = ui->txttotal4->text().replace(",",".").replace(moneda,"").toDouble();
}



void FrmPedidosProveedor::clear()
{
    ui->txtpedido->clear();
    ui->lblSerie->clear();
    ui->txtfecha->clear();
    ui->txtFechaLimite->clear();
    ui->txtcodigoProveedor->clear();
    ui->txtproveedor->clear();
    ui->txtdireccion1->clear();
    ui->txtdireccion2->clear();
    ui->txtcp->clear();
    ui->txtpoblacion->clear();
    ui->txtprovincia->clear();
    ui->txtcif->clear();
    ui->txtbase->clear();
    ui->txtsubtotal->clear();
    ui->txtimporte_descuento->clear();
    ui->txtiva->clear();
    ui->txtporc_rec1->clear();
    ui->txttotal->clear();
    ui->chklEnviado->setChecked(false);
    ui->chkRecibido->setChecked(false);
    ui->chklCompleto->setChecked(false);

    ui->txtcomentario->clear();
    ui->txtfechaRecepcion->clear();
    ui->txtdireccion1Entrega->clear();
    ui->txtdireccion1Entrega_2->clear();
    ui->txtcpEntrega->clear();
    ui->txtpoblacionEntrega->clear();
    ui->txtprovinciaEntrega->clear();
    ui->txtHorarioEntrega->clear();
    ui->txtbase1->clear();
    ui->txtbase2->clear();
    ui->txtbase3->clear();
    ui->txtbase4->clear();
    ui->txtiva1->clear();
    ui->txtiva2->clear();
    ui->txtiva3->clear();
    ui->txtiva4->clear();
    ui->txtiva1->clear();
    ui->txtiva2->clear();
    ui->txtiva3->clear();
    ui->txtiva4->clear();
    ui->txtporc_rec1->clear();
    ui->txtporc_rec2->clear();
    ui->txtporc_rec3->clear();
    ui->txtporc_rec4->clear();
    ui->txtporc_rec1->clear();
    ui->txtporc_rec2->clear();
    ui->txtporc_rec3->clear();
    ui->txtporc_rec4->clear();
    ui->txttotal1->clear();
    ui->txttotal2->clear();
    ui->txttotal3->clear();
    ui->txttotal4->clear();
    ui->lblnombreProveedor->clear();
    ui->lblnumalb->clear();
    ui->lblSerie->clear();
    ui->lblnumero_pedido->clear();
}

void FrmPedidosProveedor::resizeTable(int x)
{
    Q_UNUSED(x);
    helper.resizeTable();
}

void FrmPedidosProveedor::on_btnSiguiente_clicked()
{
    if(!pedido.next())
        TimedMessageBox * t = new TimedMessageBox(this,tr("Final de archivo alcanzado"));
}

void FrmPedidosProveedor::on_btnAnterior_clicked()
{
    if(!pedido.prev())
        TimedMessageBox * t = new TimedMessageBox(this,tr("Principio de archivo alcanzado"));
}

void FrmPedidosProveedor::on_btnAnadirEntregas_clicked()
{
    frmAddEntregasCuenta frmEntregas(this);
    if(frmEntregas.exec() == QDialog::Accepted){
        EntregasCuenta oEntrega(this);
        if(!oEntrega.Anadir(2,oPedido_proveedor->id_proveedor,frmEntregas.importe,frmEntregas.fecha,frmEntregas.concepto))
            QMessageBox::warning(this,tr("Gestión de pedidos a proveedor"),
                                 tr("Falló el insertar una nueva entrega a cuenta"),
                                 tr("Aceptar"));
        else
        {
            cargar_tabla_entregas();

        }

    }

}

void FrmPedidosProveedor::cargar_tabla_entregas()
{
    QSqlQueryModel *modelEntregas = new  QSqlQueryModel(this);
    modelEntregas->setQuery("select * from proveedor_a_cuenta where id_proveedor = "+
                            QString::number(oPedido_proveedor->id_proveedor)+" and disponible >0",QSqlDatabase::database("Maya"));
    ui->tabla_entregas->setModel(modelEntregas);
}


void FrmPedidosProveedor::on_btnImprimir_clicked()
{
    Configuracion_global->imprimir(false,true,this);
}
