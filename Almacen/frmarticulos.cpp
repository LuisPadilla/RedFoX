#include "frmarticulos.h"
#include "ui_frmarticulos.h"
#include "../Almacen/frmtarifas.h"
#include "../Busquedas/frmbuscarproveedor.h"
#include "../Almacen/frmasociarproveedor.h"
#include "../Auxiliares/spinboxdelegate.h"
#include "../db_table_view.h"

#include"../Auxiliares/monetarydelegate.h"

#include"../Auxiliares/readonlydelegate.h"
#include"frmlistadosarticulo.h"
#include"../Busquedas/db_consulta_view.h"


FrmArticulos::FrmArticulos(QWidget *parent, bool closeBtn) :
    QDialog(parent),
    ui(new Ui::FrmArticulos)
{
    modelProv = new QSqlQueryModel(this);
    oArticulo = new Articulo();
    modArt = new QSqlQueryModel();
    ui->setupUi(this);
    // Cargar valores IVA
    //Configuracion_global->CargarDatos();
    ui->cboTipoIVA->setModel(Configuracion_global->iva_model);
    ui->cboTipoIVA->setModelColumn(Configuracion_global->iva_model->fieldIndex("cTipo"));
    // Cargar empresas
    QSqlQueryModel *modelEmpresa = new QSqlQueryModel(this);
    modelEmpresa->setQuery("select * from vistaEmpresa",QSqlDatabase::database("Maya"));
    ui->cboEmpresa2->setModel(modelEmpresa);
    ui->lblCodigo->setVisible(false);
    ui->lblDescripcion->setVisible(false);
    ui->graf_prov->Clear();
    ui->graf_prov->verValoresEjeY(true);
    ui->graf_prov->verValores(true);
    qDebug() << "valores en y" << ui->graf_prov->ValoresEjeY();
    ui->checkBox->setEnabled(true);

    GraficaUnidades();


    // Control objetos
    // --------------------
    // TARIFAS
    //---------------------
    tarifa_model = new QSqlRelationalTableModel(this,QSqlDatabase::database("Maya"));
    tarifa_model->setTable("viewTarifa");
    tarifa_model->setFilter("id_Articulo = "+QString::number(oArticulo->id));
    tarifa_model->select();
    //ui->TablaTarifas->setModel(tarifa_model);
    //ui->TablaTarifas->setColumnHidden(0,true);
    QStringList headers;
    headers << "Pais" << "Moneda" << "Codigo de tarifa" << "Descripción";
    headers<< "Margen" << "Margen minimo" << "P.V.P";
    for (int i = 0; i< headers.size();i++)
        tarifa_model->setHeaderData(i+1, Qt::Horizontal, headers.at(i));

    ui->tablaProveedores->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bloquearCampos(true);
    ui->botEditar->setEnabled(false);
    ui->botAnterior->setEnabled(false);
    ui->botBorrar->setEnabled(false);

    reformateado = false;

    //--------------------------------
    // CAMPOS MONEDA
    //--------------------------------
    //ui->txtrCoste->setValidator(Configuracion_global->validator_cantidad);


    //-----------------------------------------
    // CONEXIONES
    //-----------------------------------------
    connect(ui->txtrCoste,SIGNAL(editingFinished()),Configuracion_global,SLOT(format_text()));
    connect(ui->btnAnadirProveedores,SIGNAL(clicked()),this,SLOT(anadir_proveedor_clicked()));
    connect(ui->tablaLotes,SIGNAL(clicked(QModelIndex)),this,SLOT(TablaTrazabilidad_clicked(QModelIndex)));
    connect(ui->radGrafica_unidades,SIGNAL(clicked()),this,SLOT(GraficaUnidades()));
    connect(ui->radGrafica_importes,SIGNAL(clicked()),this,SLOT(GraficaImportes()));
    connect(ui->btnEditarProveedorFrecuente,SIGNAL(clicked()), this, SLOT(editar_proveedor_clicked()));
    connect(ui->btnAsignarProveedor,SIGNAL(clicked()),this,SLOT(asignar_proveedor_principal_clicked()));
    connect(ui->btnBorrarProveedores,SIGNAL(clicked()),this,SLOT(borrar_proveedor_clicked()));
    connect(ui->btnEditartarifa,SIGNAL(clicked()),this,SLOT(btnEditarTarifa_clicked()));
    connect(ui->btnBorrarTarifa,SIGNAL(clicked()),this,SLOT(btnBorrarTarifa_clicked()));
    connect(ui->Pestanas,SIGNAL(currentChanged(int)),this,SLOT(SeleccionarPestana(int)));
    connect(ui->botListados,SIGNAL(clicked()),this,SLOT(listados()));
    connect(ui->cboEmpresa2,SIGNAL(currentIndexChanged(int)),this,SLOT(LLenarGrafica_comparativa(int)));
    connect(ui->radGrafica_importes_2,SIGNAL(toggled(bool)),this,SLOT(MostrarGrafica_comparativa(bool)));
    connect(ui->cboTipoGrafica,SIGNAL(currentIndexChanged(QString)),this,SLOT(graficar(QString)));

    this->installEventFilter(this);
    ui->btn_cerrar->setVisible(closeBtn);
}

FrmArticulos::~FrmArticulos()
{
    delete ui;
    delete oArticulo;
    delete modArt;
}

void FrmArticulos::on_botAnadir_clicked()
{
    QSqlQuery querySecciones(QSqlDatabase::database("Maya"));
    querySecciones.prepare("select id from secciones order by id desc limit 1 ");
    if (querySecciones.exec()){
        querySecciones.next();
        if (querySecciones.record().value("id").toInt()<=0)
        {
                QMessageBox message;
                message.setWindowTitle(tr("Gestión de artículos"));
                message.setText("Para poder añadir artículos debe tener registros de sección");
                message.setAutoFillBackground(true);
                message.setButtonText(1,tr("Aceptar"));
                message.exec();
        } else {
            bloquearCampos(false);
            VaciarCampos();
            oArticulo->Anadir();
            this->anadir = true;
            LLenarCampos();
            //ui->lblImagenArticulo->setPixmap(QPixmap::fromImage());
            ui->txtcCodigo->setFocus();
        }
    }
}

void FrmArticulos::on_botGuardar_clicked()
{
    if(!ui->txtcSeccion->text().isEmpty())
    {
        bloquearCampos(true);
        CargarCamposEnArticulo();
        oArticulo->Guardar();
        actualizar();
    } else
    {
        QMessageBox::warning(this,tr("Gestion de Productos/servicios"),
                             tr("Es necesario que asocie el registro a una sección antes de poder guardar"),
                             tr("Aceptar"));
    }
}


void FrmArticulos::on_botSiguiente_clicked()
{
    QString cCodigo = oArticulo->cCodigo;
    oArticulo->Recuperar("Select * from articulos where cCodigo >'"+cCodigo+"' order by cCodigo limit 1 ",1);
    LLenarCampos();
    ui->botBorrar->setEnabled(true);
    ui->botAnterior->setEnabled(true);
    ui->botEditar->setEnabled(true);
    tarifa_model->setFilter("id_Articulo = "+QString::number(oArticulo->id));
    tarifa_model->select();
}
void FrmArticulos::on_botAnterior_clicked()
{
    QString cCodigo =oArticulo->cCodigo;
    oArticulo->Recuperar("Select * from articulos where cCodigo <'"+cCodigo+"' order by cCodigo desc limit 1 ",2);
    LLenarCampos();
    tarifa_model->setFilter("id_Articulo = "+QString::number(oArticulo->id));
    tarifa_model->select();

}

void FrmArticulos::bloquearCampos(bool state) {

    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(state);
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(!state);
        //qDebug() << lineEdit->objectName();
    }
    // SpinBox
//    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
//    QSpinBox *SpinBox;
//    foreach (SpinBox, SpinBoxList) {
//        SpinBox->setReadOnly(state);
//        //qDebug() << lineEdit->objectName();
//   }
//DoubleSpinBox
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(state);
        //qDebug() << lineEdit->objectName();
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(!state);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(state);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(!state);
        //qDebug() << lineEdit->objectName();
    }

    ui->botAnadir->setEnabled(state);
    ui->botAnterior->setEnabled(state);
    ui->botBorrar->setEnabled(state);
    ui->botDeshacer->setEnabled(!state);
    ui->botEditar->setEnabled(state);
    ui->botGuardar->setEnabled(!state);
    ui->botSiguiente->setEnabled(state);
    // activo controles que deben estar activos.

    // Botones artículos
    ui->botBuscarSeccion->setEnabled(!state);
   // ui->botBuscar->setEnabled(state);
    ui->botBuscarFamilia->setEnabled(!state);
    ui->botBuscarSubfamilia->setEnabled(!state);
    ui->botBuscarSubSubFamilia->setEnabled(!state);
    ui->botCambiarImagen->setEnabled(!state);
    ui->botCambiarImagen_2->setEnabled(!state);
    ui->botCambiarImagen_3->setEnabled(!state);
    ui->botCambiarImagen_4->setEnabled(!state);
    ui->botBuscarGrupo->setEnabled(!state);
    ui->btnBuscarProveedor->setEnabled(!state);
    ui->btnAnadirProveedores->setEnabled(!state);
    ui->btnBorrarProveedores->setEnabled(!state);
    ui->btnEditarProveedorFrecuente->setEnabled(!state);
    ui->btnAsignarProveedor->setEnabled(!state);
    ui->checkBox->setEnabled(state);
    ui->cboTipoGrafica->setEnabled(state);
    ui->cboTipoGrafica_2->setEnabled(state);
    ui->radGrafica_importes_2->setEnabled(state);
    ui->radGrafica_unidades_2->setEnabled(state);
    ui->chkmostrarvalores_comparativa->setEnabled(state);
    ui->btnAnadirTarifa->setEnabled(!state);
    ui->btnEditartarifa->setEnabled(!state);
    ui->btnBorrarTarifa->setEnabled(!state);


}

