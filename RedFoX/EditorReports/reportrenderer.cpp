#include "reportrenderer.h"

#include "paper.h"

#include <QTime>
#include "section.h"
#include "Auxiliares/Globlal_Include.h"
#include "../Core/Functions.h"
ReportRenderer::ReportRenderer(QObject *parent) :
    QObject(parent)
{
}

QDomDocument ReportRenderer::render(QPrinter* printer ,QDomDocument in ,QMap<QString,QString> queryClausules,QMap<QString,QString> params, bool& error ,bool isPrev)
{
    QPainter p;//(printer);
    m_doc = preRender(&p,in,queryClausules,params,error,isPrev);

    for(int i=0;i<2;i++){        
    QDomNode root = m_doc.firstChild();
    QDomNode page = root.firstChild();
    while(!page.isNull())
    {
        if(!page.hasChildNodes())
        {
            QDomNode _aux = page;
            page = page.nextSiblingElement("Page");
            root.removeChild(_aux);
        }
        else
        {
            QDomNode section = page.firstChild();
            while(!section.isNull())
            {
                if(!section.hasChildNodes())
                {
                    QDomNode _aux = section;
                    section = section.nextSibling();
                    page.removeChild(_aux);
                }
                else
                    section = section.nextSibling();
            }
            page = page.nextSiblingElement("Page");
        }
    }
    }
    m_doc.firstChild().toElement().setAttribute("pages", m_doc.firstChild().childNodes().size());
    QFile f("/home/marco/m_doc.xml");
    if(f.open(QFile::WriteOnly))
    {
        QTextStream out(&f);
        const int Indent = 4;
        m_doc.save(out,Indent);
    }
    return m_doc;
}

void ReportRenderer::drawElement(qreal dpix, QPainter* painter, QDomElement element, int printResolution, qreal dpiy)
{
    //TODO other items
    if(element.attribute("id")== "RoundRect")
        drawRect(element, painter,dpix,dpiy,printResolution);
    else if(element.attribute("id")== "Label")
        drawLabel(element, painter,dpix,dpiy);
    else if(element.attribute("id")== "Line")
        drawLine(element, painter,dpix,dpiy,printResolution);
    else if(element.attribute("id")== "CodeBar")
        drawCodeBar(element, painter,dpix,dpiy);
    else if(element.attribute("id")== "Image")
        drawImage(element, painter,dpix,dpiy);
}

void ReportRenderer::Print(QPrinter *printer)
{
    QDomElement ele = m_doc.documentElement();

     bool lblPrinter(ele.attribute("lblPrinter") .toInt());
     double lblDistV(ele.attribute("lblDistV") .toDouble());
     double mx = ele.attribute("mx").toDouble();
     double my = ele.attribute("my").toDouble();

     if(!lblPrinter)
         printer->setPageSizeMM(QSizeF(ele.attribute("w").toDouble()*10.0,ele.attribute("h").toDouble()*10.0));
     else
     {
         double _auxDist = (ele.childNodes().size()+1)*Paper::pxTocm(lblDistV)*10.0;
         _auxDist += my;
         printer->setPageSizeMM(QSizeF(ele.attribute("w").toDouble()*10.0,ele.attribute("h").toDouble()*10.0*(ele.childNodes().size()+1)+_auxDist));
     }

    QSize margins(printer->paperRect().left() - printer->pageRect().left(), printer->paperRect().top() - printer->pageRect().top());

    QPainter painter(printer);

    qreal dpix = printer->logicalDpiX() / 96.0;
    qreal dpiy = printer->logicalDpiY() / 96.0;
    int printResolution = printer->resolution();

    painter.translate(margins.width(),margins.height());

    double usable = ele.attribute("usable").toDouble();
    double width = Paper::cmToPx(ele.attribute("w").toDouble()) - ele.attribute("mr").toDouble() - mx;
    painter.translate( mx * dpix , my * dpiy);

    QDomNodeList pages = ele.childNodes();
    Paper::docType t = static_cast<Paper::docType>(ele.attribute("DocType").toInt());
    if(t == Paper::_Report || t==Paper::_sobre)
    {
        for(int i=0;i<pages.size();i++)
        {
            painter.save();
            QDomNodeList sections = pages.at(i).childNodes();
            for(int s = 0;s<sections.size();s++)
            {
                QDomElement cSec = sections.at(s).toElement();
                int id = cSec.attribute("id").toDouble();
                if(id < 2)// Report | Page Headers
                {
                    QDomNodeList elements = cSec.childNodes();
                    for(int e = 0;e<elements.size();e++)
                    {
                        QDomElement element = elements.at(e).toElement();
                        drawElement(dpix, &painter, element, printResolution, dpiy);
                    }
                    int siz = cSec.attribute("size").replace(",",".").toDouble();
                    painter.translate(0,siz* dpiy);
                }
                else if(id == 2) // Detail Section
                {
                    QDomNodeList parts = cSec.childNodes();
                    for(int p=0;p<parts.size();p++)
                    {
                        QDomElement cPart = parts.at(p).toElement();
                        if(cPart.attribute("colored").toDouble())
                        {
                            painter.save();
                            painter.setPen(Qt::NoPen);
                            painter.setBrush(ColorFromString(cPart.attribute("color")));
                            painter.drawRect(0,0,width * dpix ,cPart.attribute("size").replace(",",".").toDouble() * dpiy);
                            painter.restore();
                        }
                        QDomNodeList elements = cPart.childNodes();
                        for(int e = 0;e<elements.size();e++)
                        {
                            QDomElement element = elements.at(e).toElement();
                            drawElement(dpix, &painter, element, printResolution, dpiy);
                        }
                        int siz = cPart.attribute("size").replace(",",".").toDouble();
                        painter.translate(0,siz* dpiy);
                    }
                }
                else // Report | Page footer
                {
                    if(s==sections.size()-1) //last section on page
                    {
                        int siz = cSec.attribute("size").replace(",",".").replace(",",".").toDouble();
                        painter.save();
                        painter.resetTransform();
                        painter.translate(margins.width(),margins.height());
                        painter.translate( mx * dpix , my * dpiy);
                        int offset = usable - siz;
                        painter.translate( 0, offset * dpiy);
                        QDomNodeList elements = cSec.childNodes();
                        for(int e = 0;e<elements.size();e++)
                        {
                            QDomElement element = elements.at(e).toElement();
                            drawElement(dpix, &painter, element, printResolution, dpiy);
                        }
                        painter.restore();
                    }
                    else
                    {
                        int siz = cSec.attribute("size").replace(",",".").toDouble();
                        int sizNext = sections.at(s+1).toElement().attribute("size").replace(",",".").toDouble();
                        painter.save();
                        painter.resetTransform();
                        painter.translate(margins.width(),margins.height());
                        painter.translate( mx * dpix , my * dpiy);
                        int offset = usable - siz - sizNext;
                        painter.translate( 0, offset * dpiy);
                        QDomNodeList elements = cSec.childNodes();
                        for(int e = 0;e<elements.size();e++)
                        {
                            QDomElement element = elements.at(e).toElement();
                            drawElement(dpix, &painter, element, printResolution, dpiy);
                        }
                        painter.restore();
                        painter.translate(0,siz* dpiy);
                    }
                }
            }
            painter.restore();
            if(i!=pages.size()-1)
                printer->newPage();
        }
    }
    else if(t == Paper::_etiqueta)
    {
        double lblDistH(ele.attribute("lblDistH") .toDouble());

        int lblColumnCount(ele.attribute("lblColumnCount") .toInt());
        int lblRowCount(ele.attribute("lblRowCount") .toInt());


        double lblPaperH(ele.attribute("lblPaperH") .toDouble());
        double lblPaperW(ele.attribute("lblPaperW") .toDouble());
        double lblPaperMarginS(ele.attribute("lblPaperMarginS") .toDouble());
        double lblPaperMarginInf(ele.attribute("lblPaperMarginInf") .toDouble());
        double lblPaperMarginIzq(ele.attribute("lblPaperMarginIzq") .toDouble());
        double lblPaperMarginDer(ele.attribute("lblPaperMarginDer") .toDouble());

        double lblW =ele.attribute("lblW") .toDouble();
        double lblH =ele.attribute("lblH") .toDouble();

        int c = 0, r = 0;
        for(int i=0;i<pages.size();i++)
        {
            QDomNodeList sections = pages.at(i).childNodes();

            painter.save();
            if(c>lblColumnCount-1)
            {
                c=0;
                r++;
            }
            if(r>lblRowCount-1 && !lblPrinter)
            {
                c=0;
                r=0;
                if(i!=pages.size()-1)
                {
                    printer->newPage();
                }
            }

            double x = c*dpix*lblW + c*dpix*lblDistH;
            double y = r*dpiy*lblH + r*dpiy*lblDistV;
            painter.translate( x , y);

            for(int s = 0;s<sections.size();s++)
            {
                QDomElement cSec = sections.at(s).toElement();
                QDomNodeList parts = cSec.childNodes();
                for(int p=0;p<parts.size();p++)
                {
                    QDomElement cPart = parts.at(p).toElement();
                    QDomNodeList elements = cPart.childNodes();
                    for(int e = 0;e<elements.size();e++)
                    {
                        QDomElement element = elements.at(e).toElement();
                        drawElement(dpix, &painter, element, printResolution, dpiy);
                    }
                }
            }
            c++;
            painter.restore();            
        }
    }
}

