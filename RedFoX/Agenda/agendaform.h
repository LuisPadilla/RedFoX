#ifndef AGENDAFORM_H
#define AGENDAFORM_H

#include <QDialog>
#include <QtGui>
#include <QGraphicsScene>
#include "graphicstable.h"
#include "../mayamodule.h"
namespace Ui {
class AgendaForm;
}

class AgendaForm : public MayaModule
{
    Q_OBJECT
    
public:
    explicit AgendaForm(QWidget *parent = 0);
    ~AgendaForm();

    module_zone module_zone(){return Utilidades;}
    QString module_name(){return "Agenda";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    QPushButton* wantShortCut(bool& ok){ok = false; return 0;}
private slots:
    void on_pushButton_clicked();

    void on_calendarWidget_activated(const QDate &date);

    void on_combo_user_currentIndexChanged(int index);

    void on_btn_hoy_clicked();

    void on_btn_buscaUser_clicked();

    void on_btn_zoomIn_clicked();
    void on_btn_zoomOut_clicked();

private:
    Ui::AgendaForm *ui;
    QGraphicsScene * scene;
    void resizeEvent(QResizeEvent * e );
    GraphicsTable * table;
    QColor event_color;
    QAction menuButton;
};

#endif // AGENDAFORM_H
