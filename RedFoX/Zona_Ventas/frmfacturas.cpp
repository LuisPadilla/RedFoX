#include "frmfacturas.h"
#include "ui_frmfacturas.h"
#include "../Busquedas/db_consulta_view.h"
#include "../Zona_Contabilidad/apuntes.h"
#include "Auxiliares/frmaddentregascuenta.h"
#include "Auxiliares/monetarydelegate.h"
#include "Auxiliares/monetarydelegate_totals.h"
#include "Auxiliares/numericdelegate.h"
#include "Auxiliares/datedelegate.h"
#include "../Almacen/frmarticulos.h"
#include "../Zona_Ventas/frmgestioncobros.h"
#include "../vencimientos.h"
#include "Auxiliares/frmeditline.h"


#include "../Almacen/articulo.h"
#include "frmlistadofac.h"
#include "frmcobrarfactura.h"

void frmFacturas::formato_tabla_lineas()
{
    QStringList header;
    QVariantList sizes;
    header << tr("id") << "id_art" << tr("Código") << tr("Descripción") << tr("cantidad") << tr("pvp") << tr("pvp recom.") << tr("Subtotal");
    header << tr("porc_dto") << tr("porc_iva") << tr("Total");
    sizes << 0 << 0 << 100 << 300 << 100 << 100 <<100 << 100 <<100 << 100 <<110;
    for(int i = 0; i <header.size();i++)
    {
        ui->Lineas->setColumnWidth(i,sizes.at(i).toInt());
        modelLineas->setHeaderData(i,Qt::Horizontal,header.at(i));
    }
    ui->Lineas->setColumnHidden(0,true);
    ui->Lineas->setColumnHidden(1,true);
}

void frmFacturas::init_querys()
{
    if(!__init)
        return;
    model_series->setQuery("select serie from series",Configuracion_global->empresaDB);

    modelLineas->setQuery("select id,id_articulo,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                          "from lin_fac where id = 0;",Configuracion_global->empresaDB);
    formato_tabla_lineas();

    m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                         " where factura <> 'BORRADOR'  and ejercicio = "+Configuracion_global->cEjercicio+
                         " order by fecha+serie+factura desc limit 0,500",Configuracion_global->empresaDB);
    series2_l.clear();
    series2_l.append(tr("TODAS"));
    for(auto i=0;i< model_series->rowCount();i++)
        series2_l.append(model_series->record(i).value("serie").toString());
    cboSeries->addItems(series2_l);

    formato_tabla_facturas();
    ui->tabWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(1);
}