void ReportRenderer::PreRender()
{
    bool error;
    _params[":fecha"] = QDate::currentDate().toString("dd-MM-yyyy");
    render( printer ,DocIn , queryClausules, _params, error,_limit);
    emit end();
}

QDomDocument ReportRenderer::preRender(QPainter* painter ,QDomDocument in,QMap<QString,QString> queryClausules,QMap<QString,QString> params, bool &error,bool isPrev)
{
    QDomElement Inroot = in.documentElement();
    if (Inroot.tagName() != "FoxReports") {
        error = true;
        return QDomDocument();
    }

    QDomDocument doc;
    QDomElement root = doc.createElement("Document");
    doc.appendChild(root);
    bool haveRHeader = false;
    bool havePHeader = false;
    bool PHeaderOnAll = false;
    bool PFooterOnAll = false;
    bool havePFooter = false;
    bool haveRFooter = false;

    int RHeaderSiz = 0;
    int PHeaderSiz = 0;
    int PFooterSiz = 0;
    int RFooterSiz = 0;

    QDomNode RHeaderElement;
    QDomNode PHeaderElement;
    QDomNode PFootElement;
    QDomNode RFootElement;

    QList<QDomNode> SectionNodes;
    QStringList querys;

    double usable = 0;

    QDomNode child = Inroot.firstChild();

    QDomElement _aux = child.toElement();

    Paper::docType t =  static_cast<Paper::docType>(_aux.attribute("DocType").toInt());

    if(t == Paper::_Report || t== Paper::_sobre)
    {
        usable =  Paper::cmToPx(_aux.attribute("h").toDouble());
        root.setAttribute("h",_aux.attribute("h"));
        root.setAttribute("w",_aux.attribute("w"));
        usable -= Paper::cmToPx(_aux.attribute("marginTop").toDouble());
        usable -= Paper::cmToPx(_aux.attribute("marginBottom").toDouble());
        root.setAttribute("my",QString::number(Paper::cmToPx(_aux.attribute("marginTop").toDouble()), 'f', 4));
        root.setAttribute("mx",QString::number(Paper::cmToPx(_aux.attribute("marginLeft").toDouble()), 'f', 4));
        root.setAttribute("mr",QString::number(Paper::cmToPx(_aux.attribute("marginRigth").toDouble()), 'f', 4));
    }
    else if(t == Paper::_etiqueta)
    {
        root.setAttribute("lblW",QString::number(Paper::cmToPx(_aux.attribute("w").toDouble()), 'f', 4));
        root.setAttribute("lblH",QString::number(Paper::cmToPx(_aux.attribute("h").toDouble()), 'f', 4));
        root.setAttribute("mx",QString::number(Paper::cmToPx(_aux.attribute("lblPaperMarginIzq").toDouble()), 'f', 4));
        root.setAttribute("mr",QString::number(Paper::cmToPx(_aux.attribute("lblPaperMarginDer").toDouble()), 'f', 4));

        usable -= Paper::cmToPx(_aux.attribute("lblPaperMarginS").toDouble());
        usable -= Paper::cmToPx(_aux.attribute("lblPaperMarginInf").toDouble());

        bool lblPrinter = _aux.attribute("lblPrinter").toInt();
        if(lblPrinter)
        {
            //FIXME Margen superior en impresora tickets
            root.setAttribute("my",QString::number(Paper::cmToPx(_aux.attribute("lblPaperMarginS").toDouble()), 'f', 4));
            double w =  _aux.attribute("lblPaperMarginIzq").toDouble() +
                        _aux.attribute("lblPaperMarginDer").toDouble() +
                        _aux.attribute("w").toDouble();
            root.setAttribute("h",_aux.attribute("h"));
            root.setAttribute("w",QString::number(w,'f',4));
        }
        else
        {
            root.setAttribute("my",QString::number(Paper::cmToPx(_aux.attribute("lblPaperMarginS").toDouble()), 'f', 4));
            root.setAttribute("h",_aux.attribute("lblPaperH"));
            root.setAttribute("w",_aux.attribute("lblPaperW"));
        }

    }
    root.setAttribute("usable",QString::number(usable, 'f', 4));

    root.setAttribute("DocType",_aux.attribute("DocType"));


    if(t == Paper::_etiqueta)
    {
        root.setAttribute("lblDistH",QString::number(Paper::cmToPx(_aux.attribute("lblDistH").toDouble()), 'f', 4));
        root.setAttribute("lblDistV",QString::number(Paper::cmToPx(_aux.attribute("lblDistV").toDouble()), 'f', 4));

        root.setAttribute("lblColumnCount",_aux.attribute("lblColumnCount"));
        root.setAttribute("lblRowCount",_aux.attribute("lblRowCount"));
        root.setAttribute("lblPrinter",_aux.attribute("lblPrinter"));

        root.setAttribute("lblPaperH",_aux.attribute("lblPaperH"));
        root.setAttribute("lblPaperW",_aux.attribute("lblPaperW"));
        root.setAttribute("lblPaperMarginS",_aux.attribute("lblPaperMarginS"));
        root.setAttribute("lblPaperMarginInf",_aux.attribute("lblPaperMarginInf"));
        root.setAttribute("lblPaperMarginIzq",_aux.attribute("lblPaperMarginIzq"));
        root.setAttribute("lblPaperMarginDer",_aux.attribute("lblPaperMarginDer"));
    }

    QMap<QString,float> _paper_acums;
    QStringList acums = _aux.attribute("acum").split(",");
    foreach (QString s, acums) {
        _paper_acums[s] = 0.0;
    }

    while (!child.isNull())
    {
        QDomNode sections = child.firstChild();
        while (!sections.isNull())
        {
            QDomElement secEle = sections.toElement();
            if(secEle.tagName() == "Section")
            {
                Section::SectionType t = static_cast<Section::SectionType>(secEle.attribute("id").toInt());
                switch (t) {
                case Section::ReportHeader:
                    haveRHeader = true;
                    RHeaderSiz = secEle.attribute("size").replace(",",".").toDouble();
                    RHeaderElement = sections.cloneNode(true);
                    break;
                case Section::PageHeader:
                    havePHeader = true;
                    PHeaderSiz = secEle.attribute("size").replace(",",".").toDouble();
                    PHeaderOnAll = secEle.attribute("OnFistPage").toDouble();
                    PHeaderElement = sections.cloneNode(true);
                    break;
                case Section::Detail:
                    SectionNodes.append(sections.cloneNode(true));
                    break;
                case Section::PageFooter:
                    havePFooter = true;
                    PFooterSiz = secEle.attribute("size").replace(",",".").toDouble();
                    PFooterOnAll= secEle.attribute("OnFistPage").toDouble();
                    PFootElement = sections.cloneNode(true);
                    break;
                case Section::ReportFooter:
                    haveRFooter = true;
                    RFooterSiz = secEle.attribute("size").replace(",",".").toDouble();
                    RFootElement = sections.cloneNode(true);
                    break;
                }
            }
            else if(secEle.tagName() == "SQL")
            {
                querys << secEle.attribute("target");
            }
            sections = sections.nextSibling();
        }
        child = child.nextSibling();
    }

    QList<QPair<QString,QString> > finalQuerys;
    QStringListIterator it(querys);
    while (it.hasNext()) {
        QPair<QString,QString> aux = getSql(it.next(),queryClausules,isPrev);
        if(!aux.first.isEmpty())
            finalQuerys.append(aux);
    }
    QListIterator<QPair<QString,QString> > qIt(finalQuerys);
    QMap<QString,QSqlRecord> selects;
    while(qIt.hasNext())
    {
        QPair<QString,QString> pair = qIt.next();
        QSqlDatabase db;

        if(pair.first.startsWith("Gen"))
            db = Configuracion_global->groupDB;
        else if(pair.first.startsWith("Emp"))
            db = Configuracion_global->empresaDB;
        QSqlQuery q(db);

        if(q.exec(pair.second))
            if(q.next())
                selects.insert(pair.first,q.record());
        //TODO otras db
    }


    QListIterator<QDomNode> sectionIt(SectionNodes);
    QList<QDomNode> parsedSections;
    while(sectionIt.hasNext())
    {                
        QDomNode n = sectionIt.next();
        QDomElement ele = n.toElement();

        //PARSE SECTIOS PARAMS
        QDomNodeList l = ele.childNodes();
        for(int i=0 ; i< l.size(); i++)
        {
            QDomElement _e = l.at(i).toElement();
            if(_e.attribute("id")== "Param")
            {
                QString value = params.value(_e.attribute("Arg"));
                _e.setAttribute("id","Label");
                _e.setAttribute("Text",value);
            }
        }
        //END PARSE SECTIOS PARAMS

        bool iSql = ele.attribute("haveSqlInterno").toDouble();
        QString cla = ele.attribute("ClausulaInterna");
        QStringList lCla = cla.split("=");
        QString columna = "";
        QString bindKey = "";
        QString clausulaInterna = "";
        if(lCla.size()==2)
        {
            columna = lCla.at(1);
            clausulaInterna = QString("%1").arg(lCla.at(0));//.arg(lCla.at(1));
            bindKey = QString(":%1").arg(lCla.at(1));
        }
        //init acums


        QPair<QString,QString> gSql = getSql(ele.attribute("SqlGlobal"),queryClausules,isPrev);
        QString first = gSql.first;

        QSqlDatabase db;
        if(first.startsWith("Gen"))
            db = Configuracion_global->groupDB;
        else if(first.startsWith("Emp"))
            db = Configuracion_global->empresaDB;

        QSqlQuery gQuery(db);
        if(gQuery.exec(gSql.second))
        {
            QMap<QString , QList<QSqlRecord> > inner_affected;
            if(iSql)
            {
                QMap<QString,bool> _ids;
                while(gQuery.next())
                {
                    _ids.insert(gQuery.record().value(columna).toString(),true);
                }
                QList<QString> id_list = _ids.uniqueKeys();

                if(!id_list.isEmpty())
                {
                    QString iSqlAttribute = ele.attribute("SqlInterno");

                    QString select;
                    if(queryClausules.value(ele.attribute("SqlGlobal")).contains(">0")) //TODAS
                        select = QString("Select * from %1 where %2>0").arg(iSqlAttribute.split(".").at(1)).arg(clausulaInterna);
                    else //ES UN RANGO
                    {
                        select = QString("Select * from %1 where %2=%3").arg(iSqlAttribute.split(".").at(1)).arg(clausulaInterna).arg(id_list.first());
                        for (int i=1; i< id_list.size();i++)
                            select.append(QString(" or %1=%2").arg(clausulaInterna).arg(id_list.at(i)));
                    }

                    QString limits = queryClausules.value(iSqlAttribute);
                    if(!limits.isEmpty())
                        select.append(QString(" and %1").arg(limits));

                    QSqlDatabase inner_db;
                    if(iSqlAttribute.startsWith("Gen"))
                        inner_db = Configuracion_global->groupDB;
                    else if(iSqlAttribute.startsWith("Emp"))
                        inner_db = Configuracion_global->empresaDB;

                    QSqlQuery inner_query(inner_db);
                    if(inner_query.exec(select))
                        while(inner_query.next())
                            inner_affected[inner_query.record().value(clausulaInterna).toString()].append(inner_query.record());
                }
                gQuery.first();
                gQuery.previous();
            }
            while(gQuery.next())
            {
                QStringList i_acums_list = ele.attribute("acum").split(",");
                QMap<QString,float> _i_acums;
                foreach (QString _a, i_acums_list) {
                    _i_acums[_a]=0.0;
                }

                QSqlRecord record = gQuery.record();
                QDomNode exit = doc.createElement("section");
                bool appenExit = true;
                QDomNode copy = n.cloneNode(true);
                QDomNode sectionPart = copy.firstChild();
                while(!sectionPart.isNull())
                {
                    QDomNode next = sectionPart.nextSibling();
                    QDomElement secEle = sectionPart.toElement();
                    if(secEle.tagName() == "Header")
                    {
                        QDomNode child = sectionPart.firstChild();
                        while(!child.isNull())
                        {
                            QDomElement ele = child.toElement();
                            if(ele.attribute("id")=="Field")
                            {
                                ele.setAttribute("id","Label");
                                QString text = "";
                                int formato = ele.attribute("formato").toDouble();
                                QStringList value = ele.attribute("Sql").split(".");
                                if(value.size()== 3)
                                        text = applyFormato(record.value(value.at(2)).toString(),formato);
                                ele.setAttribute("Text",text);
                            }
                            else if(ele.attribute("id")=="RelationalField")
                            {
                                ele.setAttribute("id","Label");
                                QString text = "";
                                int formato = ele.attribute("formato").toDouble();
                                QStringList value = ele.attribute("value").split(".");
                                if(value.size()>1)
                                {
                                    QString key = value.at(0) + "." + value.at(1);
                                    text = applyFormato(getRelationField(ele.attribute("Sql"),selects.value(key)),formato);
                                }
                                ele.setAttribute("Text",text);
                            }
                            else if(ele.attribute("id")=="CodeBar")
                            {
                                QString text = "";
                                QStringList value = ele.attribute("Sql").split(".");
                                if(value.size()== 3)
                                    text = record.value(value.at(2)).toString();
                                ele.setAttribute("Code",text);
                            }
                            else if(ele.attribute("id")=="Image")
                            {
                                QString b;
                                QStringList value = ele.attribute("Path").split(".");
                                if(value.size()== 3)
                                    b = record.value(value.at(2)).toString();
                                ele.setAttribute("Path",b);
                            }
                            child = child.nextSibling();
                        }
                        exit.appendChild(sectionPart);
                    }
                    else if (secEle.tagName() == "Body")
                    {
                        if(iSql)
                        {
                            QString c1 = ele.attribute("color1");
                            QString c2 = ele.attribute("color2");
                            bool colored = ele.attribute("colored").toDouble();
                            bool altern = ele.attribute("alternative").toDouble();
                            bool toogle = true;
                            QString current_key = record.value(columna).toString();
                            QList<QSqlRecord> current_records = inner_affected.value(current_key);
                            if(!current_records.isEmpty())
                            {
                                foreach (QSqlRecord iRecord , current_records)
                                {
                                    QDomNode iCopy = sectionPart.cloneNode(true);
                                    QDomElement iCopyEle = iCopy.toElement();
                                    iCopyEle.setAttribute("colored",colored);
                                    if(toogle)
                                    {
                                        iCopyEle.setAttribute("color",c1);
                                        if(altern)
                                            toogle = false;
                                    }
                                    else
                                    {
                                        iCopyEle.setAttribute("color",c2);
                                        toogle = true;
                                    }
                                    QDomNode child = iCopy.firstChild();
                                    while(!child.isNull())
                                    {
                                        QDomElement ele = child.toElement();
                                        if(ele.attribute("id")=="Field")
                                        {
                                            ele.setAttribute("id","Label");
                                            QString text = "";
                                            int formato = ele.attribute("formato").toDouble();
                                            QStringList value = ele.attribute("Sql").split(".");
                                            if(value.size()== 3)
                                                text = applyFormato(iRecord.value(value.at(2)).toString(),formato);
                                            ele.setAttribute("Text",text);
                                            if(ele.attribute("Expandable") == "1")
                                            {
                                                double y = ele.attribute("y").toDouble();
                                                double h = ele.attribute("h").toDouble();
                                                double w = ele.attribute("w").toDouble();
                                                double siz = sectionPart.toElement().attribute("size").replace(",",".").toDouble();
                                                double diff = siz - h - y;

                                                QFont f(ele.attribute("fontFamily"),ele.attribute("fontSize").toInt(),ele.attribute("fontWeigth").toInt(),ele.attribute("italicFont").toInt());
                                                QFontMetricsF metrics(f);
                                                QRectF r(metrics.boundingRect(QRect(0,0,w,h),Qt::TextWordWrap,text));

                                                double newH = r.height();

                                                siz = qMax(newH + diff, siz);
                                                sectionPart.toElement().setAttribute("size",siz);
                                                ele.setAttribute("h",newH);
                                            }
                                            if(_i_acums.contains(ele.attribute("name")))
                                                _i_acums[ele.attribute("name")]+= iRecord.value(value.at(2)).toDouble();
                                        }
                                        else if(ele.attribute("id")=="RelationalField")
                                        {
                                            ele.setAttribute("id","Label");
                                            QString text = "";
                                            int formato = ele.attribute("formato").toDouble();
                                            text = applyFormato(getRelationField(ele.attribute("Sql"),iRecord),formato);
                                            ele.setAttribute("Text",text);
                                        }
                                        else if(ele.attribute("id")=="CodeBar")
                                        {
                                            QString text = "";
                                            QStringList value = ele.attribute("Sql").split(".");
                                            if(value.size()== 3)
                                                text = iRecord.value(value.at(2)).toString();
                                            ele.setAttribute("value",text);
                                        }
                                        else if(ele.attribute("id")=="Image")
                                        {
                                            QString b;
                                            QStringList value = ele.attribute("Path").split(".");
                                            if(value.size()== 3)
                                                b = iRecord.value(value.at(2)).toString();
                                            ele.setAttribute("Path",b);
                                        }
                                        child = child.nextSibling();
                                    }
                                    exit.appendChild(iCopy);
                                }
                            }
                            else
                            {
                                appenExit = false;
                            }
                        }
                        else
                        {
                            QDomNode child = sectionPart.firstChild();
                            while(!child.isNull())
                            {
                                QDomElement ele = child.toElement();
                                if(ele.attribute("id")=="Field")
                                {
                                    ele.setAttribute("id","Label");
                                    QString text = "";
                                    int formato = ele.attribute("formato").toDouble();
                                    QStringList value = ele.attribute("Sql").split(".");
                                    if(value.size()== 3)
                                            text = applyFormato(record.value(value.at(2)).toString(),formato);
                                    ele.setAttribute("Text",text);
                                    if(ele.attribute("Expandable") == "1")
                                    {
                                        double y = ele.attribute("y").toDouble();
                                        double h = ele.attribute("h").toDouble();
                                        double w = ele.attribute("w").toDouble();
                                        double siz = sectionPart.toElement().attribute("size").replace(",",".").toDouble();
                                        double diff = siz - h - y;

                                        QFont f(ele.attribute("fontFamily"),ele.attribute("fontSize").toInt(),ele.attribute("fontWeigth").toInt(),ele.attribute("italicFont").toInt());
                                        QFontMetricsF metrics(f);
                                        QRectF r(metrics.boundingRect(QRect(0,0,w,h),Qt::TextWordWrap,text));

                                        double newH = r.height();

                                        siz = qMax(newH + diff, siz);
                                        sectionPart.toElement().setAttribute("size",siz);
                                        ele.setAttribute("h",newH);
                                    }
                                }
                                else if(ele.attribute("id")=="RelationalField")
                                {
                                    ele.setAttribute("id","Label");
                                    QString text = "";
                                    int formato = ele.attribute("formato").toDouble();
                                    text = applyFormato(getRelationField(ele.attribute("Sql"),record),formato);
                                    ele.setAttribute("Text",text);
                                }
                                else if(ele.attribute("id")=="CodeBar")
                                {
                                    QString text = "";
                                    QStringList value = ele.attribute("Sql").split(".");
                                    if(value.size()== 3)
                                        text = record.value(value.at(2)).toString();
                                    ele.setAttribute("Code",text);
                                }
                                else if(ele.attribute("id")=="Image")
                                {
                                    QString b;
                                    QStringList value = ele.attribute("Path").split(".");
                                    if(value.size()== 3)
                                        b = record.value(value.at(2)).toString();
                                   ele.setAttribute("Path",b);
                                }
                                child = child.nextSibling();
                            }
                            exit.appendChild(sectionPart);
                        }
                    }
                    else //(secEle.tagName() == "Foot")
                    {
                        QDomNode child = sectionPart.firstChild();
                        while(!child.isNull())
                        {
                            QDomElement ele = child.toElement();
                            if(ele.attribute("id")=="Field")
                            {
                                ele.setAttribute("id","Label");
                                QString text = "";
                                int formato = ele.attribute("formato").toDouble();
                                QStringList value = ele.attribute("Sql").split(".");
                                if(value.size()== 3)
                                        text = applyFormato(record.value(value.at(2)).toString(),formato);
                                ele.setAttribute("Text",text);
                            }
                            else if(ele.attribute("id")=="RelationalField")
                            {
                                ele.setAttribute("id","Label");
                                QString text = "";
                                int formato = ele.attribute("formato").toDouble();
                                text = applyFormato(getRelationField(ele.attribute("Sql"),record),formato);
                                ele.setAttribute("Text",text);
                            }
                            else if(ele.attribute("id")=="CodeBar")
                            {
                                QString text = "";
                                QStringList value = ele.attribute("Sql").split(".");
                                if(value.size()== 3)
                                    text = record.value(value.at(2)).toString();
                                ele.setAttribute("Code",text);
                            }
                            else if(ele.attribute("id")=="Image")
                            {
                                QString b;
                                QStringList value = ele.attribute("Path").split(".");
                                if(value.size()== 3)
                                    b = record.value(value.at(2)).toString();
                                ele.setAttribute("Path",b);
                            }
                            if(ele.attribute("id")=="Acumulador")
                            {
                                ele.setAttribute("id","Label");
                                QString text = "";
                                int formato = ele.attribute("formato").toDouble();
                                text = applyFormato(QString::number(_i_acums.value(ele.attribute("target")),'f',2),formato);
                                ele.setAttribute("Text",text);
                            }
                            child = child.nextSibling();
                        }
                        exit.appendChild(sectionPart);
                    }
                    sectionPart = next;
                }
                if(appenExit)
                    parsedSections.append(exit);
            }
        }
    }

    //PARSE PARAMS

    if(haveRHeader)
    {
       QDomNodeList l = RHeaderElement.childNodes();
       for(int i=0 ; i< l.size(); i++)
       {
           QDomElement _e = l.at(i).toElement();
           if(_e.attribute("id")== "Param")
           {
               QString value = params.value(_e.attribute("Arg"));
               _e.setAttribute("id","Label");
               _e.setAttribute("Text",value);
           }
       }
    }
    if(havePHeader)
    {
        QDomNodeList l = PHeaderElement.childNodes();
        for(int i=0 ; i< l.size(); i++)
        {
            QDomElement _e = l.at(i).toElement();
            if(_e.attribute("id")== "Param")
            {
                QString value = params.value(_e.attribute("Arg"));
                _e.setAttribute("id","Label");
                _e.setAttribute("Text",value);
            }
        }
    }
    if(havePFooter)
    {
        QDomNodeList l = PFootElement.childNodes();
        for(int i=0 ; i< l.size(); i++)
        {
            QDomElement _e = l.at(i).toElement();
            if(_e.attribute("id")== "Param")
            {
                QString value = params.value(_e.attribute("Arg"));
                _e.setAttribute("id","Label");
                _e.setAttribute("Text",value);
            }
        }
    }
    if(haveRFooter)
    {
        QDomNodeList l = RFootElement.childNodes();
        for(int i=0 ; i< l.size(); i++)
        {
            QDomElement _e = l.at(i).toElement();
            if(_e.attribute("id")== "Param")
            {
                QString value = params.value(_e.attribute("Arg"));
                _e.setAttribute("id","Label");
                _e.setAttribute("Text",value);
            }
        }
    }

    //END PARSE PARAMS

    QDomNode pageNode = startPage(usable,PFooterSiz, RHeaderSiz ,RFooterSiz,doc,havePHeader&&PHeaderOnAll,PHeaderElement,selects,haveRHeader,RHeaderElement);
    parseFooters(RFootElement , haveRFooter , PFootElement , havePFooter , selects);
    root.appendChild(pageNode);

    int ipageCount = 1;
    int pageUsable = usable;

    if(havePHeader&&PHeaderOnAll)
        pageUsable -= PHeaderSiz;
    if(haveRHeader)
        pageUsable -= RHeaderSiz;

    QListIterator<QDomNode> parsedIt(parsedSections);
    while(parsedIt.hasNext())
    {
        QDomNode sec = doc.createElement("Section");
        sec.toElement().setAttribute("id",2);
        pageNode.appendChild(sec);

        QDomNode n = parsedIt.next();
        QDomNode child = n.firstChild();
        QDomNode lastChild = n.lastChild();
        QDomNode sectionHeader;
        QDomNode sectionFoot;
        bool haveHead = false;
        bool haveFoot = false;
        int headSiz = 0;
        int footSiz = 0;
        if(child.toElement().tagName() == "Header")
        {
            haveHead = true;
            sectionHeader = child;
            headSiz = child.toElement().attribute("size").replace(",",".").toDouble();
            sec.appendChild(sectionHeader.cloneNode(true));
            pageUsable -= headSiz;
        }
        if(lastChild.toElement().tagName() == "Foot")
        {
            haveFoot = true;
            footSiz = lastChild.toElement().attribute("size").replace(",",".").toDouble();
            sectionFoot = lastChild;
        }

        QList<QDomNode> bodys;
        QDomNodeList l = n.childNodes();

        for(int i = 0;i< l.size(); i++)
        {
            if(haveFoot && i == l.size()-1)
                continue;
            bodys.append(l.at(i));
        }

        QListIterator<QDomNode> bodyIt(bodys);
        while (bodyIt.hasNext())
        {
            QDomNode body = bodyIt.next();
            QDomElement bEle = body.toElement();
            if(bEle.tagName() == "Body")
            {
                if(parsedIt.hasNext())//no report Foot yet
                {
                    if(pageUsable > bEle.attribute("size").replace(",",".").toDouble() + footSiz + PFooterSiz)
                    {
                        pageUsable -= bEle.attribute("size").replace(",",".").toDouble();
                        sec.appendChild(body);

                        QDomNode child = body.firstChild();
                        while(!child.isNull())
                        {
                            QDomElement ele = child.toElement();

                            if(_paper_acums.contains(ele.attribute("name")))
                            {
                                QString sValue = ele.attribute("Text");
                                _paper_acums[ele.attribute("name")] += getNumber(sValue,ele.attribute("formato").toInt());
                            }

                            child = child.nextSibling();
                        }

                        if(!bodyIt.hasNext())//last body of section
                        {
                            if(haveFoot)
                            {
                                sec.appendChild(sectionFoot);
                                pageUsable -= footSiz;
                            }
                        }
                    }
                    else
                    {
                        if(havePFooter)
                            endPage(pageNode,PFootElement.cloneNode(true),_paper_acums);
                        pageNode = startPage(usable,PFooterSiz, RHeaderSiz,RFooterSiz,doc,havePHeader,PHeaderElement,selects);
                        ipageCount++;
                        root.appendChild(pageNode);
                        pageUsable = usable;

                        if(havePHeader)
                            pageUsable -= PHeaderSiz;

                        sec = doc.createElement("Section");
                        sec.toElement().setAttribute("id",2);
                        pageNode.appendChild(sec);
                        if(haveHead)
                        {
                            sec.appendChild(sectionHeader.cloneNode(true));
                            pageUsable -= headSiz;
                        }
                        pageUsable -= bEle.attribute("size").replace(",",".").toDouble();
                        sec.appendChild(body);

                        QDomNode child = body.firstChild();
                        while(!child.isNull())
                        {
                            QDomElement ele = child.toElement();

                            if(_paper_acums.contains(ele.attribute("name")))
                            {
                                QString sValue = ele.attribute("Text");
                                _paper_acums[ele.attribute("name")] += getNumber(sValue,ele.attribute("formato").toInt());
                            }

                            child = child.nextSibling();
                        }

                        if(!bodyIt.hasNext())//last body
                        {
                            if(haveFoot)
                            {
                                sec.appendChild(sectionFoot);
                                pageUsable -= footSiz;
                            }
                        }
                    }
                }
                else // !parsedIt.hasNext()
                {
                    if(bodyIt.hasNext()) //No need report footer yet
                    {
                        if(pageUsable > bEle.attribute("size").replace(",",".").toDouble() + footSiz + PFooterSiz)
                        {
                            pageUsable -= bEle.attribute("size").replace(",",".").toDouble();
                            sec.appendChild(body);

                            QDomNode child = body.firstChild();
                            while(!child.isNull())
                            {
                                QDomElement ele = child.toElement();

                                if(_paper_acums.contains(ele.attribute("name")))
                                {
                                    QString sValue = ele.attribute("Text");
                                    _paper_acums[ele.attribute("name")] += getNumber(sValue,ele.attribute("formato").toInt());
                                }

                                child = child.nextSibling();
                            }
                        }
                        else
                        {
                            if(havePFooter)                               
                                endPage(pageNode,PFootElement.cloneNode(true),_paper_acums);
                            pageNode = startPage(usable,PFooterSiz, RHeaderSiz,RFooterSiz,doc,havePHeader,PHeaderElement,selects);
                            ipageCount++;
                            root.appendChild(pageNode);
                            pageUsable = usable;

                            if(havePHeader)
                                pageUsable -= PHeaderSiz;

                            sec = doc.createElement("Section");
                            sec.toElement().setAttribute("id",2);
                            pageNode.appendChild(sec);
                            if(haveHead)
                            {
                                sec.appendChild(sectionHeader.cloneNode(true));
                                pageUsable -= headSiz;
                            }
                            pageUsable -= bEle.attribute("size").replace(",",".").toDouble();
                            sec.appendChild(body);

                            QDomNode child = body.firstChild();
                            while(!child.isNull())
                            {
                                QDomElement ele = child.toElement();

                                if(_paper_acums.contains(ele.attribute("name")))
                                {
                                    QString sValue = ele.attribute("Text");
                                    _paper_acums[ele.attribute("name")] += getNumber(sValue,ele.attribute("formato").toInt());
                                }

                                child = child.nextSibling();
                            }
                        }
                    }
                    else
                    {
                        int needed = bEle.attribute("size").replace(",",".").toDouble() + footSiz + RFooterSiz;
                        if(PFooterOnAll)
                            needed += PFooterSiz;
                        if(pageUsable > needed)
                        {
                            pageUsable -= bEle.attribute("size").replace(",",".").toDouble();
                            sec.appendChild(body);

                            QDomNode child = body.firstChild();
                            while(!child.isNull())
                            {
                                QDomElement ele = child.toElement();

                                if(_paper_acums.contains(ele.attribute("name")))
                                {
                                    QString sValue = ele.attribute("Text");
                                    _paper_acums[ele.attribute("name")] += getNumber(sValue,ele.attribute("formato").toInt());
                                }

                                child = child.nextSibling();
                            }

                            if(!bodyIt.hasNext())//last body of section & report
                            {
                                if(haveFoot)
                                {
                                    sec.appendChild(sectionFoot);
                                    pageUsable -= footSiz;
                                }
                            }
                        }
                        else
                        {
                            if(havePFooter)
                                endPage(pageNode,PFootElement.cloneNode(true),_paper_acums);
                            pageNode = startPage(usable,PFooterSiz, RHeaderSiz,RFooterSiz,doc,havePHeader,PHeaderElement,selects);
                            ipageCount++;
                            root.appendChild(pageNode);
                            pageUsable = usable;

                            if(havePHeader)
                                pageUsable -= PHeaderSiz;

                            sec = doc.createElement("Section");
                            sec.toElement().setAttribute("id",2);
                            pageNode.appendChild(sec);
                            if(haveHead)
                            {
                                sec.appendChild(sectionHeader.cloneNode(true));
                                pageUsable -= headSiz;
                            }
                            pageUsable -= bEle.attribute("size").replace(",",".").toDouble();
                            sec.appendChild(body);

                            QDomNode child = body.firstChild();
                            while(!child.isNull())
                            {
                                QDomElement ele = child.toElement();

                                if(_paper_acums.contains(ele.attribute("name")))
                                {
                                    QString sValue = ele.attribute("Text");
                                    _paper_acums[ele.attribute("name")] += getNumber(sValue,ele.attribute("formato").toInt());
                                }

                                child = child.nextSibling();
                            }

                            if(!bodyIt.hasNext())//last body
                            {
                                if(haveFoot)
                                {
                                    sec.appendChild(sectionFoot);
                                    pageUsable -= footSiz;
                                }                               
                            }
                        }
                    }
                }
            }
        }
    }

    if(havePFooter && PFooterOnAll)
        endPage(pageNode,PFootElement.cloneNode(true),_paper_acums);
    if(haveRFooter)
    {
        pageNode.appendChild(RFootElement);
        QDomNode sec = pageNode.firstChild();
        for(int i= 0; i<2; i++)
        {
            if(sec.toElement().attribute("id").toDouble() == 0 || sec.toElement().attribute("id").toDouble() == 1)//"Cabecera de report" || "Cabecera de pagina"
            {
                QDomNode ele = sec.firstChild();
                while(!ele.isNull())
                {
                    if(ele.toElement().attribute("id") == "Line")
                        if(ele.toElement().attribute("endPointName")!= "Self")
                            if(ele.toElement().attribute("endPointName")!= "Pie de report")
                                ele.toElement().setAttribute("h",ele.toElement().attribute("h").toDouble()-RFooterSiz);
                    if(ele.toElement().attribute("id") == "RoundRect")
                        if(ele.toElement().attribute("endPointName")!= "Self")
                            if(ele.toElement().attribute("endPointName")!= "Pie de report")
                                ele.toElement().setAttribute("h",ele.toElement().attribute("h").toDouble()-RFooterSiz);
                    ele = ele.nextSibling();
                }
            }
            sec = sec.nextSibling();
        }

        QDomNode child = RFootElement.firstChild();
        while(!child.isNull())
        {
            QDomElement ele = child.toElement();
            if(ele.attribute("id")=="Acumulador")
            {
                ele.setAttribute("id","Label");
                QString text = "";
                int formato = ele.attribute("formato").toDouble();
                text = applyFormato(QString::number(_paper_acums.value(ele.attribute("target")),'f',2),formato);
                ele.setAttribute("Text",text);
            }
            child = child.nextSibling();
        }
    }

    root.setAttribute("pages",ipageCount);
    return doc;
}
bool ReportRenderer::limit() const
{
    return _limit;
}

