#ifndef SCENEWIZARD_H_
#define SCENEWIZARD_H_

#include <QtGui/QWizard>
#include <QtXml/qdom.h>

class QLineEdit;
class SceneFile;
/**
 * Different Wizards for creating a new SceneFile or new scene elements
 */
namespace SceneWizard
{
	
	SceneFile* createScene(QWidget* parent = 0);
	QDomElement createReference(QWidget* parent);
 	QDomElement createLight(QWidget* parent);
//  	QDomElement createCamera(QWidget* parent);
//  	QDomElement createGroup(QWidget* parent);
//  	QDomElement createOverlay(QWidget* parent);
//  	QDomElement createEmitter(QWidget* parent);
}

#endif

