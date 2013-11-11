#include "frmcierrecaja.h"
#include "ui_frmcierrecaja.h"


FrmCierreCaja::FrmCierreCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmCierreCaja)
{
    ui->setupUi(this);
    QSqlQueryModel *users = new QSqlQueryModel(this);
    users->setQuery("select nombre from usuarios",Configuracion_global->groupDB);
    ui->cboUsuarioCierre->setModel(users);

    //------------------------------------------
    // Recupero parametros empresa para cierre
    //------------------------------------------
    QMap <int, QSqlRecord> e;
    QString error;
    e = SqlCalls::SelectRecord("empresas",QString("id =%1").arg(Configuracion_global->idEmpresa),Configuracion_global->groupDB,error);
    if(error.isEmpty()){
        this->facturas_en_cierre = e.value(Configuracion_global->idEmpresa).value("facturas_en_cierre").toBool();
        this->importe_fijo_cierre = e.value(Configuracion_global->idEmpresa).value("importe_cierre").toDouble();
    } else
    {
        QMessageBox::warning(this,tr("Cierre de caja"),tr("Error al recuperar los datos de la empresa: %1 ").arg(error),
                             tr("Aceptar"));
    }
    //cargar_datos_caja();


}

FrmCierreCaja::~FrmCierreCaja()
{
    delete ui;
}



void FrmCierreCaja::cargar_datos_caja(int id)
{
    QMap <int,QSqlRecord> caja;
    QString error;
    caja = SqlCalls::SelectRecord("cierrecaja",QString("id=%1").arg(id),Configuracion_global->empresaDB,error);
    if(caja.size()<1)
        QMessageBox::warning(this,tr("Gestión de tickets"),tr("Ocurrió un error al recuperar la caja : %1").arg(error),
                             tr("Aceptar"));
    else
    {
        ui->lblimporte_abertura->setText(Configuracion_global->toFormatoMoneda(
                                             QString::number(caja.value(id).value("importe_abertura_dia").toDouble(),
                                                     'f',Configuracion_global->decimales_campos_totales)));
        QString user = SqlCalls::SelectOneField("usuarios","nombre",QString("id = %1").arg(
                                                    caja.value(id).value("id_usuario").toInt()),Configuracion_global->groupDB,
                                                error).toString();
        if(error.isEmpty())
            ui->lblUsuario_abertura->setText(user);
        else
            QMessageBox::warning(this,tr("Gestión de TPV / Cierre caja"),
                                 tr("Ocurrió un error al mostrar datos abertura caja: %1").arg(error));
    }
}