void ReportRenderer::setLimit(bool limit)
{
    _limit = limit;
}

QMap<QString, QString> ReportRenderer::params() const
{
    return _params;
}

void ReportRenderer::setParams(const QMap<QString, QString> &params)
{
    _params = params;
}


void ReportRenderer::drawRect(QDomElement e, QPainter *painter, double dpiX, double dpiY, int printResolution)
{
    painter->save();
    QPointF pos;
    pos.setX(e.attribute("x").toDouble() * dpiX);
    pos.setY(e.attribute("y").toDouble() * dpiY);

    QSizeF siz;
    siz.setWidth(e.attribute("w").toDouble()* dpiX);
    QString sH = e.attribute("h");
    sH.replace(",",".");
    siz.setHeight(sH.toDouble()* dpiY);

    QColor penColor = ColorFromString(e.attribute("PenColor"));
    int penW = e.attribute("PenWidth").toDouble();

    QColor color1 , color2;
    color1 = ColorFromString(e.attribute("Color1"));
    color2 = ColorFromString(e.attribute("Color2"));

    double r1, r2;
    r1 = e.attribute("RadiousX").toDouble()* dpiX;
    r2 = e.attribute("RadiousY").toDouble()* dpiY;

    bool vGrad , gUsed;
    gUsed = e.attribute("GradientUsed").toDouble();
    vGrad = e.attribute("GradientDirection") == "V";

    painter->setRenderHints(QPainter::Antialiasing);
    QPen pen(painter->pen());
    pen.setWidthF((penW / 100) * printResolution);
    pen.setColor(penColor);
    painter->setPen(pen);

    painter->translate(pos);
    QRectF r(QPointF(0,0),siz);
    if(gUsed)
    {
        if(vGrad)
        {
            QLinearGradient Gradient(r.width()/2,0,r.width()/2,r.height());
            Gradient.setColorAt(0,color1);
            Gradient.setColorAt(1,color2);
            painter->setBrush(QBrush(Gradient));
        }
        else
        {
            QLinearGradient Gradient(0,r.height()/2,r.width(),r.height()/2);
            Gradient.setColorAt(0,color1);
            Gradient.setColorAt(1,color2);
            painter->setBrush(QBrush(Gradient));
        }
    }
    else
    {
        painter->setBrush(QBrush(color1));
    }
    QRectF rr(pen.widthF()/2.0,
              pen.widthF()/2.0,
              r.width()- pen.widthF(),
              r.height()- pen.widthF());
    painter->drawRoundedRect(rr,r1,r2);
    painter->restore();
}