void frmFacturas::init()
{
    if(__init)
        return;
    ui->setupUi(this);
    oFactura  = new Factura(this);
    oCliente1 = new Cliente(this);
    oArticulo = new Articulo(this);

    ui->Lineas->setItemDelegateForColumn(3,new NumericDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(4,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(5,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(6,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(7,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(8,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(9,new MonetaryDelegate_totals(this,true));

    ui->tabla_facturas->setItemDelegateForColumn(6, new MonetaryDelegate(this));
    ui->tabla_facturas->setItemDelegateForColumn(3,new DateDelegate(this));
    ui->tabla_facturas->setItemDelegateForColumn(4, new DateDelegate(this));

    modelLineas       = new QSqlQueryModel(this);
    lineas_anterior   = new QSqlQueryModel(this);
    m_facturas        = new QSqlQueryModel(this);
    model_series      = new QSqlQueryModel(this);
    model_dir_entrega = new QSqlQueryModel(this);
    cboSeries = new QComboBox(this);

    setUpBusqueda();

    ui->tabWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(1);
    this->Altas = false;

    // Escondo/muestro campos según configuración //FIXME ocultar IRPF??
    //ui->txtimporte_irpf->setVisible(Configuracion_global->lProfesional);
    //ui->txtimporte_irpf_2->setVisible(Configuracion_global->lProfesional);
    //ui->lblIRPF_2->setVisible(Configuracion_global->lProfesional);
    //ui->lblIRPF_3->setVisible(Configuracion_global->lProfesional);
    ui->lbcontabilizada->setVisible(false);
    ui->lblFacturaCobrada->setVisible(false);
    ui->lblFacturaImpresa->setVisible(false);


    ui->comboPais->setModel(Configuracion_global->paises_model);
    ui->cboPais_entrega->setModel(Configuracion_global->paises_model);
    ui->cboforma_pago->setModel(Configuracion_global->formapago_model);
    ui->cboDivisa->setModel(Configuracion_global->divisas_model);
    ui->cbo_serie->setModel(model_series);
    ui->cboDireccionesEntrega->setModel(model_dir_entrega);

    ui->comboPais->setModelColumn(1);
    ui->cboPais_entrega->setModelColumn(1);
    ui->cboforma_pago->setModelColumn(2);
    ui->cboDivisa->setModelColumn(1);
    ui->cboDireccionesEntrega->setModelColumn(1);

    ui->tabla_facturas->setModel(m_facturas);
    ui->Lineas->setModel(modelLineas);

    ui->cbo_porc_gasto_iva1->addItems(Configuracion_global->ivaList);
    ui->cbo_porc_gasto_iva2->addItems(Configuracion_global->ivaList);
    ui->cbo_porc_gasto_iva3->addItems(Configuracion_global->ivaList);
    ui->cbo_porc_gasto_iva1->setCurrentIndex(0);
    ui->cbo_porc_gasto_iva2->setCurrentIndex(0);
    ui->cbo_porc_gasto_iva3->setCurrentIndex(0);

    // campo busquedas
    h_Buscar["Factura"]="factura";
    h_Buscar["Código cliente"] = "codigo_cliente";
    h_Buscar["Cliente"] = "cliente";
    h_Buscar["Fecha Factura"] = "fecha";
    h_Buscar["CIF / NIF"] = "cif";
    h_Buscar["Total"] = "total";

    ui->txtporc_iva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtporc_iva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtporc_iva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtporc_iva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));

    oFactura->id = -1;

    // Eventos
    QList<QWidget*> l = this->findChildren<QWidget*>();
    QList<QWidget*> ::Iterator it;

    for( it = l.begin() ;it!= l.end();++it )
        (*it)->installEventFilter(this);

    VaciarCampos();
    BloquearCampos(true);
    ui->tabla_facturas->selectRow(0);

    __init = true;
}

frmFacturas::frmFacturas( QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmFacturas),
    //helper(this),
    menuButton(QIcon(":/Icons/PNG/Factura.png"),tr("Facturas"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/Factura.png"),"",this))

{
    __init = false;
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de facturas a clientes"));
    this->installEventFilter(this);
}

frmFacturas::~frmFacturas()
{
    delete ui;
}

void frmFacturas::LLenarCampos()
{
    if(!__init)
        return;
    ui->lblFactura->setText(oFactura->factura.replace("\n","").replace("\r",""));
    ui->btnBorrar->setVisible(ui->lblFactura->text() == tr("BORRADOR"));
    ui->lblCliente->setText(oFactura->cliente);
    ui->txtcodigo_cliente->setText(oFactura->codigo_cliente);
    ui->txtfactura->setText(oFactura->factura);

    int index = ui->cbo_serie->findText(oFactura->serie);
    ui->cbo_serie->setCurrentIndex(index);

    ui->txtfecha->setDate(oFactura->fecha);
    ui->txtfecha_cobro->setDate(oFactura->fecha_cobro);
    ui->txtcliente->setText(oFactura->cliente);
    ui->txtdireccion1->setText(oFactura->direccion1);
    ui->txtdireccion2->setText(oFactura->direccion2);
    ui->txtcp->setText(oFactura->cp);
    ui->txtpoblacion->setText(oFactura->poblacion);
    ui->txtprovincia->setText(oFactura->provincia);

    for(int i =0;i<Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == oFactura->id_pais)
        {
            int ind_pais = ui->comboPais->findText(Configuracion_global->paises_model->record(i).value("pais").toString());
            ui->comboPais->setCurrentIndex(ind_pais);
            break;
        }
        else
        {
            ui->comboPais->setCurrentIndex(-1);
        }
    }

    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->dto,'f',Configuracion_global->decimales)));
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number( oFactura->base,'f',Configuracion_global->decimales)));
    ui->txtiva_total->setText(Configuracion_global->toFormatoMoneda(QString::number( oFactura->iva,'f',Configuracion_global->decimales)));
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total_recargo,'f',Configuracion_global->decimales)));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva,'f',
                                                                          Configuracion_global->decimales_campos_totales)));
    ui->txtcif->setText(oFactura->cif);
    ui->chkrecargo_equivalencia->setChecked(oFactura->recargo_equivalencia);

    ui->spinporc_irpf->setValue(oFactura->porc_irpf);
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda( QString::number(oFactura->subtotal,'f',Configuracion_global->decimales)));                
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total,'f',Configuracion_global->decimales)));

    ui->lblFacturaImpresa->setVisible(oFactura->impreso);
    ui->lblFacturaCobrada->setVisible(oFactura->cobrado);
    ui->btnCobrar->setVisible(!oFactura->cobrado);
    ui->lbcontabilizada->setVisible(oFactura->contablilizada);

    if(Configuracion_global->formapago_model->rowCount() >0)
    {
        for(int i =0;i<Configuracion_global->formapago_model->rowCount();i++)
        {
            if(Configuracion_global->formapago_model->record(i).value("id").toInt() == oFactura->id_forma_pago)
            {
                ui->cboforma_pago->setCurrentIndex(i);
                break;
            }
            else
            {
                ui->cboforma_pago->setCurrentIndex(-1);
            }

        }
    }

    ui->txtcomentario->setText(oFactura->comentario);
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->base1,'f',Configuracion_global->decimales)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->base2,'f',Configuracion_global->decimales)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->base3,'f',Configuracion_global->decimales)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->base4,'f',Configuracion_global->decimales)));
    ui->txtporc_iva1->setText(QString::number(oFactura->porc_iva1));
    ui->txtporc_iva2->setText(QString::number(oFactura->porc_iva2));
    ui->txtporc_iva3->setText(QString::number(oFactura->porc_iva3));
    ui->txtporc_iva4->setText(QString::number(oFactura->porc_iva4));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva1,'f',Configuracion_global->decimales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva2,'f',Configuracion_global->decimales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva3,'f',Configuracion_global->decimales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva4,'f',Configuracion_global->decimales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total1,'f',Configuracion_global->decimales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total2,'f',Configuracion_global->decimales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total3,'f',Configuracion_global->decimales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total4,'f',Configuracion_global->decimales)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec1,'f',Configuracion_global->decimales)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec2,'f',Configuracion_global->decimales)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec3,'f',Configuracion_global->decimales)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec4,'f',Configuracion_global->decimales)));
    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->rec1,'f',Configuracion_global->decimales)));
    ui->txtrec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->rec2,'f',Configuracion_global->decimales)));
    ui->txtrec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->rec3,'f',Configuracion_global->decimales)));
    ui->txtrec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->rec4,'f',Configuracion_global->decimales)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total_recargo,'f',Configuracion_global->decimales)));
    //ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->entregado_a_cuenta,'f',Configuracion_global->decimales)));
    ui->txtimporte_pendiente->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->importe_pendiente,'f',Configuracion_global->decimales)));
    ui->txtcodigo_entidad->setText(oFactura->codigo_entidad);
    ui->txtoficina_entidad->setText(oFactura->oficina_entidad);
    ui->txtdc_cuenta->setText(oFactura->dc_cuenta);
    ui->txtnumero_cuenta->setText(oFactura->cuenta_corriente);
    ui->txtpedido_cliente->setText(QString::number(oFactura->pedido_cliente));

    if (oFactura->porc_irpf != 0)
    {
        ui->lblIRPF_3->setVisible(true);
        ui->lblIrpf->setVisible(true);
        ui->spinporc_irpf->setVisible(true);
    } else {
        ui->lblIrpf->setVisible(false);
        ui->lblIRPF_3->setVisible(false);
        ui->spinporc_irpf->setVisible(false);
    }

    ui->lblFacturaCobrada->setVisible(oFactura->cobrado);
    ui->txtimporte_irpf->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->irpf,'f',Configuracion_global->decimales)));
    ui->txtimporte_irpf_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->irpf,'f',Configuracion_global->decimales)));


    oCliente1->Recuperar("Select * from clientes where id ="+QString::number(oFactura->id_cliente),false);
    //ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente1->importe_a_cuenta,'f',Configuracion_global->decimales)));

    ui->txtTransportista->setText( Configuracion_global->devolver_transportista(oFactura->id_transportista));
    ui->txtAsiento->setText(QString::number(oFactura->apunte));
    QString error;
    ui->txt_tarifa->setText(SqlCalls::SelectOneField("codigotarifa","descripcion",QString("id=%1").arg(oFactura->tarifa_cliente),
                                                     Configuracion_global->groupDB,error).toString());
    ui->txtGastoDist1->setText(oFactura->desc_gasto1);
    ui->txtGastoDist2->setText(oFactura->desc_gasto2);
    ui->txtGastoDist3->setText(oFactura->desc_gasto3);
    ui->SpinGastoDist1->setValue(oFactura->imp_gasto1);
    ui->SpinGastoDist2->setValue(oFactura->imp_gasto2);
    ui->SpinGastoDist3->setValue(oFactura->imp_gasto3);

    index = ui->cbo_porc_gasto_iva1->findText(QString::number(oFactura->porc_iva_gasto1));
    ui->cbo_porc_gasto_iva1->setCurrentIndex(index);
    index = ui->cbo_porc_gasto_iva2->findText(QString::number(oFactura->porc_iva_gasto2));
    ui->cbo_porc_gasto_iva2->setCurrentIndex(index);
    index = ui->cbo_porc_gasto_iva3->findText(QString::number(oFactura->porc_iva_gasto3));
    ui->cbo_porc_gasto_iva3->setCurrentIndex(index);

    ui->spin_iva_gasto1->setValue(oFactura->iva_gasto1);
    ui->spin_iva_gasto2->setValue(oFactura->iva_gasto2);
    ui->spin_iva_gasto3->setValue(oFactura->iva_gasto3);

    ui->txtbase_total->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->base,'f',
                                                                                     Configuracion_global->decimales_campos_totales)));
    ui->txtiva_total->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva,'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total_recargo,'f',
                                                                                        Configuracion_global->decimales_campos_totales)));
    ui->txttotal_total->setText(Configuracion_global->toFormatoMoneda(QString::number((oFactura->total),'f',
                                                                                      Configuracion_global->decimales_campos_totales)));


    // DIRECCION ALTERNATIVA    
    model_dir_entrega->setQuery(QString("Select * from cliente_direcciones where id_cliente = %1").arg(oCliente1->id));

    ui->txtDireccion1_entrega->setText(oFactura->direccion1_entrega);
    ui->txtDireccion2_entrega->setText(oFactura->direccion2_entrega);
    ui->txtCp_entrega->setText(oFactura->cp_entrega);
    ui->txtPoblacion_entrega->setText(oFactura->poblacion_entrega);
    ui->txtProvincia_entrega->setText(oFactura->poblacion_entrega);
    for(int i =0;i<Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == oFactura->id_pais)
        {                      
            ui->cboPais_entrega->setCurrentIndex(i);
            break;
        }
        else
        {
            ui->cboPais_entrega->setCurrentIndex(-1);
        }
    }


    // DIVISA FACTURA

    for(int i =0;i<Configuracion_global->divisas_model->rowCount();i++)
    {
        if(Configuracion_global->divisas_model->record(i).value("id").toInt() == oFactura->id_divisa)
        {
            int ind_divisa = ui->cboDivisa->findText(Configuracion_global->divisas_model->record(i).value("moneda").toString());
            ui->cboDivisa->setCurrentIndex(ind_divisa);
            break;
        }
        else
        {
            ui->cboDivisa->setCurrentIndex(-1);
        }
    }

    //----------------------------
    // Lineas Facturas
    //----------------------------
    modelLineas->setQuery(QString("select id,id_articulo,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                          "from lin_fac where id_cab = %1;").arg(oFactura->id),Configuracion_global->empresaDB);

}

void frmFacturas::LLenarCamposCliente()
{
    if(!__init)
        return;
    ui->lblCliente->setText(oCliente1->nombre_fiscal);
    ui->txtcodigo_cliente->setText(oCliente1->codigo_cliente);
    ui->txtcliente->setText(oCliente1->nombre_fiscal);
    ui->txtdireccion1->setText(oCliente1->direccion1);
    ui->txtdireccion2->setText(oCliente1->direccion2);
    ui->txtcp->setText(oCliente1->cp);
    ui->txtpoblacion->setText(oCliente1->poblacion);
    ui->txtprovincia->setText(oCliente1->provincia);

    for(int i =0;i<Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == oCliente1->id_pais)
        {
            int ind_pais = ui->comboPais->findText(Configuracion_global->paises_model->record(i).value("pais").toString());
            ui->comboPais->setCurrentIndex(ind_pais);
            break;
        }
    }

    ui->txtcif->setText(oCliente1->cif_nif);
    ui->chkrecargo_equivalencia->setChecked(oCliente1->recargo_equivalencia);

    if(oCliente1->recargo_equivalencia)
    {        
        ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->reList.at(0)));
        ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->reList.at(1)));
        ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->reList.at(2)));
        ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->reList.at(3)));
    }
    else
    {
        ui->txtporc_rec1->setText("0.00");
        ui->txtporc_rec2->setText("0.00");
        ui->txtporc_rec3->setText("0.00");
        ui->txtporc_rec4->setText("0.00");
    }

    QString error;
    //ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente1->importe_a_cuenta,'f',Configuracion_global->decimales)));
    ui->txt_tarifa->setText(SqlCalls::SelectOneField("codigotarifa","descripcion",QString("id=%1").arg(oCliente1->idTarifa),
                                                     Configuracion_global->groupDB,error).toString());


    oFactura->id_cliente = oCliente1->id;
    oFactura->id_transportista = oCliente1->id_transportista;
    ui->txtTransportista->setText( Configuracion_global->devolver_transportista(oCliente1->id_transportista));

    for(int i =0;i<Configuracion_global->divisas_model->rowCount();i++)
    {
        if(Configuracion_global->divisas_model->record(i).value("id").toInt() == oCliente1->id_divisa)
        {
            ui->cboDivisa->setCurrentIndex(i);
            break;
        } else
        {
            ui->cboDivisa->setCurrentIndex(-1);
        }

    }
    for(int i =0;i<Configuracion_global->formapago_model->rowCount();i++)
    {
        if(Configuracion_global->formapago_model->record(i).value("id").toInt() == oCliente1->id_forma_pago)
        {
            ui->cboforma_pago->setCurrentIndex(i);
            break;
        }
        else
        {
            ui->cboforma_pago->setCurrentIndex(-1);
        }
    }

    oFactura->id_forma_pago = oCliente1->id_forma_pago;
    oFactura->tarifa_cliente = oCliente1->idTarifa;

    model_dir_entrega->setQuery(QString("Select * from cliente_direcciones where id_cliente = %1").arg(oCliente1->id));
    ui->cboDireccionesEntrega->setCurrentIndex(0);
}