void FrmCierreCaja::on_btnIniciar_cierre_clicked()
{
    ui->txtimporte_abertura->setText(ui->lblimporte_abertura->text());
    ui->txthora_cierre->setTime(QTime::currentTime());
    QDate fecha_cierre =ui->calendarWidget->selectedDate();

    //----------------------------
    // Calculo ingresos
    // ---------------------------

    //-----------------------------------------------------------------------------------------------------------
    // Antes que nada recuperamos pagos realizados desde gestión de deudas en efectivo, tarjeta, cheques y vales
    // estos valores deben acumularse a los realizados con ticket ya que el dinero se guarda normalmente en caja
    //-----------------------------------------------------------------------------------------------------------
    QString cSQL;
    QSqlQuery result(Configuracion_global->empresaDB);
    //-------------------------
    // cobro deuda en efectivo
    //-------------------------
    cSQL = QString("select sum(importe) as importe from e_s_caja where fecha = '%1' and entrada = 3").arg(
                fecha_cierre.toString("yyyyMMdd"));
    result.exec(cSQL);
    result.next();
    double importe_deudas_efectivo = result.record().value("importe").toDouble();

    //-------------------------
    // cobro deuda en tarjeta
    //-------------------------
    cSQL = QString("select sum(importe) as importe from e_s_caja where fecha = '%1' and entrada = 4").arg(
                fecha_cierre.toString("yyyyMMdd"));
    result.exec(cSQL);
    result.next();
    double importe_deudas_tarjeta = result.record().value("importe").toDouble();

    //-------------------------
    // cobro deuda en cheque
    //-------------------------
    cSQL = QString("select sum(importe) as importe from e_s_caja where fecha = '%1' and entrada = 5").arg(
                fecha_cierre.toString("yyyyMMdd"));
    result.exec(cSQL);
    result.next();
    double importe_deudas_cheque = result.record().value("importe").toDouble();

    //-------------------------
    // cobro deuda en vale
    //-------------------------
    cSQL = QString("select sum(importe) as importe from e_s_caja where fecha = '%1' and entrada = 6").arg(
                fecha_cierre.toString("yyyyMMdd"));
    result.exec(cSQL);
    result.next();
    double importe_deudas_vale = result.record().value("importe").toDouble();

    //-----------------------------
    // Recuperamos tickets sumados
    //-----------------------------
    cSQL = QString("select  sum(importe_efectivo) as efectivo, sum(importe_tarjeta) as tarjeta, sum(importe_cheque) as cheque,"
            "sum(importe_domiciliacion) as domiciliacion, sum(importe_transferencia) as transferencia, sum(importe_vale) as vale "
                   "from cab_tpv where fecha = '%1'").arg(fecha_cierre.toString("yyyyMMdd"));


    result.exec(cSQL);
    result.next();
    double efectivo = result.record().value("efectivo").toDouble() + importe_deudas_efectivo;
    ui->txtEfectivo->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                       efectivo,'f',
                                                                       Configuracion_global->decimales_campos_totales)));
    double efect = Configuracion_global->MonedatoDouble(ui->lblimporte_abertura->text()) +
                                                         Configuracion_global->MonedatoDouble(ui->txtEfectivo->text()) +
                                                         efectivo;
    ui->txtCaja_moneda->setText(Configuracion_global->toFormatoMoneda(QString::number(efect,'f',
                                                                                      Configuracion_global->decimales_campos_totales)));

    double tarjeta =  result.record().value("tarjeta").toDouble() + importe_deudas_tarjeta;
    ui->txtTarjeta->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                      tarjeta,'f',
                                                                      Configuracion_global->decimales_campos_totales)));
    double cheques = result.record().value("cheque").toDouble() +  importe_deudas_cheque;
    ui->txtCheques->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                       cheques,'f',
                                                                       Configuracion_global->decimales_campos_totales)));
    ui->txtDomiciliacion->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                       result.record().value("domiciliacion").toDouble(),'f',
                                                                       Configuracion_global->decimales_campos_totales)));
    ui->txtTransferencia->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                       result.record().value("transferencia").toDouble(),'f',
                                                                       Configuracion_global->decimales_campos_totales)));
    double vale = result.record().value("vale").toDouble() + importe_deudas_vale;
    ui->txtVales_recibidos->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                       vale,'f',
                                                                       Configuracion_global->decimales_campos_totales)));
    double ventas = Configuracion_global->MonedatoDouble(ui->txtCaja_moneda->text()) +
            Configuracion_global->MonedatoDouble(ui->txtTarjeta->text()) +
            Configuracion_global->MonedatoDouble(ui->txtCheques->text()) +
            Configuracion_global->MonedatoDouble(ui->txtDomiciliacion->text()) +
            Configuracion_global->MonedatoDouble(ui->txtTransferencia->text()) +
            Configuracion_global->MonedatoDouble(ui->txtVales_recibidos->text());
    ui->txtVentas->setText(Configuracion_global->toFormatoMoneda(QString::number(ventas,'f',
                                                                                 Configuracion_global->decimales_campos_totales)));
    //-----------------
    // Vales emitidos
    //-----------------
    cSQL = QString("select  sum(importe) as importe from vales where fecha = '%1'").arg(fecha_cierre.toString("yyyyMMdd"));
    result.exec(cSQL);
    result.next();
    QString importe = QString::number(result.record().value("importe").toDouble(),'f',
                                      Configuracion_global->decimales_campos_totales);
    ui->txtValesEmitidos->setText(Configuracion_global->toFormatoMoneda(importe));

    //-------------------
    // Dinero insertado
    //-------------------
    cSQL = QString("select sum(importe) as importe from e_s_caja where fecha = '%1' and entrada = 1").arg(
                fecha_cierre.toString("yyyyMMdd"));
    result.exec(cSQL);
    result.next();
    importe = QString::number(result.record().value("importe").toDouble(),'f',
                                      Configuracion_global->decimales_campos_totales);
    ui->txtEntradas->setText(Configuracion_global->toFormatoMoneda(importe));
    //-------------------
    // Dinero retirado
    //-------------------
    cSQL = QString("select sum(importe) as importe from e_s_caja where fecha = '%1' and entrada = 0").arg(
                fecha_cierre.toString("yyyyMMdd"));
    result.exec(cSQL);
    result.next();
    importe = QString::number(result.record().value("importe").toDouble(),'f',
                                      Configuracion_global->decimales_campos_totales);
    ui->txtSalidas->setText(Configuracion_global->toFormatoMoneda(importe));

    //-------------------
    // Extras
    //-------------------
    cSQL = QString("select sum(importe) as importe from e_s_caja where fecha = '%1' and entrada = 2").arg(
                fecha_cierre.toString("yyyyMMdd"));
    result.exec(cSQL);
    result.next();
    importe = QString::number(result.record().value("importe").toDouble(),'f',
                                      Configuracion_global->decimales_campos_totales);
    ui->txtExtras->setText(Configuracion_global->toFormatoMoneda(importe));

    //---------------------
    // Totales finales
    //---------------------
    double total_en_caja = Configuracion_global->MonedatoDouble(ui->txtVentas->text()) +
            Configuracion_global->MonedatoDouble(ui->txtEntradas->text()) -
            Configuracion_global->MonedatoDouble(ui->txtSalidas->text()) +
            Configuracion_global->MonedatoDouble(ui->txtExtras->text());

    ui->txtTotal_en_caja->setText(Configuracion_global->toFormatoMoneda(QString::number(total_en_caja,'f',
                                                             Configuracion_global->decimales_campos_totales)));

    //---------------------
    // pre-cierre terminado
    //---------------------
    TimedMessageBox * t = new TimedMessageBox(this,tr("Se han obtenido los datos de las cabeceras de tickets"));
    ui->btnCierreCaja->setEnabled(true);
    ui->btnCierreParcial->setEnabled(true);




}