void FrmArticulos::LLenarCampos()
{

   ui->txtcCodigo->setText(oArticulo->cCodigo);
   ui->lblCodigo->setText(oArticulo->cCodigo);
   ui->txtcCodigoBarras->setText(oArticulo->cCodigoBarras);
   ui->txtcCodigoFabricante->setText(oArticulo->cCodigoFabricante);
   ui->txtcDescripcion->setText(oArticulo->cDescripcion);
   ui->lblDescripcion->setText(oArticulo->cDescripcion);
   ui->lblCodigo->setVisible(true);
   ui->lblDescripcion->setVisible(true);
   ui->txtcDescripcionResumida->setText(oArticulo->cDescripcionReducida);
   ui->txtcProveedor->setText(oArticulo->cProveedor);
   ui->txtCodigoProveedor->setText(oArticulo->cCodProveedor);
   ui->txtcFamilia->setText(oArticulo->cFamilia);
   ui->txtcSeccion->setText(oArticulo->cSeccion);
   ui->txtcSubFamilia->setText(oArticulo->cSubfamilia);
   int nIndex = ui->cboTipoIVA->findText(QString::number(oArticulo->nTipoIva));
   if (nIndex !=-1)
           ui->cboTipoIVA->setCurrentIndex(nIndex);
   ui->txtrDto->setText(QString::number(oArticulo->rDto,'f',2));
   ui->txtrCoste->setText(Configuracion_global->FormatoNumerico(QString::number(oArticulo->rCoste,'f',2)));
   ui->txtdFechaUltimaCompra->setDate(oArticulo->dUltimaCompra);
   ui->txtdFechaUltimaVenta->setDate(oArticulo->dUltimaVenta);
   ui->txtnUnidadesCompradas->setText(QString::number(oArticulo->nUnidadesCompradas));
   ui->txtnUnidadesVendidas->setText(QString::number(oArticulo->nUnidadesVendidas));
   ui->txtrAcumuladoCompras->setText(Configuracion_global->FormatoNumerico(QString::number(oArticulo->rAcumuladoCompras,'f',2)));
   ui->txtrAcumuladoVentas->setText(Configuracion_global->FormatoNumerico(QString::number(oArticulo->rAcumuladoVentas,'f',2)));
   ui->txttComentario->setText(oArticulo->tComentario);
   ui->txtnStockMaximo->setText(QString::number(oArticulo->nStockMaximo));
   ui->txtnStockMinimo->setText(QString::number(oArticulo->nStockMinimo));
   //ui->txtnStockReal->setText(QString::number(oArticulo->nStockReal));
   ui->txtnStockReal_2->setText(QString::number(oArticulo->nStockReal));
   ui->txtStockfisico->setText(QString::number(oArticulo->nStockFisico));

   if (oArticulo->lControlarStock==1)
        ui->chklControlarStock->setChecked(true);
   else
       ui->chklControlarStock->setChecked(false);

   ui->txtnCantidadPendienteRecibir->setText(QString::number(oArticulo->nCantidadPendienteRecibir));
   ui->txtdFechaPrevistaRecepcion->setDate(oArticulo->dFechaPrevistaRecepcion);
   ui->txtnReservados->setText(QString::number(oArticulo->nReservados));
   ui->txtcSeccion->setText(oArticulo->getcSeccion(oArticulo->id_Seccion));
   ui->txtcFamilia->setText(oArticulo->getcFamilia(oArticulo->id_Familia));
   ui->txtcSubFamilia->setText(oArticulo->getcSubFamilia(oArticulo->id_SubFamilia));
   ui->txtcSubSubFamilia->setText((oArticulo->getcSubSubFamilia(oArticulo->idsubsubfamilia)));
   ui->txtcGupoArt->setText(oArticulo->getcGrupo(oArticulo->idgrupoart));
  // ui->txtMargen->setValue(oArticulo->margen);
  // ui->txtMargen_min->setValue((oArticulo->margen_min));

   if (oArticulo->lMostrarWeb==1)
        ui->chklMostrarWeb->setChecked(true);
    else
       ui->chklMostrarWeb->setChecked(false);
   //-----------------------------
   // Recuperamos imagen desde BD
   //-----------------------------
   oArticulo->CargarImagen(ui->lblImagenArticulo_1,ui->lblImagenArticulo_2,
                           ui->lblImagenArticulo_3,ui->lblImagenArticulo_4);

   //-----------------------
   // llenamos combo iva
   //-----------------------
  nIndex = ui->cboTipoIVA->findText(Configuracion_global->setTipoIva(oArticulo->id_tiposiva));
  if(nIndex >-1)
      ui->cboTipoIVA->setCurrentIndex(nIndex);

  ui->chkArticulo_promocionado->setChecked(oArticulo->articulopromocionado);
  ui->framePromocion->setEnabled(false);
  ui->txtDescripcion_promocion->setText(oArticulo->descripcion_promocion);
  switch (oArticulo->tipo_oferta) {
  case 1:
      ui->radOferta1->setChecked(true);
      break;
  case 2:
      ui->radOferta2->setChecked(true);
      break;
  case 3:
      ui->radOferta3->setChecked(true);
      break;
  case 4:
      ui->radOferta4->setChecked(true);
      break;
  }

  ui->txtpor_cada->setText(QString::number(oArticulo->por_cada));
  ui->txtRegalode->setText(QString::number(oArticulo->regalode));
  ui->txt_dto_web->setText(QString::number(oArticulo->porc_dto_web,'f',2));
  ui->txtoferta_pvp_fijo->setText(QString::number(oArticulo->oferta_pvp_fijo,'f',2));
  ui->txtComentarios_promocion->setPlainText(oArticulo->comentario_oferta);


  // ------------------
  // LLENO TABLAS DATOS
  //-------------------
  LlenarTablas();
  //-------------------
  // LLENO ACUMULADOS
  //-------------------
  LLenarGraficas();



}

void FrmArticulos::CargarCamposEnArticulo()
{
    oArticulo->cCodigo= ui->txtcCodigo->text();
    oArticulo->cCodigoBarras =ui->txtcCodigoBarras->text();
    oArticulo->cCodigoFabricante=ui->txtcCodigoFabricante->text();
    oArticulo->cDescripcion =ui->txtcDescripcion->text();
    oArticulo->cDescripcionReducida = ui->txtcDescripcionResumida->text();
    oArticulo->cFamilia=ui->txtcFamilia->text();
    oArticulo->cSeccion=ui->txtcSeccion->text();
    oArticulo->cSubfamilia=ui->txtcSubFamilia->text();
    oArticulo->nTipoIva=Configuracion_global->ivas[ui->cboTipoIVA->currentText()].value("nIva").toDouble();
    oArticulo->id_tiposiva = Configuracion_global->getIdIva(ui->cboTipoIVA->currentText());
    oArticulo->rCoste=ui->txtrCoste->text().replace(",",".").toDouble();
    oArticulo->dUltimaCompra= ui->txtdFechaUltimaCompra->date();
    oArticulo->dUltimaVenta= ui->txtdFechaUltimaVenta->date();
    oArticulo->nUnidadesCompradas= ui->txtnUnidadesCompradas->text().toDouble();
    oArticulo->nUnidadesVendidas=ui->txtnUnidadesVendidas->text().toDouble();
    oArticulo->rAcumuladoCompras= ui->txtrAcumuladoCompras->text().replace(",",".").toDouble();
    oArticulo->rAcumuladoVentas= ui->txtrAcumuladoVentas->text().replace(",",".").toDouble();
    oArticulo->tComentario=ui->txttComentario->toPlainText();
    oArticulo->nStockMaximo=ui->txtnStockMaximo->text().toInt();
    oArticulo->nStockMinimo=ui->txtnStockMinimo->text().toInt();
  //  oArticulo->nStockReal=ui->txtnStockReal->text().toInt();

    if (ui->chklControlarStock->isChecked())
       oArticulo->lControlarStock=1;
    else
        oArticulo->lControlarStock=0;

    oArticulo->nCantidadPendienteRecibir=ui->txtnCantidadPendienteRecibir->text().toInt();
    oArticulo->dFechaPrevistaRecepcion =ui->txtdFechaPrevistaRecepcion->date();
    oArticulo->nReservados =ui->txtnReservados->text().toInt();
    if (ui->chklMostrarWeb->isChecked())
        oArticulo->lMostrarWeb = 1;
    else
        oArticulo->lMostrarWeb = 0;
    oArticulo->id_tiposiva = Configuracion_global->getIdIva(ui->cboTipoIVA->currentText());
    oArticulo->id_Seccion = oArticulo->getIdSeccion(ui->txtcSeccion->text());
    oArticulo->id_Familia  = oArticulo->getIdFamilia(ui->txtcFamilia->text());
    oArticulo->id_SubFamilia = oArticulo->getIdSubFamilia(ui->txtcSubFamilia->text());
    oArticulo->idsubsubfamilia = oArticulo->getIdSubSufFamilia(ui->txtcSubSubFamilia->text());
    oArticulo->idgrupoart = oArticulo->getIdGrupo(ui->txtcGupoArt->text());
    oArticulo->cCodProveedor = ui->txtCodigoProveedor->text();
    oArticulo->cProveedor = ui->txtcProveedor->text();

//    this->idweb = registro.field("idweb").value().toInt();
  //  oArticulo->stockfisico = ui->txtStockFisico->text().toInt();
    oArticulo->articulopromocionado = ui->chkArticulo_promocionado->isChecked();
    oArticulo->descripcion_promocion = ui->txtDescripcion_promocion->text();

    if(ui->radOferta1->isChecked())
            oArticulo->tipo_oferta = 1;
    if(ui->radOferta2->isChecked())
            oArticulo->tipo_oferta = 2;
    if(ui->radOferta3->isChecked())
            oArticulo->tipo_oferta = 3;
    if(ui->radOferta4->isChecked())
            oArticulo->tipo_oferta = 4;


    oArticulo->por_cada = ui->txtpor_cada->text().toInt();
    oArticulo->regalode = ui->txtRegalode->text().toInt();
    oArticulo->porc_dto_web = ui->txt_dto_web->text().replace(",",".").toDouble();
    oArticulo->oferta_pvp_fijo = ui->txtoferta_pvp_fijo->text().replace(",",".").toDouble();
    oArticulo->comentario_oferta = ui->txtComentarios_promocion->toPlainText();
   // oArticulo->margen = ui->txtMargen->value();
   // oArticulo->margen_min = ui->txtMargen_min->value();

}

void FrmArticulos::VaciarCampos()
{
   oArticulo->Vaciar();
   ui->txtcCodigo->setText("");
   ui->txtcCodigoBarras->setText("");
   ui->txtcCodigoFabricante->setText("");
   ui->txtcDescripcion->setText("");
   ui->txtcDescripcionResumida->setText("");
   ui->txtcProveedor->setText("");
   ui->txtcFamilia->setText("");
   ui->txtcSeccion->setText("");
   ui->txtcSubFamilia->setText("");
   ui->cboTipoIVA->setEditText("");
   ui->txtrCoste->setText("0,00");
   ui->txtrDto->setText("0");
   ui->txtdFechaUltimaCompra->setDate(QDate::currentDate());
   ui->txtdFechaUltimaVenta->setDate(QDate::currentDate());
   ui->txtnUnidadesCompradas->setText("0");
   ui->txtnUnidadesVendidas->setText("0");
   ui->txtrAcumuladoCompras->setText("0,00");
   ui->txtrAcumuladoVentas->setText("0,00");
   ui->txttComentario->setText("");
   ui->txtnStockMaximo->setText("0");
   ui->txtnStockMinimo->setText("0");
  // ui->txtnStockReal->setText("0");
   ui->txtnStockReal_2->setText("0");
  // ui->txtStockFisico->setText("0");
   ui->chklControlarStock->setChecked(false);
   ui->txtnCantidadPendienteRecibir->setText("0");
   ui->txtdFechaPrevistaRecepcion->setDate(QDate::currentDate());
   ui->txtnReservados->setText("0");
   ui->chklMostrarWeb->setChecked(false);
   ui->txtCodigoProveedor->setText("");
   ui->chkArticulo_promocionado->setChecked(false);
   ui->txtDescripcion_promocion->setText("");
   ui->radOferta1->setChecked(false);
   ui->radOferta2->setChecked(false);
   ui->radOferta3->setChecked(false);
   ui->radOferta4->setChecked(false);
   ui->txtpor_cada->setText("0");
   ui->txtRegalode->setText("0");
   ui->txt_dto_web->setText("0");
   ui->txtoferta_pvp_fijo->setText("0");
   ui->txtComentarios_promocion->setPlainText("");
   //ui->txtMargen->setValue(0);
   //ui->txtMargen_min->setValue(0);


}

void FrmArticulos::ChangeValues_TablaProveedores(int row, int column)
{
    if (column == 4)
        QMessageBox::information(this,tr("Se ha cambiado el valor"),tr("Valor cambiado"),tr("Aceptar"));
//    else if(column == 1 && !helped_table->item(row,0)->text().isEmpty())
//        comprobarCantidad(row);
//    else if(column == 5 && !helped_table->item(row,4)->text().isEmpty())
//        comprobarDescuento(row);
//    else if(column == 9)
//        comprobarStock(row);

    rellenar_grafica_proveedores();
}

