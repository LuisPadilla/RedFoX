#include "frmproveedores.h"
#include "ui_frmproveedores.h"

#include "frmfacturasproveedor.h"
#include "frmpedidosproveedor.h"
#include "frmalbaranproveedor.h"
#include "Auxiliares/monetarydelegate.h"
#include "Auxiliares/datedelegate.h"
#include "Auxiliares/frmaddentregascuenta.h"
#include "Auxiliares/entregascuenta.h"


#include <QDesktopServices>
frmProveedores::frmProveedores(QWidget *parent) :
  MayaModule(module_zone(),module_name(),parent),
  ui(new Ui::frmProveedores),
  menuButton(QIcon(":/Icons/PNG/proveedores_2.png"),tr("Proveedores"),this),
  push(new QPushButton(QIcon(":/Icons/PNG/proveedores_2.png"),"",this))

{    
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    __init = false;
    this->installEventFilter(this);
}

frmProveedores::~frmProveedores()
{
    delete ui;
}

void frmProveedores::init()
{
    if(__init)
        return;
    ui->setupUi(this);
    oProveedor     = new Proveedor(this);
    modelBusqueda  = new QSqlQueryModel(this);
    modelArticulo  = new QSqlQueryModel(this);
    modelAsientos  = new QSqlQueryModel(this);
    modeloDeudas   = new QSqlQueryModel(this);
    modelContactos = new QSqlQueryModel(this);
    modelPedidos   = new QSqlQueryModel(this);
    modelFacturas  = new QSqlQueryModel(this);
    modelAlbaranes = new QSqlQueryModel(this);
    modelEntregas  = new QSqlQueryModel(this);

    ui->tablaArticulos->setItemDelegateForColumn(3,new MonetaryDelegate(this));

    ui->tablaAsientos->setItemDelegateForColumn(2, new DateDelegate(this));
    ui->tablaAsientos->setItemDelegateForColumn(5, new MonetaryDelegate(this));
    ui->tablaAsientos->setItemDelegateForColumn(8, new MonetaryDelegate(this));

    ui->tablaPagos->setItemDelegateForColumn(2,new DateDelegate(this));
    ui->tablaPagos->setItemDelegateForColumn(3,new DateDelegate(this));
    ui->tablaPagos->setItemDelegateForColumn(4,new MonetaryDelegate(this));
    ui->tablaPagos->setItemDelegateForColumn(5,new MonetaryDelegate(this));
    ui->tablaPagos->setItemDelegateForColumn(6, new MonetaryDelegate(this));

    ui->tablaColumnasPedidos->setItemDelegateForColumn(2, new DateDelegate(this));
    ui->tablaColumnasPedidos->setItemDelegateForColumn(3,new MonetaryDelegate(this));

    ui->tablacolumnasFacturas->setItemDelegateForColumn(2,new DateDelegate(this));
    ui->tablacolumnasFacturas->setItemDelegateForColumn(4,new MonetaryDelegate(this));
    ui->tablacolumnasFacturas->setItemDelegateForColumn(5,new MonetaryDelegate(this));
    ui->tablacolumnasFacturas->setItemDelegateForColumn(6,new MonetaryDelegate(this));
    ui->tablacolumnasFacturas->setItemDelegateForColumn(7,new MonetaryDelegate(this));
    ui->tablacolumnasFacturas->setItemDelegateForColumn(8,new MonetaryDelegate(this));
    ui->tablacolumnasFacturas->setItemDelegateForColumn(9,new MonetaryDelegate(this));

    ui->tablaColumnasAlbaran->setItemDelegateForColumn(2,new DateDelegate(this));
    ui->tablaColumnasAlbaran->setItemDelegateForColumn(4,new MonetaryDelegate(this));
    ui->tablaColumnasAlbaran->setItemDelegateForColumn(5,new MonetaryDelegate(this));
    ui->tablaColumnasAlbaran->setItemDelegateForColumn(6,new MonetaryDelegate(this));
    ui->tablaColumnasAlbaran->setItemDelegateForColumn(7,new MonetaryDelegate(this));
    ui->tablaColumnasAlbaran->setItemDelegateForColumn(8,new MonetaryDelegate(this));
    ui->tablaColumnasAlbaran->setItemDelegateForColumn(9,new MonetaryDelegate(this));

    ui->tabla_entregas->setItemDelegateForColumn(1,new DateDelegate(this));
    ui->tabla_entregas->setItemDelegateForColumn(3,new MonetaryDelegate(this));
    ui->tabla_entregas->setItemDelegateForColumn(4,new MonetaryDelegate(this));

    //COMBO MODELS
    ui->txtcodigoFormaPago->setModel(Configuracion_global->formapago_model);
    ui->txtcodigoFormaPago->setModelColumn(1);

    ui->cboDivisas->setModel(Configuracion_global->divisas_model);
    ui->cboDivisas->setModelColumn(1);

    ui->cboPais->setModel(Configuracion_global->paises_model);
    ui->cboPais->setModelColumn(1);

    ui->txtpaisAlmacen->setModel(Configuracion_global->paises_model);
    ui->txtpaisAlmacen->setModelColumn(1);

    //TABLE MODELS
    ui->tabla                ->setModel(modelBusqueda);
    ui->tablaArticulos       ->setModel(modelArticulo);
    ui->tablaPagos           ->setModel(modeloDeudas);
    ui->tablaContactos       ->setModel(modelContactos);
    ui->tablaColumnasPedidos ->setModel(modelPedidos);
    ui->tablacolumnasFacturas->setModel(modelFacturas);
    ui->tablaColumnasAlbaran ->setModel(modelAlbaranes);
    ui->tablaAsientos        ->setModel(modelAsientos);
    ui->tabla_entregas       ->setModel(modelEntregas);


    // CONTROLES
    ui->btnGuardarContacto->setVisible(false);
    ui->txtfecha_alta->setDate(QDate::currentDate());
    ui->txtfecha_ultima_compra->setDate(QDate::currentDate());
    ui->tablaArticulos->setColumnWidth(2,200);

    // CONEXIONES
    connect(ui->btnAnadir_persona_contacto,SIGNAL(clicked()),this,SLOT(nuevo_contacto()));
    ui->tablaContactos->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tablaPagos->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tablaContactos, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(menu_contactos(const QPoint&)));
    connect(ui->tablaPagos,SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(menu_deudas(const QPoint&)));

    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    ui->stackedWidget->setCurrentIndex(1);


    setUpBusqueda();
    BloquearCampos(true);

    // CONTROL DE EVENTOS
    QList<QWidget*> l = this->findChildren<QWidget*>();
    QList<QWidget*> ::Iterator it;

    for( it = l.begin() ;it!= l.end();++it )
        (*it)->installEventFilter(this);

    pob_completer_model = new QSqlTableModel(this,QSqlDatabase::database("calles"));
    pob_completer_model->setTable("municipios");
    pob_completer = new QCompleter(pob_completer_model,this);
    pob_completer->setCaseSensitivity(Qt::CaseInsensitive);
    pob_completer->setCompletionColumn(3);
    ui->txtpoblacion->setCompleter(pob_completer);
    ui->txtpoblacion_almacen->setCompleter(pob_completer);

    calle_completer_model = new QSqlTableModel(this,QSqlDatabase::database("calles"));
    calle_completer_model->setTable("calles");
    calle_completer = new QCompleter(calle_completer_model,this);
    calle_completer->setCaseSensitivity(Qt::CaseInsensitive);
    calle_completer->setCompletionColumn(2);
    ui->txtdireccion1->setCompleter(calle_completer);
    ui->txtdireccion2->setCompleter(calle_completer);
    ui->txtdireccion_almacen->setCompleter(calle_completer);

    __init = true;
}