void frmFacturas::VaciarCampos()
{
    QDate fecha;
    ui->lblCliente->setText("");
    ui->lblFactura->setText("");
    ui->txtcodigo_cliente->setText("");
    ui->txtfactura->setText("");
    ui->txtfecha->setDate(fecha.currentDate());
    ui->txtfecha_cobro->setDate(fecha.currentDate());
    ui->txtcliente->setText("");
    ui->txtdireccion1->setText("");
    ui->txtdireccion2->setText("");
    ui->txtcp->setText("");
    ui->txtpoblacion->setText("");
    ui->txtprovincia->setText("");
    ui->comboPais->setCurrentIndex(-1);
    ui->txtcif->setText("");
    ui->txtsubtotal->setText(0);
    ui->txtimporte_descuento->setText("0,00");
    //ui->txtimporte_descuento_pp->setText("0,00");
    ui->txtbase->setText("0,00");
    ui->txtiva_total->setText(0);
    ui->txtiva->setText("0,00");
    ui->txttotal->setText("0,00");
    ui->lblFacturaImpresa->setVisible(false);
    ui->lblFacturaCobrada->setVisible(false);
    ui->lbcontabilizada->setVisible(false);
    ui->cboforma_pago->setCurrentIndex(0);
    ui->txtcomentario->setText("");
    ui->txtbase1->setText(0);
    ui->txtbase2->setText(0);
    ui->txtbase3->setText(0);
    ui->txtbase4->setText(0);

    ui->txtporc_iva1->setText(QString::number(Configuracion_global->ivaList.at(0).toFloat()));
    ui->txtporc_iva2->setText(QString::number(Configuracion_global->ivaList.at(1).toFloat()));
    ui->txtporc_iva3->setText(QString::number(Configuracion_global->ivaList.at(2).toFloat()));
    ui->txtporc_iva4->setText(QString::number(Configuracion_global->ivaList.at(3).toFloat()));
    ui->txtporc_rec1->setText(QString::number(Configuracion_global->reList.at(0).toFloat()));
    ui->txtporc_rec2->setText(QString::number(Configuracion_global->reList.at(1).toFloat()));
    ui->txtporc_rec3->setText(QString::number(Configuracion_global->reList.at(2).toFloat()));
    ui->txtporc_rec4->setText(QString::number(Configuracion_global->reList.at(3).toFloat()));

    ui->txtiva1->setText(0);
    ui->txtiva2->setText(0);
    ui->txtiva3->setText(0);
    ui->txtiva4->setText(0);
    ui->txttotal1->setText(0);
    ui->txttotal2->setText(0);
    ui->txttotal3->setText(0);
    ui->txttotal4->setText(0);
    ui->txtporc_rec1->setText(0);
    ui->txtporc_rec2->setText(0);
    ui->txtporc_rec3->setText(0);
    ui->txtporc_rec4->setText(0);
    ui->txttotal_recargo->setText(0);
    //ui->txtentregado_a_cuenta->setText("0,00");
    ui->txtimporte_pendiente->setText("0,00");
    ui->txtcodigo_entidad->setText("");
    ui->txtoficina_entidad->setText("");
    ui->txtdc_cuenta->setText("");
    ui->txtnumero_cuenta->setText("");
    ui->txtpedido_cliente->setText("");
    ui->chkrecargo_equivalencia->setCheckState(Qt::Unchecked);
    ui->txtGastoDist1->setText(tr("Portes"));
    ui->txtGastoDist2->setText("");
    ui->txtGastoDist3->setText("");
    ui->SpinGastoDist1->setValue(0);
    ui->SpinGastoDist2->setValue(0);
    ui->SpinGastoDist3->setValue(0);
    ui->cbo_porc_gasto_iva1->setCurrentIndex(ui->cbo_porc_gasto_iva1->findText(Configuracion_global->ivaList.at(0)));
    ui->cbo_porc_gasto_iva2->setCurrentIndex(ui->cbo_porc_gasto_iva2->findText(Configuracion_global->ivaList.at(0)));
    ui->cbo_porc_gasto_iva3->setCurrentIndex(ui->cbo_porc_gasto_iva3->findText(Configuracion_global->ivaList.at(0)));
    ui->spin_iva_gasto1->setValue(0);
    ui->spin_iva_gasto2->setValue(0);
    ui->spin_iva_gasto3->setValue(0);
    ui->cboDivisa->setCurrentIndex( -1);
    ui->txtbase_total->clear();
    ui->txtiva_total->clear();
    ui->txttotal_recargo->clear();
    ui->txttotal_total->clear();

}

void frmFacturas::BloquearCampos(bool state)
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

    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(!state);
    }
    // QTextEdit
    QList<QTextEdit *> textEditList = this->findChildren<QTextEdit *>();
    QTextEdit *textEdit;
    foreach (textEdit,textEditList) {
        textEdit->setReadOnly(state);
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(!state);
    }

    // Qdoblespinbox
    QList<QDoubleSpinBox *> SpinEditList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *SpinEdit;
    foreach (SpinEdit, SpinEditList) {
        SpinEdit->setEnabled(!state);
    }

    ui->btnAnadir->setEnabled(state);
    ui->btnAnterior->setEnabled(state);
    ui->btnBuscar->setEnabled(state);
    ui->btnEditar->setEnabled(state);
    ui->btnGuardar->setEnabled(!state);
    ui->btndeshacer->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->botBuscarCliente->setEnabled(!state);
    ui->btnImprimir->setEnabled(state);
    ui->btnCobrar->setEnabled(state);
    ui->btnAnadirLinea->setEnabled(!state);
    ui->btn_borrarLinea->setEnabled(!state);
    //helper.blockTable(state);
    ui->txtfactura->setReadOnly(true);
    ui->btnAsignarTransportista->setEnabled(!state);
    ui->btnBuscar->setEnabled(state);
    ui->spin_iva_gasto1->setEnabled(false);
    ui->spin_iva_gasto2->setEnabled(false);
    ui->spin_iva_gasto3->setEnabled(false);

    m_busqueda->block(!state);

  }

