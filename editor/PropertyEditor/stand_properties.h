#ifndef STAND_PROPERTIES_H
#define STAND_PROPERTIES_H

#include <QWidget>
#include "GeneratedFiles/ui_stand_properties.h"

class stand_properties : public QWidget
{
	Q_OBJECT

public:
	stand_properties(QWidget *parent = 0);
	~stand_properties();
	void set_name(QString name);
	void set_transformations( float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz );
	void get_transformations(float *tx, float *ty, float *tz,
		float *rx, float *ry, float *rz, float *sx, float *sy, float *sz);
	
private:
	Ui::stand_properties ui;
signals:
	void signal_values_changed();
};

#endif // STAND_PROPERTIES_H