void frmProveedores::init_querys()
{
    if(!__init)
        return;
    QString cSQL = "select id,codigo,proveedor,cif,telefono1,fax,movil,persona_contacto from proveedores order by proveedor";
    modelBusqueda->setQuery(cSQL,Configuracion_global->groupDB);
    formato_tabla(modelBusqueda);
    ui->tabWidget->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(1);
}

void frmProveedores::llenar_tabDatos()
{
    ui->txtcodigo->setText(oProveedor->codigo);
    ui->txtproveedor->setText(oProveedor->proveedor);
    ui->txtfecha_alta->setDate(oProveedor->fecha_alta);
    ui->txtcif->setText(oProveedor->cif);
    ui->txttelefono1->setText(oProveedor->telefono1);
    ui->txttelefono2->setText(oProveedor->telefono2);
    ui->txttelefono3->setText(oProveedor->telefono3);
    ui->txtdireccion1->setText(oProveedor->direccion1);
    ui->txtdireccion2->setText(oProveedor->direccion2);
    ui->txtcp->setText(oProveedor->cp);
    ui->txtpoblacion->setText(oProveedor->poblacion);
    ui->txtprovincia->setText(oProveedor->provincia);
    ui->cboPais->setCurrentIndex(ui->cboPais->findText(oProveedor->pais));
    ui->txtfax->setText(oProveedor->fax);
    ui->txtmovil->setText(oProveedor->movil);
    ui->txtemail->setText(oProveedor->email);
    ui->txtweb->setText(oProveedor->web);
}

void frmProveedores::llenar_tabFinanzas()
{
    if(!__init)
        return;
    ui->txtentidad_bancaria_proveedor->setText(oProveedor->entidad_bancaria_proveedor);
    ui->txtoficina_bancaria_proveedor->setText(oProveedor->oficina_bancaria_proveedor);
    ui->txtentidad_pago_proveedor->setText(oProveedor->entidad_pago_proveedor);
    ui->txtoficina_pago_proveedor->setText(oProveedor->oficina_pago_proveedor);
    ui->txtcodigoFormaPago->setCurrentIndex(ui->txtcodigoFormaPago->findText(oProveedor->codigoFormaPago));
    ui->txtforma_pago->setText(Configuracion_global->formapago_model->record(ui->txtcodigoFormaPago->currentIndex()).value("forma_pago").toString());

    switch (oProveedor->tipo_iva) {
    case 1:
        ui->radGeneral->setChecked(true);
        break;
    case 2:
        ui->radUE->setChecked(true);
        break;
    case 3:
        ui->radExportacion->setChecked(true);
        break;
    case 4:
        ui->radExcento->setChecked(true);
        break;
    }
    ui->txtdc_pago_proveedor->setText(oProveedor->dc_pago_proveedor);
    ui->txtdc_proveedor->setText(oProveedor->dc_proveedor);
    ui->txtcc_proveedor->setText(oProveedor->cc_proveedor);

    QString cOk;
    if(!ui->txtentidad_pago_proveedor->text().isEmpty() && !ui->txtoficina_pago_proveedor->text().isEmpty() &&
            !ui->txtdc_pago_proveedor->text().isEmpty() && !ui->txtcuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_pago_proveedor->text(),ui->txtoficina_pago_proveedor->text(),
                                              ui->txtdc_pago_proveedor->text(),ui->txtcuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));
    if(!ui->txtentidad_bancaria_proveedor->text().isEmpty() && !ui->txtoficina_bancaria_proveedor->text().isEmpty() &&
            !ui->txtdc_proveedor->text().isEmpty() && !ui->txtcc_proveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_bancaria_proveedor->text(),ui->txtoficina_bancaria_proveedor->text(),
                               ui->txtdc_proveedor->text(),ui->txtcc_proveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));

    for(auto i = 0; i< Configuracion_global->divisas_model->rowCount();i++)
    {
        if(Configuracion_global->divisas_model->record(i).value("id") == oProveedor->id_divisa)
        {
            ui->cboDivisas->setCurrentIndex(i);
            break;
        }
    }
}

void frmProveedores::llenar_tabPagos()
{
    if(!__init)
        return;
    QString sql = QString("select id,documento,fecha_deuda,vencimiento,importe_deuda,pagado,pendiente,pago_por,"
                          "numero_tarjeta_cuenta,asiento_numero from deudas_proveedores where id_proveedor = %1 "
                          " and id_empresa=%2 ").arg(oProveedor->id).arg(Configuracion_global->idEmpresa);
    if(ui->rdbPagados->isChecked())
        sql.append("and pendiente=0 ");
    else if(ui->rdbPendientes->isChecked())
        sql.append("and pendiente>0 ");

    sql.append("order by vencimiento");
    modeloDeudas->setQuery(sql,Configuracion_global->groupDB);

    ui->tablaPagos->setColumnHidden(0,true);
    ui->tablaPagos->setColumnWidth(1,100);
    ui->tablaPagos->setColumnWidth(2,95);
    ui->tablaPagos->setColumnWidth(3,95);
    ui->tablaPagos->setColumnWidth(4,65);
    ui->tablaPagos->setColumnWidth(5,65);
    ui->tablaPagos->setColumnWidth(6,80);
    ui->tablaPagos->setColumnWidth(8,200);
    //ui->tablaPagos->setColumnWidth(9,200);

    modeloDeudas->setHeaderData(1,Qt::Horizontal,tr("Documento"));
    modeloDeudas->setHeaderData(2,Qt::Horizontal,tr("F. Deuda"));
    modeloDeudas->setHeaderData(3,Qt::Horizontal,tr("Vto"));
    modeloDeudas->setHeaderData(4,Qt::Horizontal,tr("Importe"));
    modeloDeudas->setHeaderData(5,Qt::Horizontal,tr("Pagado"));
    modeloDeudas->setHeaderData(6,Qt::Horizontal,tr("Pendiente"));
    modeloDeudas->setHeaderData(7,Qt::Horizontal,tr("Pago por...."));
    modeloDeudas->setHeaderData(8,Qt::Horizontal,tr("N.Tarjeta/Cuenta"));
    modeloDeudas->setHeaderData(9,Qt::Horizontal,tr("Asiento numero"));

    ui->txtdeuda_actual->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->deuda_actual,'f',Configuracion_global->decimales)));
    ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->entregado_a_cuenta,'f',Configuracion_global->decimales)));
    ui->txtretencion_irpf->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->retencion_irpf,'f',Configuracion_global->decimales)));
    ui->txttipo_retencioirpf->setText(QString::number(oProveedor->tipo_retencion));
    ui->chkrecargo_equivalencia->setChecked(oProveedor->recargo_equivalencia==1);
    ui->txtdeuda_maxima->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->deuda_maxima,'f',Configuracion_global->decimales)));
    ui->txtdia_cobro->setText(QString::number(oProveedor->dia_cobro));
    ui->txtdto->setText(QString::number(oProveedor->dto));
}