void FrmArticulos::rellenar_grafica_proveedores()
{
    //----------------------
    // GRAFICA SEGÚN PVDREAL
    //----------------------
    QSqlQuery queryProveed("select cProveedor,pvdreal from proveedores_frecuentes where id_art = "+
                           QString::number(oArticulo->id),QSqlDatabase::database("Maya"));
    if(queryProveed.exec()){

        while (queryProveed.next()) {
            ui->graf_prov->addItem(queryProveed.record().value("cProveedor").toString().left(4),
                                   queryProveed.record().value("pvdreal").toFloat()/*,Qt::blue*/);


        }

        ui->graf_prov->repaint();
    }
    else
        qDebug()<< queryProveed.lastError();

}

void FrmArticulos::on_botEditar_clicked()
{
    bloquearCampos(false);
    ui->txtcCodigo->setFocus();
    if (ui->chkArticulo_promocionado->isChecked())
        ui->framePromocion->setEnabled(true);
    else
        ui->framePromocion->setEnabled(false);
}

void FrmArticulos::on_botBorrar_clicked()
{
    oArticulo->Borrar(oArticulo->id);
    oArticulo->Recuperar("Select * from articulos where id = "+QString::number(oArticulo->id));
    LLenarCampos();
}





void FrmArticulos::on_botDeshacer_clicked()
{
    if(this->anadir)
    {
        QSqlQuery qArt(QSqlDatabase::database("Maya"));
        qArt.prepare("delete from articulos where id = :nId");
        qArt.bindValue("nId",oArticulo->id);
        if(qArt.exec());
            VaciarCampos();
        this->anadir = false;
    } else
        {
        int nId = oArticulo->id;
        QString cSql = "Select * from articulos where Id =" +QString::number(oArticulo->id);
        oArticulo->Recuperar(cSql);
        oArticulo->id = nId;

        LLenarCampos();
    }
    bloquearCampos(true);
}





void FrmArticulos::on_botBuscarSeccion_clicked()
{
    Db_table_View form(this);
    form.set_db("Maya");
    form.set_table("secciones");

    form.setWindowTitle(tr("Secciones"));

    QStringList headers;
    headers << tr("Seccion");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.set_columnHide(2);

    form.set_selection("cSeccion");
    if(form.exec() == QDialog::Accepted)
    {
        ui->txtcSeccion->setText(form.selected_value);
        QSqlQuery qSeccion(QSqlDatabase::database("Maya"));
        qSeccion.prepare("select id from secciones where cSeccion = :seccion");
        qSeccion.bindValue(":seccion",form.selected_value);
        if(qSeccion.exec())
        {
            qSeccion.next();
            oArticulo->id_Seccion= qSeccion.value(0).toInt();
        }
        else
        {
            QMessageBox::warning(this,tr("Secciones"),tr("No se ha podido vincular la seccion: %1").arg(qSeccion.lastError().text()));
        }
    }
}



void FrmArticulos::AnadirSeccion()
{
    QLineEdit *txtcSeccionNueva = new QLineEdit(ventana);
    layout->addWidget(txtcSeccionNueva,2,1,2,1);
}

bool FrmArticulos::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::Show)
        actualizar();
    return false;
}


void FrmArticulos::on_botBuscarFamilia_clicked()
{
    Db_table_View form(this);
    form.set_db("Maya");
    form.set_table("familias");

    form.setWindowTitle(tr("Familias"));

    QStringList headers;
    headers << tr("Codigo") << tr("Familia") << tr("Pertenece a");
    form.set_table_headers(headers);

    form.set_relation(3,QSqlRelation("secciones","id","cSeccion"));

    form.set_columnHide(0);

    form.set_selection("cFamilia");
    QSqlQuery query(QSqlDatabase::database("Maya"));
    query.prepare(QString("SELECT id FROM secciones WHERE cFamilia = '%1' ").arg(ui->txtcFamilia->text()));
    if (query.exec())
        if(query.next())
            form.set_filter("Id_Seccion = "+query.record().value(0).toString());

    if(form.exec() == QDialog::Accepted)
    {
        ui->txtcFamilia->setText(form.selected_value);
    }
}

void FrmArticulos::on_botBuscarSubfamilia_clicked()
{
    Db_table_View form(this);
    form.set_db("Maya");
    form.set_table("subfamilias");

    form.setWindowTitle(tr("Subfamilias"));

    QStringList headers;
    headers << tr("SubFamilia") << tr("Pertenece a");
    form.set_table_headers(headers);

    form.set_relation(2,QSqlRelation("familias","id","cFamilia"));

    form.set_columnHide(0);

    form.set_selection("cSubfamilia");
    QSqlQuery query(QSqlDatabase::database("Maya"));
    query.prepare(QString("SELECT id FROM familias WHERE cFamilia = '%1'").arg(ui->txtcFamilia->text()));
    if (query.exec())
        if(query.next())
            form.set_filter("Id_Seccion = "+query.record().value(0).toString());

    if(form.exec() == QDialog::Accepted)
    {
        ui->txtcSubFamilia->setText(form.selected_value);
    }
}

void FrmArticulos::on_botBuscarSubSubFamilia_clicked()
{
    Db_table_View form(this);
    form.set_db("Maya");
    form.set_table("subsubfamilias");

    form.setWindowTitle(tr("Subsubfamilias"));

    QStringList headers;
    headers << tr("SubsubFamilia") << tr("Pertenece a");
    form.set_table_headers(headers);

    form.set_relation(2,QSqlRelation("subfamilias","id","cSubfamilia"));

    form.set_columnHide(0);

    form.set_selection("cSubsubfamilia");
    QSqlQuery query(QSqlDatabase::database("Maya"));
    query.prepare(QString("SELECT id FROM subfamilias WHERE cFamilia = '%1'").arg(ui->txtcFamilia->text()));
    if (query.exec())
        if(query.next())
            form.set_filter("idsubfamilia = "+query.record().value(0).toString());

    if(form.exec() == QDialog::Accepted)
    {
        ui->txtcSubFamilia->setText(form.selected_value);
    }
}

void FrmArticulos::on_btnBuscarProveedor_clicked()
{
    FrmBuscarProveedor buscar(this);
    if(buscar.exec()==QDialog::Accepted)
    {
        QSqlQuery qProv(QSqlDatabase::database("Maya"));
        qProv.prepare("Select * from proveedores where id =:nId");
        qProv.bindValue(":nId",buscar.nIdProv);
        if(qProv.exec()){
            qProv.next();
            ui->txtCodigoProveedor->setText(qProv.record().field("cCodigo").value().toString());
            ui->txtcProveedor->setText(qProv.record().field("cProveedor").value().toString());
            oArticulo->id_Proveedor = buscar.nIdProv;
        }
    }

}

void FrmArticulos::on_btnAnadirTarifa_clicked()
{
    FrmTarifas addTarifa(this);
    addTarifa.capturar_coste(ui->txtrCoste->text().toFloat());
    if(addTarifa.exec() ==QDialog::Accepted)
    {
        QSqlQuery qTarifa(QSqlDatabase::database("Maya"));
        qTarifa.prepare("INSERT INTO `MayaGeneral`.`tarifas` (`id_Articulo`, `id_pais`,"
                        "`id_monedas`, `margen`, `margenminimo`, `pvp`, `id_codigotarifa`) "
                        "VALUES (:id, :id_pais,:id_monedas,:margen,:margenminimo,:pvp,:id_codigotarifa);");
        qTarifa.bindValue(":id",oArticulo->id);
        qTarifa.bindValue(":id_pais",addTarifa.id_pais);
        qTarifa.bindValue(":id_monedas",addTarifa.id_moneda);
        qTarifa.bindValue(":margen",addTarifa.margen);
        qTarifa.bindValue(":margenminimo",addTarifa.margen_min);
        qTarifa.bindValue(":pvp",addTarifa.pvpDivisa);
        qTarifa.bindValue(":id_codigotarifa",addTarifa.codigoTarifa);
        if(qTarifa.exec()) {
            LlenarTablas();
        } else {
            QMessageBox::information(this,tr("Gestión de Productos/Servicios"),
                                      tr("Ocurrió un error al insertar una tarifa en el artículo: %1").arg(qTarifa.lastError().text()),
                                     tr("Acceptar"));
        }
    }
}


void FrmArticulos::btnEditarTarifa_clicked()
{
    QModelIndex celda=ui->TablaTarifas->currentIndex();
    QModelIndex index1=tarifa_model->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey = tarifa_model->data(index1,Qt::EditRole);

    FrmTarifas editTarifa(this);
    editTarifa.capturar_datos(pKey.toInt(),ui->txtrCoste->text());
    if(editTarifa.exec() ==QDialog::Accepted) {
        QSqlQuery queryTarifas(QSqlDatabase::database("Maya"));
        queryTarifas.prepare(
        "UPDATE tarifas SET "
        "margen = :margen,"
        "margenminimo = :margenminimo,"
        "pvp = :pvp "
        " WHERE id = :id");
        queryTarifas.bindValue(":margen",editTarifa.margen);
        queryTarifas.bindValue(":margenMinimo",editTarifa.margen_min);
        queryTarifas.bindValue(":pvp",editTarifa.pvpDivisa);
        queryTarifas.bindValue(":id",pKey);
        if(!queryTarifas.exec())
            QMessageBox::warning(this,tr("ATENCIÓN"),
                                 tr("Ocurrió un error al actualizar BD: %1").arg(queryTarifas.lastError().text()),
                                 tr("Aceptar"));

        LlenarTablas();
    }
}

void FrmArticulos::btnBorrarTarifa_clicked()
{
    QModelIndex celda=ui->TablaTarifas->currentIndex();
    QModelIndex index1=tarifa_model->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey = tarifa_model->data(index1,Qt::EditRole);
    if(QMessageBox::question(this,tr("Borrar Tarifa"),
                             tr("¿Desea realmente borrar esta tarifa para este artículo?"),
                             tr("No"),
                             tr("Borrar"))==QMessageBox::Accepted){
        QSqlQuery queryTarifa(QSqlDatabase::database("Maya"));
        if(!queryTarifa.exec("delete from tarifas where id ="+QString::number(pKey.toInt())))
            QMessageBox::warning(this,tr("Borrar"),
                                 tr("Ocurrió un error al borrar: %1").arg(queryTarifa.lastError().text()));
        LlenarTablas();

    }

}

void FrmArticulos::anadir_proveedor_clicked()
{
    FrmAsociarProveedor frmAsociar;
    frmAsociar.setAnadir();
    if(frmAsociar.exec() == QDialog::Accepted) {
        bool ok = oArticulo->agregar_proveedor_alternativo(oArticulo->id,frmAsociar.id_proveedor,frmAsociar.codigo,frmAsociar.pvd,
                                                 frmAsociar.DescOferta,frmAsociar.Oferta,frmAsociar.pvdreal,frmAsociar.id_divisa);
        if (!ok)
            QMessageBox::warning(this,tr("Nuevo proveedor frecuente"),tr("Falló la inserción del proveedor"),tr("Aceptar"));
        LLenarCampos();

    }

    //-----------------------
    // Proveedores frecuentes
    //-----------------------
    modelProv = new QSqlQueryModel(this);
    modelProv->setQuery("Select id,codpro,cProveedor,codigo,pvd,pvdreal,moneda,descoferta from proveedores_frecuentes where id_art = "+
                        QString::number(oArticulo->id),
                        QSqlDatabase::database("Maya"));

    ui->tablaProveedores->setModel(modelProv);
    ui->tablaProveedores->setColumnHidden(0,true);
}