void ReportRenderer::drawLabel(QDomElement e, QPainter *painter, double dpiX, double dpiY)
{
    QString m_Text = e.attribute("Text");
    _Orientacion m_Orientacion = e.attribute("Orientacion") == "V" ? Vertical : Horizontal;

    _Aling m_Alineacion =  e.attribute("Alineacion") == "L" ?  Left :
                           e.attribute("Alineacion") == "R" ?  Rigth:
                                                               Center;
    QString m_fontFamily = e.attribute("fontFamily");
    int m_fontSize = e.attribute("fontSize").toDouble();
    int m_fontWeigth = e.attribute("fontWeigth").toDouble();
    bool m_italicFont = e.attribute("italicFont").toDouble();

    QColor m_fontColor = ColorFromString(e.attribute("color"));
    bool m_underlined = e.attribute("underlined").toDouble();

    QPointF pos;
    pos.setX(e.attribute("x").toDouble() * dpiX);
    pos.setY(e.attribute("y").toDouble() * dpiY);

    QSizeF siz;
    siz.setWidth(e.attribute("w").toDouble()* dpiX);
    siz.setHeight(e.attribute("h").toDouble()* dpiY);

    painter->save();

    painter->translate(pos);
    QRectF r(QPointF(0,0),siz);

    QFont f(m_fontFamily,m_fontSize);
    f.setUnderline(m_underlined);
    f.setWeight(m_fontWeigth);
    f.setItalic(m_italicFont);
    painter->setPen(m_fontColor);
    painter->setFont(f);
    if(m_Orientacion == Vertical)
    {
        QRectF r2(0,0,r.height(),r.width());
       // QRectF r2(painter->fontMetrics().boundingRect(aux1.toRect(),Qt::TextWordWrap,m_Text));

        painter->translate(r.bottomLeft());
        painter->rotate(270);
        switch (m_Alineacion) {
        case Center:
            painter->drawText(r2,Qt::TextWordWrap|Qt::AlignCenter,m_Text);
            break;
        case Rigth:
            painter->drawText(r2,Qt::TextWordWrap|Qt::AlignRight,m_Text);
            break;
        default:
            painter->drawText(r2,Qt::TextWordWrap|Qt::AlignLeft,m_Text);
            break;
        }
       // painter->drawRect(aux1);
    }
    else
    {
        switch (m_Alineacion) {
        case Center:
            painter->drawText(r,Qt::TextWordWrap|Qt::AlignCenter,m_Text);
            break;
        case Rigth:
            painter->drawText(r,Qt::TextWordWrap|Qt::AlignRight,m_Text);
            break;
        default:
            painter->drawText(r,Qt::TextWordWrap|Qt::AlignLeft,m_Text);
            break;
        }
    }
    painter->restore();
}

