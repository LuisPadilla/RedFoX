#include "monetarydelegate.h"
//#include "configuracion.h"
#include "Globlal_Include.h"

MonetaryDelegate::MonetaryDelegate(QObject *parent) :
    QItemDelegate(parent)
{

}

void MonetaryDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineedit = static_cast<QLineEdit*>(editor);
    lineedit->text();
    QString valor = Configuracion_global->FormatoNumerico(lineedit->text());
    model->setData(index,valor,Qt::EditRole);
}

void MonetaryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString texto = index.model()->data(index, Qt::DisplayRole).toString();
     texto = Configuracion_global->FormatoNumerico(texto);

     QStyleOptionViewItem myOption = option;
     myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
     drawDisplay(painter, myOption, myOption.rect,texto);
     drawFocus(painter, myOption, myOption.rect);
}