void frmFacturas::LLenarFactura()
{
    oFactura->codigo_cliente = (ui->txtcodigo_cliente->text());
    oFactura->factura = (ui->txtfactura->text());
    oFactura->serie = ui->cbo_serie->currentText();
    oFactura->fecha = (ui->txtfecha->date());
    oFactura->fecha_cobro = (ui->txtfecha_cobro->date());
    oFactura->cliente = (ui->txtcliente->text());
    oFactura->direccion1 = (ui->txtdireccion1->text());
    oFactura->direccion2 = (ui->txtdireccion2->text());
    oFactura->cp = (ui->txtcp->text());
    oFactura->poblacion = (ui->txtpoblacion->text());
    oFactura->provincia = (ui->txtprovincia->text());

    oFactura->id_pais = Configuracion_global->paises_model->record(ui->comboPais->currentIndex()).value("id").toInt();

    oFactura->direccion1_entrega = (ui->txtDireccion1_entrega->text());
    oFactura->direccion2_entrega = (ui->txtDireccion2_entrega->text());
    oFactura->cp_entrega = (ui->txtCp_entrega->text());
    oFactura->poblacion_entrega = (ui->txtPoblacion_entrega->text());
    oFactura->provincia_entrega = (ui->txtProvincia_entrega->text());

    oFactura->id_pais_entrega= Configuracion_global->paises_model->record(ui->cboPais_entrega->currentIndex()).value("id").toInt();
    oFactura->cif = (ui->txtcif->text());  
    oFactura->recargo_equivalencia = ui->chkrecargo_equivalencia->isChecked();

    oFactura->subtotal = (ui->txtsubtotal->text().replace(".","").replace(",",".").replace(moneda,"").toDouble());
    oFactura->porc_dto = (ui->spinPorc_dto->value());
    oFactura->dto = (ui->txtimporte_descuento->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oFactura->porc_dto_pp = (ui->spinPorc_dto_pp->value());
    oFactura->dto = Configuracion_global->MonedatoDouble(ui->txtimporte_descuento->text());
    oFactura->dto_pp = Configuracion_global->MonedatoDouble(ui->txtDtoPP->text());
    oFactura->base = (ui->txtbase->text().replace(".","").replace(",",".").replace(moneda,"").toDouble());
    oFactura->iva = (ui->txtiva->text().replace(".","").replace(",",".").replace(moneda,"").toDouble());
    oFactura->total = (ui->txttotal->text().replace(".","").replace(",",".").replace(moneda,"").toDouble());
    oFactura->forma_pago = (ui->cboforma_pago->currentText());
    oFactura->comentario = (ui->txtcomentario->toPlainText());
    oFactura->base1 = (ui->txtbase1->text().replace(".","").replace(",",".").toDouble());
    oFactura->base2 = (ui->txtbase2->text().replace(".","").replace(",",".").toDouble());
    oFactura->base3 = (ui->txtbase3->text().replace(".","").replace(",",".").toDouble());
    oFactura->base4 = (ui->txtbase4->text().replace(".","").replace(",",".").toDouble());
    oFactura->porc_iva1 = (ui->txtporc_iva1->text().replace(".","").replace(",",".").toDouble());
    oFactura->porc_iva2 = (ui->txtporc_iva2->text().replace(".","").replace(",",".").toDouble());
    oFactura->porc_iva3 = (ui->txtporc_iva3->text().replace(".","").replace(",",".").toDouble());
    oFactura->porc_iva4 = (ui->txtporc_iva4->text().replace(".","").replace(",",".").toDouble());
    oFactura->iva1 = (ui->txtiva1->text().replace(".","").replace(",",".").toDouble());
    oFactura->iva2 = (ui->txtiva2->text().replace(".","").replace(",",".").toDouble());
    oFactura->iva3 = (ui->txtiva3->text().replace(".","").replace(",",".").toDouble());
    oFactura->iva4 = (ui->txtiva4->text().replace(".","").replace(",",".").toDouble());
    oFactura->total1 = (ui->txttotal1->text().replace(".","").replace(",",".").toDouble());
    oFactura->total2 = (ui->txttotal2->text().replace(".","").replace(",",".").toDouble());
    oFactura->total3 = (ui->txttotal3->text().replace(".","").replace(",",".").toDouble());
    oFactura->total4 = (ui->txttotal4->text().replace(".","").replace(",",".").toDouble());
    oFactura->porc_rec1 = Configuracion_global->MonedatoDouble(ui->txtporc_rec1->text());
    oFactura->porc_rec2 = Configuracion_global->MonedatoDouble(ui->txtporc_rec2->text());
    oFactura->porc_rec3 = Configuracion_global->MonedatoDouble(ui->txtporc_rec3->text());
    oFactura->porc_rec4 = Configuracion_global->MonedatoDouble(ui->txtporc_rec4->text());
    oFactura->rec1 = Configuracion_global->MonedatoDouble(ui->txtrec1->text());
    oFactura->rec2 = Configuracion_global->MonedatoDouble(ui->txtrec2->text());
    oFactura->rec3 = Configuracion_global->MonedatoDouble(ui->txtrec3->text());
    oFactura->rec4 = Configuracion_global->MonedatoDouble(ui->txtrec4->text());
    oFactura->total_recargo = (ui->txttotal_recargo->text().replace(".","").replace(",",".").toDouble());
    //oFactura->entregado_a_cuenta = (ui->txtentregado_a_cuenta->text().replace(".","").replace(",",".").toDouble());
    oFactura->importe_pendiente = (ui->txtimporte_pendiente->text().replace(".","").replace(",",".").toDouble());
    oFactura->codigo_entidad = (ui->txtcodigo_entidad->text());
    oFactura->oficina_entidad = (ui->txtoficina_entidad->text());
    oFactura->dc_cuenta = (ui->txtdc_cuenta->text());
    oFactura->cuenta_corriente = (ui->txtnumero_cuenta->text());
    oFactura->pedido_cliente = ui->txtpedido_cliente->text().toInt();
    oFactura->irpf = (ui->txtimporte_irpf->text().replace(".","").replace(",",".").toDouble());
    oFactura->desc_gasto1 = ui->txtGastoDist1->text();
    oFactura->desc_gasto2 = ui->txtGastoDist2->text();
    oFactura->desc_gasto3 = ui->txtGastoDist3->text();
    oFactura->imp_gasto1 = ui->SpinGastoDist1->value();
    oFactura->imp_gasto2 = ui->SpinGastoDist2->value();
    oFactura->imp_gasto3 = ui->SpinGastoDist3->value();
    oFactura->porc_iva_gasto1 = ui->cbo_porc_gasto_iva1->currentText().toFloat();
    oFactura->porc_iva_gasto2 = ui->cbo_porc_gasto_iva2->currentText().toFloat();
    oFactura->porc_iva_gasto3 = ui->cbo_porc_gasto_iva1->currentText().toFloat();
    oFactura->iva_gasto1 = ui->spin_iva_gasto1->value();
    oFactura->iva_gasto2 = ui->spin_iva_gasto2->value();
    oFactura->iva_gasto3 = ui->spin_iva_gasto3->value();

    oFactura->id_divisa  = Configuracion_global->divisas_model->record(ui->cboDivisa->currentIndex()).value("id").toInt();
    oFactura->tarifa_cliente = oCliente1->idTarifa;
    oFactura->id_forma_pago = Configuracion_global->formapago_model->record(ui->cboforma_pago->currentIndex()).value("id").toInt();
}

void frmFacturas::on_btnSiguiente_clicked()
{
    QString cSQL = QString("Select * from cab_fac where factura > '%1' and ejercicio = '%2' order by factura limit 0,1")
            .arg(oFactura->factura)
            .arg(Configuracion_global->cEjercicio);

    if(oFactura->RecuperarFactura(cSQL))
    {
        LLenarCampos();
    }
    else
    {
        TimedMessageBox::Box(this,tr("Se ha llegado al final del archivo.\n"
                                                          "No hay mas facturas disponibles"));
    }
}
void frmFacturas::on_btnAnterior_clicked()
{
    QString cSQL = QString("Select * from cab_fac where factura < '%1' and ejercicio = '%2' order by factura desc limit 0,1")
            .arg(oFactura->factura)
            .arg(Configuracion_global->cEjercicio);
    if(oFactura->RecuperarFactura(cSQL))
    {
        LLenarCampos();
    }
    else
    {
        TimedMessageBox::Box(this,tr("Se ha llegado al final del archivo.\n"
                                                          "No hay mas presupuestos disponibles"));
    }
}

void frmFacturas::on_btnAnadir_clicked()
{
    Configuracion_global->empresaDB.transaction();
    Configuracion_global->groupDB.transaction();
    if(Configuracion_global->contabilidad)
        Configuracion_global->contaDB.transaction();

    if(oFactura->AnadirFactura())
    {
        m_busqueda->hideMe();
        BloquearCampos(false);
        in_edit = false;

        VaciarCampos();

        ui->chkrecargo_equivalencia->setCheckState(Qt::Unchecked);
        ui->btnCobrar->setVisible(true);
        ui->txtcodigo_cliente->setFocus();

        ui->spinporc_irpf->setValue(oFactura->porc_irpf);
        modelLineas->setQuery(QString("select id,id_articulo,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                                      "from lin_fac where id_cab = %1;").arg(oFactura->id),Configuracion_global->empresaDB);

        calcular_factura();
        ui->Lineas->setFocus();

        ui->txtGastoDist1->setText(tr("Portes:"));
        ui->stackedWidget->setCurrentIndex(0);
        ui->tabWidget_2->setCurrentIndex(0);
        ui->cbo_serie->setCurrentIndex(0);

        emit block();
    }
    else
    {
        Configuracion_global->empresaDB.rollback();
        Configuracion_global->groupDB.rollback();
        if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.rollback();
    }
}


void frmFacturas::on_botBuscarCliente_clicked()
{
    blockSignals(true);
    oCliente1->clear();
    db_consulta_view consulta(this);
    QStringList campos;
    campos <<"nombre_fiscal" <<"codigo_cliente" << "cif_nif"<< "poblacion" << "telefono1";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("clientes");
    consulta.set_db("Maya");
    consulta.set_SQL("select id,codigo_cliente,nombre_fiscal,cif_nif,poblacion,telefono1 from clientes");
    QStringList cabecera;
    QVariantList tamanos;
    cabecera  << tr("Código") << tr("Nombre") << tr("CIF/NIF") << tr("Población") << tr("Teléfono");
    tamanos <<"0" << "100" << "300" << "100" << "180" <<"130";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de Clientes");
    if(consulta.exec())
    {
        int id = consulta.get_id();
        oCliente1->Recuperar("select * from clientes where id="+QString::number(id));
        if(oCliente1->bloqueado)
        {
            QMessageBox::warning(this,tr("¡CLIENTE BLOQUEADO!"),tr("NO SE PUEDE VENDER A ESTE CLIENTE:\n%1").arg(oCliente1->comentario_bloqueo));
            blockSignals(false);
            return;
        }
        oFactura->id_cliente = id;
        LLenarCamposCliente();
    }
    blockSignals(false);
}

void frmFacturas::on_btnBuscar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    mostrarBusqueda();
}

void frmFacturas::on_btnImprimir_clicked()
{
    if(ui->stackedWidget->currentIndex()==1)
    {
        if(!ui->tabla_facturas->currentIndex().isValid())
            return;
        oFactura->RecuperarFactura(m_facturas->record(ui->tabla_facturas->currentIndex().row()).value("id").toInt());
    }
    oCliente1->Recuperar(oFactura->id_cliente);
    FrmDialogoImprimir dlg_print(this);
    dlg_print.set_email(oCliente1->email);
    dlg_print.set_preview(false);
    if(dlg_print.exec() == dlg_print.Accepted)
    {
        bool ok = oFactura->set_impresa(true);
        if(ok)
        {
            ui->lblFacturaImpresa->setVisible(true);
            int valor = dlg_print.get_option();
            QMap <QString,QString> parametros_sql;
            parametros_sql["General.empresas"] = QString("id = %1").arg(Configuracion_global->idEmpresa);
            parametros_sql["Empresa.cab_fac"] = QString("id = %1").arg(oFactura->id);
            parametros_sql["Empresa.lin_fac"] = QString("id_cab = %1").arg(oFactura->id);
            QString report = "factura_"+QString::number(oCliente1->ididioma);


            QMap <QString,QString> parametros;
            //TODO parametros
            switch (valor) {
            case 1: // Impresora
                Configuracion::ImprimirDirecto(report,parametros_sql,parametros);
                break;
            case 2: // email
                Configuracion::EviarMail(report,parametros_sql,parametros,"Paga_ya",dlg_print.get_email(),oCliente1->nombre,"Haber si pagas nen","¡Te envio esta factura para que te acuerdes que tienes deudas con nosotros!\n:P");
                break;
            case 3: // PDF
                Configuracion::ImprimirPDF(report,parametros_sql,parametros);
                break;
            case 4: //preview
                Configuracion::ImprimirPreview(report,parametros_sql,parametros);
                break;
            default:
                break;
            }

        }
    }
}

void frmFacturas::mostrar_factura()
{
    if(ui->tabla_facturas->currentIndex().isValid())
        on_tabla_facturas_doubleClicked(ui->tabla_facturas->currentIndex());
}

void frmFacturas::refrescar_modelo()
{
    if(!__init)
        return;
    modelLineas->setQuery(QString("select id,id_articulo,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                              "from lin_fac where id_cab = %1;").arg(oFactura->id),Configuracion_global->empresaDB);
    calcular_factura();
}

void frmFacturas::on_btnEditar_clicked()
{
    if (oFactura->editable)
    {
        BloquearCampos(false);
        m_busqueda->hideMe();
        in_edit = true;
        emit block();
        // ----------------------------
        // Guardo datos para acumulados
        //-----------------------------

        total_anterior = oFactura->base;
        fecha_anterior = oFactura->fecha;
        lineas_anterior->setQuery(QString("select id,id_articulo,cantidad,total from lin_fac where id_cab = %1;").arg(oFactura->id),Configuracion_global->empresaDB);

        Configuracion_global->empresaDB.transaction();
        Configuracion_global->groupDB.transaction();
        if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.transaction();

        ui->txtcodigo_cliente->setFocus();
        ui->stackedWidget->setCurrentIndex(0);
    } else
        QMessageBox::warning(this,tr("Facturas a clientes"),tr("No se puede editar la factura"),tr("Aceptar"));

}

void frmFacturas::desbloquear_factura()
{
    if(!ui->tabla_facturas->currentIndex().isValid())
        return;
    if(QMessageBox::question(this,tr("Gestion de facturas"),tr("¿Desbloquear factura?"),
                             tr("No"),tr("Si")) == QMessageBox::Accepted)
    {
        QModelIndex Model = ui->tabla_facturas->currentIndex();
        int id_fact = ui->tabla_facturas->model()->data(ui->tabla_facturas->model()->index(Model.row(),0)).toInt();
        QHash <QString, QVariant> h;
        h["editable"] = 1;
        QString error;
        bool success = SqlCalls::SqlUpdate(h,"cab_fac",Configuracion_global->empresaDB,QString("id=%1").arg(id_fact),error);
        if(!success)
            QMessageBox::warning(this,tr("Edicion de facturas"),tr("No se pudo desbloquear: %1").arg(error),
                                 tr("Aceptar"));

    }
}

bool frmFacturas::crear_asiento()
{
    bool creado;
    oCliente1->Recuperar("select * from clientes where id = "+QString::number(oFactura->id_cliente));
    if(oFactura->apunte == 0)
        creado = oFactura->Apunte();
    else
        creado = oFactura->EditApunte(oFactura->apunte);
    return creado;
}

void frmFacturas::on_btndeshacer_clicked()
{
    Configuracion_global->groupDB.rollback();
    Configuracion_global->empresaDB.rollback();
    if(Configuracion_global->contabilidad)
        Configuracion_global->contaDB.rollback();
//    oFactura->RecuperarFactura(oFactura->id);
    LLenarCampos();
    BloquearCampos(true);
    emit unblock();
}

void frmFacturas::on_txtcodigo_cliente_editingFinished()
{
    //FIXME comprobar estos acumulados
    if(ui->txtcodigo_cliente->text() != oFactura->codigo_cliente)
    {
        oCliente1->decrementar_acumulados(oCliente1->id,oFactura->total,QDate::currentDate());
    }
    if((ui->txtcodigo_cliente->text() != oFactura->codigo_cliente)&& !ui->txtcodigo_cliente->text().isEmpty())
    {
        if(ui->txtcodigo_cliente->text().trimmed().size() < Configuracion_global->digitos_cuentas_contables)
        {
            QString ccod= Configuracion_global->cuenta_clientes;
            QString ccli = ui->txtcodigo_cliente->text().trimmed();
            int tam = Configuracion_global->digitos_cuentas_contables - (ccod.size() + ccli.size());
            QString zeros;
            for(int i = 0; i<tam;i++)
            {
                zeros.append("0");
            }
            ui->txtcodigo_cliente->setText(ccod+zeros+ccli);

        }
        oCliente1->Recuperar("select * from clientes where codigo_cliente='"+ui->txtcodigo_cliente->text()+"'");
        oFactura->id_cliente = oCliente1->id;
        LLenarCamposCliente();
    }
}

void frmFacturas::on_anadirEntrega_clicked()
{
    frmAddEntregasCuenta entregas(this);
    entregas.set_id_cliente(oCliente1->id);
    entregas.set_concepto(tr("A cta ntra factura: ")+ui->txtfactura->text());
    if(entregas.exec() ==QDialog::Accepted)
    {
        oCliente1->Recuperar("select * from clientes where codigo_cliente='"+ui->txtcodigo_cliente->text()+"'");
        //ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente1->importe_a_cuenta)));
    }

}

void frmFacturas::formato_tabla_facturas()
{
    ui->tabla_facturas->setColumnHidden(0,true);
    QVariantList lista;
    QStringList  titulos;
    lista << 0 << 50<< 120 <<120 <<120 <<120 <<120 <<300;
    titulos <<tr("id") <<tr("S") << tr("FACTURA") <<tr("FECHA") << tr("FECHA COBRO") << tr("CIF/NIF") << tr("TOTAL") << tr("CLIENTE");
    for(int pos = 0;pos<lista.size();pos++)
    {
        ui->tabla_facturas->setColumnWidth(pos,lista.at(pos).toInt());
        m_facturas->setHeaderData(pos,Qt::Horizontal,titulos.at(pos));
    }
}

void frmFacturas::filter_table(QString texto, QString orden, QString modo)
{
    if(!__init)
        return;
    this->texto = texto;
    this->orden = orden;
    this->modo = modo;
    this->serie = cboSeries->currentText();

    ui->stackedWidget->setCurrentIndex(1);
    QString indice_tabla = h_Buscar.value(orden);
    if(indice_tabla.isEmpty())
        indice_tabla = "factura";

    if (modo == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";

    QString order;
    if(indice_tabla == "factura")
        order = "fecha+serie+factura";
    else
        order = indice_tabla;


    if(cboSeries->currentText() == tr("TODAS"))
    {
        m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                                 " where "+indice_tabla+
                                 " like '%"+texto+"%'  and ejercicio = "+Configuracion_global->cEjercicio+
                                 " order by "+order +" "+modo,Configuracion_global->empresaDB);
    } else
    {
        m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                                 " where  "+indice_tabla+
                                 " like '%"+texto+"%'  and ejercicio = "+Configuracion_global->cEjercicio+
                                 " and serie ='"+this->serie +"'"
                                 " order by "+order +" "+modo,Configuracion_global->empresaDB);

    }
    //qDebug() << m_facturas->query().lastQuery();
}

bool frmFacturas::eventFilter(QObject *obj, QEvent *event)
{

    if(event->type() == QEvent::Show && obj == this)
    {
        init_querys();
    }

    if (event->type() == QEvent::KeyPress) {

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(obj == ui->Lineas){
            if (keyEvent->key() == Qt::Key_Plus)
            {
                on_btnAnadirLinea_clicked();
            }
            if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
                on_Lineas_doubleClicked(ui->Lineas->currentIndex());

        }
        if(obj == ui->txtcodigo_cliente)
        {
            if(keyEvent->key() == Qt::Key_F1 && ui->btnGuardar->isEnabled())
                on_botBuscarCliente_clicked();
        }

        if(keyEvent->key() == Qt::Key_F1)
        {
            if(ui->btnEditar->isEnabled()){
                if(m_busqueda->isShow())
                    ocultarBusqueda();
                else
                    mostrarBusqueda();
            }
            return true;
        }
        if (obj == ui->tabla_facturas)
            if(keyEvent->key() == Qt::Key_Return)
                on_tabla_facturas_doubleClicked(ui->tabla_facturas->currentIndex());
        if(keyEvent->key() == Qt::Key_Escape)
            return true;
    }
    if(event->type() == QEvent::Resize && obj == this)
    {
        if(!__init)
            init();
        _resizeBarraBusqueda(m_busqueda);
    }
    return MayaModule::eventFilter(obj,event);
}

void frmFacturas::calcular_factura()
{
    if(!__init)
        return;
    double subtotal,dto,dtopp,base,iva,re,total;
    double base1,iva1,re1,total1;
    double base2,iva2,re2,total2;
    double base3,iva3,re3,total3;
    double base4,iva4,re4,total4;
    base1= 0; base2 = 0; base3 =0;base4 = 0;
    iva1=0; iva2 =0; iva3=0; iva4 = 0;
    re1=0;re2=0;re3=0;re4 =0;
    total1 =0; total2 =0;total3=0; total4=0;
    subtotal =0; dto=0; dtopp =0; base =0; iva =0; re =0; total =0;

    double _dtoPP = 1-((ui->spinPorc_dto_pp->value()+ui->spinPorc_dto->value())/100);

    QMap <int,QSqlRecord> l;
    QString error;
    l = SqlCalls::SelectRecord("lin_fac",QString("id_cab=%1").arg(oFactura->id),Configuracion_global->empresaDB,error);
    QMapIterator <int,QSqlRecord> li(l);
    while(li.hasNext())
    {
        li.next();
        subtotal += li.value().value("subtotal").toDouble();
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva1->text().toFloat())
        {
            // base1
            base1 += li.value().value("total").toDouble()* _dtoPP;
        }
        // base2
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva2->text().toFloat())
        {
            base2 += li.value().value("total").toDouble()* _dtoPP;

        }
        // base3
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva3->text().toFloat())
        {
            base3 += li.value().value("total").toDouble()* _dtoPP;

        }
        // base4
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva4->text().toFloat())
        {
            base4 += li.value().value("total").toDouble()* _dtoPP;
        }
    }
    if(ui->chkrecargo_equivalencia->isChecked())
    {
        re1 = base1 * (Configuracion_global->MonedatoDouble(ui->txtporc_rec1->text())/100);
        re2 = base2 * (Configuracion_global->MonedatoDouble(ui->txtporc_rec2->text())/100);
        re3 = base3 * (Configuracion_global->MonedatoDouble(ui->txtporc_rec3->text())/100);
        re4 = base4 * (Configuracion_global->MonedatoDouble(ui->txtporc_rec4->text())/100);
    }

    // añadir gastos extras
    if(ui->cbo_porc_gasto_iva1->currentText().toFloat() == ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist1->value();
    if(ui->cbo_porc_gasto_iva2->currentText().toFloat() == ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist2->value();
    if(ui->cbo_porc_gasto_iva3->currentText().toFloat()== ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist3->value();
    iva1 = base1 * (ui->txtporc_iva1->text().toFloat()/100);    

    if(ui->cbo_porc_gasto_iva1->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist1->value();
    if(ui->cbo_porc_gasto_iva2->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist2->value();
    if(ui->cbo_porc_gasto_iva3->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist3->value();
    iva2 = base2 * (ui->txtporc_iva2->text().toFloat()/100);    

    if(ui->cbo_porc_gasto_iva1->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist1->value();
    if(ui->cbo_porc_gasto_iva2->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist2->value();
    if(ui->cbo_porc_gasto_iva3->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist3->value();
    iva3 = base3 * (ui->txtporc_iva3->text().toFloat()/100);    

    if(ui->cbo_porc_gasto_iva1->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist1->value();
    if(ui->cbo_porc_gasto_iva2->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist2->value();
    if(ui->cbo_porc_gasto_iva3->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist3->value();
    iva4 = base4 * (ui->txtporc_iva4->text().toFloat()/100);    

    // TOTALES GENERALES
    dtopp = subtotal *(ui->spinPorc_dto_pp->value()/100.0);
    double _dto = subtotal *(ui->spinPorc_dto->value()/100.0);
    base = base1 + base2+base3+base4;
    iva = iva1 + iva2 +iva3+iva4;
    re  = re1 +re2 + re3 + re4;
    double irpf = base * (ui->spinporc_irpf->value()/100.0);

    total = (base - irpf) + iva +re;
    total1 = base1 +iva1 +re1;
    total2 = base2 +iva2 +re2;
    total3 = base3 +iva3 +re3;
    total4 = base4 +iva4 +re4;

    ui->txtimporte_irpf->setText     (Configuracion_global->toFormatoMoneda(QString::number(irpf ,'f',2)));

    ui->txtDtoPP->setText(Configuracion_global->toFormatoMoneda(QString::number(dtopp,'f',
                                                Configuracion_global->decimales_campos_totales)));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(_dto ,'f',Configuracion_global->decimales_campos_totales)));



    // Desglose llenar controles
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base1,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva1,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re1,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total1,'f',
                                                                 Configuracion_global->decimales_campos_totales)));

    // Desglose llenar controles
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base2,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva2,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtrec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re2,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total2,'f',
                                                                 Configuracion_global->decimales_campos_totales)));
    // Desglose llenar controles
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base3,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva3,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtrec3->setText(Configuracion_global->toFormatoMoneda(QString::number(re3,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total3,'f',
                                                                 Configuracion_global->decimales_campos_totales)));
    // Desglose llenar controles
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base4,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva4,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtrec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re4,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total4,'f',
                                                                 Configuracion_global->decimales_campos_totales)));

    //ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(dtopp,Configuracion_global->decimales_campos_totales)));
    iva = iva1 + iva2 +iva3+iva4;
    re  = re1 +re2 + re3 + re4;
    total = base + iva +re;


    ui->txtimporte_irpf->setText(Configuracion_global->toFormatoMoneda(QString::number(irpf,'f',2)));
    ui->txtimporte_irpf_2->setText(Configuracion_global->toFormatoMoneda(QString::number(irpf,'f',2)));
    this->moneda = moneda;
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    //ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));

    ui->txtbase_total->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva_total->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_total->setText(Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));

}