void ReportRenderer::drawLine(QDomElement e, QPainter *painter, double dpiX, double dpiY, int printResolution)
{
    int  m_penWidth = e.attribute("penWidth").toDouble();
    QColor m_penColor = ColorFromString(e.attribute("penColor"));
    _Orientacion m_Orientacion = e.attribute("Orientacion") == "V" ? Vertical : Horizontal;
    Qt::PenStyle m_penStyle = static_cast<Qt::PenStyle>(e.attribute("penStyle").toInt());

    QPointF pos;
    pos.setX(e.attribute("x").toDouble() * dpiX);
    pos.setY(e.attribute("y").toDouble() * dpiY);

    QSizeF siz;
    siz.setWidth(e.attribute("w").toDouble()* dpiX);
    QString sH = e.attribute("h");
    sH.replace(",",".");
    siz.setHeight(sH.toDouble()* dpiY);

    painter->save();

    painter->translate(pos);
    QRectF r(QPointF(0,0),siz);

    QPen pen;
    pen.setWidthF((m_penWidth / 100) * printResolution);
    pen.setStyle(m_penStyle);
    pen.setColor(m_penColor);
    painter->setPen(pen);
    if(m_Orientacion == Horizontal)
        painter->drawLine(0,r.height()/2,r.width(),r.height()/2);
    else
        painter->drawLine(r.width()/2,0,r.width()/2,r.height());
    painter->restore();
}