void FrmArticulos::editar_proveedor_clicked()
{
    QModelIndex celda=ui->tablaProveedores->currentIndex();
    QModelIndex index1=modelProv->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=modelProv->data(index1,Qt::EditRole);

    FrmAsociarProveedor frmAsociar;
    frmAsociar.seteditar(pKey.toString());
    if(frmAsociar.exec() == QDialog::Accepted) {
        bool ok = oArticulo->guardarProveedorAlternativo(pKey.toInt(),frmAsociar.codigo,frmAsociar.pvd,
                                                 frmAsociar.DescOferta,frmAsociar.Oferta,frmAsociar.pvdreal,frmAsociar.id_divisa);
        if (!ok)
            QMessageBox::warning(this,tr("Guardar proveedor frecuente"),tr("Falló la edición del proveedor"),tr("Aceptar"));
        LLenarCampos();

    }
}

void FrmArticulos::borrar_proveedor_clicked()
{
    QModelIndex celda=ui->tablaProveedores->currentIndex();
    QModelIndex index1=modelProv->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=modelProv->data(index1,Qt::EditRole);


    QMessageBox mensaje(this);
    mensaje.setStyleSheet("background-color: rgb(221,221,221)");
    mensaje.setWindowTitle(tr("Borrar proveedor frecuente"));
    mensaje.setText(tr("¿Desea borrar la ficha del proveedor ?"));
    mensaje.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    mensaje.setButtonText(0,tr("No"));
    mensaje.setButtonText(1,tr("Sí"));

    mensaje.setIcon(QMessageBox::Question);

    if(mensaje.exec()==QMessageBox::Yes) {
        QSqlQuery queryProv(QSqlDatabase::database("Maya"));
        queryProv.prepare("delete from articulos_prov_frec where id =:id");
        queryProv.bindValue(":id",pKey.toString());
        if(!queryProv.exec())
            QMessageBox::warning(this,tr("Borrar proveedor frecuente"),
                                 tr("Se ha producido un error al borrar el proveedor frecuente: %1").arg(queryProv.lastError().text()),
                                 tr("Aceptar"));
        else

            LlenarTablas();
    }
}

void FrmArticulos::asignar_proveedor_principal_clicked()
{
    QModelIndex celda=ui->tablaProveedores->currentIndex();
    QModelIndex index1=modelProv->index(celda.row(),8);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=modelProv->data(index1,Qt::EditRole);


//    QMessageBox mensaje(this);
//    mensaje.setStyleSheet("background-color: rgb(221,221,221)");
//    mensaje.setWindowTitle(tr("Assignar proveedor"));
//    mensaje.setText(tr("¿Desea hacer de este proveedor el principal?"));
//    mensaje.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
//    mensaje.setButtonText(0,tr("No"));
//    mensaje.setButtonText(1,tr("Sí"));
    if (QMessageBox::question(this,tr("Re-asignar proveedor"),
                          tr("¿Desea cambiar el proveedor principal?"),tr("No"),tr("Sí")) == QMessageBox::Accepted){

        bool resultado = oArticulo->cambiarProveedorPrincipal(oArticulo->id,pKey.toInt());
        if(resultado)

            index1=modelProv->index(celda.row(),1);     ///< '0' es la posicion del registro que nos interesa
            pKey=modelProv->data(index1,Qt::EditRole);
            ui->txtCodigoProveedor->setText(pKey.toString());
            index1=modelProv->index(celda.row(),2);
            pKey=modelProv->data(index1,Qt::EditRole);
            ui->txtcProveedor->setText(pKey.toString());
    }

}
void FrmArticulos::calcular_codigo()
{
    if (Configuracion_global->Autocodigoart == true) {
        if(!ui->txtcSeccion->text().isEmpty() && (ui->txtcCodigo->text().isEmpty() || ui->txtcCodigo->text() =="autocodigo"))
                ui->txtcCodigo->setText(oArticulo->autocodigo());
        else {

            QMessageBox mensaje;
            mensaje.setWindowTitle(tr("Nuevo código"));
            mensaje.setText(tr("Para autogenerar un código de producto como mínimo debe haber insertado una sección"));
            mensaje.setAutoFillBackground(true);
            mensaje.warning(this,tr("Nuevo código"),
                            tr("Para autogenerar un código de producto como mínimo debe haber insertado una sección"),
                            tr("Aceptar"));

        }

    }

}

void FrmArticulos::trazabilidad2(int id)
{
    modelTrazabilidad2 = new QSqlQueryModel(this);
    modelTrazabilidad2->setQuery("select codigocuentacliente,cliente, documento_venta,numero_ticket,unidades_vendidas,"
                                 "fecha_venta from trazabilidad2 where id_trazabilidad1 ="+QString::number(id),
                                 QSqlDatabase::database("Maya"));
    ui->tablaVentas->setModel(modelTrazabilidad2);
    modelTrazabilidad2->setHeaderData(0,Qt::Horizontal,tr("C.CLIENTE"));
    modelTrazabilidad2->setHeaderData(1,Qt::Horizontal,tr("CLIENTE"));
    modelTrazabilidad2->setHeaderData(2,Qt::Horizontal,tr("DOCUMENTO"));
    modelTrazabilidad2->setHeaderData(3,Qt::Horizontal,tr("Ticket"));
    modelTrazabilidad2->setHeaderData(4,Qt::Horizontal,tr("UNID. VEND."));
    modelTrazabilidad2->setHeaderData(5,Qt::Horizontal,tr("FECHA VENTA"));
}

void FrmArticulos::TablaTrazabilidad_clicked(QModelIndex)
{
    trazabilidad2(1);
}

void FrmArticulos::graficar(QString Tipo)
{
    if(ui->radGrafica_unidades->isChecked())
        GraficaUnidades();
    else
        GraficaImportes();
}

void FrmArticulos::GraficaUnidades()
{

    if(ui->cboTipoGrafica->currentText()==tr("Grafica de Barras")) {
        ui->grafica->setTipo(OpenChart::BarraMultiple);
        ui->grafica->Clear();
        ui->grafica->addMulibarColor("Ventas",Qt::darkGreen);
        ui->grafica->addMulibarColor("Compras",Qt::darkBlue);
        QVector <float> enero;
        enero << ui->txtUnid_compras_enero->text().toInt()<<ui->txtUnid_ventas_enero->text().toInt();

        ui->grafica->addItem("Ene",enero);

        QVector <float> febrero;
        febrero <<ui->txtUnid_compras_febrero->text().toInt() <<ui->txtUnid_ventas_febrero->text().toInt();

        ui->grafica->addItem("Feb",febrero);

        QVector <float> marzo;
        marzo <<ui->txtUnid_compras_marzo->text().toInt() <<ui->txtUnid_ventas_marzo->text().toInt();

        ui->grafica->addItem("Mar",marzo);

        QVector <float> abril;
        abril <<ui->txtUnid_compras_abril->text().toInt() <<ui->txtUnid_ventas_abril->text().toInt();

        ui->grafica->addItem("Abr",abril);

        QVector <float> mayo;
        mayo <<ui->txtUnid_compras_mayo->text().toInt() <<ui->txtUnid_ventas_mayo->text().toInt();

        ui->grafica->addItem("May",mayo);

        QVector <float> junio;
        junio <<ui->txtUnid_compras_junio->text().toInt() <<ui->txtUnid_ventas_junio->text().toInt();

        ui->grafica->addItem("jun",junio);

        QVector <float> julio;
        julio <<ui->txtUnid_compras_julio->text().toInt() <<ui->txtUnid_ventas_julio->text().toInt();

        ui->grafica->addItem("Jul",julio);

        QVector <float> agosto;
        agosto <<ui->txtUnid_compras_agosto->text().toInt() <<ui->txtUnid_ventas_agosto->text().toInt();
        ui->grafica->addItem("Ago",agosto);

        QVector <float> septiembre;
        septiembre <<ui->txtUnid_compras_septiembre->text().toInt() <<ui->txtUnid_ventas_septiembre->text().toInt();
        ui->grafica->addItem("Sep",septiembre);

        QVector <float> octubre;
        octubre <<ui->txtUnid_compras_octubre->text().toInt()<<ui->txtUnid_ventas_octubre->text().toInt();
        ui->grafica->addItem("Oct",octubre);

        QVector <float> noviembre;
        noviembre <<ui->txtUnid_compras_noviembre->text().toInt() <<ui->txtUnid_ventas_noviembre->text().toInt();
        ui->grafica->addItem("Nov",noviembre);

        QVector <float> diciembre;
        diciembre <<ui->txtUnid_compras_diciembre->text().toInt() <<ui->txtUnid_ventas_diciembre->text().toInt();
        ui->grafica->addItem("Dic",diciembre);
        ui->grafica->repaint();
    } else{
        ui->grafica->Clear();
        ui->grafica->setTipo(OpenChart::Lineas);
        QStringList lista;
        lista << tr("Ene") << tr("Feb") << tr("Mar") <<tr("Abr") << tr("May") << tr("Jun") << tr("Jul") << tr("Ago")
              <<tr("Sep") <<tr("Oct") <<tr("Nov") << tr("Dic");
        ui->grafica->addLineaStops(lista);
        QVector<float> valoresLineaCompras;
        valoresLineaCompras << ui->txtUnid_compras_enero->text().toFloat() << ui->txtUnid_compras_febrero->text().toFloat()
                     <<ui->txtUnid_compras_marzo->text().toFloat() << ui->txtUnid_compras_abril->text().toFloat()
                     <<ui->txtUnid_compras_mayo->text().toFloat() << ui->txtUnid_compras_junio->text().toFloat()
                     <<ui->txtUnid_compras_julio->text().toFloat() << ui->txtUnid_compras_agosto->text().toFloat()
                     <<ui->txtUnid_compras_septiembre->text().toFloat() << ui->txtUnid_compras_octubre->text().toFloat()
                     <<ui->txtUnid_compras_noviembre->text().toFloat() << ui->txtUnid_compras_diciembre->text().toFloat();
        ui->grafica->addItem("Compras",valoresLineaCompras,Qt::red);

        QVector<float> valoresLineaVentas;
        valoresLineaVentas << ui->txtUnid_ventas_enero->text().toFloat() << ui->txtUnid_ventas_febrero->text().toFloat()
                     <<ui->txtUnid_ventas_marzo->text().toFloat() << ui->txtUnid_ventas_abril->text().toFloat()
                     <<ui->txtUnid_ventas_mayo->text().toFloat() << ui->txtUnid_ventas_junio->text().toFloat()
                     <<ui->txtUnid_ventas_julio->text().toFloat() << ui->txtUnid_ventas_agosto->text().toFloat()
                     <<ui->txtUnid_ventas_septiembre->text().toFloat() << ui->txtUnid_ventas_octubre->text().toFloat()
                     <<ui->txtUnid_ventas_noviembre->text().toFloat() << ui->txtUnid_compras_diciembre->text().toFloat();
        ui->grafica->addItem("Ventas",valoresLineaVentas,Qt::green);
           ui->grafica->repaint();
    }
}