void frmFacturas::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);

    QStringList orden;
    orden  <<  tr("Factura") << tr("Código cliente") << tr("Cliente") << tr("Fecha Factura") << tr("CIF/NIF") << tr("Total");
    m_busqueda->setOrderCombo(orden);

    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    m_busqueda->setModeCombo(modo);

    connect(m_busqueda,SIGNAL(showMe()),this,SLOT(mostrarBusqueda()));
    connect(m_busqueda,SIGNAL(hideMe()),this,SLOT(ocultarBusqueda()));
    connect(m_busqueda,SIGNAL(doSearch(QString,QString,QString)),this,SLOT(filter_table(QString,QString,QString)));


    QLabel *lblserie = new QLabel(tr("Serie:"));
    cboSeries->addItems(series2_l);
    QHBoxLayout *layoutserie = new QHBoxLayout(this);
    layoutserie->addWidget(lblserie);
    layoutserie->addWidget(cboSeries);
    m_busqueda->addLayoutZ1(layoutserie);
    connect(cboSeries,SIGNAL(currentIndexChanged(QString)),this,SLOT(on_cboseries_currentIndexChanged(QString)));

    QPushButton* open = new QPushButton(QIcon(":/Icons/PNG/Factura.png"),tr("Mostrar"),this);
    connect(open,SIGNAL(clicked()),this,SLOT(mostrar_factura()));
    m_busqueda->addWidget(open);

    QPushButton* unlock = new QPushButton(QIcon(":/Icons/PNG/abrecaja.png"),tr("Desbloquear"),this);
    connect(unlock,SIGNAL(clicked()),this,SLOT(desbloquear_factura()));
    m_busqueda->addWidget(unlock);

    QPushButton* add = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir"),this);
    connect(add,SIGNAL(clicked()),this,SLOT(on_btnAnadir_clicked()));
    m_busqueda->addWidget(add);