void ReportRenderer::drawImage(QDomElement e, QPainter *painter, double dpiX, double dpiY)
{
    QString m_ruta = e.attribute("Path");
    bool m_fromDB = e.attribute("fromBD").toDouble();
    QImage m_image;
    bool m_dinamica = e.attribute("Dinamic").toDouble();

    QPointF pos;
    pos.setX(e.attribute("x").toDouble() * dpiX);
    pos.setY(e.attribute("y").toDouble() * dpiY);

    QSizeF siz;
    siz.setWidth(e.attribute("w").toDouble()* dpiX +10);
    siz.setHeight(e.attribute("h").toDouble()* dpiY);

    QPixmap pix = RedFoX::Core::Functions::getPixmap(m_ruta);
    m_image = pix.toImage();

    painter->save();
    m_image = m_image.scaled(siz.toSize(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    painter->translate(pos);
    QRectF r(QPointF(0,0),siz);

    painter->drawImage(r,m_image);
    painter->restore();

}

void ReportRenderer::drawCodeBar(QDomElement e, QPainter *painter, double dpiX, double dpiY)
{
    QString m_code = e.attribute("Code");
    bool m_visibleCode = e.attribute("visibleCode").toDouble();

    QPointF pos;
    pos.setX(e.attribute("x").toDouble() * dpiX);
    pos.setY(e.attribute("y").toDouble() * dpiY);

    QSizeF siz;
    siz.setWidth(e.attribute("w").toDouble()* dpiX +10);
    siz.setHeight(e.attribute("h").toDouble()* dpiY+10);

    int codeSize = (e.attribute("codeSize").toInt());
    int barSize = (e.attribute("barSize").toInt());
    bool vertical = (e.attribute("vertical").toInt());

    painter->save();

    painter->translate(pos);


    QFont f1 = QFont(painter->font().family(),codeSize);

    QFont f("Free 3 of 9 Extended",barSize);
    painter->setFont(f);

    if(vertical)
    {
        QRectF _r(QPointF(0,0),siz);
        QRectF r(0,0,siz.height(),siz.width()+codeSize*dpiX);
        painter->translate(_r.bottomLeft());
        painter->rotate(270);
        painter->drawText(r,Qt::TextWordWrap|Qt::AlignCenter,m_code);
        if(m_visibleCode)
        {
            painter->setFont(f1);
            painter->drawText(r,Qt::AlignBottom|Qt::AlignCenter|Qt::TextJustificationForced,m_code);
        }
    }
    else
    {
        QRectF r(QPointF(0,0),siz);
        painter->drawText(r,Qt::AlignTop|Qt::AlignHCenter,m_code);

        if(m_visibleCode)
        {
            painter->setFont(f1);
            painter->drawText(r,Qt::AlignBottom|Qt::AlignCenter|Qt::TextJustificationForced,m_code);
        }
    }
    painter->restore();
}

QPair<QString,QString> ReportRenderer::getSql(QString s, QMap<QString, QString> queryClausules, bool prev)
{
    QStringList l = s.split(".");
    if(l.size()==2)
    {
        QString second = l.at(1);
        QString clausula = queryClausules.value(s);
        QString final;
        QTextStream t(&final);
        t << "SELECT * FROM " << second;
        if(!clausula.isEmpty())
            t << " WHERE " << clausula;
        if(prev)
            t << " limit 100";
        t<<";";
        QPair<QString,QString> p;
        p.first = s;
        p.second= final;
        return p;
    }
    else if(l.size()>2)
    {
        //s = General.articulos.id_familia=General.familias.id->familia
        QStringList split = s.split("=");
        if(split.size()==2)
        {
            QStringList l2 = split.at(0).split(".");
            QString out = QString("%1.%2").arg(l2.at(0)).arg(l2.at(1));
            return getSql(out , queryClausules,prev);
        }
    }
    return QPair<QString,QString>(QString(),QString());
}

QString ReportRenderer::getRelationField(QString s , QSqlRecord r)
{
    //s = General.articulos.id_familia=General.familias.id->familia
    QStringList split = s.split("=");
    if(split.size()==2)
    {
        QString value = r.value(split.at(0).split(".").at(2)).toString();
        QStringList to = split.at(1).split("->");
        QString key = to.at(0);

        QString final;
        QStringList l = key.split(".");
        if(l.size()==3)
        {
            QString second = l.at(1);
            QTextStream t(&final);
            t << "SELECT * FROM " << second;
            t << " WHERE " << l.at(2) << "=" << value;
            t<<";";
        }
        //TODO others db
        QSqlDatabase db;
        if(key.startsWith("Gen"))
            db = Configuracion_global->groupDB;
        else if(key.startsWith("Emp"))
            db = Configuracion_global->empresaDB;

        QSqlQuery q(db);
        if(q.exec(final))
            if(q.next())
                return q.record().value(to.at(1)).toString();
    }
    return "";
}

QDomNode ReportRenderer::startPage(double pageUsable ,  int PFooterSiz, int RHSiz,int RFootSiz, QDomDocument doc, bool pageHeader, QDomNode pHeaderNode, QMap<QString, QSqlRecord> selects, bool reporHeader, QDomNode rHeaderNode)
{
    QDomNode toRet = doc.createElement("Page");
    if(reporHeader)
    {
        QDomNode child = rHeaderNode.firstChild();
        while(!child.isNull())
        {
            QDomElement ele = child.toElement();
            if(ele.attribute("id")=="Field")
            {
                ele.setAttribute("id","Label");
                QString text = "";
                int formato = ele.attribute("formato").toDouble();
                QStringList value = ele.attribute("Sql").split(".");
                if(value.size()== 3)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    text = applyFormato(selects.value(key).value(value.at(2)).toString(),formato);
                }
                ele.setAttribute("Text",text);
            }
            else if(ele.attribute("id")=="RelationalField")
            {
                ele.setAttribute("id","Label");
                QString text = "";
                int formato = ele.attribute("formato").toDouble();
                QStringList value = ele.attribute("Sql").split(".");
                if(value.size()>1)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    text = applyFormato(getRelationField(ele.attribute("Sql"),selects.value(key)),formato);
                }
                ele.setAttribute("Text",text);
            }
            else if(ele.attribute("id")=="CodeBar")
            {
                QString text = "";
                QStringList value = ele.attribute("Sql").split(".");
                if(value.size()== 3)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    text = selects.value(key).value(value.at(2)).toString();
                }
                ele.setAttribute("Code",text);
            }
            else if(ele.attribute("id")=="Image")
            {
                QString b;
                QStringList value = ele.attribute("Path").split(".");
                if(value.size()== 3)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    b = selects.value(key).value(value.at(2)).toString();
                    ele.setAttribute("Path",b);
                }
            }
            else if(ele.attribute("id")=="Line")
            {
                bool globlaLine = ele.attribute("endPointName") != "Self";
                if(globlaLine)
                {
                    double footStart = pageUsable;
                    if(ele.attribute("endPointName") == "Pie de pagina")
                        footStart-= PFooterSiz;
                    else if(ele.attribute("endPointName") == "Pie de report")
                        footStart-= RFootSiz;

                    double LineEnd = footStart + ele.attribute("endPointPoint").toDouble();
                    double LineStart = ele.attribute("y").toDouble();
                    QString sH = QString::number(LineEnd - LineStart,'f',2).replace(",",".");
                    ele.setAttribute("h",sH);
                }
            }
            else if(ele.attribute("id")=="RoundRect")
            {
                bool globlaLine = ele.attribute("endPointName") != "Self";
                if(globlaLine)
                {
                    double footStart = pageUsable;
                    if(ele.attribute("endPointName") == "Pie de pagina")
                        footStart-= PFooterSiz;
                    else if(ele.attribute("endPointName") == "Pie de report")
                        footStart-= RFootSiz;

                    double LineEnd = footStart + ele.attribute("endPointPoint").toDouble();
                    double LineStart = ele.attribute("y").toDouble();
                    QString sH = QString::number(LineEnd - LineStart,'f',2).replace(",",".");
                    ele.setAttribute("h",sH);
                }
            }
            child = child.nextSibling();
        }
        toRet.appendChild(rHeaderNode);
        pageUsable -= rHeaderNode.toElement().attribute("size").replace(",",".").toDouble();
    }
    if(pageHeader)
    {
        QDomNode node = pHeaderNode.cloneNode(true);
        QDomNode child = node.firstChild();
        while(!child.isNull())
        {
            QDomElement ele = child.toElement();
            if(ele.attribute("id")=="Field")
            {
                ele.setAttribute("id","Label");
                QString text = "";
                int formato = ele.attribute("formato").toDouble();
                QStringList value = ele.attribute("Sql").split(".");
                if(value.size()== 3)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    text = applyFormato(selects.value(key).value(value.at(2)).toString(),formato);
                }
                ele.setAttribute("Text",text);
            }
            else if(ele.attribute("id")=="RelationalField")
            {
                ele.setAttribute("id","Label");
                QString text = "";
                int formato = ele.attribute("formato").toDouble();
                QStringList value = ele.attribute("Sql").split(".");
                if(value.size()>1)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    text = applyFormato(getRelationField(ele.attribute("Sql"),selects.value(key)),formato);
                }
                ele.setAttribute("Text",text);
            }
            else if(ele.attribute("id")=="CodeBar")
            {
                QString text = "";
                QStringList value = ele.attribute("Sql").split(".");
                if(value.size()== 3)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    text = selects.value(key).value(value.at(2)).toString();
                }
                ele.setAttribute("Code",text);
            }
            else if(ele.attribute("id")=="Image")
            {
                QString b;
                QStringList value = ele.attribute("Path").split(".");
                if(value.size()== 3)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    b = selects.value(key).value(value.at(2)).toString();
                    ele.setAttribute("Path",b);
                }
            }
            else if(ele.attribute("id")=="Line")
            {
                bool globlaLine = ele.attribute("endPointName") != "Self";
                if(globlaLine)
                {
                    double footStart = pageUsable - PFooterSiz;
                    double LineEnd = footStart + ele.attribute("endPointPoint").toDouble();
                    double LineStart = ele.attribute("y").toDouble();

                   // if(reporHeader)
                   //     LineStart+= RHSiz;
                    QString sH = QString::number(LineEnd - LineStart,'f',2).replace(",",".");
                    ele.setAttribute("h",sH);
                }
            }
            else if(ele.attribute("id")=="RoundRect")
            {
                bool globlaLine = ele.attribute("endPointName") != "Self";
                if(globlaLine)
                {
                    double footStart = pageUsable - PFooterSiz;
                    double LineEnd = footStart + ele.attribute("endPointPoint").toDouble();
                    double LineStart = ele.attribute("y").toDouble();
                    QString sH = QString::number((qreal)(LineEnd - LineStart),'f',2);
                    ele.setAttribute("h",sH);
                }
            }
            child = child.nextSibling();
        }
        toRet.appendChild(node);
    }
    doc.appendChild(toRet);
    return toRet;
}