void FrmArticulos::GraficaImportes()
{

    if(ui->cboTipoGrafica->currentText()==tr("Grafica de Barras")) {
        ui->grafica->Clear();
        ui->grafica->setTipo(OpenChart::BarraMultiple);
        ui->grafica->addMulibarColor("Compras",Qt::darkRed);
        ui->grafica->addMulibarColor("Ventas",Qt::darkGreen);
        QVector <float> enero;
        enero << ui->txtImporte_compras_enero->text().toFloat() <<ui->txtImporte_ventas_enero->text().toFloat();//Añadir tantos colores como elementos tenga el vector!

        ui->grafica->addItem("Ene",enero);


        QVector <float> febrero;
        febrero <<ui->txtImporte_compras_febrero->text().toFloat() <<ui->txtImporte_ventas_febrero->text().toFloat();//Añadir tantos colores como elementos tenga el vector!

        ui->grafica->addItem("Feb",febrero);

        QVector <float> marzo;
        marzo <<ui->txtImporte_compras_marzo->text().toFloat() << ui->txtImporte_ventas_marzo->text().toFloat();//Añadir tantos colores como elementos tenga el vector!

        ui->grafica->addItem("Mar",marzo);

        QVector <float> abril;
        abril <<ui->txtImporte_compras_abril->text().toFloat() << ui->txtImporte_ventas_abril->text().toFloat();//Añadir tantos colores como elementos tenga el vector!

        ui->grafica->addItem("Abr",abril);

        QVector <float> mayo;
        mayo <<ui->txtImporte_compras_mayo->text().toFloat() <<ui->txtImporte_ventas_mayo->text().toFloat();//Añadir tantos colores como elementos tenga el vector!

        ui->grafica->addItem("May",mayo);

        QVector <float> junio;
        junio <<ui->txtImporte_compras_junio->text().toFloat() <<ui->txtImporte_ventas_junio->text().toFloat();

        ui->grafica->addItem("jun",junio);

        QVector <float> julio;
        julio <<ui->txtImporte_compras_julio->text().toFloat() << ui->txtImporte_ventas_julio->text().toFloat();

        ui->grafica->addItem("Jul",julio);

        QVector <float> agosto;
        agosto <<ui->txtImporte_compras_agosto->text().toFloat() <<ui->txtImporte_ventas_agosto->text().toFloat();
        ui->grafica->addItem("Ago",agosto);

        QVector <float> septiembre;
        septiembre <<ui->txtImporte_compras_septiembre->text().toFloat() <<ui->txtImporte_ventas_septiembre->text().toFloat();
        ui->grafica->addItem("Sep",septiembre);

        QVector <float> octubre;
        octubre <<ui->txtImporte_compras_octubre->text().toFloat() << ui->txtImporte_ventas_octubre->text().toFloat();
        ui->grafica->addItem("Oct",octubre);

        QVector <float> noviembre;
        noviembre <<ui->txtImporte_compras_noviembre->text().toFloat() <<ui->txtImporte_ventas_noviembre->text().toFloat();
        ui->grafica->addItem("Nov",noviembre);

        QVector <float> diciembre;
        diciembre <<ui->txtImporte_compras_diciembre->text().toFloat() <<ui->txtImporte_ventas_diciembre->text().toFloat();
        ui->grafica->addItem("Dic",diciembre);

        ui->grafica->repaint();
    } else {
        ui->grafica->Clear();
        ui->grafica->setTipo(OpenChart::Lineas);
        QStringList lista;
        lista << tr("Ene") << tr("Feb") << tr("Mar") <<tr("Abr") << tr("May") << tr("Jun") << tr("Jul") << tr("Ago")
              <<tr("Sep") <<tr("Oct") <<tr("Nov") << tr("Dic");
        ui->grafica->addLineaStops(lista);
        QVector<float> valoresLineaCompras;
        valoresLineaCompras << ui->txtImporte_compras_enero->text().toFloat() << ui->txtImporte_compras_febrero->text().toFloat()
                     <<ui->txtImporte_compras_marzo->text().toFloat() << ui->txtImporte_compras_abril->text().toFloat()
                     <<ui->txtImporte_compras_mayo->text().toFloat() << ui->txtImporte_compras_junio->text().toFloat()
                     <<ui->txtImporte_compras_julio->text().toFloat() << ui->txtImporte_compras_agosto->text().toFloat()
                     <<ui->txtImporte_compras_septiembre->text().toFloat() << ui->txtImporte_compras_octubre->text().toFloat()
                     <<ui->txtImporte_compras_noviembre->text().toFloat() << ui->txtImporte_compras_diciembre->text().toFloat();
        ui->grafica->addItem("Compras",valoresLineaCompras,Qt::red);

        QVector<float> valoresLineaVentas;
        valoresLineaVentas << ui->txtImporte_ventas_enero->text().toFloat() << ui->txtImporte_ventas_febrero->text().toFloat()
                     <<ui->txtImporte_ventas_marzo->text().toFloat() << ui->txtImporte_ventas_abril->text().toFloat()
                     <<ui->txtImporte_ventas_mayo->text().toFloat() << ui->txtImporte_ventas_junio->text().toFloat()
                     <<ui->txtImporte_ventas_julio->text().toFloat() << ui->txtImporte_ventas_agosto->text().toFloat()
                     <<ui->txtImporte_ventas_septiembre->text().toFloat() << ui->txtImporte_ventas_octubre->text().toFloat()
                     <<ui->txtImporte_ventas_noviembre->text().toFloat() << ui->txtImporte_compras_diciembre->text().toFloat();
        ui->grafica->addItem("Ventas",valoresLineaVentas,Qt::green);
           ui->grafica->repaint();
    }

}

void FrmArticulos::GraficaUnidades_comparativa()
{
    if(ui->cboTipoGrafica_2->currentText()==tr("Grafica de Barras")) {
        ui->Grafica_comparativa->setTipo(OpenChart::BarraMultiple);
        ui->Grafica_comparativa->Clear();
        ui->Grafica_comparativa->addMulibarColor("Empresa Activa",Qt::darkGreen);
        ui->Grafica_comparativa->addMulibarColor(ui->cboEmpresa2->currentText(),Qt::darkBlue);
        QVector <float> enero;
        enero << ui->txtUnid_ventas_enero_EA->text().toInt()<<ui->txtUnid_ventas_enero_3->text().toInt();

        ui->Grafica_comparativa->addItem("Ene",enero);

        QVector <float> febrero;
        febrero <<ui->txtUnid_ventas_febrero_EA->text().toInt() <<ui->txtUnid_ventas_febrero_3->text().toInt();

        ui->Grafica_comparativa->addItem("Feb",febrero);

        QVector <float> marzo;
        marzo <<ui->txtUnid_ventas_marzo_EA->text().toInt() <<ui->txtUnid_ventas_marzo_3->text().toInt();

        ui->Grafica_comparativa->addItem("Mar",marzo);

        QVector <float> abril;
        abril <<ui->txtUnid_ventas_abril_EA->text().toInt() <<ui->txtUnid_ventas_abril_3->text().toInt();

        ui->Grafica_comparativa->addItem("Abr",abril);

        QVector <float> mayo;
        mayo <<ui->txtUnid_ventas_mayo_EA->text().toInt() <<ui->txtUnid_ventas_mayo_3->text().toInt();

        ui->Grafica_comparativa->addItem("May",mayo);

        QVector <float> junio;
        junio <<ui->txtUnid_ventas_junio_EA->text().toInt() <<ui->txtUnid_ventas_junio_3->text().toInt();

        ui->Grafica_comparativa->addItem("jun",junio);

        QVector <float> julio;
        julio <<ui->txtUnid_ventas_julio_EA->text().toInt() <<ui->txtUnid_ventas_julio_3->text().toInt();

        ui->Grafica_comparativa->addItem("Jul",julio);

        QVector <float> agosto;
        agosto <<ui->txtUnid_ventas_agosto_EA->text().toInt() <<ui->txtUnid_ventas_agosto_3->text().toInt();
        ui->Grafica_comparativa->addItem("Ago",agosto);

        QVector <float> septiembre;
        septiembre <<ui->txtUnid_ventas_septiembre_EA->text().toInt() <<ui->txtUnid_ventas_septiembre_3->text().toInt();
        ui->Grafica_comparativa->addItem("Sep",septiembre);

        QVector <float> octubre;
        octubre <<ui->txtUnid_ventas_octubre_EA->text().toInt()<<ui->txtUnid_ventas_octubre_3->text().toInt();
        ui->Grafica_comparativa->addItem("Oct",octubre);

        QVector <float> noviembre;
        noviembre <<ui->txtUnid_ventas_noviembre_EA->text().toInt() <<ui->txtUnid_ventas_noviembre_3->text().toInt();
        ui->Grafica_comparativa->addItem("Nov",noviembre);

        QVector <float> diciembre;
        diciembre <<ui->txtUnid_ventas_diciembre_EA->text().toInt() <<ui->txtUnid_ventas_diciembre_3->text().toInt();
        ui->Grafica_comparativa->addItem("Dic",diciembre);
        ui->Grafica_comparativa->repaint();
    } else{
        ui->Grafica_comparativa->Clear();
        ui->Grafica_comparativa->setTipo(OpenChart::Lineas);
        QStringList lista;
        lista << "12" << "34" ;
        ui->Grafica_comparativa->addLineaStops(lista);
        QStringList lista1;
        lista1 << "20" << "60";
        ui->Grafica_comparativa->addLineaStops(lista1);

        ui->Grafica_comparativa->repaint();
    }
}

void FrmArticulos::GraficaImportes_comparativa()
{
    ui->Grafica_comparativa->Clear();
    ui->Grafica_comparativa->addMulibarColor("EmpresaActiva",Qt::darkBlue);
    ui->Grafica_comparativa->addMulibarColor(ui->cboEmpresa2->currentText(),Qt::darkGreen);
    QVector <float> enero;
    enero << ui->txtImporte_ventas_enero_EA->text().toFloat() <<ui->txtImporte_ventas_enero_3->text().toFloat();//Añadir tantos colores como elementos tenga el vector!

    ui->Grafica_comparativa->addItem("Ene",enero);


    QVector <float> febrero;
    febrero <<ui->txtImporte_ventas_febrero_EA->text().toFloat() <<ui->txtImporte_ventas_febrero_3->text().toFloat();//Añadir tantos colores como elementos tenga el vector!

    ui->Grafica_comparativa->addItem("Feb",febrero);

    QVector <float> marzo;
    marzo <<ui->txtImporte_ventas_marzo_EA->text().toFloat() << ui->txtImporte_ventas_marzo_3->text().toFloat();//Añadir tantos colores como elementos tenga el vector!

    ui->Grafica_comparativa->addItem("Mar",marzo);

    QVector <float> abril;
    abril <<ui->txtImporte_ventas_abril_EA->text().toFloat() << ui->txtImporte_ventas_abril_3->text().toFloat();//Añadir tantos colores como elementos tenga el vector!

    ui->Grafica_comparativa->addItem("Abr",abril);

    QVector <float> mayo;
    mayo <<ui->txtImporte_ventas_mayo_EA->text().toFloat() <<ui->txtImporte_ventas_mayo_3->text().toFloat();//Añadir tantos colores como elementos tenga el vector!

    ui->Grafica_comparativa->addItem("May",mayo);

    QVector <float> junio;
    junio <<ui->txtImporte_ventas_junio_EA->text().toFloat() <<ui->txtImporte_ventas_junio_3->text().toFloat();

    ui->Grafica_comparativa->addItem("jun",junio);

    QVector <float> julio;
    julio <<ui->txtImporte_ventas_julio_EA->text().toFloat() << ui->txtImporte_ventas_julio_3->text().toFloat();

    ui->Grafica_comparativa->addItem("Jul",julio);

    QVector <float> agosto;
    agosto <<ui->txtImporte_ventas_agosto_EA->text().toFloat() <<ui->txtImporte_ventas_agosto_3->text().toFloat();
    ui->Grafica_comparativa->addItem("Ago",agosto);

    QVector <float> septiembre;
    septiembre <<ui->txtImporte_ventas_septiembre_EA->text().toFloat() <<ui->txtImporte_ventas_septiembre_3->text().toFloat();
    ui->Grafica_comparativa->addItem("Sep",septiembre);

    QVector <float> octubre;
    octubre <<ui->txtImporte_ventas_octubre_EA->text().toFloat() << ui->txtImporte_ventas_octubre_3->text().toFloat();
    ui->Grafica_comparativa->addItem("Oct",octubre);

    QVector <float> noviembre;
    noviembre <<ui->txtImporte_ventas_noviembre_EA->text().toFloat() <<ui->txtImporte_ventas_noviembre_3->text().toFloat();
    ui->Grafica_comparativa->addItem("Nov",noviembre);

    QVector <float> diciembre;
    diciembre <<ui->txtImporte_ventas_diciembre_EA->text().toFloat() <<ui->txtImporte_ventas_diciembre_3->text().toFloat();
    ui->Grafica_comparativa->addItem("Dic",diciembre);
}