void frmProveedores::llenar_tabAlmacen()
{
    ui->txtdireccion_almacen->setText(oProveedor->direccion_almacen);
    ui->txtcp_almacen->setText(oProveedor->cp_almacen);
    ui->txtpoblacion_almacen->setText(oProveedor->poblacion_almacen);
    ui->txtprovinciaAmacen->setText(oProveedor->provincia_almacen);
    ui->txtpaisAlmacen->setCurrentIndex(ui->txtpaisAlmacen->findText(oProveedor->paisAlmacen));
    ui->txttelefono_almacen->setText(oProveedor->telefono_almacen);
    ui->txtfax_almacen->setText(oProveedor->fax_almacen);
}

void frmProveedores::llenar_tabConta()
{
    if(!__init)
        return;
    ui->txtcuenta_aplicacion->setText(oProveedor->cuenta_aplicacion);

    modelAsientos->setQuery("select id,asiento,fecha_asiento,cuenta_d,descripcion_d, importe_d,cuenta_h,descripcion_h,importe_h "
                            "from diario where cta_principal = '"+oProveedor->cuenta_aplicacion +
                            "' order by asiento + ' '+pos_en_asiento",
                            Configuracion_global->contaDB);

    ui->tablaAsientos->setColumnHidden(0,true);
    ui->tablaAsientos->setColumnWidth(1,55);
    ui->tablaAsientos->setColumnWidth(2,63);
    ui->tablaAsientos->setColumnWidth(3,55);
    ui->tablaAsientos->setColumnWidth(4,75);
    ui->tablaAsientos->setColumnWidth(5,60);
    ui->tablaAsientos->setColumnWidth(6,55);
    ui->tablaAsientos->setColumnWidth(7,75);
    ui->tablaAsientos->setColumnWidth(8,60);
}

void frmProveedores::LLenarCampos()
{
    if(!__init)
        return;
    ui->lblcodigo_proveedor->setText(oProveedor->codigo);
    ui->lblproveedor->setText(oProveedor->proveedor);

    /* TabDatos => index 0  */
    llenar_tabDatos();

    /* TabContacto => index 1   */
    contactos();

    /* TabFinanzas => index 2 */
    llenar_tabFinanzas();

    /* tabPagos => index 3 */
    llenar_tabPagos();


    /* tabAlmacen => index 4   */
    llenar_tabAlmacen();

    /* tabArticulos => index 5  */
    modelArticulo->setQuery("select codigo,codigo_barras, descripcion_reducida,coste "
                           "from articulos where id_proveedor =" +QString::number(oProveedor->id),
                           Configuracion_global->groupDB);
    QStringList header;
    QVariantList sizes;
    header  << tr("Código") << tr("Codigo de barras") <<tr("Descripción") <<tr("Coste");
    sizes << 120 << 120 <<300 << 120 ;
    for(int cnt =0;cnt<header.length();cnt++)
    {
        modelArticulo->setHeaderData(cnt,Qt::Horizontal,header.at(cnt));
        ui->tabla->setColumnWidth(cnt,sizes.at(cnt).toInt());

    }

    /* txttexto_para_pedidos => index 6 */
    ui->txttexto_para_pedidos->setPlainText(oProveedor->texto_para_pedidos);

    /* tabConta => index 7 */
    llenar_tabConta();

    /* tabEstad => index 8  */
    acumulados();

    /* tabGraf => index 9 */
    grafica();

    /* tabComen => index 10 */
    ui->txtcomentarios->setText(oProveedor->comentarios);

    /* tabHisto => index 11*/
    historiales();
}

void frmProveedores::CargarCamposEnProveedor()
{
    oProveedor->codigo = ui->txtcodigo->text();
    oProveedor->proveedor =ui->txtproveedor->text();
    oProveedor->cif = ui->txtcif->text();
    oProveedor->direccion1 = ui->txtdireccion1->text();
    oProveedor->direccion2 = ui->txtdireccion2->text();
    oProveedor->cp = ui->txtcp->text();
    oProveedor->poblacion = ui->txtpoblacion->text();
    oProveedor->provincia = ui->txtprovincia->text();
    oProveedor->pais = ui->cboPais->currentText();
    oProveedor->telefono1 = ui->txttelefono1->text();
    oProveedor->telefono2 = ui->txttelefono2->text();
    oProveedor->telefono3 = ui->txttelefono3->text();
    oProveedor->fax = ui->txtfax->text();
    oProveedor->movil = ui->txtmovil->text();
    oProveedor->email = ui->txtemail->text();
    oProveedor->web = ui->txtweb->text();
    //oProveedor->persona_contacto = ui->txtpersona_contacto->text();
    oProveedor->dia_cobro = ui->txtdia_cobro->text().toInt();
    oProveedor->direccion_almacen = ui->txtdireccion_almacen->text();
    oProveedor->cp_almacen = ui->txtcp_almacen->text();
    oProveedor->poblacion_almacen = ui->txtpoblacion_almacen->text();
    oProveedor->provincia_almacen = ui->txtprovinciaAmacen->text();
    oProveedor->paisAlmacen = ui->txtpaisAlmacen->currentText();
    oProveedor->telefono_almacen = ui->txttelefono_almacen->text();
    oProveedor->fax_almacen = ui->txtfax_almacen->text();
    oProveedor->codigoFormaPago = ui->txtcodigoFormaPago->currentText();
    oProveedor->fecha_ultima_compra = ui->txtfecha_ultima_compra->date();
    oProveedor->importe_acumulado_compras = ui->txtimporte_acumulado_compras->text().replace(".","").replace(",",".").toDouble();
    oProveedor->entidad_bancaria_proveedor = ui->txtentidad_bancaria_proveedor->text();
    oProveedor->oficina_bancaria_proveedor = ui->txtoficina_bancaria_proveedor->text();
    oProveedor->dc_proveedor = ui->txtdc_proveedor->text();
    oProveedor->cc_proveedor = ui->txtcc_proveedor->text();
    oProveedor->entidad_pago_proveedor = ui->txtentidad_pago_proveedor->text();
    oProveedor->oficina_pago_proveedor = ui->txtoficina_pago_proveedor->text();
    oProveedor->dc_pago_proveedor = ui->txtdc_pago_proveedor->text();
    oProveedor->retencion_irpf = ui->txtretencion_irpf->text().replace(".","").replace(",",".").toDouble();
    oProveedor->tipo_retencion = ui->txttipo_retencioirpf->text().toInt();
    oProveedor->cuenta_aplicacion = ui->txtcuenta_aplicacion->text();
    oProveedor->comentarios = ui->txtcomentarios->toPlainText();
    oProveedor->dto = ui->txtdto->text().replace(".","").replace(",",".").toDouble();
    oProveedor->fecha_alta = ui->txtfecha_alta->date();
    oProveedor->deuda_maxima = ui->txtdeuda_maxima->text().replace(".","").replace(",",".").toDouble();
    oProveedor->deuda_actual = ui->txtdeuda_actual->text().replace(".","").replace(",",".").toDouble();
    oProveedor->recargo_equivalencia = ui->chkrecargo_equivalencia->isChecked();
    oProveedor->texto_para_pedidos = ui->txttexto_para_pedidos->toPlainText();
    if(ui->radGeneral->isChecked())
        oProveedor->tipo_iva = 1;
    if(ui->radUE->isChecked())
        oProveedor->tipo_iva = 2;
    if(ui->radExportacion->isChecked())
        oProveedor->tipo_iva = 3;
    if(ui->radExcento->isChecked())
        oProveedor->tipo_iva = 4;
    oProveedor->idFormadePago = Configuracion_global->formapago_model->record(ui->txtcodigoFormaPago->currentIndex()).value("id").toInt();
    oProveedor->id_pais = Configuracion_global->paises_model->record(ui->cboPais->currentIndex()).value("id").toInt();
    oProveedor->id_divisa = Configuracion_global->divisas_model->record(ui->cboDivisas->currentIndex()).value("id").toInt();
}

