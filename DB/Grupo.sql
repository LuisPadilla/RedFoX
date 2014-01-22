CREATE SCHEMA IF NOT EXISTS `@grupo@` DEFAULT CHARACTER SET utf8 ;
CREATE TABLE `@grupo@`.`accesousuarios` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_user` int(11) DEFAULT NULL,
  `id_modulo` int(11) DEFAULT NULL,
  `id_nivel_acceso` int(11) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`agenda` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `fecha` date DEFAULT NULL,
  `hora` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_usuario` int(11) DEFAULT '0',
  `inicio` datetime DEFAULT NULL,
  `fin` datetime DEFAULT NULL,
  `asunto` varchar(40) COLLATE utf8_unicode_ci DEFAULT NULL,
  `descripcion_asunto` longtext COLLATE utf8_unicode_ci,
  `estado` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `avisar_tiempo` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `importancia` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `color` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_especialidad` int(11) DEFAULT NULL,
  `id_departamento` int(11) DEFAULT NULL,
  `is_medica` tinyint(1) DEFAULT NULL,
  `id_Cliente` int(11) DEFAULT NULL,
  `is_cita` tinyint(1) DEFAULT NULL,
  `is_privado` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`agentes` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(45) DEFAULT NULL,
  `codigo` varchar(15) DEFAULT NULL,
  `dni` varchar(15) DEFAULT NULL,
  `telefono` varchar(15) DEFAULT NULL,
  `movill` varchar(15) DEFAULT NULL,
  `email` varchar(150) DEFAULT NULL,
  `comentarios` text,
  `facturado` double DEFAULT NULL,
  `pendiente` double DEFAULT NULL,
  `com1` double DEFAULT '0',
  `com2` double DEFAULT '0',
  `com3` double DEFAULT '0',
  `com4` double DEFAULT '0',
  `com5` double DEFAULT '0',
  `com6` double DEFAULT '0',
  `com_clientes_contado` double DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`articulos` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `codigo` varchar(200) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL,
  `codigo_barras` varchar(30) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL,
  `codigo_fabricante` varchar(100) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL,
  `kit` tinyint(1) DEFAULT '0',
  `descripcion` text CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  `descripcion_reducida` varchar(100) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_proveedor` int(11) DEFAULT NULL,
  `id_familia` int(11) DEFAULT NULL,
  `id_seccion` int(11) DEFAULT NULL,
  `id_subfamilia` int(11) DEFAULT NULL,
  `tipo_iva` float DEFAULT '0',
  `coste` double DEFAULT '0',
  `porc_dto` float DEFAULT '0',
  `dto_proveedor` double DEFAULT '0',
  `fecha_ultima_compra` date DEFAULT NULL,
  `fecha_ultima_venta` date DEFAULT NULL,
  `unidades_compradas` float DEFAULT '0',
  `unidades_vendidas` float DEFAULT '0',
  `importe_acumulado_compras` double DEFAULT '0',
  `importe_acumulado_ventas` double DEFAULT '0' COMMENT '	',
  `imagen` blob,
  `comentario` text CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  `stock_maximo` float DEFAULT '0',
  `stock_minimo` float DEFAULT '0',
  `stock_real` float DEFAULT '0',
  `tipo_unidad` varchar(100) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL,
  `controlar_stock` int(11) DEFAULT '0',
  `pvp_incluye_iva` tinyint(1) DEFAULT '0',
  `fecha_prevista_recepcion` date DEFAULT NULL,
  `cantidad_pendiente_recibir` float DEFAULT '0',
  `unidades_reservadas` float DEFAULT '0',
  `mostrar_web` tinyint(1) DEFAULT '0',
  `etiquetas` int(11) DEFAULT '0',
  `paquetes` int(11) DEFAULT '0',
  `localizacion_en_almacen` varchar(100) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_tipos_iva` int(11) DEFAULT NULL,
  `id_subsub_familia` int(11) DEFAULT NULL,
  `id_grupo_art` int(11) DEFAULT NULL,
  `id_web` int(11) DEFAULT '0',
  `stock_fisico_almacen` float DEFAULT NULL,
  `articulo_promocionado` tinyint(1) DEFAULT NULL,
  `mostrar_en_cuadro` tinyint(1) NOT NULL DEFAULT '0',
  `imagen2` blob,
  `imagen3` blob,
  `imagen4` blob,
  `margen` double DEFAULT NULL,
  `margen_min` double DEFAULT NULL,
  `coste_real` double DEFAULT NULL,
  `valor_stock` double NOT NULL DEFAULT '0',
  `lotes` tinyint(255) NOT NULL DEFAULT '0',
  `pvp` double DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `cCodigo_UNIQUE` (`codigo`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;

CREATE TABLE `@grupo@`.`articulos_lotes` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `id_articulo` INT NULL,
  `lote` VARCHAR(45) NULL,
  `caducidad` DATE NULL,
  PRIMARY KEY (`id`));

CREATE TABLE `@grupo@`.`articulos_prov_frec` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_articulo` int(11) DEFAULT NULL,
  `id_proveedor` int(11) DEFAULT NULL,
  `pvd` double DEFAULT NULL,
  `oferta` varchar(45) DEFAULT NULL,
  `codigo` varchar(20) DEFAULT NULL,
  `descoferta` varchar(45) DEFAULT NULL,
  `pvd_real` double DEFAULT NULL,
  `id_divisa` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`articulos_volumen` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_producto` int(11) DEFAULT NULL,
  `id_tarifa` int(11) DEFAULT '1',
  `desde` float DEFAULT NULL,
  `hasta` float DEFAULT NULL,
  `precio` double DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`avisos` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_tipoaviso` int(11) DEFAULT NULL,
  `id_usuario_origen` int(11) DEFAULT NULL,
  `id_usuario_destino` int(11) DEFAULT NULL,
  `aviso` text,
  `fecha_hora_aviso` datetime DEFAULT NULL,
  `completado` tinyint(1) DEFAULT '0',
  `id_empresa` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`bancos` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `entidad` varchar(4) COLLATE utf8_unicode_ci DEFAULT NULL,
  `oficina` varchar(4) COLLATE utf8_unicode_ci DEFAULT NULL,
  `dc` varchar(2) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cuenta` varchar(10) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '\n',
  `saldo` decimal(9,2) DEFAULT '0.00',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`calles` (
  `nombre_calle` varchar(60) DEFAULT NULL,
  `inp_inf` varchar(60) DEFAULT NULL,
  `inp_sup` varchar(60) DEFAULT NULL,
  `par_inf` varchar(60) DEFAULT NULL,
  `par_sup` varchar(60) DEFAULT NULL,
  `cp` varchar(60) DEFAULT NULL,
  `poblacion` varchar(60) DEFAULT NULL,
  `provincia` varchar(60) DEFAULT NULL,
  `id` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`cliente_direcciones` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(45) DEFAULT NULL,
  `direccion1` varchar(45) DEFAULT NULL,
  `direccion2` varchar(45) DEFAULT NULL,
  `cp` varchar(8) DEFAULT NULL,
  `poblacion` varchar(45) DEFAULT NULL,
  `provincia` varchar(45) DEFAULT NULL,
  `id_pais` int(11) DEFAULT NULL,
  `email` varchar(100) DEFAULT NULL,
  `comentarios` text,
  `id_cliente` int(11) DEFAULT NULL,
  `direccion_envio` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`clientes` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `codigo_cliente` varchar(25) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_divisa` int(11) NOT NULL DEFAULT '1',
  `apellido1` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `apellido2` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `nombre` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `nombre_fiscal` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `nombre_comercial` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `persona_contacto` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cif_nif` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `direccion1` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `direccion2` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cp` varchar(12) COLLATE utf8_unicode_ci DEFAULT NULL,
  `poblacion` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `provincia` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_pais` int(11) DEFAULT NULL,
  `telefono1` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `telefono2` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `fax` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `movil` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `email` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `web` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `fecha_alta` date DEFAULT NULL,
  `fecha_ultima_compra` date DEFAULT NULL,
  `acumulado_ventas` double DEFAULT '0',
  `ventas_ejercicio` double DEFAULT '0',
  `riesgo_maximo` double DEFAULT '0',
  `deuda_actual` double DEFAULT '0',
  `comentarios` text COLLATE utf8_unicode_ci,
  `bloqueado` double DEFAULT '0',
  `comentario_bloqueo` text COLLATE utf8_unicode_ci,
  `porc_dto_cliente` float DEFAULT '0',
  `recargo_equivalencia` tinyint(1) DEFAULT '0',
  `cuenta_contable` varchar(30) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cuenta_iva_repercutido` varchar(30) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cuenta_deudas` varchar(30) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cuenta_cobros` varchar(30) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_forma_pago` int(11) DEFAULT NULL,
  `dia_pago1` int(11) DEFAULT '0',
  `dia_pago2` int(11) DEFAULT '0',
  `id_tarifa` double DEFAULT '1',
  `importe_a_cuenta` double DEFAULT '0',
  `vales` double DEFAULT '0',
  `entidad_bancaria` varchar(4) COLLATE utf8_unicode_ci DEFAULT NULL,
  `oficina_bancaria` varchar(4) COLLATE utf8_unicode_ci DEFAULT NULL,
  `dc` varchar(2) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cuenta_corriente` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `fecha_nacimiento` date DEFAULT NULL,
  `importe_pendiente` double DEFAULT '0',
  `id_tipo_cliente` int(11) DEFAULT NULL,
  `acceso_web` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `password_web` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `irpf` int(11) DEFAULT '0',
  `id_empresa` int(11) DEFAULT NULL,
  `id_idioma_documentos` int(11) DEFAULT NULL,
  `cif_vies` varchar(35) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_web` int(11) DEFAULT NULL,
  `observaciones` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `visa_distancia1` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `visa_distancia2` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `visa1_caduca_mes` int(2) DEFAULT NULL,
  `visa2_caduca_mes` int(2) DEFAULT NULL,
  `visa1_caduca_ano` int(4) DEFAULT NULL,
  `visa2_caduca_ano` int(4) DEFAULT NULL,
  `visa1_cod_valid` int(3) DEFAULT NULL,
  `visa2_cod_valid` int(3) DEFAULT NULL,
  `id_agente` int(11) DEFAULT NULL,
  `id_transportista` int(11) DEFAULT NULL,
  `grupo_iva` int(1) DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`clientes_deuda` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_empresa` int(11) NOT NULL DEFAULT '0',
  `fecha` date DEFAULT NULL,
  `vencimiento` date DEFAULT NULL,
  `documento` varchar(50) COLLATE utf8_unicode_ci DEFAULT '0',
  `id_ticket` int(11) DEFAULT '0',
  `id_factura` int(11) DEFAULT '0',
  `tipo` int(11) DEFAULT '0',
  `importe` double DEFAULT '0',
  `pagado` double DEFAULT '0',
  `pendiente_cobro` double DEFAULT '0',
  `entidad` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `oficina` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `dc` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cuenta` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `importe_efectivo` double DEFAULT NULL,
  `importe_tarjeta` double DEFAULT NULL,
  `importe_cheque` double DEFAULT NULL,
  `importe_transferencia` double DEFAULT NULL,
  `importe_domiciliacion` double DEFAULT NULL,
  `importe_internet` double DEFAULT NULL,
  `id_cliente` int(11) DEFAULT NULL,
  `importe_vale` double DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`clientes_entregas` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_cliente` int(11) DEFAULT '0',
  `fecha_entrega` date DEFAULT NULL,
  `importe` double DEFAULT '0',
  `concepto` int(11) DEFAULT '0',
  `disponible` double DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`clientes_intervalo_horario` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `dia_semana` varchar(45) DEFAULT NULL,
  `horario_manana_inicio` time DEFAULT NULL,
  `horario_manana_fin` time DEFAULT NULL,
  `horario_tarde_inicio` time DEFAULT NULL,
  `horario_tarde_fin` time DEFAULT NULL,
  `id_cliente` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`codigotarifa` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(45) DEFAULT NULL,
  `codigo_tarifa` varchar(20) DEFAULT NULL,  
  `id_pais` int(11) DEFAULT NULL,
  `id_monedas` int(11) DEFAULT NULL,
  `margen` double DEFAULT NULL,
  `margen_min` double DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `descripcion_UNIQUE` (`descripcion`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`departamento` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `departamento` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`deudas_proveedores` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '	',
  `id_documento` int(11) DEFAULT NULL,
  `documento` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `fecha_deuda` date DEFAULT NULL,
  `vencimiento` date DEFAULT NULL,
  `importe_deuda` double DEFAULT NULL,
  `pago_por` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `numero_tarjeta_cuenta` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_proveedor` int(11) DEFAULT NULL,
  `pendiente` double DEFAULT NULL,
  `id_asiento` int(11) DEFAULT NULL,
  `asiento_numero` int(11) DEFAULT NULL,
  `pagado` double DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`empresas` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `codigo` varchar(10) DEFAULT NULL,
  `nombre` varchar(75) DEFAULT NULL,
  `digitos_factura` int(11) DEFAULT NULL,
  `serie` varchar(11) DEFAULT NULL,
  `decimales_campos_totales` int(2) DEFAULT '2',
  `decimales` int(2) DEFAULT '2',
  `mostrarsiempre` tinyint(1) DEFAULT '0',
  `ruta_bd_sqlite` text,
  `ruta_db_conta` text,
  `ruta_bd_medica_sqlite` text,
  `nombre_bd` varchar(50) DEFAULT NULL,
  `nombre_db_conta` varchar(100) DEFAULT NULL,
  `nombre_bd_medica` varchar(45) DEFAULT NULL,
  `direccion` varchar(150) DEFAULT NULL,
  `cp` varchar(8) DEFAULT NULL,
  `poblacion` varchar(60) DEFAULT NULL,
  `provincia` varchar(50) DEFAULT NULL,
  `pais` varchar(40) DEFAULT NULL,
  `telefono1` varchar(20) DEFAULT NULL,
  `telefono2` varchar(20) DEFAULT NULL,
  `fax` varchar(20) DEFAULT NULL,
  `mail` text,
  `web` text,
  `cif` varchar(25) DEFAULT NULL,
  `inscripcion` text,
  `comentario_albaran` text,
  `comentario_factura` text,
  `comentario_ticket` text,
  `ejercicio` int(11) DEFAULT NULL,
  `usar_irpf` int(11) DEFAULT NULL,
  `porc_irpf` double NOT NULL DEFAULT '0',
  `codigo_cuenta_clientes` varchar(25) DEFAULT NULL,
  `codigo_cuenta_proveedores` varchar(25) DEFAULT NULL,
  `codigo_cuenta_acreedores` varchar(25) DEFAULT NULL,
  `digitos_cuenta` int(11) DEFAULT NULL,
  `clave1` text,
  `clave2` text,
  `medica` int(11) DEFAULT NULL,
  `internacional` int(11) DEFAULT NULL,
  `auto_codigo` tinyint(1) DEFAULT NULL,
  `tamano_codigo` int(11) DEFAULT NULL,
  `cuenta_cobros` varchar(45) DEFAULT NULL,
  `cuenta_pagos` varchar(45) DEFAULT NULL,
  `id_divisa` int(11) DEFAULT NULL,
  `enlace_web` tinyint(1) DEFAULT NULL,
  `contabilidad` tinyint(1) DEFAULT NULL,
  `consultas` int(11) DEFAULT NULL,
  `primer_dia_laborable` varchar(45) DEFAULT NULL,
  `ultimo_dia_laborable` varchar(45) DEFAULT NULL,
  `horario_primer_dia` varchar(45) DEFAULT NULL,
  `horario_dia_normal` varchar(45) DEFAULT NULL,
  `horario_ultimo_dia` varchar(45) DEFAULT NULL,
  `ticket_factura` tinyint(1) DEFAULT NULL,
  `margen` double DEFAULT NULL,
  `margen_minimo` double DEFAULT NULL,
  `seguimiento` tinyint(1) DEFAULT NULL,
  `id_tarifa_predeterminada` int(11) DEFAULT NULL,
  `actualizar_divisas` tinyint(1) DEFAULT NULL,
  `cuenta_ventas_mercaderias` varchar(45) DEFAULT NULL,
  `cuenta_ventas_servicios` varchar(45) DEFAULT NULL,
  `cuenta_iva_soportado1` varchar(45) DEFAULT NULL,
  `cuenta_iva_soportado2` varchar(45) DEFAULT NULL,
  `cuenta_iva_soportado3` varchar(45) DEFAULT NULL,
  `cuenta_iva_soportado4` varchar(45) DEFAULT NULL,
  `cuenta_iva_repercutido1` varchar(45) DEFAULT NULL,
  `cuenta_iva_repercutido2` varchar(45) DEFAULT NULL,
  `cuenta_iva_repercutido3` varchar(45) DEFAULT NULL,
  `cuenta_iva_repercutido4` varchar(45) DEFAULT NULL,
  `cuenta_iva_repercutido1_re` varchar(45) DEFAULT NULL,
  `cuenta_iva_repercutido2_re` varchar(45) DEFAULT NULL,
  `cuenta_iva_repercutido3_re` varchar(45) DEFAULT NULL,
  `cuenta_iva_repercutido4_re` varchar(45) DEFAULT NULL,
  `cuenta_iva_soportado1_re` varchar(45) DEFAULT NULL,
  `cuenta_iva_soportado2_re` varchar(45) DEFAULT NULL,
  `cuenta_iva_soportado3_re` varchar(45) DEFAULT NULL,
  `cuenta_iva_soportado4_re` varchar(45) DEFAULT NULL,
  `importada_sp` tinyint(1) DEFAULT '0',
  `importe_cierre` double DEFAULT NULL,
  `facturas_en_cierre` tinyint(1) DEFAULT NULL,
  `tpv_forzar_cantidad` tinyint(1) DEFAULT '0',
  `caducidad_vales` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`familias` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `codigo` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `familia` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_seccion` int(11) DEFAULT '0',
  `image` blob,
  `type` varchar(4) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`formpago` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `codigo` varchar(8) COLLATE utf8_unicode_ci DEFAULT NULL,
  `forma_pago` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `dia_pago1` int(11) DEFAULT '0',
  `dia_pago2` int(11) DEFAULT '0',
  `dia_pago3` int(11) DEFAULT '0',
  `dia_pago4` int(11) DEFAULT '0',
  `dias_entre_plazos` int(11) DEFAULT '0',
  `cuenta_cont_pago` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `numero_plazos` int(11) DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`grupos_gasto` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(100) DEFAULT NULL,
  `enero` double DEFAULT NULL,
  `febrero` double DEFAULT NULL,
  `marzo` double DEFAULT NULL,
  `abril` double DEFAULT NULL,
  `mayo` double DEFAULT NULL,
  `junio` double DEFAULT NULL,
  `julio` double DEFAULT NULL,
  `agosto` double DEFAULT NULL,
  `septiembre` double DEFAULT NULL,
  `octubre` double DEFAULT NULL,
  `noviembre` double DEFAULT NULL,
  `diciembre` double DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`gruposart` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_subsubfamilia` int(11) DEFAULT NULL,
  `grupo_art` varchar(45) DEFAULT NULL,
  `image` blob,
  `type` varchar(4) DEFAULT NULL,
  `codigo` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`histo_clientes_deuda` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_cab` int(11) NOT NULL DEFAULT '0',
  `fecha_movimiento` date DEFAULT NULL,
  `importe_anterior` double DEFAULT '0',
  `pagado` double DEFAULT '0',
  `importe_cambio` double DEFAULT NULL,
  `pendiente_cobro` double DEFAULT '0',
  `entidad` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `oficina` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `dc` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cuenta` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `importe_efectivo` double DEFAULT NULL,
  `importe_tarjeta` double DEFAULT NULL,
  `importe_cheque` double DEFAULT NULL,
  `importe_transferencia` double DEFAULT NULL,
  `importe_domiciliacion` double DEFAULT NULL,
  `importe_internet` double DEFAULT NULL,
  `importe_vale` double DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`idiomas` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `idioma` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`kits` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `codigo_kit` varchar(200) COLLATE utf8_unicode_ci DEFAULT NULL,
  `codigo_componente` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cantidad` double NOT NULL DEFAULT '0',
  `coste_base` double NOT NULL DEFAULT '0',
  `porc_dto` float DEFAULT '0',
  `coste_final` double DEFAULT NULL,
  `descripcion` varchar(200) COLLATE utf8_unicode_ci NOT NULL,
  `descontar_stock` tinyint(1) NOT NULL DEFAULT '1',
  `id_componente` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`maestro_familia_cliente` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`maestro_subfamilia_cliente` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(45) DEFAULT NULL,
  `id_maestro_familia_cliente` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`modulos` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `module_zone` int(11) DEFAULT NULL,
  `module_name` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`monedas` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `moneda` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `nombre_corto` varchar(10) COLLATE utf8_unicode_ci NOT NULL,
  `simbolo` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cambio` decimal(9,4) DEFAULT NULL,
  `fecha_cambio` date DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `nombreCorto_UNIQUE` (`nombre_corto`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`nivelacceso` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`paises` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `pais` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_monedas` int(11) DEFAULT NULL,
  `country_code` varchar(45) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `pais_UNIQUE` (`pais`),
  KEY `fk_paises_1_monedas_id_idx` (`id_monedas`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`permisos_agenda` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_usuario_editor` int(11) DEFAULT NULL,
  `id_usuario_agenda` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`personascontactocliente` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `telefono1` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `telefono2` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `telefono3` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `movil` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `movil2` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_cliente` int(11) DEFAULT NULL,
  `desc_telefono1` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `desc_telefono2` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `desc_telefono3` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `desc_movil1` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `desc_movil2` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cargo_empresa` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `email` varchar(150) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`personascontactoproveedor` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_proveedor` int(11) DEFAULT NULL,
  `nombre` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `telefono1` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `telefono2` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `telefono3` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `movil1` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `movil2` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `desc_telefono1` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `desc_telefono2` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `desc_telefono3` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `desc_movil1` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `desc_movil2` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cargo_empresa` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `email` varchar(150) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;


CREATE TABLE `@grupo@`.`personascontactotransportista` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `telefono1` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `telefono2` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `telefono3` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `movil` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `movil2` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_cliente` int(11) DEFAULT NULL,
  `desc_telefono1` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `desc_telefono2` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `desc_telefono3` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `desc_movil1` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `desc_movil2` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cargo_empresa` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `email` varchar(150) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

CREATE TABLE `@grupo@`.`poblaciones` (
  `cod_pais` int(11) DEFAULT NULL,
  `poblacion` varchar(60) DEFAULT NULL,
  `cp` varchar(60) DEFAULT NULL,
  `cod_pro` varchar(60) DEFAULT NULL,
  `provincia` varchar(60) DEFAULT NULL,
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_pais` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`proveedor_a_cuenta` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `importe` double DEFAULT NULL,
  `fecha_entrega` date DEFAULT NULL,
  `concepto` varchar(45) DEFAULT NULL,
  `id_proveedor` int(11) DEFAULT NULL,
  `disponible` double DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`proveedores` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `codigo` int(11) DEFAULT '0',
  `proveedor` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cif` varchar(15) COLLATE utf8_unicode_ci DEFAULT NULL,
  `direccion1` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `direccion2` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cp` varchar(12) COLLATE utf8_unicode_ci DEFAULT NULL,
  `poblacion` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `provincia` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `telefono1` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `telefono2` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `telefono3` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `fax` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `movil` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,
  `email` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `web` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `tipo_iva` tinyint not null default '1' ,
  `persona_contacto` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `dia_cobro` int(11) DEFAULT '0',
  `direccion_almacen` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cp_almacen` varchar(12) COLLATE utf8_unicode_ci DEFAULT NULL,
  `poblacion_almacen` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `provincia_almacen` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_pais_almacen` int(11) DEFAULT NULL,
  `telefono_almacen` varchar(15) COLLATE utf8_unicode_ci DEFAULT NULL,
  `fax_almacen` varchar(15) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_forma_pago` int(11) DEFAULT NULL,
  `fecha_ultima_compra` date DEFAULT NULL,
  `importe_acumulado_compras` double DEFAULT '0',
  `entidad_bancaria_proveedor` varchar(4) COLLATE utf8_unicode_ci DEFAULT NULL,
  `oficina_bancaria_proveedor` varchar(4) COLLATE utf8_unicode_ci DEFAULT NULL,
  `dc_proveedor` varchar(2) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cc_proveedor` varchar(10) COLLATE utf8_unicode_ci DEFAULT NULL,
  `entidad_pago_proveedor` varchar(4) COLLATE utf8_unicode_ci DEFAULT NULL,
  `oficina_pago_proveedor` varchar(4) COLLATE utf8_unicode_ci DEFAULT NULL,
  `dc_pago_proveedor` varchar(2) COLLATE utf8_unicode_ci DEFAULT NULL,
  `retencion_irpf` double DEFAULT '0',
  `tipo_retencion` int(11) DEFAULT '0',
  `cuenta_aplicacion` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `comentarios` text COLLATE utf8_unicode_ci,
  `dto` float DEFAULT '0',
  `fecha_alta` date DEFAULT NULL,
  `deuda_maxima` double DEFAULT '0',
  `deuda_actual` double DEFAULT '0',
  `recargo_equivalencia` int(11) DEFAULT '0',
  `texto_para_pedidos` text COLLATE utf8_unicode_ci,
  `entregado_a_cuenta` double DEFAULT '0',
  `id_pais` int(11) DEFAULT NULL,
  `id_monedas` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`secciones` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `seccion` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_web` int(11) DEFAULT NULL,
  `image` blob,
  `type` varchar(4) COLLATE utf8_unicode_ci DEFAULT NULL,
  `codigo` varchar(3) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`subfamilias` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `sub_familia` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `id_familia` int(11) DEFAULT '0',
  `image` blob,
  `type` varchar(4) COLLATE utf8_unicode_ci DEFAULT NULL,
  `codigo` varchar(3) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`subsubfamilias` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_subfamilia` int(11) DEFAULT NULL,
  `subsub_familia` varchar(45) DEFAULT NULL,
  `im` blob,
  `type` varchar(4) DEFAULT NULL,
  `codigo` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`tarifas` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_articulo` int(11) DEFAULT NULL,
  `id_pais` int(11) DEFAULT NULL,
  `id_monedas` int(11) DEFAULT NULL,
  `margen` float DEFAULT NULL,
  `margen_minimo` float DEFAULT NULL,
  `pvp` double DEFAULT NULL,
  `id_codigo_tarifa` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`tipocliente` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_tipo_cliente` int(11) DEFAULT NULL,
  `id_subtipo_cliente` int(11) DEFAULT NULL,
  `tipo_cliente` varchar(45) DEFAULT NULL,
  `id_cliente` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`tiposaviso` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tipoaviso` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`tiposiva` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre_interno` varchar(45) COLLATE utf8_unicode_ci NOT NULL,
  `tipo` varchar(25) COLLATE utf8_unicode_ci DEFAULT NULL,
  `descripcion_tipo_iva` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `iva` decimal(9,2) DEFAULT '0.00',
  `recargo_equivalencia` decimal(9,2) DEFAULT '0.00',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@grupo@`.`transportista` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `codigo` varchar(15) DEFAULT NULL,
  `transportista` varchar(70) DEFAULT NULL,
  `cif` varchar(13) DEFAULT NULL,
  `direccion1` varchar(45) DEFAULT NULL,
  `direccion2` varchar(45) DEFAULT NULL,
  `cp` varchar(10) DEFAULT NULL,
  `poblacion` varchar(45) DEFAULT NULL,
  `provincia` varchar(45) DEFAULT NULL,
  `pais` varchar(45) DEFAULT NULL,
  `telefono1` varchar(20) DEFAULT NULL,
  `telefono2` varchar(20) DEFAULT NULL,
  `fax` varchar(20) DEFAULT NULL,
  `movil` varchar(20) DEFAULT NULL,
  `email` varchar(100) DEFAULT NULL,
  `web` varchar(100) DEFAULT NULL,
  `id_proveedor` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;
CREATE TABLE `@grupo@`.`trazabilidad1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_articulo` int(11) DEFAULT NULL,
  `lote` varchar(45) DEFAULT NULL,
  `cantidad` int(11) DEFAULT NULL,
  `stock_lote` int(11) DEFAULT NULL,
  `stock_real` int(11) DEFAULT NULL,
  `fecha_compra` date DEFAULT NULL,
  `fecha_caducidad` date DEFAULT NULL,
  `id_fac_pro` int(11) DEFAULT NULL,
  `fac_pro` varchar(45) DEFAULT NULL,
  `proveedor` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`trazabilidad2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_trazabilidad1` int(11) DEFAULT NULL,
  `id_documento_venta` int(11) DEFAULT NULL,
  `documento_venta` varchar(45) DEFAULT NULL,
  `id_ticket` int(11) DEFAULT NULL,
  `numero_ticket` int(11) DEFAULT NULL,
  `unidades_vendidas` varchar(45) DEFAULT NULL,
  `fecha_venta` date DEFAULT NULL,
  `codigo_cuenta_cliente` varchar(25) DEFAULT NULL,
  `cliente` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `@grupo@`.`usuarios` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(45) DEFAULT NULL,
  `contrasena` varchar(45) DEFAULT NULL,
  `nivel_acceso` int(4) DEFAULT NULL,
  `categoria` varchar(45) DEFAULT NULL,
  `cuenta_smtp` varchar(100) DEFAULT NULL,
  `usuario_mail` varchar(100) DEFAULT NULL,
  `password_mail` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `nombre_UNIQUE` (`nombre`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `@grupo@`.`proveedores_frecuentes` AS select `@grupo@`.`articulos_prov_frec`.`id_articulo` AS `id_art`,`@grupo@`.`proveedores`.`codigo` AS `cod_pro`,`@grupo@`.`proveedores`.`proveedor` AS `proveedor`,`@grupo@`.`proveedores`.`id` AS `id_prov`,`@grupo@`.`articulos_prov_frec`.`pvd` AS `pvd`,`@grupo@`.`articulos_prov_frec`.`oferta` AS `oferta`,`@grupo@`.`articulos_prov_frec`.`codigo` AS `codigo`,`@grupo@`.`articulos_prov_frec`.`descoferta` AS `descoferta`,`@grupo@`.`articulos_prov_frec`.`pvd_real` AS `pvd_real`,`@grupo@`.`monedas`.`moneda` AS `moneda`,`@grupo@`.`articulos_prov_frec`.`id` AS `id` from ((`@grupo@`.`articulos_prov_frec` join `@grupo@`.`proveedores` on((`@grupo@`.`articulos_prov_frec`.`id_proveedor` = `@grupo@`.`proveedores`.`id`))) left join `@grupo@`.`monedas` on((`@grupo@`.`articulos_prov_frec`.`id_divisa` = `@grupo@`.`monedas`.`id`)));
CREATE OR REPLACE ALGORITHM = UNDEFINED DEFINER = `root`@`localhost` SQL SECURITY DEFINER VIEW `@grupo@`.`viewtarifa` AS select `@grupo@`.`tarifas`.`id` AS `id`,
`@grupo@`.`paises`.`pais` AS `pais`,`@grupo@`.`monedas`.`moneda` AS `moneda`,`@grupo@`.`monedas`.`simbolo` AS `simbolo`,`@grupo@`.`codigotarifa`.`codigo_tarifa` AS `codigo_tarifa`, `@grupo@`.`codigotarifa`.`descripcion` AS `descripcion`,`@grupo@`.`tarifas`.`margen` AS `margen`,`@grupo@`.`tarifas`.`margen_minimo` AS `margen_minimo`,`@grupo@`.`tarifas`.`pvp` AS `pvp`,`@grupo@`.`tarifas`.`pvp` + (`@grupo@`.`tarifas`.`pvp` *(`@grupo@`.`articulos`.`tipo_iva`/100)) AS `pvp_con_iva`,
`@grupo@`.`tarifas`.`id_articulo` AS `id_Articulo` from ((((`@grupo@`.`tarifas` join `@grupo@`.`articulos` ON ((`@grupo@`.`tarifas`.`id_articulo` = `@grupo@`.`articulos`.`id`))) join `@grupo@`.`monedas` ON ((`@grupo@`.`tarifas`.`id_monedas` = `@grupo@`.`monedas`.`id`))) join `@grupo@`.`paises` ON ((`@grupo@`.`tarifas`.`id_pais` = `@grupo@`.`paises`.`id`))) join `@grupo@`.`codigotarifa` ON ((`@grupo@`.`tarifas`.`id_codigo_tarifa` = `@grupo@`.`codigotarifa`.`id`)));
CREATE OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `@grupo@`.`viewtrazabilidad1` AS select `@grupo@`.`trazabilidad1`.`id` AS `id`,`@grupo@`.`trazabilidad1`.`lote` AS `lote`,`@grupo@`.`trazabilidad1`.`cantidad` AS `cantidad`,`@grupo@`.`trazabilidad1`.`stock_lote` AS `stock_lote`,`@grupo@`.`trazabilidad1`.`stock_real` AS `stock_real`,`@grupo@`.`trazabilidad1`.`fac_pro` AS `fac_pro`,`@grupo@`.`trazabilidad1`.`id_articulo` AS `id_articulo`,`@grupo@`.`trazabilidad1`.`fecha_compra` AS `fecha_compra`,`@grupo@`.`trazabilidad1`.`fecha_caducidad` AS `fecha_caducidad`,`@grupo@`.`trazabilidad1`.`proveedor` AS `proveedor`,`@grupo@`.`articulos`.`descripcion` AS `descripcion`,`@grupo@`.`articulos`.`codigo` AS `codigo` from (`@grupo@`.`trazabilidad1` join `@grupo@`.`articulos` on((`@grupo@`.`trazabilidad1`.`id_articulo` = `@grupo@`.`articulos`.`id`)));
CREATE OR REPLACE ALGORITHM = UNDEFINED DEFINER = `root`@`localhost` SQL SECURITY DEFINER VIEW `@grupo@`.`vistaart_tarifa` AS select `@grupo@`.`articulos`.`codigo` AS `codigo`,`@grupo@`.`articulos`.`codigo_barras` AS `codigo_barras`,`@grupo@`.`articulos`.`codigo_fabricante` AS `codigo_fabricante`,
        `@grupo@`.`articulos`.`id` AS `id`, `@grupo@`.`tarifas`.`pvp` + (`@grupo@`.`tarifas`.`pvp` *(`@grupo@`.`articulos`.`tipo_iva`/100)) AS `pvp_con_iva`,
        `@grupo@`.`articulos`.`descripcion` AS `descripcion`, `@grupo@`.`articulos`.`descripcion_reducida` AS `descripcion_reducida`,
        `@grupo@`.`articulos`.`tipo_iva` AS `tipo_iva`,`@grupo@`.`articulos`.`coste` AS `coste`,`@grupo@`.`tarifas`.`pvp` AS `pvp`,
        `@grupo@`.`articulos`.`cantidad_pendiente_recibir` AS `cantidad_pendiente_recibir`,
        `@grupo@`.`articulos`.`fecha_prevista_recepcion` AS `fecha_prevista_recepcion`,
        `@grupo@`.`articulos`.`unidades_reservadas` AS `unidades_reservadas`,
        `@grupo@`.`articulos`.`stock_maximo` AS `stock_maximo`,`@grupo@`.`articulos`.`stock_minimo` AS `stock_minimo`,`@grupo@`.`articulos`.`stock_real` AS `stock_real`,`@grupo@`.`articulos`.`controlar_stock` AS `controlar_stock`,`@grupo@`.`articulos`.`kit` AS `kit`,`@grupo@`.`articulos`.`stock_fisico_almacen` AS `stock_fisico_almacen`,
        `@grupo@`.`tarifas`.`id_codigo_tarifa` AS `tarifa`,`@grupo@`.`articulos`.`lotes` AS `lotes` from ((`@grupo@`.`articulos` left join `@grupo@`.`proveedores` ON ((`@grupo@`.`articulos`.`id_proveedor` = `@grupo@`.`proveedores`.`id`))) join `@grupo@`.`tarifas` ON ((`@grupo@`.`articulos`.`id` = `@grupo@`.`tarifas`.`id_articulo`)));
CREATE OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `@grupo@`.`vistaarticulos` AS select `@grupo@`.`articulos`.`codigo` AS `codigo`,`@grupo@`.`articulos`.`codigo_barras` AS `codigo_barras`,`@grupo@`.`articulos`.`codigo_fabricante` AS `codigo_fabricante`,`@grupo@`.`articulos`.`descripcion` AS `descripcion`,`@grupo@`.`proveedores`.`proveedor` AS `proveedor`,`@grupo@`.`articulos`.`tipo_iva` AS `tipo_iva`,`@grupo@`.`articulos`.`coste` AS `coste`,`@grupo@`.`articulos`.`fecha_ultima_compra` AS `fecha_ultima_compra`,`@grupo@`.`articulos`.`fecha_ultima_venta` AS `fecha_ultima_venta`,`@grupo@`.`articulos`.`unidades_compradas` AS `unidades_compradas`,`@grupo@`.`articulos`.`unidades_vendidas` AS `unidades_vendidas`,`@grupo@`.`articulos`.`stock_maximo` AS `stock_maximo`,`@grupo@`.`articulos`.`stock_minimo` AS `stock_minimo`,`@grupo@`.`articulos`.`stock_real` AS `stock_real`,`@grupo@`.`articulos`.`controlar_stock` AS `controlar_stock`,`@grupo@`.`articulos`.`stock_fisico_almacen` AS `stock_fisico_almacen` from (`@grupo@`.`articulos` left join `@grupo@`.`proveedores` on((`@grupo@`.`articulos`.`id_proveedor` = `@grupo@`.`proveedores`.`id`)));
CREATE OR REPLACE ALGORITHM = UNDEFINED DEFINER = `root`@`localhost` SQL SECURITY DEFINER VIEW `@grupo@`.`inventario` AS select `@grupo@`.`articulos`.`id` AS `id`,
`@grupo@`.`articulos`.`codigo` AS `codigo`,`@grupo@`.`articulos`.`codigo_barras` AS `codigo_barras`,`@grupo@`.`articulos`.`codigo_fabricante` AS `codigo_fabricante`,
`@grupo@`.`articulos`.`descripcion` AS `descripcion`,`@grupo@`.`articulos`.`stock_real` AS `stock_real`,`@grupo@`.`articulos`.`stock_fisico_almacen` AS `stock_fisico_almacen`from `@grupo@`.`articulos`;
CREATE OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `@grupo@`.`vistaempresa` AS select concat_ws(' ',`@grupo@`.`empresas`.`nombre`,`@grupo@`.`empresas`.`ejercicio`) AS `empresa`,`@grupo@`.`empresas`.`id` AS `id` from `@grupo@`.`empresas`;
INSERT INTO `@grupo@`.`codigotarifa` (`id`, `descripcion`, `codigo_tarifa`, `id_pais`, `id_monedas`) VALUES ('1', 'Precio venta público', 'PVP', '1', '1');