void FrmArticulos::MostrarGrafica_comparativa(bool grafica_unidades)
{
    if (!grafica_unidades == true)
        GraficaUnidades_comparativa();
    else
        GraficaImportes_comparativa();
}

void FrmArticulos::LLenarGraficas()
{
    QSqlQuery queryAcumulados(QSqlDatabase::database("empresa"));
    if(!queryAcumulados.exec("select * from acum_articulos where id_Articulo=" +QString::number(oArticulo->id))){
        QMessageBox::warning(this,tr("Acumulados ejercicio"),
                             tr("Ocurrió un error al recuperar ficha de acumulados: %1").arg(queryAcumulados.lastError().text()),
                             tr("Aceptar"));
    } else {
        queryAcumulados.next();
        //--------------------
        // Undidades compradas
        //--------------------
        ui->txtUnid_compras_enero->setText(queryAcumulados.record().value("unid_comp_enero").toString());
        ui->txtUnid_compras_febrero->setText(queryAcumulados.record().value("unid_comp_febrero").toString());
        ui->txtUnid_compras_marzo->setText(queryAcumulados.record().value("unid_comp_marzo").toString());
        ui->txtUnid_compras_abril->setText(queryAcumulados.record().value("unid_comp_abril").toString());
        ui->txtUnid_compras_mayo->setText(queryAcumulados.record().value("unid_comp_mayo").toString());
        ui->txtUnid_compras_junio->setText(queryAcumulados.record().value("unid_comp_junio").toString());
        ui->txtUnid_compras_julio->setText(queryAcumulados.record().value("unid_comp_julio").toString());
        ui->txtUnid_compras_agosto->setText(queryAcumulados.record().value("unid_comp_agosto").toString());
        ui->txtUnid_compras_septiembre->setText(queryAcumulados.record().value("unid_comp_septiembre").toString());
        ui->txtUnid_compras_octubre->setText(queryAcumulados.record().value("unid_comp_octubre").toString());
        ui->txtUnid_compras_noviembre->setText(queryAcumulados.record().value("unid_comp_noviembre").toString());
        ui->txtUnid_compras_diciembre->setText(queryAcumulados.record().value("unid_comp_diciembre").toString());

        //--------------------
        // Undidades vendidas
        //--------------------
        ui->txtUnid_ventas_enero->setText(queryAcumulados.record().value("unid_vent_enero").toString());
        ui->txtUnid_ventas_febrero->setText(queryAcumulados.record().value("unid_vent_febrero").toString());
        ui->txtUnid_ventas_marzo->setText(queryAcumulados.record().value("unid_vent_marzo").toString());
        ui->txtUnid_ventas_abril->setText(queryAcumulados.record().value("unid_vent_abril").toString());
        ui->txtUnid_ventas_mayo->setText(queryAcumulados.record().value("unid_vent_mayo").toString());
        ui->txtUnid_ventas_junio->setText(queryAcumulados.record().value("unid_vent_junio").toString());
        ui->txtUnid_ventas_julio->setText(queryAcumulados.record().value("unid_vent_julio").toString());
        ui->txtUnid_ventas_agosto->setText(queryAcumulados.record().value("unid_vent_agosto").toString());
        ui->txtUnid_ventas_septiembre->setText(queryAcumulados.record().value("unid_vent_septiembre").toString());
        ui->txtUnid_ventas_octubre->setText(queryAcumulados.record().value("unid_vent_octubre").toString());
        ui->txtUnid_ventas_noviembre->setText(queryAcumulados.record().value("unid_vent_noviembre").toString());
        ui->txtUnid_ventas_diciembre->setText(queryAcumulados.record().value("unid_vent_diciembre").toString());

        //--------------------
        // Importe compras
        //--------------------
        ui->txtImporte_compras_enero->setText(queryAcumulados.record().value("acum_comp_enero").toString());
        ui->txtImporte_compras_febrero->setText(queryAcumulados.record().value("acum_comp_febrero").toString());
        ui->txtImporte_compras_marzo->setText(queryAcumulados.record().value("acum_comp_marzo").toString());
        ui->txtImporte_compras_abril->setText(queryAcumulados.record().value("acum_comp_abril").toString());
        ui->txtImporte_compras_mayo->setText(queryAcumulados.record().value("acum_comp_mayo").toString());
        ui->txtImporte_compras_junio->setText(queryAcumulados.record().value("acum_comp_junio").toString());
        ui->txtImporte_compras_julio->setText(queryAcumulados.record().value("acum_comp_julio").toString());
        ui->txtImporte_compras_agosto->setText(queryAcumulados.record().value("acum_comp_agosto").toString());
        ui->txtImporte_compras_septiembre->setText(queryAcumulados.record().value("acum_comp_septiembre").toString());
        ui->txtImporte_compras_octubre->setText(queryAcumulados.record().value("acum_comp_octubre").toString());
        ui->txtImporte_compras_noviembre->setText(queryAcumulados.record().value("acum_comp_noviembre").toString());
        ui->txtImporte_compras_diciembre->setText(queryAcumulados.record().value("acum_comp_diciembre").toString());

        //--------------------
        // Importe Ventas
        //--------------------
        ui->txtImporte_ventas_enero->setText(queryAcumulados.record().value("acum_vent_enero").toString());
        ui->txtImporte_ventas_febrero->setText(queryAcumulados.record().value("acum_vent_febrero").toString());
        ui->txtImporte_ventas_marzo->setText(queryAcumulados.record().value("acum_vent_marzo").toString());
        ui->txtImporte_ventas_abril->setText(queryAcumulados.record().value("acum_vent_abril").toString());
        ui->txtImporte_ventas_mayo->setText(queryAcumulados.record().value("acum_vent_mayo").toString());
        ui->txtImporte_ventas_junio->setText(queryAcumulados.record().value("acum_vent_junio").toString());
        ui->txtImporte_ventas_julio->setText(queryAcumulados.record().value("acum_vent_julio").toString());
        ui->txtImporte_ventas_agosto->setText(queryAcumulados.record().value("acum_vent_agosto").toString());
        ui->txtImporte_ventas_septiembre->setText(queryAcumulados.record().value("acum_vent_septiembre").toString());
        ui->txtImporte_ventas_octubre->setText(queryAcumulados.record().value("acum_vent_octubre").toString());
        ui->txtImporte_ventas_noviembre->setText(queryAcumulados.record().value("acum_vent_noviembre").toString());
        ui->txtImporte_ventas_diciembre->setText(queryAcumulados.record().value("acum_vent_diciembre").toString());

        //-----------------------------------------
        // Undidades vendidas (Grafica comparativa)
        //-----------------------------------------
        ui->txtUnid_ventas_enero_EA->setText(queryAcumulados.record().value("unid_vent_enero").toString());
        ui->txtUnid_ventas_febrero_EA->setText(queryAcumulados.record().value("unid_vent_febrero").toString());
        ui->txtUnid_ventas_marzo_EA->setText(queryAcumulados.record().value("unid_vent_marzo").toString());
        ui->txtUnid_ventas_abril_EA->setText(queryAcumulados.record().value("unid_vent_abril").toString());
        ui->txtUnid_ventas_mayo_EA->setText(queryAcumulados.record().value("unid_vent_mayo").toString());
        ui->txtUnid_ventas_junio_EA->setText(queryAcumulados.record().value("unid_vent_junio").toString());
        ui->txtUnid_ventas_julio_EA->setText(queryAcumulados.record().value("unid_vent_julio").toString());
        ui->txtUnid_ventas_agosto_EA->setText(queryAcumulados.record().value("unid_vent_agosto").toString());
        ui->txtUnid_ventas_septiembre_EA->setText(queryAcumulados.record().value("unid_vent_septiembre").toString());
        ui->txtUnid_ventas_octubre_EA->setText(queryAcumulados.record().value("unid_vent_octubre").toString());
        ui->txtUnid_ventas_noviembre_EA->setText(queryAcumulados.record().value("unid_vent_noviembre").toString());
        ui->txtUnid_ventas_diciembre_EA->setText(queryAcumulados.record().value("unid_vent_diciembre").toString());

        //--------------------
        // Importe Ventas
        //--------------------
        ui->txtImporte_ventas_enero_EA->setText(queryAcumulados.record().value("acum_vent_enero").toString());
        ui->txtImporte_ventas_febrero_EA->setText(queryAcumulados.record().value("acum_vent_febrero").toString());
        ui->txtImporte_ventas_marzo_EA->setText(queryAcumulados.record().value("acum_vent_marzo").toString());
        ui->txtImporte_ventas_abril_EA->setText(queryAcumulados.record().value("acum_vent_abril").toString());
        ui->txtImporte_ventas_mayo_EA->setText(queryAcumulados.record().value("acum_vent_mayo").toString());
        ui->txtImporte_ventas_junio_EA->setText(queryAcumulados.record().value("acum_vent_junio").toString());
        ui->txtImporte_ventas_julio_EA->setText(queryAcumulados.record().value("acum_vent_julio").toString());
        ui->txtImporte_ventas_agosto_EA->setText(queryAcumulados.record().value("acum_vent_agosto").toString());
        ui->txtImporte_ventas_septiembre_EA->setText(queryAcumulados.record().value("acum_vent_septiembre").toString());
        ui->txtImporte_ventas_octubre_EA->setText(queryAcumulados.record().value("acum_vent_octubre").toString());
        ui->txtImporte_ventas_noviembre_EA->setText(queryAcumulados.record().value("acum_vent_noviembre").toString());
        ui->txtImporte_ventas_diciembre_EA->setText(queryAcumulados.record().value("acum_vent_diciembre").toString());

    }
    if(ui->Pestanas->currentIndex()==5){
        if(ui->radGrafica_importes->isChecked())
            GraficaImportes();
        else
            GraficaUnidades();
    }
    if(ui->Pestanas->currentIndex()==6)
        LLenarGrafica_comparativa(1);
}