void FrmCierreCaja::on_btnCierreParcial_clicked()
{
    ui->btnResumenEntradas->setEnabled(true);
    ui->btnResumenExtras->setEnabled(true);
    ui->btnResumenSalidas->setEnabled(true);
    ui->btnResumentickets->setEnabled(true);

}

void FrmCierreCaja::cargarmoneda()
{
    // TODO - Arreglar moneda
//    QMap <int, QSqlRecord> mon;
//    QString error;
//    QStringList orden;
//    orden << "order by valor_moneda";
//    mon = SqlCalls::SelectRecord("moneda_caja",QString("id_moneda_base= %1").arg(1),
//                                 orden,Configuracion_global->empresaDB,error);
//    QMapIterator <int,QSqlRecord> m(mon);
//    while (m.hasNext())
//    {
//        m.next();
//        if(m.value().value("orden").toInt() == 1)
//        {
//            ui->lblmoneda1->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda1->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }
//        if(m.value().value("orden").toInt() == 2)
//        {
//            ui->lblmoneda2->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda2->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }
//        if(m.value().value("orden").toInt() == 3)
//        {
//            ui->lblmoneda3->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda3->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }
//        if(m.value().value("orden").toInt() == 4)
//        {
//            ui->lblmoneda4->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda4->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }
//        if(m.value().value("orden").toInt() == 5)
//        {
//            ui->lblmoneda5->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda5->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }
//        if(m.value().value("orden").toInt() == 6)
//        {
//            ui->lblmoneda6->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda6->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }
//        if(m.value().value("orden").toInt() == 7)
//        {
//            ui->lblmoneda7->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda7->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }
//        if(m.value().value("orden").toInt() == 8)
//        {
//            ui->lblmoneda8->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda8->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }
//        if(m.value().value("orden").toInt() == 9)
//        {
//            ui->lblmoneda9->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda9->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }
//        if(m.value().value("orden").toInt() == 10)
//        {
//            ui->lblmoneda10->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda10->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }
//        if(m.value().value("orden").toInt() == 11)
//        {
//            ui->lblmoneda11->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda11->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }
//        if(m.value().value("orden").toInt() == 12)
//        {
//            ui->lblmoneda12->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda12->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }
//        if(m.value().value("orden").toInt() == 13)
//        {
//            ui->lblmoneda13->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda13->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }
//        if(m.value().value("orden").toInt() == 14)
//        {
//            ui->lblmoneda14->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda14->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }
//        if(m.value().value("orden").toInt() == 15)
//        {
//            ui->lblmoneda15->setText(m.value().value("nombre_moneda").toString());
//            ui->lblmoneda15->setProperty("valorMoneda",m.value().value("valor_moneda").toDouble());
//        }



//    }


}
