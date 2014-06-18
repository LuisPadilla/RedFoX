#include "login.h"
#include "ui_login.h"

#include "mainwindow.h"



#include "Zona_Administrador/frmempresas.h"
#include "Zona_Administrador/arearestringida_form.h"
#include "Zona_Administrador/frmconfigmaya.h"
#include "mainwindow.h"

#include "Core/Functions.h"
using namespace RedFoX;

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{  
    ui->setupUi(this);    
    connect(ui->btnEmpresa,SIGNAL(clicked()),this,SLOT(btnEmpresa_clicked()));
    connect(ui->Crearconfiguracin,SIGNAL(clicked()),this,SLOT(Crearconfiguracion_clicked()));

    if (! QFile::exists(qApp->applicationDirPath()+"/MayaConfig.ini"))
    {
        frmConfigmaya frmConf;
        if(frmConf.exec()==QDialog::Accepted)
            TimedMessageBox::Box(this,tr("Configuración inicial realizada con éxito"));
    }


    if(!init())
    {
        //if(Configuracion_global->globalDB.isOpen())
        //{
            frmConfigmaya frmConf;
            if(frmConf.exec()==QDialog::Accepted){
                TimedMessageBox::Box(this,tr("Configuración inicial realizada con éxito"));
                init();
            }
        //}
        //else
        //    QMessageBox::critical(0, "error:", Configuracion_global->globalDB.lastError().text());
    }
}

Login::~Login()
{
    delete ui;
}

const QString Login::getUsuario() const
{
    return ui->cboUsers->currentText();
}

const QString Login::getPass() const
{
    return ui->linePassword->text();
}

const QString Login::getEmpresaName()
{
    return getEmpresa().value("nombre").toString();
}

const QSqlRecord Login::getEmpresa() const
{
    return _empresas.value(ui->comboGroup->currentText()).rec_empresas.value(ui->cboEmpresa->currentText());
}

int Login::getid_user()
{
    return m_id;
}


void Login::on_btnAcceder_clicked()
{
    QSqlRecord rGrupo = _empresas.value(ui->comboGroup->currentText()).rec_grupo;
    QString bd_driver = rGrupo.value("bd_driver").toString();
    QString bd_name = rGrupo.value("bd_name").toString();
    QString bd_host = rGrupo.value("bd_host").toString();
    QString bd_user = rGrupo.value("bd_user").toString();
    QString bd_pass = rGrupo.value("bd_pass").toString();
    int bd_port = rGrupo.value("bd_port").toInt();


    Configuracion_global->group_host = bd_host;
    Configuracion_global->group_user = bd_user;
    Configuracion_global->group_pass = bd_pass;
    Configuracion_global->group_port = bd_port;
    Configuracion_global->group_Driver = bd_driver;
    Configuracion_global->group_DBName = bd_name;
    Configuracion_global->groupDB = QSqlDatabase::addDatabase(bd_driver , "Maya");

    QJsonDocument document = QJsonDocument::fromJson(rGrupo.value("ruta_imagenes").toString().toUtf8());
    Configuracion_global->groupIMGJson = document.object();

    Configuracion_global->groupDB.setHostName(bd_host);
    Configuracion_global->groupDB.setUserName(bd_user);
    Configuracion_global->groupDB.setPassword(bd_pass);
    Configuracion_global->groupDB.setPort(bd_port);
    Configuracion_global->groupDB.setDatabaseName(bd_name);
    if(!Configuracion_global->groupDB.open())
    {
        QMessageBox::critical(this,tr("Error"),Configuracion_global->groupDB.lastError().text());
        return;
    }
    else
    {
        QSqlQuery q(Configuracion_global->groupDB);
        q.exec("SET GLOBAL max_allowed_packet=1073741824;");
    }

    QSqlRecord rEmpresa = _empresas.value(ui->comboGroup->currentText()).rec_empresas.value(ui->cboEmpresa->currentText());

    Configuracion_global->nombre_bd_empresa = rEmpresa.value("nombre_bd").toString();
    Configuracion_global->nombre_bd_conta = rEmpresa.value("nombre_db_conta").toString();
    Configuracion_global->nombre_bd_medica = rEmpresa.value("nombre_bd_medica").toString();
    Configuracion_global->decimales = rEmpresa.value("decimales").toInt();
    Configuracion_global->decimales_campos_totales = rEmpresa.value("decimales_campos_totales").toInt();
    Configuracion_global->porc_irpf = rEmpresa.value("porc_irpf").toFloat();

    QSqlQuery q(Configuracion_global->globalDB);
    q.prepare( "SELECT * FROM redfoxglobal.usuarios where nombre =:Nombre" );
    q.bindValue(":Nombre",ui->cboUsers->currentText());//NOTE cambiar esto para que busque en el model
    if( !q.exec() )
    {
        QMessageBox::critical(qApp->activeWindow(), tr("Error:"), q.lastError().text());
    }
    else
    {
        if (q.next())
        {
            QSqlRecord rUsuario = q.record();
            if (Configuracion::SHA256HashString(ui->linePassword->text()) == rUsuario.value("contrasena").toString().trimmed())
            {
                m_id = q.value(0).toInt();
                Configuracion_global->user_name = rUsuario.value("nombre").toString();
                Configuracion_global->user_long_name = rUsuario.value("nombre_completo").toString();
                Configuracion_global->id_usuario_activo = m_id;
                Configuracion_global->user_mail_smpt = rUsuario.value("cuenta_smtp").toString();
                Configuracion_global->user_mail_acc= rUsuario.value("usuario_mail").toString();
                Configuracion_global->user_mail_pass = Configuracion::DeCrypt(rUsuario.value("password_mail").toString());
                Configuracion_global->user_mail_port = rUsuario.value("port_mail").toInt();
                Configuracion_global->super_user = rUsuario.value("super_user").toBool();
                Login::done(QDialog::Accepted);
            }
        }
        else
        {
            QMessageBox::critical(this,tr("Error"),tr("Datos de acceso incorrectos"));
            ui->linePassword->setText("");
        }
    }
}
void Login::Crearconfiguracion_clicked()
{
    AreaRestringida_form check(this);
    check.exec();
    if(check.es_valido())    {
        frmConfigmaya frmConf;
        if(frmConf.exec()==QDialog::Accepted)
            TimedMessageBox::Box(this,tr("Configuración inicial realizada con éxito"));
        ui->cboUsers->setModel(Configuracion_global->usuarios_model);
        ui->cboUsers->setModelColumn(1);
    }
}