void FrmArticulos::LLenarGrafica_comparativa(int index)
{
    // Conecto a la BD de la otra empresa
    QSqlQuery queryEmpresa_2(QSqlDatabase::database("Maya"));
    queryEmpresa_2.prepare("Select * from vistaEmpresa where empresa =:empresa");
    queryEmpresa_2.bindValue(":empresa",ui->cboEmpresa2->currentText().trimmed());
    if (queryEmpresa_2.exec()){
        if(queryEmpresa_2.next()){
            QSqlQuery queryEmpresa(QSqlDatabase::database("Maya"));
            queryEmpresa.exec("select * from empresas where id="+ queryEmpresa_2.record().value("id").toString());
            queryEmpresa.next();
            QString cDriver;
            cDriver =queryEmpresa.record().value("driverBD").toString();
            QSqlDatabase dbEmpresa2  = QSqlDatabase::addDatabase(cDriver,"empresa2");

            if (queryEmpresa.record().value("driverBD").toString() == "QSQLITE")
                {
                dbEmpresa2.setDatabaseName(queryEmpresa.record().value("RutaBDSqLite").toString());
                    dbEmpresa2.open();
            } else  {

                dbEmpresa2.setDatabaseName(queryEmpresa.record().value("nombreBD").toString());
                dbEmpresa2.setHostName(queryEmpresa.record().value("host").toString());
                dbEmpresa2.open(queryEmpresa.record().value("user").toString(),
                                    queryEmpresa.record().value("contrasena").toString());
            }
            if (dbEmpresa2.lastError().isValid()) {
                QMessageBox::critical(0, "error:", dbEmpresa2.lastError().text());
            }
            if(dbEmpresa2.isOpen()) {
                QSqlQuery acum2(QSqlDatabase::database("empresa2"));
                if(acum2.exec("select * from acum_articulos where id_Articulo ="+QString::number(oArticulo->id))){
                    acum2.next();
                    //-----------------------------------------
                    // Undidades vendidas (Grafica comparativa)
                    //-----------------------------------------
                    ui->txtUnid_ventas_enero_3->setText(acum2.record().value("unid_vent_enero").toString());
                    ui->txtUnid_ventas_febrero_3->setText(acum2.record().value("unid_vent_febrero").toString());
                    ui->txtUnid_ventas_marzo_3->setText(acum2.record().value("unid_vent_marzo").toString());
                    ui->txtUnid_ventas_abril_3->setText(acum2.record().value("unid_vent_abril").toString());
                    ui->txtUnid_ventas_mayo_3->setText(acum2.record().value("unid_vent_mayo").toString());
                    ui->txtUnid_ventas_junio_3->setText(acum2.record().value("unid_vent_junio").toString());
                    ui->txtUnid_ventas_julio_3->setText(acum2.record().value("unid_vent_julio").toString());
                    ui->txtUnid_ventas_agosto_3->setText(acum2.record().value("unid_vent_agosto").toString());
                    ui->txtUnid_ventas_septiembre_3->setText(acum2.record().value("unid_vent_septiembre").toString());
                    ui->txtUnid_ventas_octubre_3->setText(acum2.record().value("unid_vent_octubre").toString());
                    ui->txtUnid_ventas_noviembre_3->setText(acum2.record().value("unid_vent_noviembre").toString());
                    ui->txtUnid_ventas_diciembre_3->setText(acum2.record().value("unid_vent_diciembre").toString());

                    //--------------------
                    // Importe Ventas
                    //--------------------
                    ui->txtImporte_ventas_enero_3->setText(acum2.record().value("acum_vent_enero").toString());
                    ui->txtImporte_ventas_febrero_3->setText(acum2.record().value("acum_vent_febrero").toString());
                    ui->txtImporte_ventas_marzo_3->setText(acum2.record().value("acum_vent_marzo").toString());
                    ui->txtImporte_ventas_abril_3->setText(acum2.record().value("acum_vent_abril").toString());
                    ui->txtImporte_ventas_mayo_3->setText(acum2.record().value("acum_vent_mayo").toString());
                    ui->txtImporte_ventas_junio_3->setText(acum2.record().value("acum_vent_junio").toString());
                    ui->txtImporte_ventas_julio_3->setText(acum2.record().value("acum_vent_julio").toString());
                    ui->txtImporte_ventas_agosto_3->setText(acum2.record().value("acum_vent_agosto").toString());
                    ui->txtImporte_ventas_septiembre_3->setText(acum2.record().value("acum_vent_septiembre").toString());
                    ui->txtImporte_ventas_octubre_3->setText(acum2.record().value("acum_vent_octubre").toString());
                    ui->txtImporte_ventas_noviembre_3->setText(acum2.record().value("acum_vent_noviembre").toString());
                    ui->txtImporte_ventas_diciembre_3->setText(acum2.record().value("acum_vent_diciembre").toString());
                    if(ui->radGrafica_importes_2->isChecked())
                        GraficaImportes_comparativa();
                    else
                        GraficaUnidades_comparativa();

                }
            } else {
                QMessageBox::warning(this,tr("ERROR:"),
                                     tr("No se encontró el registro de la empresa"),
                                     tr("Aceptar"));
            }
            dbEmpresa2.close();
        }
    }
}



void FrmArticulos::LlenarTablas()
{
    //---------------------
    // Tarifas
    //---------------------
    QSqlQueryModel *modelTarifa = new QSqlQueryModel(this);
    modelTarifa->setQuery("select id,codigo_tarifa,descripcion,pais,moneda,margen, margenminimo, pvp,simbolo "
                         "from viewTarifa where id_Articulo = "+QString::number(oArticulo->id),
                         QSqlDatabase::database("Maya"));
    ui->TablaTarifas->setModel(modelTarifa);
    MonetaryDelegate *Delegado = new MonetaryDelegate(this);
    ui->TablaTarifas->setItemDelegateForColumn(7,Delegado);
    modelTarifa->setHeaderData(0,Qt::Horizontal,tr("ID"));
    modelTarifa->setHeaderData(1,Qt::Horizontal,tr("CODIGO"));
    modelTarifa->setHeaderData(2,Qt::Horizontal,tr("DESCRIPCIÓN"));
    modelTarifa->setHeaderData(3,Qt::Horizontal,tr("PAIS"));
    modelTarifa->setHeaderData(4,Qt::Horizontal,tr("DIVISA"));
    modelTarifa->setHeaderData(5,Qt::Horizontal,tr("%MARG."));
    modelTarifa->setHeaderData(6,Qt::Horizontal,tr("%M. MÍN."));
    modelTarifa->setHeaderData(7,Qt::Horizontal,tr("PVP"));
    modelTarifa->setHeaderData(8,Qt::Horizontal,tr("S"));
    ui->TablaTarifas->setItemDelegateForColumn(5, new MonetaryDelegate);
    ui->TablaTarifas->setItemDelegateForColumn(6, new MonetaryDelegate);
    ui->TablaTarifas->setItemDelegateForColumn(7, new MonetaryDelegate);

    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(0,0);

    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(1,105);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(2,165);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(3,65);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(4,70);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(5,70);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(6,70);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(7,85);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(8,20);
    //-----------------------
    // Proveedores frecuentes
    //-----------------------

    modelProv->setQuery("Select id,codpro,cProveedor,codigo,pvd,pvdreal,moneda,descoferta,id_prov from proveedores_frecuentes where id_art = "+QString::number(oArticulo->id),
                        QSqlDatabase::database("Maya"));

    ui->tablaProveedores->setItemDelegateForColumn(5,new MonetaryDelegate);
    ui->tablaProveedores->setItemDelegateForColumn(4,new MonetaryDelegate);
    ui->tablaProveedores->setModel(modelProv);

    modelProv->setHeaderData(1,Qt::Horizontal,tr("COD.PROV."));
    modelProv->setHeaderData(2,Qt::Horizontal,tr("PROVEEDOR"));
    modelProv->setHeaderData(3,Qt::Horizontal,tr("COD.ART.PRO."));
    modelProv->setHeaderData(4,Qt::Horizontal,tr("COSTE"));
    modelProv->setHeaderData(5,Qt::Horizontal,tr("C.REAL"));
    modelProv->setHeaderData(6,Qt::Horizontal,tr("DIVISA"));
    modelProv->setHeaderData(7,Qt::Horizontal,tr("D.OFERTA"));
    ui->tablaProveedores->setColumnHidden(0,true);
    ui->tablaProveedores->setColumnHidden(8,true);

    ui->tablaProveedores->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->tablaProveedores->horizontalHeader()->resizeSection(1,90);
    ui->tablaProveedores->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->tablaProveedores->horizontalHeader()->resizeSection(2,130);
    ui->tablaProveedores->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    ui->tablaProveedores->horizontalHeader()->resizeSection(3,95);
    ui->tablaProveedores->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Fixed);
    ui->tablaProveedores->horizontalHeader()->resizeSection(4,80);
    ui->tablaProveedores->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Fixed);
    ui->tablaProveedores->horizontalHeader()->resizeSection(5,80);
    ui->tablaProveedores->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Fixed);
    ui->tablaProveedores->horizontalHeader()->resizeSection(6,90);
    ui->tablaProveedores->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Fixed);
    ui->tablaProveedores->horizontalHeader()->resizeSection(7,87);

    //--------------------
    // Grafica proveedores
    //--------------------

    ui->graf_prov->Clear();
    ////uigraf_prov->verValoresEjeY(false);
    rellenar_grafica_proveedores();

    // ------------------
    // TABLA TRAZABILIDAD
    // ------------------
    modelTrazabilidad1 = new QSqlQueryModel(this);
    modelTrazabilidad1->setQuery( "select * from viewTrazabilidad1 where id_Articulo = "+QString::number(oArticulo->id),
                                  QSqlDatabase::database("Maya"));
    ui->tablaLotes->setModel(modelTrazabilidad1);
    ui->tablaLotes->setColumnHidden(0,true);
    ui->tablaLotes->setColumnHidden(6,true);
     modelTrazabilidad1->setHeaderData(1,Qt::Horizontal,tr("LOTE"));
     modelTrazabilidad1->setHeaderData(2,Qt::Horizontal,tr("CANTIDAD"));
     modelTrazabilidad1->setHeaderData(3,Qt::Horizontal,tr("STOCK LOTE"));
     modelTrazabilidad1->setHeaderData(4,Qt::Horizontal,tr("STOCK REAL"));
     modelTrazabilidad1->setHeaderData(5,Qt::Horizontal,tr("FAC.COMPRA"));
     modelTrazabilidad1->setHeaderData(6,Qt::Horizontal,tr("ID"));
     modelTrazabilidad1->setHeaderData(7,Qt::Horizontal,tr("FEC.COMPRA"));
     modelTrazabilidad1->setHeaderData(8,Qt::Horizontal,tr("CADUCIDAD"));
     modelTrazabilidad1->setHeaderData(9,Qt::Horizontal,tr("PROVEEDOR"));
     modelTrazabilidad1->setHeaderData(10,Qt::Horizontal,tr("DESCRIPCIÓN"));
     modelTrazabilidad1->setHeaderData(11,Qt::Horizontal,tr("CÓDIGO"));


}

void FrmArticulos::SeleccionarPestana(int pestana)
{
    if(pestana ==5)
        GraficaUnidades();
}

void FrmArticulos::listados()
{
    frmListadosArticulo *listados = new frmListadosArticulo(this);
    listados->exec();

}