QDomNode ReportRenderer::endPage(QDomNode pageNode, QDomNode footerNode , QMap<QString, float> _Acums)
{
    QDomNode n = footerNode.cloneNode(true);
    pageNode.appendChild(n);
    QDomNode child = n.firstChild();
    while(!child.isNull())
    {
        QDomElement ele = child.toElement();
        if(ele.attribute("id")=="Acumulador")
        {
            ele.setAttribute("id","Label");
            QString text = "";
            int formato = ele.attribute("formato").toDouble();
            text = applyFormato(QString::number(_Acums.value(ele.attribute("target")),'f',2),formato);
            ele.setAttribute("Text",text);
        }
        child = child.nextSibling();
    }
    return n;
}

void ReportRenderer::parseFooters(QDomNode RFooter, bool haveRfooter, QDomNode PFooter, bool havePFooter, QMap<QString, QSqlRecord> selects)
{
    if(haveRfooter)
    {
        QDomNode child = RFooter.firstChild();
        while(!child.isNull())
        {
            QDomElement ele = child.toElement();
            if(ele.attribute("id")=="Field")
            {
                ele.setAttribute("id","Label");
                QString text = "";
                int formato = ele.attribute("formato").toDouble();
                QStringList value = ele.attribute("Sql").split(".");
                if(value.size()== 3)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    text = applyFormato(selects.value(key).value(value.at(2)).toString(),formato);
                }
                ele.setAttribute("Text",text);
            }
            else if(ele.attribute("id")=="RelationalField")
            {
                ele.setAttribute("id","Label");
                QString text = "";
                int formato = ele.attribute("formato").toDouble();
                QStringList value = ele.attribute("Sql").split(".");
                if(value.size()>1)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    text = applyFormato(getRelationField(ele.attribute("Sql"),selects.value(key)),formato);
                }
                ele.setAttribute("Text",text);
            }
            else if(ele.attribute("id")=="CodeBar")
            {
                QString text = "";
                QStringList value = ele.attribute("Sql").split(".");
                if(value.size()== 3)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    text = selects.value(key).value(value.at(2)).toString();
                }
                ele.setAttribute("Code",text);
            }
            else if(ele.attribute("id")=="Image")
            {
                QString b;
                QStringList value = ele.attribute("Path").split(".");
                if(value.size()== 3)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    b = selects.value(key).value(value.at(2)).toString();
                   ele.setAttribute("Path",QString(b));
                }
            }
            child = child.nextSibling();
        }
        //toRet.appendChild(rHeaderNode);
    }
    if(havePFooter)
    {
        QDomNode child = PFooter.firstChild();
        while(!child.isNull())
        {
            QDomElement ele = child.toElement();
            if(ele.attribute("id")=="Field")
            {
                ele.setAttribute("id","Label");
                QString text = "";
                int formato = ele.attribute("formato").toDouble();
                QStringList value = ele.attribute("Sql").split(".");
                if(value.size()== 3)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    text = applyFormato(selects.value(key).value(value.at(2)).toString(),formato);
                }
                ele.setAttribute("Text",text);
            }
            else if(ele.attribute("id")=="RelationalField")
            {
                ele.setAttribute("id","Label");
                QString text = "";
                int formato = ele.attribute("formato").toDouble();
                QStringList value = ele.attribute("Sql").split(".");
                if(value.size()>1)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    text = applyFormato(getRelationField(ele.attribute("Sql"),selects.value(key)),formato);
                }
                ele.setAttribute("Text",text);
            }
            else if(ele.attribute("id")=="CodeBar")
            {
                QString text = "";
                QStringList value = ele.attribute("Sql").split(".");
                if(value.size()== 3)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    text = selects.value(key).value(value.at(2)).toString();
                }
                ele.setAttribute("Code",text);
            }
            else if(ele.attribute("id")=="Image")
            {
                QString b;
                QStringList value = ele.attribute("Path").split(".");
                if(value.size()== 3)
                {
                    QString key = value.at(0) + "." + value.at(1);
                    b = selects.value(key).value(value.at(2)).toString();
                    ele.setAttribute("Path",b);
                }
            }
            child = child.nextSibling();
        }
    }
}


