DROP SCHEMA IF EXISTS `@empresa@` ;

CREATE SCHEMA IF NOT EXISTS `@empresa@` DEFAULT CHARACTER SET utf8 COLLATE utf8_unicode_ci ;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`acum_articulos` (
  `id_articulo` INT(11) NOT NULL ,
  `unid_comp_enero` INT(11) NULL DEFAULT NULL ,
  `unid_comp_febrero` INT(11) NULL DEFAULT NULL ,
  `unid_comp_marzo` INT(11) NULL DEFAULT NULL ,
  `unid_comp_abril` INT(11) NULL DEFAULT NULL ,
  `unid_comp_mayo` INT(11) NULL DEFAULT NULL ,
  `unid_comp_junio` INT(11) NULL DEFAULT NULL ,
  `unid_comp_julio` INT(11) NULL DEFAULT NULL ,
  `unid_comp_agosto` INT(11) NULL DEFAULT NULL ,
  `unid_comp_septiembre` INT(11) NULL DEFAULT NULL ,
  `unid_comp_octubre` INT(11) NULL DEFAULT NULL ,
  `unid_comp_noviembre` INT(11) NULL DEFAULT NULL ,
  `unid_comp_diciembre` INT(11) NULL DEFAULT NULL ,
  `unid_comp_ejercicio` INT(11) NULL DEFAULT NULL ,
  `acum_comp_enero` DOUBLE NULL DEFAULT NULL ,
  `acum_comp_febrero` DOUBLE NULL DEFAULT NULL ,
  `acum_comp_marzo` DOUBLE NULL DEFAULT NULL ,
  `acum_comp_abril` DOUBLE NULL DEFAULT NULL ,
  `acum_comp_mayo` DOUBLE NULL DEFAULT NULL ,
  `acum_comp_junio` DOUBLE NULL DEFAULT NULL ,
  `acum_comp_julio` DOUBLE NULL DEFAULT NULL ,
  `acum_comp_agosto` DOUBLE NULL DEFAULT NULL ,
  `acum_comp_septiembre` DOUBLE NULL DEFAULT NULL ,
  `acum_comp_octubre` DOUBLE NULL DEFAULT NULL ,
  `acum_comp_noviembre` DOUBLE NULL DEFAULT NULL ,
  `acum_comp_diciembre` DOUBLE NULL DEFAULT NULL ,
  `acum_comp_ejercicio` DOUBLE NULL DEFAULT NULL ,
  `unid_vent_enero` DOUBLE NULL DEFAULT NULL ,
  `unid_vent_febrero` DOUBLE NULL DEFAULT NULL ,
  `unid_vent_marzo` DOUBLE NULL DEFAULT NULL ,
  `unid_vent_abril` DOUBLE NULL DEFAULT NULL ,
  `unid_vent_mayo` DOUBLE NULL DEFAULT NULL ,
  `unid_vent_junio` DOUBLE NULL DEFAULT NULL ,
  `unid_vent_julio` DOUBLE NULL DEFAULT NULL ,
  `unid_vent_agosto` DOUBLE NULL DEFAULT NULL ,
  `unid_vent_septiembre` DOUBLE NULL DEFAULT NULL ,
  `unid_vent_octubre` DOUBLE NULL DEFAULT NULL ,
  `unid_vent_noviembre` DOUBLE NULL DEFAULT NULL ,
  `unid_vent_diciembre` DOUBLE NULL DEFAULT NULL ,
  `unid_vent_acumulado` DOUBLE NULL DEFAULT NULL ,
  `acum_vent_enero` DOUBLE NULL DEFAULT NULL ,
  `acum_vent_febrero` DOUBLE NULL DEFAULT NULL ,
  `acum_vent_marzo` DOUBLE NULL DEFAULT NULL ,
  `acum_vent_abril` DOUBLE NULL DEFAULT NULL ,
  `acum_vent_mayo` DOUBLE NULL DEFAULT NULL ,
  `acum_vent_junio` DOUBLE NULL DEFAULT NULL ,
  `acum_vent_julio` DOUBLE NULL DEFAULT NULL ,
  `acum_vent_agosto` DOUBLE NULL DEFAULT NULL ,
  `acum_vent_septiembre` DOUBLE NULL DEFAULT NULL ,
  `acum_vent_octubre` DOUBLE NULL DEFAULT NULL ,
  `acum_vent_noviembre` DOUBLE NULL DEFAULT NULL ,
  `acum_vent_diciembre` DOUBLE NULL DEFAULT NULL ,
  `acum_vent_ejercicio` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id_articulo`) ,
  UNIQUE INDEX `id_Articulo_UNIQUE` (`id_articulo` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`acum_clientes` (
  `id_cliente` INT(11) NOT NULL DEFAULT '0' ,
  `acum_enero` DOUBLE NULL DEFAULT NULL ,
  `acum_febrero` DOUBLE NULL DEFAULT NULL ,
  `acum_marzo` DOUBLE NULL DEFAULT NULL ,
  `acum_abril` DOUBLE NULL DEFAULT NULL ,
  `acum_mayo` DOUBLE NULL DEFAULT NULL ,
  `acum_junio` DOUBLE NULL DEFAULT NULL ,
  `acum_julio` DOUBLE NULL DEFAULT NULL ,
  `acum_agosto` DOUBLE NULL DEFAULT NULL ,
  `acum_septiembre` DOUBLE NULL DEFAULT NULL ,
  `acum_octubre` DOUBLE NULL DEFAULT NULL ,
  `acum_noviembre` DOUBLE NULL DEFAULT NULL ,
  `acum_diciembre` DOUBLE NULL DEFAULT NULL ,
  `acum_ejercicio` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id_cliente`) ,
  UNIQUE INDEX `Id_Cliente_UNIQUE` (`id_cliente` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`acum_proveedores` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_proveedor` INT(11) NULL DEFAULT NULL ,
  `acum_enero` DOUBLE NULL DEFAULT NULL ,
  `acum_febrero` DOUBLE NULL DEFAULT NULL ,
  `acum_marzo` DOUBLE NULL DEFAULT NULL ,
  `acum_abril` DOUBLE NULL DEFAULT NULL ,
  `acum_mayo` DOUBLE NULL DEFAULT NULL ,
  `acum_junio` DOUBLE NULL DEFAULT NULL ,
  `acum_julio` DOUBLE NULL DEFAULT NULL ,
  `acum_agosto` DOUBLE NULL DEFAULT NULL ,
  `acum_septiembre` DOUBLE NULL DEFAULT NULL ,
  `acum_octubre` DOUBLE NULL DEFAULT NULL ,
  `acum_noviembre` DOUBLE NULL DEFAULT NULL ,
  `acum_diciembre` DOUBLE NULL DEFAULT NULL ,
  `acum_total` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 3
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`alb_pro` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `albaran` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fecha` DATE NULL DEFAULT NULL ,
  `id_proveedor` INT(11) NULL DEFAULT '0' ,
  `proveedor` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cif_proveedor` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `total1` DOUBLE NULL DEFAULT '0' ,
  `total2` DOUBLE NULL DEFAULT '0' ,
  `total3` DOUBLE NULL DEFAULT '0' ,
  `total4` DOUBLE NULL DEFAULT '0' ,
  `porc_iva1` FLOAT NULL DEFAULT '0' ,
  `porc_iva2` FLOAT NULL DEFAULT '0' ,
  `porc_iva3` FLOAT NULL DEFAULT '0' ,
  `porc_iva4` FLOAT NULL DEFAULT '0' ,
  `iva1` DOUBLE NULL DEFAULT '0' ,
  `iva2` DOUBLE NULL DEFAULT '0' ,
  `iva3` DOUBLE NULL DEFAULT '0' ,
  `iva4` DOUBLE NULL DEFAULT '0' ,
  `base1` DOUBLE NULL DEFAULT '0' ,
  `base2` DOUBLE NULL DEFAULT '0' ,
  `base3` DOUBLE NULL DEFAULT '0' ,
  `base4` DOUBLE NULL DEFAULT '0' ,
  `factura` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `base_total` DOUBLE NULL DEFAULT '0' ,
  `iva_total` DOUBLE NULL DEFAULT '0' ,
  `total` DOUBLE NULL DEFAULT '0' ,
  `comentario` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `pedido` INT(11) NULL DEFAULT '0' ,
  `recargo_equivalencia` TINYINT(1) NULL DEFAULT NULL ,
  `porcentaje_rec1` FLOAT NULL DEFAULT NULL ,
  `porc_rec2` FLOAT NULL DEFAULT NULL ,
  `porc_rec3` FLOAT NULL DEFAULT NULL ,
  `porc_rec4` FLOAT NULL DEFAULT NULL ,
  `importe_rec1` DOUBLE NULL DEFAULT NULL ,
  `importe_rec2` DOUBLE NULL DEFAULT NULL ,
  `importe_rec3` DOUBLE NULL DEFAULT NULL ,
  `importe_rec4` DOUBLE NULL DEFAULT NULL ,
  `importe_rec_total` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `id_UNIQUE` (`id` ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`cab_alb` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `albaran` INT(11) NULL DEFAULT '0' ,
  `fecha` DATE NULL DEFAULT NULL ,
  `pedido_cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_cliente` INT(11) NULL DEFAULT '0' ,
  `codigo_cliente` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion1` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion2` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `poblacion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `provincia` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cp` VARCHAR(8) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_pais` INT(11) NULL DEFAULT NULL ,
  `cif` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `recargo_equivalencia` TINYINT(1) NULL DEFAULT '0' ,
  `subtotal` DOUBLE NULL DEFAULT '0' ,
  `dto` DOUBLE NULL DEFAULT '0' ,
  `porc_dto` FLOAT NULL DEFAULT '0' ,
  `base1` DOUBLE NULL DEFAULT '0' ,
  `base2` DOUBLE NULL DEFAULT '0' ,
  `base3` DOUBLE NULL DEFAULT '0' ,
  `base4` DOUBLE NULL DEFAULT '0' ,
  `porc_iva1` FLOAT NULL DEFAULT '0' ,
  `porc_iva2` FLOAT NULL DEFAULT '0' ,
  `porc_iva3` FLOAT NULL DEFAULT '0' ,
  `porc_iva4` FLOAT NULL DEFAULT '0' ,
  `iva1` DOUBLE NULL DEFAULT '0' ,
  `iva2` DOUBLE NULL DEFAULT '0' ,
  `iva3` DOUBLE NULL DEFAULT '0' ,
  `iva4` DOUBLE NULL DEFAULT '0' ,
  `porc_rec1` FLOAT NULL DEFAULT '0' ,
  `porc_rec2` FLOAT NULL DEFAULT '0' ,
  `porc_rec3` FLOAT NULL DEFAULT '0' ,
  `porc_rec4` FLOAT NULL DEFAULT '0' ,
  `rec1` DOUBLE NULL DEFAULT '0' ,
  `rec2` DOUBLE NULL DEFAULT '0' ,
  `rec3` DOUBLE NULL DEFAULT '0' ,
  `rec4` DOUBLE NULL DEFAULT '0' ,
  `total1` DOUBLE NULL DEFAULT '0' ,
  `total2` DOUBLE NULL DEFAULT '0' ,
  `total3` DOUBLE NULL DEFAULT '0' ,
  `total4` DOUBLE NULL DEFAULT '0' ,
  `base_total` DOUBLE NULL DEFAULT '0' ,
  `iva_total` DOUBLE NULL DEFAULT '0' ,
  `rec_total` DOUBLE NULL DEFAULT '0' ,
  `total_albaran` DOUBLE NULL DEFAULT '0' ,
  `impreso` TINYINT(1) NULL DEFAULT '0' ,
  `facturado` TINYINT(1) NULL DEFAULT '0' ,
  `factura` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fecha_factura` DATE NULL DEFAULT NULL ,
  `comentario` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `entregado_a_cuenta` DOUBLE NULL DEFAULT '0' ,
  `desc_gasto1` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_gasto2` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_gasto3` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `imp_gasto1` DOUBLE NULL DEFAULT NULL ,
  `imp_gasto2` DOUBLE NULL DEFAULT NULL ,
  `imp_gasto3` DOUBLE NULL DEFAULT NULL ,
  `gasto_to_coste` TINYINT(1) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`cab_fac` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `codigo_cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `factura` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fecha` DATE NULL DEFAULT NULL ,
  `fecha_cobro` DATE NULL DEFAULT NULL ,
  `id_cliente` INT(11) NOT NULL DEFAULT '0' ,
  `cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion1` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion2` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cp` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `poblacion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `provincia` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_pais` INT(11) NULL DEFAULT NULL ,
  `cif` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `recargo_equivalencia` TINYINT(1) NULL DEFAULT '0' ,
  `subtotal` DOUBLE NULL DEFAULT '0' ,
  `porc_dto` FLOAT NULL DEFAULT '0' ,
  `porc_dtp_pp` FLOAT NULL DEFAULT '0' ,
  `importe_dto` DOUBLE NULL DEFAULT '0' ,
  `importe_dto_pp` DOUBLE NULL DEFAULT '0' ,
  `base` DOUBLE NULL DEFAULT '0' ,
  `irpf` DOUBLE NULL DEFAULT '0' ,
  `importe_irpf` DOUBLE NULL DEFAULT '0' ,
  `porc_iva` FLOAT NULL DEFAULT '0' ,
  `iva` DOUBLE NULL DEFAULT '0' ,
  `total` DOUBLE NULL DEFAULT '0' ,
  `impreso` TINYINT(1) NULL DEFAULT '0' ,
  `cobrado` TINYINT(1) NULL DEFAULT '0' ,
  `contabilizado` TINYINT(1) NULL DEFAULT '0' ,
  `id_forma_pago` INT(11) NULL DEFAULT '0' ,
  `forma_pago` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `comentario` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `base1` DOUBLE NULL DEFAULT '0' ,
  `base2` DOUBLE NULL DEFAULT '0' ,
  `base3` DOUBLE NULL DEFAULT '0' ,
  `base4` DOUBLE NULL DEFAULT '0' ,
  `porc_iva1` FLOAT NULL DEFAULT '0' ,
  `porc_iva2` FLOAT NULL DEFAULT '0' ,
  `porc_iva3` FLOAT NULL DEFAULT '0' ,
  `porc_iva4` FLOAT NULL DEFAULT '0' ,
  `iva1` DOUBLE NULL DEFAULT '0' ,
  `iva2` DOUBLE NULL DEFAULT '0' ,
  `iva3` DOUBLE NULL DEFAULT '0' ,
  `iva4` DOUBLE NULL DEFAULT '0' ,
  `total1` DOUBLE NULL DEFAULT '0' ,
  `total2` DOUBLE NULL DEFAULT '0' ,
  `total3` DOUBLE NULL DEFAULT '0' ,
  `total4` DOUBLE NULL DEFAULT '0' ,
  `porc_rec1` FLOAT NULL DEFAULT '0' ,
  `porc_rec2` FLOAT NULL DEFAULT '0' ,
  `porc_rec3` FLOAT NULL DEFAULT '0' ,
  `porc_rec4` FLOAT NULL DEFAULT '0' ,
  `rec1` DOUBLE NULL DEFAULT '0' ,
  `rec2` DOUBLE NULL DEFAULT '0' ,
  `rec3` DOUBLE NULL DEFAULT '0' ,
  `rec4` DOUBLE NULL DEFAULT '0' ,
  `total_recargo` DOUBLE NULL DEFAULT '0' ,
  `entregado_a_cuenta` DOUBLE NULL DEFAULT '0' ,
  `importe_pendiente` DOUBLE NULL DEFAULT '0' ,
  `codigo_entidad` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `oficina_entidad` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `dc_cuenta` VARCHAR(2) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cuenta_corriente` VARCHAR(10) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `pedido_pendiente` TINYINT(1) NULL DEFAULT '0' ,
  `desc_gasto1` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_gasto2` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_gasto3` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `imp_gasto1` DOUBLE NULL DEFAULT NULL ,
  `imp_gasto2` DOUBLE NULL DEFAULT NULL ,
  `imp_gasto3` DOUBLE NULL DEFAULT NULL ,
  `gasto_to_coste` TINYINT(1) NULL DEFAULT NULL ,
  `asiento` INT(11) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`, `id_cliente`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`cab_pre` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `presupuesto` INT(11) NULL DEFAULT '0' ,
  `fecha` DATE NULL DEFAULT NULL ,
  `valido_hasta` DATE NULL DEFAULT NULL ,
  `codigo_cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_cliente` INT(11) NULL DEFAULT '-1' ,
  `cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cif` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion1` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion2` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cp` VARCHAR(8) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `poblacion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `provincia` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_pais` INT(11) NULL DEFAULT NULL ,
  `telefono` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `movil` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fax` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `dto` DECIMAL(9,2) NULL DEFAULT '0.00' ,
  `comentarios` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `importe` DOUBLE NULL DEFAULT '0' ,
  `base` DOUBLE NULL DEFAULT '0' ,
  `porc_dto` FLOAT NULL DEFAULT '0' ,
  `total` DOUBLE NULL DEFAULT '0' ,
  `impreso` TINYINT(1) NULL DEFAULT '0' ,
  `aprobado` TINYINT(1) NULL DEFAULT '0' ,
  `fecha_aprobacion` DATE NULL DEFAULT NULL ,
  `importe_factura` DOUBLE NULL DEFAULT '0' ,
  `importe_pendiente` DOUBLE NULL DEFAULT '0' ,
  `factura` VARCHAR(25) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT '0' ,
  `albaran` INT(11) NULL DEFAULT '0' ,
  `pedido` INT(11) NULL DEFAULT '0' ,
  `id_forma_pago` INT(11) NULL DEFAULT '0' ,
  `lugar_entrega` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `atencion_de` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `base1` DOUBLE NULL DEFAULT '0' ,
  `base2` DOUBLE NULL DEFAULT '0' ,
  `base3` DOUBLE NULL DEFAULT '0' ,
  `base4` DOUBLE NULL DEFAULT '0' ,
  `porc_iva1` FLOAT NULL DEFAULT '0' ,
  `porc_iva2` FLOAT NULL DEFAULT '0' ,
  `porc_iva3` FLOAT NULL DEFAULT '0' ,
  `porc_iva4` FLOAT NULL DEFAULT '0' ,
  `iva1` DOUBLE NULL DEFAULT '0' ,
  `iva2` DOUBLE NULL DEFAULT '0' ,
  `iva3` DOUBLE NULL DEFAULT '0' ,
  `iva4` DOUBLE NULL DEFAULT '0' ,
  `porc_rec1` FLOAT NULL DEFAULT '0' ,
  `porc_rec2` FLOAT NULL DEFAULT '0' ,
  `porc_rec3` FLOAT NULL DEFAULT '0' ,
  `porc_rec4` FLOAT NULL DEFAULT '0' ,
  `rec1` DOUBLE NULL DEFAULT '0' ,
  `rec2` DOUBLE NULL DEFAULT '0' ,
  `rec3` DOUBLE NULL DEFAULT '0' ,
  `rec4` DOUBLE NULL DEFAULT '0' ,
  `total1` DOUBLE NULL DEFAULT '0' ,
  `total2` DOUBLE NULL DEFAULT '0' ,
  `total3` DOUBLE NULL DEFAULT '0' ,
  `total4` DOUBLE NULL DEFAULT '0' ,
  `recargo_equivalencia` TINYINT(1) NULL DEFAULT '0' ,
  `email` VARCHAR(70) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `total_iva` DOUBLE NULL DEFAULT '0' ,
  `total_recargo` DOUBLE NULL DEFAULT '0' ,
  `importe1` DOUBLE NULL DEFAULT NULL ,
  `importe2` DOUBLE NULL DEFAULT NULL ,
  `importe3` DOUBLE NULL DEFAULT NULL ,
  `importe4` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`cab_tpv` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `caja` INT(11) NULL DEFAULT '0' ,
  `ticket` INT(11) NULL DEFAULT '0' ,
  `fecha` DATE NULL DEFAULT NULL ,
  `hora` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_usuario` INT(11) NULL DEFAULT NULL ,
  `id_cliente` INT(11) NULL DEFAULT '0' ,
  `codigo_` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `nombre_cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `ticket_en_espera` TINYINT(1) NULL DEFAULT '0' ,
  `pendiente_cobro` TINYINT(1) NULL DEFAULT '0' ,
  `id_forma_pago` INT(11) NULL DEFAULT NULL ,
  `tipo_targeta` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `impreso` TINYINT(1) NULL DEFAULT '0' ,
  `importe` DOUBLE NULL DEFAULT '0' ,
  `base1` DOUBLE NULL DEFAULT '0' ,
  `base2` DOUBLE NULL DEFAULT '0' ,
  `base3` DOUBLE NULL DEFAULT '0' ,
  `base4` DOUBLE NULL DEFAULT '0' ,
  `porc_iva1` FLOAT NULL DEFAULT '0' ,
  `porc_iva2` FLOAT NULL DEFAULT '0' ,
  `porc_iva3` FLOAT NULL DEFAULT '0' ,
  `porc_iva4` FLOAT NULL DEFAULT '0' ,
  `iva1` DOUBLE NULL DEFAULT '0' ,
  `iva2` DOUBLE NULL DEFAULT '0' ,
  `iva3` DOUBLE NULL DEFAULT '0' ,
  `iva4` DOUBLE NULL DEFAULT '0' ,
  `total1` DOUBLE NULL DEFAULT '0' ,
  `total2` DOUBLE NULL DEFAULT '0' ,
  `total3` DOUBLE NULL DEFAULT '0' ,
  `total4` DOUBLE NULL DEFAULT '0' ,
  `subtotal` DOUBLE NULL DEFAULT '0' ,
  `dto` DOUBLE NULL DEFAULT '0' ,
  `porc_rec1` FLOAT NULL DEFAULT '0' ,
  `porc_rec2` FLOAT NULL DEFAULT '0' ,
  `porc_rec3` FLOAT NULL DEFAULT '0' ,
  `porc_rec4` FLOAT NULL DEFAULT '0' ,
  `rec1` DOUBLE NULL DEFAULT '0' ,
  `rec2` DOUBLE NULL DEFAULT '0' ,
  `rec3` DOUBLE NULL DEFAULT '0' ,
  `rec4` DOUBLE NULL DEFAULT '0' ,
  `total_recargo` DOUBLE NULL DEFAULT '0' ,
  `total_iva` DOUBLE NULL DEFAULT '0' COMMENT '	' ,
  `cobrado` DOUBLE NULL DEFAULT '0' ,
  `importe_pendiente_cobro` DOUBLE NULL DEFAULT '0' ,
  `importe_efectivo` DOUBLE NULL DEFAULT '0' ,
  `importe_tarjeta` DOUBLE NULL DEFAULT '0' ,
  `importe_cheque` DOUBLE NULL DEFAULT '0' ,
  `importe_credito` DOUBLE NULL DEFAULT '0' ,
  `importe_vale` DOUBLE NULL DEFAULT NULL ,
  `importe_dto_redondeo` DOUBLE NULL DEFAULT '0' ,
  `importe_cambio` DOUBLE NULL DEFAULT '0' ,
  `entregado_a_cuenta` DOUBLE NULL DEFAULT '0' ,
  `pasado_a_albaran_factura` INT(11) NULL DEFAULT '0' ,
  `numero_albaran` INT(11) NULL DEFAULT '0' ,
  `numero_factura` INT(11) NULL DEFAULT '0' ,
  `id_cierre` INT(11) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_cab_tpv_cierrecaja_id_idx` (`id_cierre` ASC) ,
  INDEX `fk_cab_tpv__clientes_id_idx` (`id_cliente` ASC) ,
  INDEX `fk_cab_tpv_formapago_id_idx` (`id_forma_pago` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`cajas` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `desc_caja` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`cierrecaja` (
  `id` INT(11) NOT NULL ,
  `fecha_hora_abertura` DATETIME NULL DEFAULT NULL ,
  `fecha_hora_cierre` DATETIME NULL DEFAULT NULL ,
  `vales_emitidos` DOUBLE NULL DEFAULT NULL ,
  `entregas_a_cuenta` DOUBLE NULL DEFAULT NULL ,
  `intresos_efectivo` DOUBLE NULL DEFAULT NULL ,
  `ingresos_tarjetas` DOUBLE NULL DEFAULT NULL ,
  `ingresos_cheques` DOUBLE NULL DEFAULT NULL ,
  `ingresos_credito` DOUBLE NULL DEFAULT NULL ,
  `ingresos_extraordinarios` DOUBLE NULL DEFAULT NULL ,
  `importe_abertura_dia` DOUBLE NULL DEFAULT NULL ,
  `importe_cierre` DOUBLE NULL DEFAULT NULL ,
  `id_usuario` INT(11) NULL DEFAULT NULL ,
  `importe_real_caja` DOUBLE NULL DEFAULT NULL ,
  `descuadre` DOUBLE NULL DEFAULT NULL ,
  `descuentos` DOUBLE NULL DEFAULT NULL ,
  `beneficio_bruto` DOUBLE NULL DEFAULT NULL ,
  `id_caja` INT(11) NULL DEFAULT NULL ,
  `ingresos_vales` DOUBLE NULL DEFAULT NULL ,
  `gastos_caja` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`clientes_deuda` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_cliente` INT(11) NULL DEFAULT '0' ,
  `fecha` DATE NULL DEFAULT NULL ,
  `vencimiento` DATE NULL DEFAULT NULL ,
  `documento` INT(11) NULL DEFAULT '0' ,
  `id_ticket` INT(11) NULL DEFAULT '0' ,
  `id_factura` INT(11) NULL DEFAULT '0' ,
  `tipo` INT(11) NULL DEFAULT '0' ,
  `importe` DOUBLE NULL DEFAULT '0' ,
  `pagado` DOUBLE NULL DEFAULT '0' ,
  `pendiente_cobro` DOUBLE NULL DEFAULT '0' ,
  `entidad` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `oficina_entidad` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `dc_cuenta` VARCHAR(2) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cuenta_corriente` VARCHAR(10) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `importe_efectivo` DOUBLE NULL DEFAULT NULL ,
  `importe_tarjeta` DOUBLE NULL DEFAULT NULL ,
  `importe_cheque` DOUBLE NULL DEFAULT NULL ,
  `importe_transferencia` DOUBLE NULL DEFAULT NULL ,
  `importe_internet` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`desglose_caja` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_cierre` INT(11) NULL DEFAULT NULL ,
  `nombre_moneda` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cantidad` INT(11) NULL DEFAULT NULL ,
  `importe` DOUBLE NULL DEFAULT NULL ,
  `id_moneda` INT(11) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`deudas_proveedores` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '	' ,
  `id_documento` INT(11) NULL DEFAULT NULL ,
  `documento` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fecha_deuda` DATE NULL DEFAULT NULL ,
  `vencimiento` DATE NULL DEFAULT NULL ,
  `importe_deuda` DOUBLE NULL DEFAULT NULL ,
  `pago_por` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `numero_tarjeta_cuenta` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_proveedor` INT(11) NULL DEFAULT NULL ,
  `pendiente` DOUBLE NULL DEFAULT NULL ,
  `id_asiento` INT(11) NULL DEFAULT NULL ,
  `asiento_numero` INT(11) NULL DEFAULT NULL ,
  `pagado` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`devoluciones` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `ticket` INT(11) NULL DEFAULT '0' ,
  `fecha_devolucion` DATE NULL DEFAULT NULL ,
  `atendido_por` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_cliente` INT(11) NULL DEFAULT '0' ,
  `cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `comentarios` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `codigo_articulo` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `descripcion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_articulo` INT(11) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`fac_pro` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `factura` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fecha` DATE NULL DEFAULT NULL ,
  `fecha_recepcion` DATE NULL DEFAULT NULL ,
  `pedido` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_proveedor` INT(11) NULL DEFAULT '0' ,
  `proveedor` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cif_proveedor` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `retencion_irpf` FLOAT NULL DEFAULT '0' ,
  `recargo_equivalencia` FLOAT NULL DEFAULT '0' ,
  `base1` DOUBLE NULL DEFAULT '0' ,
  `base2` DOUBLE NULL DEFAULT '0' ,
  `base3` DOUBLE NULL DEFAULT '0' COMMENT '	' ,
  `base4` DOUBLE NULL DEFAULT '0' ,
  `porc_iva1` FLOAT NULL DEFAULT '0' ,
  `porc_iva2` FLOAT NULL DEFAULT '0' ,
  `porc_iva3` FLOAT NULL DEFAULT '0' ,
  `porc_iva4` FLOAT NULL DEFAULT '0' ,
  `iva1` DOUBLE NULL DEFAULT '0' ,
  `iva2` DOUBLE NULL DEFAULT '0' ,
  `iva3` DOUBLE NULL DEFAULT '0' ,
  `iva4` DOUBLE NULL DEFAULT '0' ,
  `porc_rec1` FLOAT NULL DEFAULT '0' ,
  `porc_rec2` FLOAT NULL DEFAULT '0' ,
  `porc_rec3` FLOAT NULL DEFAULT '0' ,
  `porc_rec4` FLOAT NULL DEFAULT '0' ,
  `rec1` DOUBLE NULL DEFAULT '0' ,
  `rec2` DOUBLE NULL DEFAULT '0' ,
  `rec3` DOUBLE NULL DEFAULT '0' ,
  `rec4` DOUBLE NULL DEFAULT '0' ,
  `total1` DOUBLE NULL DEFAULT '0' ,
  `total2` DOUBLE NULL DEFAULT '0' ,
  `total3` DOUBLE NULL DEFAULT '0' ,
  `total4` DOUBLE NULL DEFAULT '0' ,
  `total_base` DOUBLE NULL DEFAULT '0' ,
  `total_iva` DOUBLE NULL DEFAULT '0' ,
  `total_retencion` DOUBLE NULL DEFAULT '0' ,
  `total_recargo` DOUBLE NULL DEFAULT '0' ,
  `total` DOUBLE NULL DEFAULT '0' ,
  `forma_pago` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `importe_pagadp_total` DOUBLE NULL DEFAULT '0' ,
  `id_tipo_gasto` INT(11) NULL DEFAULT '0' ,
  `comentario` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `pagado` TINYINT(1) NULL DEFAULT '0' ,
  `importe_deuda_pendiente` DOUBLE NULL DEFAULT '0' ,
  `albaran` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_gasto1` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_gasto2` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_gasto3` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `imp_gasto1` DOUBLE NULL DEFAULT NULL ,
  `imp_gasto2` DOUBLE NULL DEFAULT NULL ,
  `imp_gasto3` DOUBLE NULL DEFAULT NULL ,
  `gasto_to_coste` TINYINT(1) NULL DEFAULT NULL ,
  `total_dto` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `id_tipo_gasto` (`id_tipo_gasto` ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`histovales` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_vale` INT(11) NULL DEFAULT '0' ,
  `fecha` DATE NULL DEFAULT NULL ,
  `vencimiento_vale` DATE NULL DEFAULT NULL ,
  `id_usuario` INT(11) NULL DEFAULT NULL ,
  `importe` DECIMAL(9,2) NULL DEFAULT '0.00' ,
  `id_ticket` INT(11) NULL DEFAULT '0' ,
  `id_cliente` INT(11) NULL DEFAULT '0' ,
  `cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`lin_alb` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_cab` INT(11) NULL DEFAULT NULL ,
  `id_articulo` INT(11) NULL DEFAULT NULL ,
  `codigo` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cantidad` FLOAT NULL DEFAULT '0' ,
  `descripcion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `pvp` DOUBLE NULL DEFAULT '0' ,
  `subtotal` DOUBLE NULL DEFAULT '0' ,
  `dto` DOUBLE NULL DEFAULT '0' ,
  `porc_dto` FLOAT NULL DEFAULT '0' ,
  `porc_iva` FLOAT NULL DEFAULT '0' ,
  `total` DOUBLE NULL DEFAULT '0' ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`lin_alb_pro` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_cab` INT(11) NULL DEFAULT NULL ,
  `id_articulo` INT(11) NULL DEFAULT NULL ,
  `codigo` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `descripcion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `coste_` DOUBLE NULL DEFAULT '0' ,
  `porc_dto` FLOAT NULL DEFAULT '0' ,
  `porc_iva` FLOAT NULL DEFAULT '0' ,
  `cantidad` INT(11) NULL DEFAULT '0' ,
  `total` DOUBLE NULL DEFAULT '0' ,
  `dto` DOUBLE NULL DEFAULT '0' ,
  `sub_total` DOUBLE NULL DEFAULT '0' ,
  `iva` DOUBLE NULL DEFAULT '0' ,
  `codigo_articulo_proveedor` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `coste_real` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`lin_fac` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_cab` INT(11) NULL DEFAULT NULL ,
  `id_articulo` INT(11) NULL DEFAULT NULL ,
  `codigo` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cantidad` FLOAT NULL DEFAULT '0' ,
  `descripcion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `pvp` DOUBLE NULL DEFAULT '0' ,
  `subtotal` DOUBLE NULL DEFAULT '0' ,
  `dto` DOUBLE NULL DEFAULT '0' ,
  `porc_dto` FLOAT NULL DEFAULT '0' ,
  `porc_iva` FLOAT NULL DEFAULT '0' ,
  `total` DOUBLE NULL DEFAULT '0' ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`lin_fac_pro` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_cab` INT(11) NULL DEFAULT NULL ,
  `id_articulo` INT(11) NULL DEFAULT NULL ,
  `codigo` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `codigo_articulo_proveedor` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cantidad` FLOAT NULL DEFAULT '0' ,
  `descripcion` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `coste_` DOUBLE NULL DEFAULT '0' ,
  `sub_total` DOUBLE NULL DEFAULT NULL ,
  `dto` DOUBLE NULL DEFAULT '0' ,
  `porc_dto` FLOAT NULL DEFAULT '0' ,
  `porc_iva` FLOAT NULL DEFAULT '0' ,
  `total` DOUBLE NULL DEFAULT '0' ,
  `iva` DOUBLE NULL DEFAULT '0' ,
  `pedido` INT(11) NULL DEFAULT '0' ,
  `porc_rec` FLOAT NULL DEFAULT '0' ,
  PRIMARY KEY (`id`) ,
  INDEX `cCodigo` (`codigo` ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`lin_ped` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_cab` INT(11) NULL DEFAULT NULL ,
  `id_articulo` INT(11) NULL DEFAULT NULL ,
  `codigo` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cantidad` FLOAT NULL DEFAULT '0' ,
  `descripcion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `pvp` DOUBLE NULL DEFAULT '0' ,
  `subtotal` DOUBLE NULL DEFAULT '0' ,
  `dto` DOUBLE NULL DEFAULT '0' ,
  `porc_dto` DOUBLE NULL DEFAULT '0' ,
  `porc_iva` DOUBLE NULL DEFAULT '0' ,
  `total` DOUBLE NULL DEFAULT '0' ,
  `cantidad_a_servir` FLOAT NULL DEFAULT '0' ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`lin_ped_pro` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_cab` INT(11) NULL DEFAULT NULL ,
  `id_articulo` INT(11) NULL DEFAULT '0' ,
  `codigo_articulo_proveedor` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `descripcion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cantidad` INT(11) NULL DEFAULT '0' ,
  `coste_bruto` DOUBLE NULL DEFAULT '0' ,
  `subtotal_coste` DOUBLE NULL DEFAULT '0' ,
  `porc_dto` FLOAT NULL DEFAULT '0' ,
  `dto` DOUBLE NULL DEFAULT '0' ,
  `porc_iva` FLOAT NULL DEFAULT '0' ,
  `iva` DOUBLE NULL DEFAULT '0' ,
  `total` DOUBLE NULL DEFAULT '0' ,
  `cantidad_recibida` INT(11) NULL DEFAULT '0' ,
  `etiquetas` INT(11) NULL DEFAULT '0' ,
  `cantidad_pendiente` INT(11) NULL DEFAULT '0' ,
  `cerrado` TINYINT(1) NULL DEFAULT '0' ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 20
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`lin_pre` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_cab` INT(11) NULL DEFAULT NULL ,
  `id_articulo` INT(11) NULL DEFAULT NULL ,
  `codigo` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cantidad` FLOAT NULL DEFAULT '0' ,
  `descripcion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `pvp` DOUBLE NULL DEFAULT '0' ,
  `sub_total` DOUBLE NULL DEFAULT '0' ,
  `porc_dto` FLOAT NULL DEFAULT '0' ,
  `dto` DOUBLE NULL DEFAULT '0' ,
  `porc_iva` FLOAT NULL DEFAULT '0' ,
  `total` DOUBLE NULL DEFAULT '0' ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`lin_res` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_cab` INT NULL ,
  `id_articulo` INT(11) NULL DEFAULT NULL ,
  `codigo` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `descripcion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cantidad` INT(11) NULL DEFAULT '0' ,
  `importe` DOUBLE NULL DEFAULT '0' ,
  `total` DOUBLE NULL DEFAULT '0' ,
  `fecha_reserva` DATE NULL DEFAULT NULL ,
  `reservado_hasta` DATE NULL DEFAULT NULL ,
  `importe_dto` DOUBLE NULL DEFAULT '0' ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`lin_tpv` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_cab` INT NULL ,
  `id_articulo` INT(11) NULL DEFAULT '0' ,
  `codigo` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `descripcion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `pvp` DOUBLE NULL DEFAULT '0' ,
  `cantidad` FLOAT NULL DEFAULT '0' ,
  `importe` DOUBLE NULL DEFAULT '0' ,
  `porc_iva` FLOAT NULL DEFAULT '0' ,
  `porc_dto` FLOAT NULL DEFAULT '0' ,
  `total` DOUBLE NULL DEFAULT '0' ,
  `subtotal` DOUBLE NULL DEFAULT '0' ,
  `fecha_linea` DATE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `id_Articulo` (`id_articulo` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`moneda_caja` (
  `id` INT(11) NOT NULL ,
  `nombre_moneda` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `valor_moneda` DOUBLE NULL DEFAULT NULL ,
  `id_moneda_base` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`ped_cli` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `albaran` INT(11) NULL DEFAULT '0' ,
  `pedido` INT(11) NULL DEFAULT '0' ,
  `fecha` DATE NULL DEFAULT NULL ,
  `pedido_cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_cliente` INT(11) NULL DEFAULT '0' ,
  `codigo_cliente` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion1` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion2` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `poblacion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `provincia` VARCHAR(25) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cp` VARCHAR(8) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_pais` INT(11) NULL DEFAULT NULL ,
  `cif` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `recargo_equivalencia` TINYINT(1) NULL DEFAULT '0' ,
  `subtotal` DOUBLE NULL DEFAULT '0' ,
  `porc_dto` FLOAT NULL DEFAULT '0' ,
  `dto` DOUBLE NULL DEFAULT '0' ,
  `base1` DOUBLE NULL DEFAULT '0' ,
  `base2` DOUBLE NULL DEFAULT '0' ,
  `base3` DOUBLE NULL DEFAULT '0' ,
  `base4` DOUBLE NULL DEFAULT '0' ,
  `porc_iva1` FLOAT NULL DEFAULT '0' ,
  `porc_iva2` FLOAT NULL DEFAULT '0' ,
  `porc_iva3` FLOAT NULL DEFAULT '0' ,
  `porc_iva4` FLOAT NULL DEFAULT '0' ,
  `iva1` DOUBLE NULL DEFAULT '0' ,
  `iva2` DOUBLE NULL DEFAULT '0' ,
  `iva3` DOUBLE NULL DEFAULT '0' ,
  `iva4` DOUBLE NULL DEFAULT '0' ,
  `porc_rec1` FLOAT NULL DEFAULT '0' ,
  `porc_rec2` FLOAT NULL DEFAULT '0' ,
  `porc_rec3` FLOAT NULL DEFAULT '0' ,
  `porc_rec4` FLOAT NULL DEFAULT '0' ,
  `importe_rec1` DOUBLE NULL DEFAULT '0' ,
  `importe_rec2` DOUBLE NULL DEFAULT '0' ,
  `importe_rec3` DOUBLE NULL DEFAULT '0' ,
  `importe_rec4` DOUBLE NULL DEFAULT '0' ,
  `total1` DOUBLE NULL DEFAULT '0' ,
  `total2` DOUBLE NULL DEFAULT '0' ,
  `total3` DOUBLE NULL DEFAULT '0' ,
  `total4` DOUBLE NULL DEFAULT '0' ,
  `base_total` DOUBLE NULL DEFAULT '0' ,
  `iva_total` DOUBLE NULL DEFAULT '0' ,
  `total_recargo` DOUBLE NULL DEFAULT '0' ,
  `total_albaran` DOUBLE NULL DEFAULT '0' ,
  `impreso` TINYINT(1) NULL DEFAULT '0' ,
  `facturado` TINYINT(1) NULL DEFAULT '0' ,
  `factura` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT '0' ,
  `fecha_factura` DATE NULL DEFAULT NULL ,
  `comentario` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `Entregas a cuenta` DOUBLE NULL DEFAULT '0' ,
  `traspasado_albaran` TINYINT(1) NULL DEFAULT '0' ,
  `traspasado_factura` TINYINT(1) NULL DEFAULT '0' ,
  `direccion_entrega1` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion_entrega2` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cp_entrega` VARCHAR(5) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `poblacion_entrega` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `provincia_entrega` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `pais_entrega` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `enviado` TINYINT(1) NULL DEFAULT '0' ,
  `completo` TINYINT(1) NULL DEFAULT '0' ,
  `entregado` TINYINT(1) NULL DEFAULT '0' ,
  `fecha_limite_entrega` DATE NULL DEFAULT NULL ,
  `total_pedido` DOUBLE NULL DEFAULT NULL ,
  `desc_gasto1` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_gasto2` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_gasto3` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `imp_gasto1` DOUBLE NULL DEFAULT NULL ,
  `imp_gasto2` DOUBLE NULL DEFAULT NULL ,
  `imp_gasto3` DOUBLE NULL DEFAULT NULL ,
  `gasto_to_coste` TINYINT(1) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`ped_pro` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `pedido` INT(11) NULL DEFAULT '0' ,
  `ejercicio` INT(11) NULL DEFAULT NULL ,
  `fecha` DATE NULL DEFAULT NULL ,
  `recepcion` DATE NULL DEFAULT NULL ,
  `id_proveedor` INT(11) NULL DEFAULT '0' ,
  `codigo_proveedor` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `proveedor` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion1` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion2` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cp` VARCHAR(8) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `poblacion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `provincia` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_pais` INT(11) NULL DEFAULT NULL ,
  `cif_nif` VARCHAR(25) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `base_total` DOUBLE NULL DEFAULT '0' ,
  `sub_total` DOUBLE NULL DEFAULT '0' ,
  `porc_dto` FLOAT NULL DEFAULT '0' ,
  `rec_total` DOUBLE NULL DEFAULT '0' ,
  `total` DOUBLE NULL DEFAULT '0' ,
  `enviado` TINYINT(1) NULL DEFAULT '0' ,
  `recibido` TINYINT(1) NULL DEFAULT '0' ,
  `recibido_completo` TINYINT(1) NULL DEFAULT '0' ,
  `genero_pendiente` TINYINT(1) NULL DEFAULT '0' ,
  `traspasado` TINYINT(1) NULL DEFAULT '0' ,
  `pedido_cliente` INT(11) NULL DEFAULT '0' ,
  `id_forma_pago` INT(11) NULL DEFAULT '0' ,
  `comentario` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fecha_entrega` DATE NULL DEFAULT NULL ,
  `direccion1_entrega` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion2_entrega` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cp_entrega` VARCHAR(8) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `poblacion_entrega` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `provincia_entrega` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_pais_entrega` INT(11) NULL DEFAULT NULL ,
  `nombre_cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `horario_activo` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `base1` DOUBLE NULL DEFAULT NULL ,
  `base2` DOUBLE NULL DEFAULT NULL ,
  `base3` DOUBLE NULL DEFAULT NULL ,
  `base4` DOUBLE NULL DEFAULT NULL ,
  `porc_iva1` INT(11) NULL DEFAULT NULL ,
  `porc_iva2` INT(11) NULL DEFAULT NULL ,
  `porc_iva3` INT(11) NULL DEFAULT NULL ,
  `porc_iva4` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `iva1` DOUBLE NULL DEFAULT NULL ,
  `iva2` DOUBLE NULL DEFAULT NULL ,
  `iva3` DOUBLE NULL DEFAULT NULL ,
  `iva4` DOUBLE NULL DEFAULT NULL ,
  `porc_rec1` FLOAT NULL DEFAULT NULL ,
  `porc_rec2` FLOAT NULL DEFAULT NULL ,
  `porc_rec3` FLOAT NULL DEFAULT NULL ,
  `porc_rec4` FLOAT NULL DEFAULT NULL ,
  `rec1` DOUBLE NULL DEFAULT NULL ,
  `rec2` DOUBLE NULL DEFAULT NULL ,
  `rec3` DOUBLE NULL DEFAULT NULL ,
  `rec4` DOUBLE NULL DEFAULT NULL ,
  `total1` DOUBLE NULL DEFAULT NULL ,
  `total2` DOUBLE NULL DEFAULT NULL ,
  `rtotal3` DOUBLE NULL DEFAULT NULL ,
  `rtotal4` DOUBLE NULL DEFAULT NULL ,
  `impreso` TINYINT(1) NULL DEFAULT NULL ,
  `desc_gasto1` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_gasto2` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_gasto3` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `imp_gasto1` DOUBLE NULL DEFAULT NULL ,
  `imp_gasto2` DOUBLE NULL DEFAULT NULL ,
  `imp_gasto3` DOUBLE NULL DEFAULT NULL ,
  `gasto_to_coste` TINYINT(1) NULL DEFAULT NULL ,
  `recargo_equivalencia` TINYINT(1) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`recibos` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `fecha` DATE NULL DEFAULT NULL ,
  `fecha_cobro` DATE NULL DEFAULT NULL ,
  `descripcion_fecha1` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `descripcion_fecha2` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `localidad` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `recibo` INT(11) NULL DEFAULT '0' ,
  `id_cliente` INT(11) NULL DEFAULT '0' ,
  `cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion1` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion2` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cp` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `poblacion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `provincia` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_pais` INT(11) NULL DEFAULT NULL ,
  `concepto` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `importe` DOUBLE NULL DEFAULT '0' ,
  `importe_en_texto` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `descripcion_entidad` VARCHAR(255) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `descripcion_oficina` VARCHAR(254) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `entidad` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `oficina_entidad` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `dc_cuenta` VARCHAR(2) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cuenta_corriente` VARCHAR(10) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cobrado` TINYINT(1) NULL DEFAULT '0' ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`reservas` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `reserva` INT(11) NULL DEFAULT '0' ,
  `fecha` DATE NULL DEFAULT NULL ,
  `id_cliente` INT(11) NULL DEFAULT '0' ,
  `cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion1` VARCHAR(50) CHARACTER SET 'ucs2' NULL DEFAULT NULL ,
  `direccion2` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cp` VARCHAR(8) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `poblacion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `provincia` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_pais` INT(11) NULL DEFAULT NULL ,
  `telefono1` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `telefono2` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fecha_limite` DATE NULL DEFAULT NULL ,
  `importe` DOUBLE NULL DEFAULT '0' ,
  `pendiente` DOUBLE NULL DEFAULT '0' ,
  `entregado` DOUBLE NULL DEFAULT '0' ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`seguimiento` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `usuario` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fecha` DATETIME NULL DEFAULT NULL ,
  `accion` VARCHAR(200) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `comentarios` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@empresa@`.`vales` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `numero_` INT(11) NULL DEFAULT '0' ,
  `codigo` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fecha` DATE NULL DEFAULT NULL ,
  `fecha_vencimiento_vale` DATE NULL DEFAULT NULL ,
  `id_usuario` INT(11) NULL DEFAULT NULL ,
  `importe` DOUBLE NULL DEFAULT '0' ,
  `id_ticket` INT(11) NULL DEFAULT '0' ,
  `id_cliente` INT(11) NULL DEFAULT '0' ,
  `cliente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `agotado` TINYINT(1) NULL DEFAULT '0' ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;