void frmProveedores::BloquearCampos(bool state)
{
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
    }
    // SpinBox
        QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
        QSpinBox *SpinBox;
        foreach (SpinBox, SpinBoxList) {
            SpinBox->setReadOnly(state);
        }

    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(!state);

    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(state);
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(!state);
    }

    ui->btnAnadir->setEnabled(state);
    ui->btnAnterior->setEnabled(state);
    ui->btnBorrar->setEnabled(state);
    ui->btnBuscar->setEnabled(state);
    ui->btnDeshacer->setEnabled(!state);
    ui->btnEditar->setEnabled(state);
    ui->btnGuardar->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->btnAnadir_persona_contacto->setEnabled(!state);

    m_busqueda->block(!state);

    if(state)
        emit unblock();
    else
        emit block();
}

void frmProveedores::on_btnSiguiente_clicked()
{
    if(oProveedor->Next())
        LLenarCampos();
}

void frmProveedores::on_btnAnterior_clicked()
{
    if(oProveedor->Prev())
        LLenarCampos();
}

void frmProveedores::on_btnGuardar_clicked()
{
    bool lGuardar = true;
    QString cTexto;
    cTexto = tr("Hay campos que debe rellenar para poder guardar:")+"\n";

    if (ui->txtcodigoFormaPago->currentIndex() == -1)
    {
        lGuardar = false;
        cTexto.append(tr("Debe elegir una forma de pago")).append("\n");
    }

    if (ui->cboPais->currentIndex() == -1)
    {
        lGuardar = false;
        cTexto.append(tr("Debe elegir el pais del Proveedor")).append("\n");
    }

    if(ui->txtproveedor->text().isEmpty())
    {
        lGuardar = false;
        cTexto.append(tr("Debe especificar el nombre del Proveedor")).append("\n");
    }

    if(ui->txtcif->text().isEmpty())
    {
        lGuardar = false;
        cTexto.append(tr("Debe especificar el cif del Proveedor")).append("\n");
    }

    if (lGuardar)
    {
        CargarCamposEnProveedor();
        oProveedor->Guardar();
        Configuracion_global->commit();
        BloquearCampos(true);
        editing = true;
    }
    else
    {
        QMessageBox::warning(this,tr("No se puede guardar, falta especificar datos"),cTexto,tr("Aceptar"));
    }
}

void frmProveedores::on_btnEditar_clicked()
{
    if(ui->stackedWidget->currentIndex() == 1)
    {
        if(!ui->tabla->currentIndex().isValid())
            return;
        int _id = modelBusqueda->record(ui->tabla->currentIndex().row()).value("id").toInt();
        oProveedor->Recuperar(_id);
        LLenarCampos();
        ui->stackedWidget->setCurrentIndex(0);
    }
    Configuracion_global->transaction();
    BloquearCampos(false);
    ui->tabWidget->setCurrentIndex(0);
    ocultarBusqueda();
    ui->txtcodigo->setFocus();
    editing = true;
}

void frmProveedores::on_btnAnadir_clicked()
{
    if(Configuracion_global->formapago_model->rowCount() == 0)
    {
        QMessageBox::information(this,tr("Formas de pago"),
                                 tr("Debe añadir al menos una forma de pago antes de poder añadir proveedores"));
        return;
    }    
    oProveedor->Vaciar();
    Configuracion_global->transaction();
    if(oProveedor->Anadir())
    {
        LLenarCampos();
        BloquearCampos(false);
        ocultarBusqueda();
        ui->txtcodigo->setFocus();
        ui->tabWidget->setCurrentIndex(0);
        ui->stackedWidget->setCurrentIndex(0);
        editing = false;
    }
    else
    {
        Configuracion_global->rollback();
    }
}

void frmProveedores::on_txtcp_editingFinished()
{
    if(!QSqlDatabase::database("calles").isOpen())
        return;
    QString cp = QString("CodPostal = '%1'").arg(ui->txtcp->text());
    pob_completer_model->setFilter(cp);
    pob_completer_model->select();

    calle_completer_model->setFilter(cp);
    calle_completer_model->select();

    if(pob_completer_model->rowCount() > 0)
    {
        ui->txtpoblacion->setText(pob_completer_model->record(0).value("Municipio").toString());
        ui->txtprovincia->setText(pob_completer_model->record(0).value("CodProv").toString());
        ui->cboPais->setCurrentText("España");

        if(pob_completer_model->rowCount() > 1)
            ui->txtpoblacion->setFocus();
        else
            ui->txtdireccion1->setFocus();
    }
}

void frmProveedores::on_btnBorrar_clicked()
{
    int ndev = QMessageBox::question(this,tr("Gestión de proveedores"),tr("Está seguro/a de borrar el proveedor?\n"
                                                                          "se borrarán todos los historiales de ese proveedor.\n"
                                                                          " Esta opción no se puede deshacer"),
                                     tr("Borrar"),tr("Cancelar Borrado"));
    if(ndev ==0)
    {
        oProveedor->Borrar(oProveedor->id);
        oProveedor->Vaciar();
        LLenarCampos();
    }
}

void frmProveedores::on_btnDeshacer_clicked()
{
    if(QMessageBox::question(NULL,tr("Gestión de proveedores"),
                             tr("Se perderán los cambios realizados en la ficha\n"
                                "Esta opción no se puede deshacer\n\n"
                                "¿Desea anular los cambios o continuar la edición?"),
                             tr("Continuar Edición"),tr("Anular cambios")) == QDialog::Accepted)
    {
        Configuracion_global->rollback();
        LLenarCampos();
        BloquearCampos(true);
        if(!editing)
            mostrarBusqueda();
    }
}