//    QPushButton* edit = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar"),this);
//    connect(edit,SIGNAL(clicked()),this,SLOT(on_btnEditar_clicked()));
//    m_busqueda->addWidget(edit);

    QPushButton* print = new QPushButton(QIcon(":/Icons/PNG/print2.png"),tr("Imprimir"),this);
    connect(print,SIGNAL(clicked()),this,SLOT(on_btnImprimir_clicked()));
    m_busqueda->addWidget(print);

    QPushButton* del = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar"),this);
    connect(del,SIGNAL(clicked()),this,SLOT(on_btnBorrar_clicked()));
    m_busqueda->addWidget(del);

    m_busqueda->addSpacer();
    QPushButton* list = new QPushButton(QIcon(":/Icons/PNG/reports.png"),tr("Listados"),this);
    connect(list,SIGNAL(clicked()),this,SLOT(listados()));
    m_busqueda->addWidget(list);

    connect(m_busqueda,SIGNAL(key_Down_Pressed()), ui->tabla_facturas,SLOT(setFocus()));   
}

void frmFacturas::on_btnAsignarTransportista_clicked()
{
    db_consulta_view consulta;
    QStringList campos;
    campos <<"codigo" <<"transportista" << "telefono1" <<"movil" << "poblacion"<< "provincia" << "pais";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("transportistas");
    consulta.set_db("Group");
    consulta.set_SQL("select id,codigo,transportista from transportista");
    QStringList cabecera;
    QVariantList tamanos;
    cabecera  << tr("Código") << tr("Transportista");

    tamanos <<"0" << "100" << "250" ;
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de transportistas");
    if(consulta.exec())
    {
        oFactura->id_transportista = consulta.get_id();
        ui->txtTransportista->setText(Configuracion_global->devolver_transportista(oFactura->id_transportista));
   }
}