void FrmArticulos::on_checkBox_toggled(bool checked)
{
    ui->checkBox->setEnabled(false);
    if(checked)
    {
        ui->cajaValores->show();
        QPropertyAnimation* animation = new QPropertyAnimation(ui->cajaValores, "pos",this);

        animation->setDuration(1000);
        animation->setEasingCurve(QEasingCurve::OutElastic);
        QPoint pos = ui->cajaValores->pos();
        animation->setStartValue(QPoint(pos.x()+ui->cajaValores->width(),pos.y()));
        animation->setEndValue(pos);

        QPropertyAnimation* animation0 = new QPropertyAnimation(ui->cajaValores, "size",this);
        //connect(animation,SIGNAL(finished()),this,SLOT(ani_end()));
        animation0->setDuration(1000);
        animation0->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz0 = ui->cajaValores->size();
        animation0->setStartValue(QSize(0,siz0.height()));
        animation0->setEndValue(siz0);

        QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
        //this->setGraphicsEffect(fade_effect);
        ui->cajaValores->setGraphicsEffect(fade_effect);
        QPropertyAnimation *animationOp = new QPropertyAnimation(fade_effect, "opacity");
        animationOp->setEasingCurve(QEasingCurve::Linear);
        animationOp->setDuration(1000);
        animationOp->setStartValue(0.0);
        animationOp->setEndValue(1.0);

        QPropertyAnimation* animation1 = new QPropertyAnimation(ui->frame_9, "size",this);
        //connect(animation1,SIGNAL(finished()),this,SLOT(ani_end()));
        animation1->setDuration(1000);
        animation1->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz = ui->frame_9->size();
        animation1->setStartValue(QSize(siz.width()+siz0.width(),siz.height()));
        animation1->setEndValue(siz);

        QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
        group->addAnimation(animation);
        group->addAnimation(animation0);
        group->addAnimation(animation1);
        group->addAnimation(animationOp);

        connect(group,SIGNAL(finished()),group,SLOT(deleteLater()));
         connect(group,SIGNAL(finished()),this,SLOT(toggleChecbox()));
        group->start();
    }
    else
    {
        QPropertyAnimation* animation = new QPropertyAnimation(ui->cajaValores, "pos",this);
        //connect(animation,SIGNAL(finished()),this,SLOT(ani_end()));
        animation->setDuration(1300);
        animation->setEasingCurve(QEasingCurve::OutElastic);
        QPoint pos = ui->cajaValores->pos();
        animation->setStartValue(pos);
        animation->setEndValue(QPoint(pos.x()+ui->cajaValores->width(),pos.y()));

        QPropertyAnimation* animation0 = new QPropertyAnimation(ui->cajaValores, "size",this);
        //connect(animation,SIGNAL(finished()),this,SLOT(ani_end()));
        animation0->setDuration(1300);
        animation0->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz0 = ui->cajaValores->size();
        animation0->setStartValue(siz0);
        animation0->setEndValue(QSize(0,siz0.height()));

        QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
        //this->setGraphicsEffect(fade_effect);
        ui->cajaValores->setGraphicsEffect(fade_effect);
        QPropertyAnimation *animationOp = new QPropertyAnimation(fade_effect, "opacity");
        animationOp->setEasingCurve(QEasingCurve::Linear);
        animationOp->setDuration(1000);
        animationOp->setStartValue(1.0);
        animationOp->setEndValue(0.0);

        QPropertyAnimation* animation1 = new QPropertyAnimation(ui->frame_9, "size",this);
        //connect(animation1,SIGNAL(finished()),this,SLOT(ani_end()));
        animation1->setDuration(1300);
        animation1->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz = ui->frame_9->size();
        animation1->setStartValue(siz);
        animation1->setEndValue(QSize(siz.width()+ui->cajaValores->width(),siz.height()));

        QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
        group->addAnimation(animation);
        group->addAnimation(animation0);
        group->addAnimation(animation1);
        group->addAnimation(animationOp);

        connect(group,SIGNAL(finished()),this,SLOT(ani_end()));
        connect(group,SIGNAL(finished()),this,SLOT(toggleChecbox()));
        group->start();
    }
}

void FrmArticulos::ani_end()
{
    ui->cajaValores->hide();
    sender()->deleteLater();
}

void FrmArticulos::toggleChecbox()
{
    ui->checkBox->setEnabled(true);
}

void FrmArticulos::on_chkmostrarvalores_comparativa_toggled(bool checked)
{
    ui->chkmostrarvalores_comparativa->setEnabled(false);
    if(checked)
    {
        ui->cajaValores__comparativa->show();
        QPropertyAnimation* animation = new QPropertyAnimation(ui->cajaValores__comparativa, "pos",this);

        animation->setDuration(1000);
        animation->setEasingCurve(QEasingCurve::OutElastic);
        QPoint pos = ui->cajaValores__comparativa->pos();
        animation->setStartValue(QPoint(pos.x()+ui->cajaValores__comparativa->width(),pos.y()));
        animation->setEndValue(pos);

        QPropertyAnimation* animation0 = new QPropertyAnimation(ui->cajaValores__comparativa, "size",this);
        //connect(animation,SIGNAL(finished()),this,SLOT(ani_end()));
        animation0->setDuration(1000);
        animation0->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz0 = ui->cajaValores__comparativa->size();
        animation0->setStartValue(QSize(0,siz0.height()));
        animation0->setEndValue(siz0);

        QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
        //this->setGraphicsEffect(fade_effect);
        ui->cajaValores__comparativa->setGraphicsEffect(fade_effect);
        QPropertyAnimation *animationOp = new QPropertyAnimation(fade_effect, "opacity");
        animationOp->setEasingCurve(QEasingCurve::Linear);
        animationOp->setDuration(1000);
        animationOp->setStartValue(0.0);
        animationOp->setEndValue(1.0);

        QPropertyAnimation* animation1 = new QPropertyAnimation(ui->frame_graficas_comparativas, "size",this);
        //connect(animation1,SIGNAL(finished()),this,SLOT(ani_end()));
        animation1->setDuration(1000);
        animation1->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz = ui->frame_graficas_comparativas->size();
        animation1->setStartValue(QSize(siz.width()+siz0.width(),siz.height()));
        animation1->setEndValue(siz);

        QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
        group->addAnimation(animation);
        group->addAnimation(animation0);
        group->addAnimation(animation1);
        group->addAnimation(animationOp);

        connect(group,SIGNAL(finished()),group,SLOT(deleteLater()));
        connect(group,SIGNAL(finished()),this,SLOT(togglechkmostrarvalores_comparativa()));
        group->start();
    }
    else
    {
        QPropertyAnimation* animation = new QPropertyAnimation(ui->cajaValores__comparativa, "pos",this);
        //connect(animation,SIGNAL(finished()),this,SLOT(ani_end()));
        animation->setDuration(1300);
        animation->setEasingCurve(QEasingCurve::OutElastic);
        QPoint pos = ui->cajaValores__comparativa->pos();
        animation->setStartValue(pos);
        animation->setEndValue(QPoint(pos.x()+ui->cajaValores__comparativa->width(),pos.y()));

        QPropertyAnimation* animation0 = new QPropertyAnimation(ui->cajaValores__comparativa, "size",this);
        //connect(animation,SIGNAL(finished()),this,SLOT(ani_end()));
        animation0->setDuration(1300);
        animation0->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz0 = ui->cajaValores__comparativa->size();
        animation0->setStartValue(siz0);
        animation0->setEndValue(QSize(0,siz0.height()));

        QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
        //this->setGraphicsEffect(fade_effect);
        ui->cajaValores__comparativa->setGraphicsEffect(fade_effect);
        QPropertyAnimation *animationOp = new QPropertyAnimation(fade_effect, "opacity");
        animationOp->setEasingCurve(QEasingCurve::Linear);
        animationOp->setDuration(1000);
        animationOp->setStartValue(1.0);
        animationOp->setEndValue(0.0);

        QPropertyAnimation* animation1 = new QPropertyAnimation(ui->frame_graficas_comparativas, "size",this);
        //connect(animation1,SIGNAL(finished()),this,SLOT(ani_end()));
        animation1->setDuration(1300);
        animation1->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz = ui->frame_graficas_comparativas->size();
        animation1->setStartValue(siz);
        animation1->setEndValue(QSize(siz.width()+ui->cajaValores__comparativa->width(),siz.height()));

        QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
        group->addAnimation(animation);
        group->addAnimation(animation0);
        group->addAnimation(animation1);
        group->addAnimation(animationOp);

        connect(group,SIGNAL(finished()),this,SLOT(ani_comparativas_end()));
        connect(group,SIGNAL(finished()),this,SLOT(togglechkmostrarvalores_comparativa()));
        group->start();
    }
}

void FrmArticulos::ani_comparativas_end()
{
    ui->cajaValores__comparativa->hide();
    sender()->deleteLater();
}

void FrmArticulos::togglechkmostrarvalores_comparativa()
{
        ui->chkmostrarvalores_comparativa->setEnabled(true);
}

void FrmArticulos::on_botCambiarImagen_clicked()
{
    CambiarImagen_clicked(ui->lblImagenArticulo_1,"bImagen");
}

void FrmArticulos::on_botCambiarImagen_2_clicked()
{
    CambiarImagen_clicked(ui->lblImagenArticulo_2,"bImagen2");
}

void FrmArticulos::on_botCambiarImagen_3_clicked()
{
    CambiarImagen_clicked(ui->lblImagenArticulo_3,"bImagen3");
}

void FrmArticulos::on_botCambiarImagen_4_clicked()
{
    CambiarImagen_clicked(ui->lblImagenArticulo_4,"bImagen4");
}

void FrmArticulos::actualizar()
{
    if(!oArticulo->cCodigo.isEmpty())
    {
        QString cCodigo = oArticulo->cCodigo;
        oArticulo->Recuperar("Select * from articulos where cCodigo ='"+cCodigo+"' order by cCodigo limit 1 ",1);
        LLenarCampos();
        tarifa_model->setFilter("id_Articulo = "+QString::number(oArticulo->id));
        tarifa_model->select();
        //qDebug() << "FrmArticulos::actualizar()";
    }
}



void FrmArticulos::CambiarImagen_clicked(QLabel *label, QString campo)
{
    QString ficheroImagen;
    ficheroImagen = QFileDialog::getOpenFileName(this,tr("Abrir fichero de imagen"),"","Imagenes (*.bmp *.png *.xpm *.jpg)");
    if (!ficheroImagen.isEmpty()) {

        QImage imagen(ficheroImagen);

        label->setPixmap(QPixmap::fromImage(imagen));
        label->setScaledContents(true);
        QByteArray ba;
        QFile f(ficheroImagen);
        if(f.open(QIODevice::ReadOnly)) {
            ba = f.readAll();
            f.close();
        }
        QSqlQuery *Articulo = new QSqlQuery(QSqlDatabase::database("Maya"));
        Articulo->prepare("update articulos set "+campo+" =:imagen where Id = :nid");
        Articulo->bindValue(":imagen",ba);
        Articulo->bindValue(":nid",oArticulo->id);
        if (!Articulo->exec())
            QMessageBox::warning(qApp->activeWindow(),tr("Guardar Imagen"),tr("No se ha podido guardar la imagen en la base de datos"),tr("Ok"));
        delete Articulo;
    }
}

void FrmArticulos::on_botBuscar_clicked()
{
    db_consulta_view busqueda(this);
    busqueda.set_db("Maya");
    busqueda.set_texto_tabla("Artículos");
    QStringList listacampos;
    listacampos <<"cDescripcion" << "cCodigo" << "cCodigoBarras";
    busqueda.set_campoBusqueda(listacampos);
    busqueda.set_SQL("select id,cCodigo,cCodigoBarras,cCodigoFabricante,cDescripcion,cDescripcionReducida from articulos");
   // busqueda.set_filtro("id >0 limit 0,100");
    QStringList cabecera;
    cabecera << tr("Código") <<tr("Cod. Barras") << tr("Cod. prov.") << tr("Descripción") << tr("Desc. Reducida");
    busqueda.set_headers(cabecera);
    QVariantList tamanos;
    tamanos <<"0" << "120" << "120" <<"120" <<"300" <<"200";
    busqueda.set_tamano_columnas(tamanos);
    busqueda.set_titulo(tr("Búsqueda de artículos"));
    if(busqueda.exec() == QDialog::Accepted) {
        int id_art = busqueda.get_id();
        oArticulo->Recuperar("Select * from articulos where id = "+QString::number(id_art));
        LLenarCampos();
        ui->botAnterior->setEnabled(true);
        ui->botEditar->setEnabled(true);
        ui->botBorrar->setEnabled(true);
    }

}

void FrmArticulos::on_txtrCoste_editingFinished()
{
    double inicio, fin;
    inicio = oArticulo->rCoste;
    fin = ui->txtrCoste->text().replace(",",".").toDouble();
    if (inicio != fin)
    {
        blockSignals(true);
        // TODO recalcular tarifas
         blockSignals(false);
    }
}