void frmProveedores::on_txtentidad_bancaria_proveedor_editingFinished()
{
    QString cOk;
    if(!ui->txtentidad_bancaria_proveedor->text().isEmpty() && !ui->txtoficina_bancaria_proveedor->text().isEmpty() &&
            !ui->txtdc_proveedor->text().isEmpty() && !ui->txtcc_proveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_bancaria_proveedor->text(),ui->txtoficina_bancaria_proveedor->text(),
                                 ui->txtdc_proveedor->text(),ui->txtcc_proveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));
}

void frmProveedores::on_txtoficina_bancaria_proveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtentidad_bancaria_proveedor->text().isEmpty() && !ui->txtoficina_bancaria_proveedor->text().isEmpty() &&
            !ui->txtdc_proveedor->text().isEmpty() && !ui->txtcc_proveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_bancaria_proveedor->text(),ui->txtoficina_bancaria_proveedor->text(),
                                 ui->txtdc_proveedor->text(),ui->txtcc_proveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));

}

void frmProveedores::on_txtdc_proveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtentidad_bancaria_proveedor->text().isEmpty() && !ui->txtoficina_bancaria_proveedor->text().isEmpty() &&
            !ui->txtdc_proveedor->text().isEmpty() && !ui->txtcc_proveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_bancaria_proveedor->text(),ui->txtoficina_bancaria_proveedor->text(),
                                 ui->txtdc_proveedor->text(),ui->txtcc_proveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));
}

void frmProveedores::on_txtcc_proveedor_editingFinished()
{
    QString cOk;
    if(!ui->txtentidad_bancaria_proveedor->text().isEmpty() && !ui->txtoficina_bancaria_proveedor->text().isEmpty() &&
            !ui->txtdc_proveedor->text().isEmpty() && !ui->txtcc_proveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_bancaria_proveedor->text(),ui->txtoficina_bancaria_proveedor->text(),
                                 ui->txtdc_proveedor->text(),ui->txtcc_proveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));

}

void frmProveedores::on_txtentidad_pago_proveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtentidad_pago_proveedor->text().isEmpty() && !ui->txtoficina_pago_proveedor->text().isEmpty() &&
            !ui->txtdc_pago_proveedor->text().isEmpty() && !ui->txtcuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_pago_proveedor->text(),ui->txtoficina_pago_proveedor->text(),
                                 ui->txtdc_pago_proveedor->text(),ui->txtcuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));

}

void frmProveedores::on_txtoficina_pago_proveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtentidad_pago_proveedor->text().isEmpty() && !ui->txtoficina_pago_proveedor->text().isEmpty() &&
            !ui->txtdc_pago_proveedor->text().isEmpty() && !ui->txtcuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_pago_proveedor->text(),ui->txtoficina_pago_proveedor->text(),
                                 ui->txtdc_pago_proveedor->text(),ui->txtcuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));

}

void frmProveedores::on_txtdc_pago_proveedor_editingFinished()
{
    QString cOk;
    if(!ui->txtentidad_pago_proveedor->text().isEmpty() && !ui->txtoficina_pago_proveedor->text().isEmpty() &&
            !ui->txtdc_pago_proveedor->text().isEmpty() && !ui->txtcuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_pago_proveedor->text(),ui->txtoficina_pago_proveedor->text(),
                                 ui->txtdc_pago_proveedor->text(),ui->txtcuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));
}

void frmProveedores::on_txtcuentaPagoProveedor_editingFinished()
{
    QString cOk;
    if(!ui->txtentidad_pago_proveedor->text().isEmpty() && !ui->txtoficina_pago_proveedor->text().isEmpty() &&
            !ui->txtdc_pago_proveedor->text().isEmpty() && !ui->txtcuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_pago_proveedor->text(),ui->txtoficina_pago_proveedor->text(),
                                 ui->txtdc_pago_proveedor->text(),ui->txtcuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));
}

void frmProveedores::on_txtcp_almacen_editingFinished()
{
    if(!QSqlDatabase::database("calles").isOpen())
        return;
    QString cp = QString("CodPostal = '%1'").arg(ui->txtcp_almacen->text());
    pob_completer_model->setFilter(cp);
    pob_completer_model->select();

    calle_completer_model->setFilter(cp);
    calle_completer_model->select();

    if(pob_completer_model->rowCount() > 0)
    {
        ui->txtpoblacion_almacen->setText(pob_completer_model->record(0).value("Municipio").toString());
        ui->txtprovinciaAmacen->setText(pob_completer_model->record(0).value("CodProv").toString());
        ui->txtpaisAlmacen->setCurrentText("España");

        if(pob_completer_model->rowCount() > 1)
            ui->txtpoblacion_almacen->setFocus();
        else
            ui->txtdireccion_almacen->setFocus();
    }
}

void frmProveedores::pagar_deuda()
{
    if(QMessageBox::question(this,tr("Gestión de Proveedores"),
                             tr("¿Cobrar la deuda?"),
                             tr("Cancelar"),tr("Cobrar"))==QMessageBox::Accepted)
    {
        //TODO frmProveedores::pagar_deuda()
    }
}

void frmProveedores::pagar_fraccion()
{
    //TODO frmProveedores::pagar_fraccion()
}

void frmProveedores::ver_asiento()
{
    //TODO frmProveedores::ver_asiento()
}