void frmFacturas::on_tabla_facturas_doubleClicked(const QModelIndex &index)
{
     int id = ui->tabla_facturas->model()->data(ui->tabla_facturas->model()->index(index.row(),0),Qt::EditRole).toInt();
     oFactura->RecuperarFactura(id);
     LLenarCampos();
     QString filter = QString("id_Cab = '%1'").arg(oFactura->id);
     //helper.fillTable("empresa","lin_fac",filter);
     ui->stackedWidget->setCurrentIndex(0);
     ui->btnEditar->setEnabled(true);
     ui->btnImprimir->setEnabled(true);
     ocultarBusqueda();
}

void frmFacturas::on_btnArticulos_clicked()
{
    FrmArticulos artic;
    artic.exec();
}

void frmFacturas::on_cboseries_currentIndexChanged(const QString &arg1)
{
    this->serie = arg1;
    filter_table(this->texto,this->orden,this->modo);
}

void frmFacturas::on_btnBorrar_clicked()
{
    if(!ui->tabla_facturas->currentIndex().isValid())
        return;
    oFactura->id = ui->tabla_facturas->model()->data(ui->tabla_facturas->model()->index(
                                                         ui->tabla_facturas->currentIndex().row(),0)).toInt();

    oFactura->borrar(oFactura->id);
    Cliente::incrementar_acumulados(oFactura->id_cliente,-1.0 * oFactura->base,oFactura->fecha);
    oFactura->clear();
    LLenarCampos();
    filter_table(this->texto,this->orden,this->modo);

}

void frmFacturas::on_spinPorc_dto_editingFinished()
{
    calcular_factura();
}

void frmFacturas::on_spinPorc_dto_pp_editingFinished()
{
    calcular_factura();
}

void frmFacturas::on_btnCobrar_clicked()
{
    FrmCobrarFactura cobrar(this);
    cobrar.setFactura(oFactura->id);
    cobrar.setWindowTitle(tr("Cobrar factura %1/%2").arg(oFactura->serie).arg(oFactura->factura));
    cobrar.exec();

    QString error;
    oFactura->cobrado = SqlCalls::SelectOneField("cab_fac","cobrado",QString("id=%1").arg(oFactura->id),
                                                 Configuracion_global->empresaDB,error).toBool();
    ui->btnCobrar->setVisible(!oFactura->cobrado);
    ui->lblFacturaCobrada->setVisible(oFactura->cobrado);
}

void frmFacturas::on_btnGuardar_clicked()
{
    bool succes = true;

    LLenarFactura();
    if(oFactura->factura.isEmpty())
        oFactura->factura = oFactura->NuevoNumeroFactura(ui->cbo_serie->currentText());


    // Acumulados clientes
    double acumular = oFactura->base;
    if(in_edit)
    {
        //BORRAR LOS ACUMULADOS ANTERIORES COMPLETAMENTE
        if(!Cliente::incrementar_acumulados(oFactura->id_cliente,-1.0 * total_anterior,fecha_anterior))
        {
            Configuracion_global->rollback();
            return;
        }
        for(auto i=0;i< lineas_anterior->rowCount();++i)
        {
            QSqlRecord r = lineas_anterior->record(i);
            if(!Articulo::acumulado_ventas(r.value("id_articulo").toInt(),-1.0* r.value("cantidad").toDouble(),-1.0* r.value("total").toDouble(),fecha_anterior))
            {
                Configuracion_global->rollback();
                return;
            }
        }
    }

    if(Cliente::incrementar_acumulados(oFactura->id_cliente,acumular,oFactura->fecha))
    {
        for(auto i=0; i<modelLineas->rowCount();++i)
        {
            QSqlRecord r = modelLineas->record(i);
            if(!Articulo::acumulado_ventas(r.value("id_articulo").toInt(),r.value("cantidad").toDouble(),r.value("total").toDouble(),oFactura->fecha))
            {
                Configuracion_global->rollback();
                return;
            }
        }
    }

    if(succes)
    {
        succes = oFactura->GuardarFactura(oFactura->id,false);
        if(succes)
        {
            LLenarCampos();
            if(Configuracion_global->contabilidad && oFactura->apunte == 0)
                succes = crear_asiento();
            if(!succes)

                QMessageBox::critical(this,tr("Error"),
                                      tr("Error al crear el asiento contable")+Configuracion_global->empresaDB.lastError().text(),
                                      tr("&Aceptar"));
            else
                ui->txtAsiento->setText(QString::number(oFactura->apunte));
        }
        if(succes)
        {


            TimedMessageBox::Box(this,tr("Factura guardada con éxito"));

            // -------------------------------------------------
            // Borrar vencimientos si existen para no duplicar
            //--------------------------------------------------
            QString clausula = QString("id_factura = %1 and id_empresa=%2").arg(oFactura->id).arg(Configuracion_global->idEmpresa);
            QString error;
            SqlCalls::SqlDelete("clientes_deuda",Configuracion_global->groupDB,clausula,error);
            //----------------------
            // Crear vencimientos
            //----------------------
            vencimientos oVto(this);
            oVto.calcular_vencimiento(oFactura->fecha,oFactura->id_forma_pago,oFactura->id_cliente,0,oFactura->id,oFactura->serie+"/"+oFactura->factura,1,
                                      true,oFactura->total);
            BloquearCampos(true);
            emit unblock();
            if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.commit();
            Configuracion_global->empresaDB.commit();
            Configuracion_global->groupDB.commit();
        }
        else
        {
            QMessageBox::critical(this,tr("Error"),
                                  tr("Error al guardar la factura.\n")+Configuracion_global->empresaDB.lastError().text(),
                                  tr("Aceptar"));
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
            if(Configuracion_global->contabilidad)
                Configuracion_global->contaDB.rollback();
        }
    }


}

