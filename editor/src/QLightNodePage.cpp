// ****************************************************************************************
//
// Horde3D Scene Editor 
// --------------------------------------
// Copyright (C) 2007 Volker Wiendl
// 
// This file is part of the Horde3D Scene Editor.
//
// The Horde3D Scene Editor is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 3 of the License 
//
// The Horde3D Scene Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ****************************************************************************************

#include "QLightNodePage.h"

#include <QDir>
#include <Horde3D.h>

QLightNodePage::QLightNodePage(QWidget* parent /*= 0*/) : QWizardPage(parent)
{
	setupUi(this);
	setTitle(tr("Add new light"));
	setSubTitle(tr("Create a new light source for the current scene"));
	registerField("lightfov", m_fov, "value", SIGNAL(valueChanged(double)));
	registerField("radius", m_radius, "value", SIGNAL(valueChanged(double)));
	registerField("lightname", m_name);
	registerField("shadowmapbias", m_shadowMapBias, "value", SIGNAL(valueChanged(double)));
	registerField("shadowcontext", m_shadowContext);
	registerField("lightingcontext", m_lightContext);	
	registerField("material", m_material, "currentText", SIGNAL(currentIndexChanged ( int )));	
	registerField("slices", m_slices);
	registerField("lambda", m_lambda);
		
}


QLightNodePage::~QLightNodePage()
{
}

void QLightNodePage::initializePage()
{
	QDir dir = QDir::currentPath()+"/materials";
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Size | QDir::Reversed);

	QFileInfoList list = dir.entryInfoList();
	//std::cout << "     Bytes Filename" << std::endl;
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		m_material->addItem(QString("materials/")+fileInfo.fileName());
	}
}