void frmProveedores::historiales()
{
    if(!__init)
        return;
    // Cargar historial Pedidos
    modelPedidos->setQuery("select id,pedido,fecha,total,recibido_completo from ped_pro where id_proveedor ="+
                           QString::number(oProveedor->id)+" order by pedido desc",Configuracion_global->empresaDB);        

    ui->tablaColumnasPedidos->setColumnHidden(0,true);
    ui->tablaColumnasPedidos->setColumnWidth(1,120);
    ui->tablaColumnasPedidos->setColumnWidth(1,120);
    ui->tablaColumnasPedidos->setColumnWidth(3,120);
    modelPedidos->setHeaderData(1,Qt::Horizontal,"Pedido");
    modelPedidos->setHeaderData(2,Qt::Horizontal,"Fecha");    
    modelPedidos->setHeaderData(3,Qt::Horizontal,"Total");
    modelPedidos->setHeaderData(4,Qt::Horizontal,"C.");


    // Cargar Historial Facturas    
    modelFacturas->setQuery("select id,factura,fecha,pedido,total_base,total_iva,total_retencion,total_recargo,total,importe_deuda_pendiente"
                            " from fac_pro where id_proveedor = "+QString::number(oProveedor->id)+
                            " order by factura desc",Configuracion_global->empresaDB);

    ui->tablacolumnasFacturas->setColumnHidden(0,true);
    modelFacturas->setHeaderData(1,Qt::Horizontal,"N.Factura");
    modelFacturas->setHeaderData(2,Qt::Horizontal,"Fecha");
    modelFacturas->setHeaderData(3,Qt::Horizontal,"Pedido");
    modelFacturas->setHeaderData(4,Qt::Horizontal,"Base IMP.");
    modelFacturas->setHeaderData(5,Qt::Horizontal,"Tot.IVA");
    modelFacturas->setHeaderData(6,Qt::Horizontal,"Ret IRPF");
    modelFacturas->setHeaderData(7,Qt::Horizontal,"Rec. Eq.");
    modelFacturas->setHeaderData(8,Qt::Horizontal,"TOTAL");
    modelFacturas->setHeaderData(9,Qt::Horizontal,"Pendiente");

    // Cargar Historial Albaranes    
    modelAlbaranes->setQuery("select id,albaran,fecha,base_total,iva_total,"
                             " total from alb_pro where id_proveedor = "+QString::number(oProveedor->id)+
                             " order by albaran desc ",Configuracion_global->empresaDB);

    ui->tablaColumnasAlbaran->setColumnHidden(0,true);
    modelAlbaranes->setHeaderData(1,Qt::Horizontal,"N.Albarán");
    modelAlbaranes->setHeaderData(2,Qt::Horizontal,"Fecha");
    modelAlbaranes->setHeaderData(3,Qt::Horizontal,"Pedido");
    modelAlbaranes->setHeaderData(4,Qt::Horizontal,"Base IMP.");
    modelAlbaranes->setHeaderData(5,Qt::Horizontal,"Tot.IVA");
    modelAlbaranes->setHeaderData(6,Qt::Horizontal,"Ret IRPF");
    modelAlbaranes->setHeaderData(7,Qt::Horizontal,"Rec. Eq.");
    modelAlbaranes->setHeaderData(8,Qt::Horizontal,"TOTAL");
    modelAlbaranes->setHeaderData(9,Qt::Horizontal,"Pendiente");

    // Historial entregas a cuenta    
    modelEntregas->setQuery("select id,fecha_entrega,concepto,importe,disponible from proveedor_a_cuenta where id_proveedor = "+QString::number(oProveedor->id),
                            Configuracion_global->groupDB);

    ui->tabla_entregas->setColumnHidden(0,true);
    ui->tabla_entregas->setColumnWidth(1,100);    
    ui->tabla_entregas->setColumnWidth(2,250);
    ui->tabla_entregas->setColumnWidth(3,80);    
    ui->tabla_entregas->setColumnWidth(4,80);

    QStringList cHeader;
    cHeader << tr("Fecha") <<tr("concepto") << tr("importe") <<tr("disponible");
    for(int pos = 0; pos < cHeader.size();pos++)
    {
        modelEntregas->setHeaderData(pos,Qt::Horizontal,cHeader.at(pos));
    }
}

void frmProveedores::acumulados()
{
    ui->txtfecha_ultima_compra->setDate(oProveedor->fecha_ultima_compra);
    ui->txtimporte_acumulado_compras->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->importe_acumulado_compras,'f',Configuracion_global->decimales)));

    ui->txtEnero->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->enero,'f',Configuracion_global->decimales)));
    ui->txtFebrero->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->febrero,'f',Configuracion_global->decimales)));
    ui->txtMarzo->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->marzo,'f',Configuracion_global->decimales)));
    ui->txtAbril->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->abril,'f',Configuracion_global->decimales)));
    ui->txtMayo->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->mayo,'f',Configuracion_global->decimales)));
    ui->txtJunio->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->junio,'f',Configuracion_global->decimales)));
    ui->txtjulio->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->julio,'f',Configuracion_global->decimales)));
    ui->txtAgosto->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->agosto,'f',Configuracion_global->decimales)));
    ui->txtSeptiembre->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->septiembre,'f',Configuracion_global->decimales)));
    ui->txtOctubre->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->octubre,'f',Configuracion_global->decimales)));
    ui->txtNoviembre->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->noviembre,'f',Configuracion_global->decimales)));
    ui->txtDiciembre->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->diciembre,'f',Configuracion_global->decimales)));
}

void frmProveedores::grafica()
{
    ui->Grafica->Clear();
    if(editing)
    {
        ui->Grafica->addItem(tr("Ene"),oProveedor->enero);
        ui->Grafica->addItem(tr("Feb"),oProveedor->febrero);
        ui->Grafica->addItem(tr("Mar"),oProveedor->marzo);
        ui->Grafica->addItem(tr("Abr"),oProveedor->abril);
        ui->Grafica->addItem(tr("May"),oProveedor->mayo);
        ui->Grafica->addItem(tr("Jun"),oProveedor->junio);
        ui->Grafica->addItem(tr("Jul"),oProveedor->julio);
        ui->Grafica->addItem(tr("Ago"),oProveedor->agosto);
        ui->Grafica->addItem(tr("Sep"),oProveedor->septiembre);
        ui->Grafica->addItem(tr("Oct"),oProveedor->octubre);
        ui->Grafica->addItem(tr("Nov"),oProveedor->noviembre);
        ui->Grafica->addItem(tr("Dic"),oProveedor->diciembre);
    }
}

void frmProveedores::contactos()
{
    if(!__init)
        return;
    modelContactos->setQuery("select id,id_proveedor,cargo_empresa, nombre, desc_telefono1,telefono1,desc_telefono2,telefono2,"
                             "desc_telefono3, telefono3, desc_movil1,movil1,desc_movil2,movil2,email "
                             "from personascontactoproveedor where id_proveedor ="+QString::number(oProveedor->id),
                             Configuracion_global->groupDB);

    modelContactos->setHeaderData(0,Qt::Horizontal,tr("id"));
    modelContactos->setHeaderData(1,Qt::Horizontal,tr("id_proveedor"));
    modelContactos->setHeaderData(2,Qt::Horizontal,tr("CARGO"));
    modelContactos->setHeaderData(3,Qt::Horizontal,tr("NOMBRE"));
    modelContactos->setHeaderData(4,Qt::Horizontal,tr("DESC.T1"));
    modelContactos->setHeaderData(5,Qt::Horizontal,tr("TEL1"));
    modelContactos->setHeaderData(6,Qt::Horizontal,tr("DESC.T2"));
    modelContactos->setHeaderData(7,Qt::Horizontal,tr("TEL2"));
    modelContactos->setHeaderData(8,Qt::Horizontal,tr("DESC.T3"));
    modelContactos->setHeaderData(9,Qt::Horizontal,tr("TEL3"));
    modelContactos->setHeaderData(10,Qt::Horizontal,tr("DESC.M1"));
    modelContactos->setHeaderData(11,Qt::Horizontal,tr("Mov.1"));
    modelContactos->setHeaderData(12,Qt::Horizontal,tr("DESC.M2"));
    modelContactos->setHeaderData(13,Qt::Horizontal,tr("Mov.2"));

    ui->tablaContactos->setColumnHidden(0,true);
    ui->tablaContactos->setColumnHidden(1,true);
}

void frmProveedores::menu_contactos(const QPoint &position)
{
    QPoint globalPos = ui->tablaContactos->mapToGlobal(position);
    QMenu myMenu;
    QAction *actionEditar = new QAction(tr("Editar Contacto"),this);
    QAction *actionBorrar = new QAction(tr("Borrar contacto"),this);
    myMenu.addAction(actionEditar);
    myMenu.addAction(actionBorrar);

    connect(actionEditar, SIGNAL(triggered()), this, SLOT(editar_contacto()));
    connect(actionBorrar,SIGNAL(triggered()),this,SLOT(borrar_contacto()));

    myMenu.exec(globalPos);

    actionEditar->deleteLater();
    actionBorrar->deleteLater();
}