void frmFacturas::mostrarBusqueda()
{
     _showBarraBusqueda(m_busqueda);
     ui->stackedWidget->setCurrentIndex(1);
     m_busqueda->doFocustoText();
}

void frmFacturas::ocultarBusqueda()
{
     _hideBarraBusqueda(m_busqueda);
}

void frmFacturas::on_SpinGastoDist1_editingFinished()
{
    double importe_iva = ui->SpinGastoDist1->value() *(ui->cbo_porc_gasto_iva1->currentText().toFloat()/100);
    ui->spin_iva_gasto1->setValue(importe_iva);
    calcular_factura();
}

void frmFacturas::on_spin_porc_iva_gasto1_editingFinished()
{
    double importe_iva = ui->SpinGastoDist1->value() *(ui->cbo_porc_gasto_iva1->currentText().toFloat()/100);
    ui->spin_iva_gasto1->setValue(importe_iva);
    calcular_factura();
}

void frmFacturas::on_SpinGastoDist2_editingFinished()
{
    double importe_iva = ui->SpinGastoDist2->value() *(ui->cbo_porc_gasto_iva2->currentText().toFloat()/100);
    ui->spin_iva_gasto2->setValue(importe_iva);
    calcular_factura();
}

void frmFacturas::on_spin_porc_iva_gasto2_editingFinished()
{
    double importe_iva = ui->SpinGastoDist2->value() *(ui->cbo_porc_gasto_iva2->currentText().toFloat()/100);
    ui->spin_iva_gasto2->setValue(importe_iva);
    calcular_factura();

}

void frmFacturas::on_SpinGastoDist3_editingFinished()
{
    double importe_iva = ui->SpinGastoDist3->value() *(ui->cbo_porc_gasto_iva3->currentText().toFloat()/100);
    ui->spin_iva_gasto3->setValue(importe_iva);
    calcular_factura();
}

void frmFacturas::on_spin_porc_iva_gasto3_editingFinished()
{
    double importe_iva = ui->SpinGastoDist3->value() *(ui->cbo_porc_gasto_iva3->currentText().toFloat()/100);
    ui->spin_iva_gasto3->setValue(importe_iva);
    calcular_factura();
}

void frmFacturas::on_btnAnadirLinea_clicked()
{
    if(ui->btnGuardar->isEnabled())
    {
            frmEditLine frmeditar(this);
            frmeditar.init();
            connect(&frmeditar,SIGNAL(refrescar_lineas()),this,SLOT(refrescar_modelo()));
            frmeditar.setUse_re(ui->chkrecargo_equivalencia->isChecked());
            frmeditar.set_acumula(true);
            frmeditar.set_linea(0,"lin_fac");
            frmeditar.set_tabla("lin_fac");
            frmeditar.set_id_cliente(oCliente1->id);
            frmeditar.set_id_tarifa(oFactura->tarifa_cliente);
            frmeditar.set_id_cab(oFactura->id);
            frmeditar.set_venta(true);
            frmeditar.setTipoDoc(frmEditLine::Factura);
            frmeditar.exec();
            refrescar_modelo();

    } else{
        QMessageBox::warning(this,tr("Gestión de facturas"),tr("Debe editar la factura para añadir líneas"),
                             tr("Aceptar"));
    }
}

void frmFacturas::on_Lineas_doubleClicked(const QModelIndex &index)
{
    if(ui->btnGuardar->isEnabled())
    {
        int id_lin = ui->Lineas->model()->data(index.model()->index(index.row(),0)).toInt();
        if(id_lin >0)
        {
            frmEditLine frmeditar(this);
            frmeditar.init();
            connect(&frmeditar,SIGNAL(refrescar_lineas()),this,SLOT(refrescar_modelo()));
            frmeditar.setUse_re(ui->chkrecargo_equivalencia->isChecked());
            frmeditar.set_acumula(true);
            frmeditar.set_id_cliente(oCliente1->id);
            frmeditar.set_id_tarifa(oFactura->tarifa_cliente);
            frmeditar.set_id_cab(oFactura->id);
            frmeditar.set_venta(true);
            frmeditar.setTipoDoc(frmEditLine::Factura);
            frmeditar.set_linea(id_lin,"lin_fac");
            frmeditar.set_tabla("lin_fac");
            frmeditar.set_editando();
            frmeditar.exec();

            refrescar_modelo();
            ui->Lineas->setFocus();

        } else
        {
            QMessageBox::warning(this, tr("Gestión de facturas"),tr("Debe editar la factura para poder modificar las líneas"),
                                 tr("Aceptar"));

        }
    }
}


void frmFacturas::on_btn_borrarLinea_clicked()
{
    if(QMessageBox::question(this,tr("Lineas de facturas"), tr("¿Borrar la linea?"),
                             tr("No"),tr("Borrar")) == QMessageBox::Accepted)
    {
        QModelIndex index = ui->Lineas->currentIndex();
        int id_lin = ui->Lineas->model()->index(index.row(),0).data().toInt();
        oFactura->BorrarLineasFactura(id_lin);
        modelLineas->setQuery(QString("select id,id_articulo,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                                      "from lin_fac where id_cab = %1;").arg(oFactura->id),Configuracion_global->empresaDB);
        calcular_factura();
        ui->Lineas->setFocus();

    }
}

void frmFacturas::on_cboDireccionesEntrega_currentIndexChanged(int index)
{
    if(!__init)
        return;
    QSqlRecord r = model_dir_entrega->record(index);
    ui->txtDireccion1_entrega->setText(r.value("direccion1").toString());
    ui->txtDireccion2_entrega->setText(r.value("direccion2").toString());
    ui->txtCp_entrega->setText(r.value("cp").toString());
    ui->txtPoblacion_entrega->setText(r.value("poblacion").toString());
    ui->txtProvincia_entrega->setText(r.value("provincia").toString());
    int id_pais= r.value("id_pais").toInt();
    for(auto i=0; i< Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == id_pais)
        {
            ui->cboPais_entrega->setCurrentIndex(i);
            break;
        }
    }

}

void frmFacturas::listados()
{
    frmListadoFac d(this);
    d.exec();
}

void frmFacturas::on_chkrecargo_equivalencia_toggled(bool checked)
{
    if (checked) {
        ui->chkrecargo_equivalencia->setChecked(true);
        oFactura->recargo_equivalencia = 1;
        ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->reList.at(0)));
        ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->reList.at(1)));
        ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->reList.at(2)));
        ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->reList.at(3)));
        oFactura->porc_rec1 = ui->txtporc_rec1->text().toFloat();
        oFactura->porc_rec2 = ui->txtporc_rec2->text().toFloat();
        oFactura->porc_rec3 = ui->txtporc_rec3->text().toFloat();
        oFactura->porc_rec4 = ui->txtporc_rec4->text().toFloat();
    } else {
        ui->chkrecargo_equivalencia->setChecked(false);
        oFactura->recargo_equivalencia = 0;
        ui->txtporc_rec1->setText("0.00");
        ui->txtporc_rec2->setText("0.00");
        ui->txtporc_rec3->setText("0.00");
        ui->txtporc_rec4->setText("0.00");
        oFactura->porc_rec1 = 0;
        oFactura->porc_rec2 = 0;
        oFactura->porc_rec3 = 0;
        oFactura->porc_rec4 = 0;
    }
    QString error;
    for(auto i = 0; i<modelLineas->rowCount();++i)
    {
        QSqlRecord r = modelLineas->record(i);
        QHash<QString,QVariant> lin;
        double iva_art = r.value("porc_iva").toDouble();
        double porc_re_art;

        if(iva_art == oFactura->porc_iva1)
            porc_re_art = oFactura->porc_rec1;
        if(iva_art == oFactura->porc_iva2)
            porc_re_art = oFactura->porc_rec2;
        if(iva_art == oFactura->porc_iva3)
            porc_re_art = oFactura->porc_rec3;
        if(iva_art == oFactura->porc_iva4)
            porc_re_art = oFactura->porc_rec4;

        lin["porc_rec"] = porc_re_art;
        lin["rec"] = r.value("total").toDouble() * (porc_re_art/100.0);

        if(!SqlCalls::SqlUpdate(lin,"lin_fac",Configuracion_global->empresaDB,QString("id=%1").arg(r.value("id").toInt()),error))
        {
            QMessageBox::critical(this,tr("Error al actualizar lineas"),error);
            break;
        }
    }
    calcular_factura();
}