QColor ReportRenderer::ColorFromString(QString s)
{
    QStringList l= s.split(",");
    QColor c;
    c.setRed  (l.at(0).toDouble());
    c.setGreen(l.at(1).toDouble());
    c.setBlue (l.at(2).toDouble());
    c.setAlpha(l.at(3).toDouble());
    return c;
}
float ReportRenderer::getNumber(QString in, int formato)
{
    /*0 = sin formato
    *1 = 999.999.999,99
    *2 = 999,999,999.99
    *3 = 99999999999,99
    *4 = 99999999999.99*
    *5 = 999.999.999,999
    *6 = 999.999.999,9999
    *7 = dd/mm/aa
    *8 = dd/mm/aaaa
    */
    float f = 0;
    switch (formato) {
    case 0:
    case 3:
    case 4:
        f= in.toFloat();
        break;
    case 1:
    case 5:
    case 6:
        in.replace(".","");
        in.replace(",",".");
        f= in.toFloat();
        break;
    case 2:
        in.replace(",","");
        f = in.toFloat();
        break;
    default:
        f= 0;
    }
    return f;
}

QString ReportRenderer::applyFormato(QString in, int formato)
{
    /*0 = sin formato
    *1 = 999.999.999,99
    *2 = 999,999,999.99
    *3 = 99999999999,99
    *4 = 99999999999.99*
    *5 = 999.999.999,999
    *6 = 999.999.999,9999
    *7 = dd/mm/aa
    *8 = dd/mm/aaaa
    * 9 = 0:1 -> Si:No
    */
        // TODO - TERMINAR FORMATO FECHA
        if(formato == 0 || formato > 9 /*9 es el maximo ahora, si metes mas, aumenta esto*/)
            return in.trimmed();

        bool ok;
        double d = in.toDouble(&ok);
        if(!ok)
            return in;

        QString aux = QString::number(d, 'f' , 2); //9999999999.99 ,2 porque solo queriamos dos decimales
        QString aux2 = QString::number(d, 'f' , 3);
        QString aux3 = QString::number(d, 'f' , 4);



        if(formato == 4)
            return aux;
        else if(formato == 3)
            return aux.replace(".",",");
        else if(formato == 2)
        {
            QString entero = aux.split(".").at(0);
            QString final;
            int count = 0;
            for(int i = entero.size()-1 ; i>= 0 ; i--)
            {
                final.prepend(entero.at(i));
                count++;
                if(count%3 == 0 && i != 0)
                    final.prepend(",");

            }
            final.append(".");
            final.append(aux.split(".").at(1));
            return final;
        }
        else if(formato == 1)
        {
            aux.replace(".",",");//9999999999,99
            QString entero = aux.split(",").at(0);
            QString final;
            int count = 0;
            for(int i = entero.size()-1 ; i>= 0 ; i--)
            {
                final.prepend(entero.at(i));
                count++;
                if(count%3 == 0 && i != 0)
                    final.prepend(".");

            }
            final.append(",");
            final.append(aux.split(",").at(1));
            return final;
        }
        else if(formato == 5 /*6 , 7 , 8 etc*/)//5 = 999.999.999,999
        {
            //haces el formato y lo devuelves
            aux2.replace(".",",");//9999999999,999
            QString entero = aux2.split(",").at(0);
            QString final;
            int count = 0;
            for(int i = entero.size()-1 ; i>= 0 ; i--)
            {
                final.prepend(entero.at(i));
                count++;
                if(count%3 == 0 && i != 0)
                    final.prepend(".");

            }
            final.append(",");
            final.append(aux2.split(",").at(1));
            return final;
        }
        else if(formato == 6 /*6 , 7 , 8 etc*/)//5 = 999.999.999,9999
        {
            //haces el formato y lo devuelves
            aux3.replace(".",",");//9999999999,999
            QString entero = aux3.split(",").at(0);
            QString final;
            int count = 0;
            for(int i = entero.size()-1 ; i>= 0 ; i--)
            {
                final.prepend(entero.at(i));
                count++;
                if(count%3 == 0 && i != 0)
                    final.prepend(".");

            }
            final.append(",");
            final.append(aux3.split(",").at(1));
            return final;
        }
    // else if(formato == 7) //5 = dd/mm/aa
    // {
    // QDate fecha_formateada;
    // //haces el formato y lo devuelves
    // //aux3 = fecha
    // return fecha_formateada;
    // }
    // else if(formato == 8) //5 = dd/mm/aaaa
    // {
    // QDate fecha_formateada;
    // //haces el formato y lo devuelves
    // //aux3 = fecha
    // return fecha_formateada;
    // }
        else if(formato == 9)
        {
            QString out;
            (in == "1") ? out = tr("Sí") : out = tr("No");
            return out;
        }
}

QString ReportRenderer::ColorString(QColor c)
{
    return QString("%1,%2,%3,%4").arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
}

QPrinter *ReportRenderer::getPrinter() const
{
    return printer;
}

void ReportRenderer::setPrinter(QPrinter *value)
{
    printer = value;
}

QDomDocument ReportRenderer::getDocIn() const
{
    return DocIn;
}

void ReportRenderer::setDocIn(const QDomDocument &value)
{
    DocIn = value;
}

QMap<QString, QString> ReportRenderer::getQueryClausules() const
{
    return queryClausules;
}

void ReportRenderer::setQueryClausules(const QMap<QString, QString> &value)
{
    queryClausules = value;
}
