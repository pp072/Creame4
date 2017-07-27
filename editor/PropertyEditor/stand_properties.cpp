#include "stand_properties.h"
#define stop __asm nop
stand_properties::stand_properties(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.TX, SIGNAL(valueChanged ( double )),
		this, SIGNAL(signal_values_changed()));
	connect(ui.TY, SIGNAL(valueChanged ( double )),
		this, SIGNAL(signal_values_changed()));
	connect(ui.TZ, SIGNAL(valueChanged ( double )),
		this, SIGNAL(signal_values_changed()));

	connect(ui.RX, SIGNAL(valueChanged ( double )),
		this, SIGNAL(signal_values_changed()));
	connect(ui.RY, SIGNAL(valueChanged ( double )),
		this, SIGNAL(signal_values_changed()));
	connect(ui.RZ, SIGNAL(valueChanged ( double )),
		this, SIGNAL(signal_values_changed()));

	connect(ui.SX, SIGNAL(valueChanged ( double )),
		this, SIGNAL(signal_values_changed()));
	connect(ui.SY, SIGNAL(valueChanged ( double )),
		this, SIGNAL(signal_values_changed()));
	connect(ui.SZ, SIGNAL(valueChanged ( double )),
		this, SIGNAL(signal_values_changed()));
}

stand_properties::~stand_properties()
{

}

void stand_properties::set_name( QString name )
{
	ui.name_label->setText(name);
}

void stand_properties::set_transformations( float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz )
{
	ui.TX->setValue(tx);ui.TY->setValue(ty);ui.TZ->setValue(tz);

	ui.RX->setValue(rx);ui.RY->setValue(ry);ui.RZ->setValue(rz);

	ui.SX->setValue(sx);ui.SY->setValue(sy);ui.SZ->setValue(sz);
}

void stand_properties::get_transformations( float *tx, float *ty, float *tz, float *rx, float *ry, float *rz, float *sx, float *sy, float *sz )
{
	if( tx != 0x0 ) *tx = ui.TX->value(); if( ty != 0x0 ) *ty = ui.TY->value(); if( tz != 0x0 ) *tz = ui.TZ->value();
	if( rx != 0x0 ) *rx = ui.RX->value(); if( ry != 0x0 ) *ry = ui.RY->value(); if( rz != 0x0 ) *rz = ui.RZ->value();
	if( sx != 0x0 ) *sx = ui.SX->value(); if( sy != 0x0 ) *sy = ui.SY->value(); if( sz != 0x0 ) *sz = ui.SZ->value();
}

