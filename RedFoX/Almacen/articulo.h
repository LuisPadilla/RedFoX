#ifndef ARTICULO_H
#define ARTICULO_H

#include "Auxiliares/Globlal_Include.h"

class Articulo : public QObject
{
    Q_OBJECT
public:

    int id;
    QString codigo;
    QString codigo_barras;
    QString codigo_fabricante;
    QString descripcion;
    QString descripcion_reducida;
    int idProveedor;
    bool kit;
    QString proveedor;
    QString cCodProveedor;
    int id_familia;
    QString familia;
    QString cod_familia;
    int id_seccion;
    QString seccion;
    QString cod_seccion;
    int id_subfamilia;
    QString subfamilia;
    QString cod_subfamilia;
    QString codigo_iva;
    double tipo_iva;
    float porc_dto;
    QDate fecha_ultima_compra;
    QDate fecha_ultima_venta;
    double importe_acumulado_compras;
    double importe_acumulado_ventas;
    QString comentario;
    int stock_maximo;
    int stock_minimo;
    int stock_real;
    int nstock_fisico_almacen;
    QString tipo_unidad;
    bool controlar_stock;
    QString modelo;
    QString talla;
    QString color;
    QString composicion;
    int pvp_incluye_iva;
    QDate fecha_prevista_recepcion;
    int cantidad_pendiente_recibir;
    int unidades_reservadas;
    int mostrar_web;
    int etiquetas;
    int paquetes;
    QString localizacion_en_almacen;
    double pvp;
    int id_tipos_iva;


    int id_web;
    int unidades_compradas;
    int unidades_vendidas;
    double coste;
    bool articulo_promocionado;
    bool mostrar_en_cuadro;
    double margen;
    double margen_min;
    double coste_real;
    Articulo(QObject* parent = 0);
    //-----------
    // Metodos
    //-----------
    void Anadir();
    bool Recuperar(QString cSQL);
    void Recuperar(QString cSQL,int nProcede);
    bool Recuperar(int id);
    bool Next();
    bool Prev();
    void Cargar(QSqlRecord registro);
    bool Guardar();
    void Vaciar();
    void Borrar(int nid,bool isKit, bool ask , QString codigo = QString());
    QHash<QString,QVariant> Vender(QString codigo, int cantidad, int tarifa, int tipo_dto_tarifa, int id_familia_cliente,
                                   int id_cliente);
    bool Devolucion(int id, double cantidad, double pvp);


    static bool acumulado_ventas   (int id_articulo,float cantidad,double total, QDate fecha);
    static bool acumulado_compras  (int id_articulo,float cantidad,double total, QDate fecha);
    static bool acum_devolucion_cli(int id_articulo,float cantidad,double total, QDate fecha);
    static bool acum_devolucion_pro(int id_articulo,float cantidad,double total, QDate fecha);

    static bool set_pendiente_recibir(int id_articulo, double cantidad);
    static bool agregar_stock_fisico(int id_articulo, double cantidad);
    static bool reservar(int id_articulo, double cantidad);

    static bool update_coste_kits(int id_articulo, double new_coste);


    int getidSeccion(QString seccion_);
    int getidFamilia(QString familia_);
    int getidSubFamilia(QString subfamilia_);

    QString getseccion(int nid);
    QString getfamilia(int nid);
    QString getsubfamilia(int nid);

    bool agregar_proveedor_alternativo(int id_art, int idProveedor, QString codigo, double pvd, QString descoferta,
                                       QString oferta, double pvd_real, int id_divisa);
    bool guardarProveedorAlternativo(int id, QString codigo, double pvd, QString descoferta,
                                       QString oferta, double pvd_real, int id_divisa);
    bool cambiarProveedorPrincipal(int id,int idProveedor);

    float asigna_dto_linea(int id_art, int id_cliente, float dto_esp, float dto_lin);

private:

    QSqlQuery qryArticulo;
    QSqlQuery qryTipoIva;
    double coste_real_anterior;
    QString codigo_anterior; //Solo valido para nuevos articulos
};

#endif // ARTICULO_H