void frmProveedores::menu_deudas(const QPoint &position)
{
    QPoint globalPos = ui->tablaPagos->mapToGlobal(position);
    QMenu myMenu;
    QAction *actionPagar = new QAction(tr("Pagar vencimiento"),this);
    myMenu.addAction(actionPagar);
    QAction *actionFraccion = new QAction(tr("Pago parcial"),this);
    myMenu.addAction(actionFraccion);
    QAction *actionAsiento = new QAction(tr("Ver asiento contable"),this);
    myMenu.addAction(actionAsiento);

    connect(actionPagar, SIGNAL(triggered()), this, SLOT(pagar_deuda()));
    connect(actionFraccion,SIGNAL(triggered()),this,SLOT(pagar_fraccion()));
    connect(actionAsiento,SIGNAL(triggered()),this,SLOT(ver_asiento()));

    myMenu.exec(globalPos);

    actionPagar->deleteLater();
    actionFraccion->deleteLater();
    actionAsiento->deleteLater();
}

void frmProveedores::nuevo_contacto()
{
    if(ui->txtNombre->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Nombre de contacto"),tr("Debe especificar u nombre  para el contacto"));
        return;
    }
    oProveedor->anadir_persona_contacto(oProveedor->id,ui->txtNombre->text(),ui->txtDescripcionT1->text(),
                                   ui->txtTelefono1->text(),ui->txtDescripcionT2->text(),ui->txtTelefono2->text(),
                                   ui->txtDescripcionT3->text(),ui->txtTelefono3->text(),ui->txtDescripcionM1->text(),
                                   ui->txtMovil1->text(),ui->txtDescripcionM2->text(),ui->txtMovil2->text(),ui->txtCargo->text(),
                                        ui->txtemail_contacto->text());
    ui->txtDescripcionM1->setText("");
    ui->txtDescripcionM2->setText("");
    ui->txtDescripcionT1->setText("");
    ui->txtDescripcionT2->setText("");
    ui->txtDescripcionT3->setText("");
    ui->txtMovil1->setText("");
    ui->txtMovil2->setText("");
    ui->txtNombre->setText("");
    ui->txtTelefono1->setText("");
    ui->txtTelefono2->setText("");
    ui->txtTelefono3->setText("");
    ui->txtCargo->setText("");
    ui->txtemail_contacto->setText("");
    ui->txtNombre->setFocus();

    contactos();
}

void frmProveedores::guardar_contacto()
{
    oProveedor->guardar_persona_contacto(this->id_contacto,ui->txtNombre->text(),ui->txtDescripcionT1->text(),
                                   ui->txtTelefono1->text(),ui->txtDescripcionT2->text(),ui->txtTelefono2->text(),
                                   ui->txtDescripcionT3->text(),ui->txtTelefono3->text(),ui->txtDescripcionM1->text(),
                                   ui->txtMovil1->text(),ui->txtDescripcionM2->text(),ui->txtMovil2->text(),ui->txtCargo->text(),
                                   ui->txtemail_contacto->text());
    ui->txtDescripcionM1->setText("");
    ui->txtDescripcionM2->setText("");
    ui->txtDescripcionT1->setText("");
    ui->txtDescripcionT2->setText("");
    ui->txtDescripcionT3->setText("");
    ui->txtMovil1->setText("");
    ui->txtMovil2->setText("");
    ui->txtNombre->setText("");
    ui->txtTelefono1->setText("");
    ui->txtTelefono2->setText("");
    ui->txtTelefono3->setText("");
    ui->txtCargo->setText("");
    ui->txtemail_contacto->setText("");
    ui->txtNombre->setFocus();

    contactos();
    ui->btnGuardarContacto->setVisible(false);
    ui->txtDescripcionM1->setReadOnly(true);
    ui->txtDescripcionM2->setReadOnly(true);
    ui->txtDescripcionT1->setReadOnly(true);
    ui->txtDescripcionT2->setReadOnly(true);
    ui->txtDescripcionT3->setReadOnly(true);
    ui->txtMovil1->setReadOnly(true);
    ui->txtMovil2->setReadOnly(true);
    ui->txtNombre->setReadOnly(true);
    ui->txtTelefono1->setReadOnly(true);
    ui->txtTelefono2->setReadOnly(true);
    ui->txtTelefono3->setReadOnly(true);
    ui->txtCargo->setReadOnly(true);
    ui->txtemail->setReadOnly(true);
}

void frmProveedores::editar_contacto()
{    
    QModelIndex index= ui->tablaContactos->currentIndex();
    if(!index.isValid())
        return;

    QSqlRecord r = modelContactos->record(index.row());
    this->id_contacto = r.value("id").toInt();
    ui->txtCargo->setText(r.value("cargo_empresa").toString());
    ui->txtNombre->setText(r.value("nombre").toString());
    ui->txtDescripcionT1->setText(r.value("desc_telefono1").toString());
    ui->txtDescripcionT2->setText(r.value("desc_telefono2").toString());
    ui->txtDescripcionT3->setText(r.value("desc_telefono3").toString());
    ui->txtDescripcionM1->setText(r.value("desc_movil1").toString());
    ui->txtDescripcionM2->setText(r.value("desc_movil2").toString());
    ui->txtTelefono1->setText(r.value("telefono1").toString());
    ui->txtTelefono2->setText(r.value("telefono2").toString());
    ui->txtTelefono3->setText(r.value("telefono3").toString());
    ui->txtMovil1->setText(r.value("movil1").toString());
    ui->txtMovil2->setText(r.value("movil2").toString());
    ui->txtCargo->setText(r.value("cargo_empresa").toString());
    ui->txtemail_contacto->setText(r.value("email").toString());

    if(ui->btnGuardar->isEnabled())
    {
        ui->btnGuardarContacto->setVisible("true");
        ui->txtDescripcionM1->setReadOnly(false);
        ui->txtDescripcionM2->setReadOnly(false);
        ui->txtDescripcionT1->setReadOnly(false);
        ui->txtDescripcionT2->setReadOnly(false);
        ui->txtDescripcionT3->setReadOnly(false);
        ui->txtMovil1->setReadOnly(false);
        ui->txtMovil2->setReadOnly(false);
        ui->txtNombre->setReadOnly(false);
        ui->txtTelefono1->setReadOnly(false);
        ui->txtTelefono2->setReadOnly(false);
        ui->txtTelefono3->setReadOnly(false);
        ui->txtCargo->setReadOnly(false);
        ui->txtemail_contacto->setReadOnly(false);
        ui->txtNombre->setFocus();
    }
}

void frmProveedores::borrar_contacto()
{
    QModelIndex index= ui->tablaContactos->currentIndex();
    if(!index.isValid())
        return;

    QSqlRecord r = modelContactos->record(index.row());
    this->id_contacto = r.value("id").toInt();
    QString error;
    if(!SqlCalls::SqlDelete("personascontactoproveedor",Configuracion_global->groupDB,QString("id = %1").arg(this->id_contacto),error))
    {
        QMessageBox::warning(this,tr("ATENCIÓN:"),
                             tr("Ocurrió un error al borrar el contacto: %1").arg(error),
                             tr("Aceptar"));
    } else
    {
        contactos();
    }
}

