#include "reportimage.h"
#include "editimagedlg.h"

#include "Auxiliares/Globlal_Include.h"
#include "../Core/Functions.h"
ReportImage::ReportImage(QString name,QGraphicsItem *parent) :
    Container(name,parent)
{
    m_ruta = "";
    m_fromDB = false;
    m_dinamica = false;
    this->setRect(0,0,150,150);
}

QDomElement ReportImage::xml(QDomDocument doc, QPointF relPos, QList<Section *> sectionPool)
{
    QDomElement node = doc.createElement("Element");
    node.setAttribute("id","Image");

    Container::apendXML(node,doc, relPos);

    node.setAttribute("Path",m_ruta);
    node.setAttribute("fromBD",m_fromDB);
    node.setAttribute("Dinamic",m_dinamica);

    return node;
}

void ReportImage::parseXml(QDomElement element, QPointF origin)
{
    this->setPos(element.attribute("x").toDouble() + origin.x(),element.attribute("y").toDouble()+origin.y());
    this->setSize(element.attribute("w").toDouble(),element.attribute("h").toDouble());

    this->setfromDB(element.attribute("fromBD").toDouble());
    this->setdinamica(element.attribute("Dinamic").toDouble());
    this->setruta(element.attribute("Path"));
    this->setName(element.attribute("name"));
}


void ReportImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    painter->drawImage(this->rect(),m_image);
    painter->restore();
    Container::paint(painter, option, widget);
}

QString ReportImage::ruta() const {
    return m_ruta;
}

bool ReportImage::fromDB() const {
    return m_fromDB;
}

void ReportImage::setruta(QString arg) {
    if (m_ruta != arg) {
        m_ruta = arg;

        if(m_fromDB)
        {
            QStringList l = arg.split(".");

            QSqlDatabase db;
            if(l.at(0).startsWith("Gen"))
                db = QSqlDatabase::database("grupo");
            else if(l.at(0).startsWith("Emp"))
                db = QSqlDatabase::database("empresa");

            QString err;
            QString path = SqlCalls::SelectOneField(l.at(1),l.at(2),QString(),db,err).toString();

            QPixmap pm1 = RedFoX::Core::Functions::getPixmap(path);

            m_image = pm1.toImage();       
        }
        else
        {
            QFile f(arg);
            if(f.open(QFile::ReadOnly))
            {
                m_image = QImage(arg);         
            }
        }
        emit rutaChanged(arg);
    }
}

void ReportImage::setfromDB(bool arg) {
    if (m_fromDB != arg) {
        m_fromDB = arg;
        emit fromDBChanged(arg);
    }
}

bool ReportImage::dinamica() const {
    return m_dinamica;
}

void ReportImage::setdinamica(bool arg) {
    if (m_dinamica != arg) {
        m_dinamica = arg;
        emit dinamicaChanged(arg);
    }
}

void ReportImage::editMe()
{
    EditImageDlg dlg(this,qApp->activeWindow());
    dlg.exec();
}

QString ReportImage::query()
{
    QStringList l = ruta().split(".");
    if(l.size()==3)
    {
        QString zona = l.at(0);
        QString tabla = l.at(1);
        QString s = QString("%1.%2").arg(zona).arg(tabla);
        return s;
    }
    else
        return "";
}