void Login::btnEmpresa_clicked()
{
    AreaRestringida_form check(this);
    check.exec();
    if(check.es_valido())
    {
        FrmEmpresas formEmpresa(this);
        formEmpresa.setWindowState(Qt::WindowMaximized);        
        formEmpresa.exec();
        init();
    }
}

void Login::on_pushButton_clicked()
{
	this->hide();
}

bool Login::init()
{
    if(!Configuracion_global->CargarDatosBD())
        return false;

    ui->comboGroup->clear();
    _empresas.clear();
    QString grupo = "";

    QSqlQuery QryEmpresas(QSqlDatabase::database("Global"));

    QryEmpresas.prepare("Select * from redfoxglobal.grupos");

    if(QryEmpresas.exec())
	{
        while (QryEmpresas.next())
		{
            strc_empresa current;
            QSqlRecord rEmpresa = QryEmpresas.record();
            current.rec_grupo = rEmpresa;
            QString nombreGrupo = rEmpresa.field("nombre").value().toString();
            ui->comboGroup->addItem(nombreGrupo);

            if(grupo == "")
                grupo = nombreGrupo;

            QString bd_driver = rEmpresa.value("bd_driver").toString();
            QString bd_name = rEmpresa.value("bd_name").toString();
            QString bd_host = rEmpresa.value("bd_host").toString();
            QString bd_user = rEmpresa.value("bd_user").toString();
            QString bd_pass = rEmpresa.value("bd_pass").toString();
            int bd_port = rEmpresa.value("bd_port").toInt();

            QSqlDatabase db = QSqlDatabase::addDatabase(bd_driver);
            db.setHostName(bd_host);
            db.setUserName(bd_user);
            db.setPassword(bd_pass);
            db.setPort(bd_port);
            db.setDatabaseName(bd_name);

            if(!db.open()){
                QMessageBox::warning(qApp->activeWindow(),tr("Estableciendo conexión"),
                                     tr("No se pudo conectar con la base de datos:%1").arg(db.lastError().text()),
                                     tr("Aceptar"));
                qDebug() << db.lastError().text();
                continue;
            }

            QString query = QString("SELECT * FROM `%1`.empresas;").arg(bd_name);

            QSqlQuery q(db);
            q.exec(query);

            while(q.next())
            {
                QString emp = q.record().value("nombre").toString();

                current.empresas.append(emp);
                current.rec_empresas.insert(emp,q.record());
            }
            _empresas.insert(nombreGrupo,current);
		}
	}

    ui->cboEmpresa->addItems(_empresas.value(grupo).empresas);

    ui->cboUsers->setModel(Configuracion_global->usuarios_model);
    ui->cboUsers->setModelColumn(1);

    QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
    QString nombreusuario = settings.value("cUsuarioActivo").toString();
    QString contrasena = Configuracion_global->DeCrypt(settings.value("contrasenaactiva").toString());
    Configuracion_global->caja = QString::number(settings.value("cajaactiva").toInt());
    Configuracion_global->Importe_apertura = Configuracion_global->MonedatoDouble(settings.value("importe_apertura").toString());
    ui->cboUsers->setCurrentIndex(ui->cboUsers->findText(nombreusuario));
    ui->linePassword->setText(contrasena);

    return true;
}

void Login::on_comboGroup_currentTextChanged(const QString &arg1)
{
    ui->cboEmpresa->clear();
    ui->cboEmpresa->addItems(_empresas.value(arg1).empresas);
}