void frmProveedores::on_btnBuscar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void frmProveedores::on_btnAnadirEntrega_clicked()
{
    frmAddEntregasCuenta frmEntregas(this);
    if(frmEntregas.exec() == QDialog::Accepted){
        EntregasCuenta oEntrega(this);
        if(!oEntrega.Anadir(2,oProveedor->id,frmEntregas.importe,frmEntregas.fecha,frmEntregas.concepto))
            QMessageBox::warning(this,tr("Gestión de proveedores"),
                                 tr("Falló el insertar una nueva entrega a cuenta"),
                                 tr("Aceptar"));
        else
            historiales();
    }

}

void frmProveedores::on_txtcif_editingFinished()
{
    blockSignals(true);
    if (ui->txtcif->text().length() == 8)
    {
        QString cif = Configuracion::letraDNI(ui->txtcif->text());
        ui->txtcif->setText(cif.toUpper());
    }
    blockSignals(false);
}

void frmProveedores::formato_tabla(QSqlQueryModel *modelo)
{
    // id,codigo,proveedor,cif,telefono1,fax,movil,persona_contacto
   QStringList titulos;
   QVariantList tamanos;
   titulos << tr("id") << tr("código") << tr("proveedor") << tr("cif") << tr("telefono1") << tr("fax");
   titulos <<tr("Movil") << tr("Contacto");
   tamanos << 0 << 120 << 300 << 120 << 120 << 120 <<120 <<250;

   for(int i = 0;i<titulos.size();i++ )
   {
       ui->tabla->setColumnWidth(i,tamanos.at(i).toInt());
       modelo->setHeaderData(i,Qt::Horizontal,titulos.at(i));
   }
   ui->tabla->setColumnHidden(0,true);
}

void frmProveedores::filter_table(QString texto, QString orden, QString modo)
{
    if(ui->stackedWidget->currentIndex() == 0)
        ui->stackedWidget->setCurrentIndex(1);
    QHash <QString, QString> h;
    h["Proveedor"] = "proveedor";
    h["código"] = "codigo";
    h["CIF"] = "cif";
    h["Teléfono"] = "telefono1";
    h["Persona contacto"] = "persona_contacto";
    QString campo = h.value(orden);

    if(modo == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";



    QString cSQL = "select id,codigo,proveedor,cif,telefono1,fax,movil,persona_contacto from proveedores "
            "where " +campo+" like '%"+texto.trimmed()+"%' order by "+campo+" "+modo;

    modelBusqueda->setQuery(cSQL,Configuracion_global->groupDB);
    formato_tabla(modelBusqueda);
    ui->tabla->selectRow(0);
}

void frmProveedores::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);    

    QStringList orden;
    orden  <<tr("Proveedor")<<tr("código") << tr("CIF") <<tr("Teléfono") << tr("Persona contacto");
    m_busqueda->setOrderCombo(orden);

    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    m_busqueda->setModeCombo(modo);

    connect(m_busqueda,SIGNAL(showMe()),this,SLOT(mostrarBusqueda()));
    connect(m_busqueda,SIGNAL(hideMe()),this,SLOT(ocultarBusqueda()));
    connect(m_busqueda,SIGNAL(doSearch(QString,QString,QString)),this,SLOT(filter_table(QString,QString,QString)));

    QPushButton *AddProvider = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir"),this);
    connect(AddProvider,SIGNAL(clicked()),this,SLOT(on_btnAnadir_clicked()));
    m_busqueda->addWidget(AddProvider);

    QPushButton *EditProvider = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar"),this);
    connect(EditProvider,SIGNAL(clicked()),this,SLOT(on_btnEditar_clicked()));
    m_busqueda->addWidget(EditProvider);

    QPushButton *EraseProvider = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar"),this);
    connect(EraseProvider,SIGNAL(clicked()),this,SLOT(on_btnBorrar_clicked()));
    m_busqueda->addWidget(EraseProvider);

    m_busqueda->addSpacer();
    connect(m_busqueda, SIGNAL(key_Down_Pressed()),ui->tabla,SLOT(setFocus()));
}

void frmProveedores::on_tabla_doubleClicked(const QModelIndex &index)
{
    int id = ui->tabla->model()->data(ui->tabla->model()->index(index.row(),0),Qt::EditRole).toInt();
    oProveedor->Recuperar(id);
    LLenarCampos();
    ocultarBusqueda();
    ui->tabWidget->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);
}

void frmProveedores::on_tablaContactos_doubleClicked(const QModelIndex & /*index*/)
{
    editar_contacto();
}

void frmProveedores::mostrarBusqueda()
{
    ui->stackedWidget->setCurrentIndex(1);
    _showBarraBusqueda(m_busqueda);
    m_busqueda->doFocustoText();
}

void frmProveedores::ocultarBusqueda()
{
    _hideBarraBusqueda(m_busqueda);
}

bool frmProveedores::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Show && obj == this)
        init_querys();

    if(event->type() == QEvent::Resize)
    {
        if(!__init)
            init();
        _resizeBarraBusqueda(m_busqueda);
    }
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(ui->stackedWidget->currentIndex()==1)
        {
            if(keyEvent->key() == Qt::Key_Return)
            {
                on_tabla_doubleClicked(ui->tabla->currentIndex());
                return true;
            }
        }
        if(keyEvent->key() == Qt::Key_Escape)
            return true;

        if(keyEvent->key() == Qt::Key_F1)
        {
            if(ui->btnEditar->isEnabled())
            {
                if(m_busqueda->isShow())
                    ocultarBusqueda();
                else
                    mostrarBusqueda();
                return true;
            }
        }
    }
    return MayaModule::eventFilter(obj,event);
}

void frmProveedores::on_txtdireccion2_editingFinished()
{
    if(!ui->txtprovincia->text().isEmpty())
        ui->txttelefono1->setFocus();
}

void frmProveedores::on_btnAbrirWeb_clicked()
{
    if(!ui->txtweb->text().isEmpty())
    {
        if(ui->txtweb->text().startsWith("http"))
            QDesktopServices::openUrl(QUrl(ui->txtweb->text()));
        else
            QDesktopServices::openUrl(QUrl(QString("http://%1").arg(ui->txtweb->text())));
    }
}

void frmProveedores::on_txtcodigoFormaPago_currentIndexChanged(int index)
{
    ui->txtforma_pago->setText(Configuracion_global->formapago_model->record(index).value("forma_pago").toString());
}

void frmProveedores::on_rdbPendientes_toggled(bool /*checked*/)
{
    llenar_tabPagos();
}

void frmProveedores::on_rdbPagados_toggled(bool /*checked*/)
{
    llenar_tabPagos();
}

void frmProveedores::on_rdbTodos_toggled(bool /*checked*/)
{
    llenar_tabPagos();
}
