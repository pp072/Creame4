#include "designer.h"

#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QDesignerComponents>
#include <QtDesigner/QDesignerWidgetBoxInterface>
#include <QtDesigner/QDesignerFormEditorPluginInterface>
#include <QtDesigner/QDesignerFormWindowManagerInterface>
#include <QtDesigner/QDesignerObjectInspectorInterface>
#include <QtDesigner/QDesignerPropertyEditorInterface>
#include <QtDesigner/QDesignerComponents>
#include "internals/qdesigner_integration_p.h"
#include "internals/pluginmanager_p.h"
#include "internals/formwindowbase_p.h"

#include <QDebug>
#include <QFile>
#include <QPluginLoader>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QDir>
#include <QCloseEvent>
#include <QScrollBar>
#include <QTextCodec>
#include <QProcess>
#define stop __asm nop
Designer::Designer(QWidget *parentWidget, Qt::WFlags flags , QString fileName)
	: QWidget(parentWidget ), _form(NULL)
{
	ui.setupUi(this);
	ui.modifyLbl->setVisible(false);
	ui.splitter_2->setStretchFactor(1, 100);

	if (!_iface){
		_iface = QDesignerComponents::createFormEditor(parentWidget);
		QDesignerComponents::createTaskMenu(_iface, parentWidget);
		QDesignerComponents::initializePlugins(_iface);
		QDesignerComponents::initializeResources();
	}

	_iface->setTopLevel(this);
	_wb = QDesignerComponents::createWidgetBox(_iface, this);
	ui.widgetsPlace->layout()->addWidget(_wb);
	_iface->setWidgetBox(_wb);

	_oi = QDesignerComponents::createObjectInspector(_iface, this);
	ui.objectsPlace->layout()->addWidget(_oi);
	_iface->setObjectInspector(_oi);

	_pe = QDesignerComponents::createPropertyEditor(_iface, this);
	ui.propertyPlace->layout()->addWidget(_pe);
	_iface->setPropertyEditor(_pe);


	_ae = QDesignerComponents::createActionEditor(_iface, this);
	ui.actionTabPlace->layout()->addWidget((QWidget*)_ae);
	_iface->setActionEditor(_ae);

	ui.slotTabPLace->layout()->addWidget(QDesignerComponents::createSignalSlotEditor(_iface, this));

	_designer = new qdesigner_internal::QDesignerIntegration(_iface, this);
	_iface->setIntegration(_designer);

	QList<QObject*> plugins = QPluginLoader::staticInstances();
	plugins += _iface->pluginManager()->instances();

	foreach (QObject *plugin, plugins) {
		if (QDesignerFormEditorPluginInterface *formEditorPlugin = qobject_cast<QDesignerFormEditorPluginInterface*>(plugin)) {
			if (!formEditorPlugin->isInitialized()){
				formEditorPlugin->initialize(_iface);
			}
		}
	}

	connect(_designer->core()->formWindowManager()->actionUndo(), SIGNAL(changed()), SLOT(onUndoAvailable()));
	connect(_designer->core()->formWindowManager()->actionRedo(), SIGNAL(changed()), SLOT(onRedoAvailable()));
	connect(_designer->core()->formWindowManager()->actionCopy(), SIGNAL(changed()), SLOT(onCopyAvailable()));
	connect(_designer->core()->formWindowManager()->actionPaste(), SIGNAL(changed()), SLOT(onPasteAvailable()));

	//connect(ui.closeBtn, SIGNAL(clicked()), SIGNAL(closeRequest()));
	
	loadSettings();
	addToolBar();

	if (!fileName.isEmpty())
		loadFile(fileName);

	
}
void Designer::addToolBar()
{
	_toolBar = new QToolBar(this);
	_toolBar->setIconSize(QSize(20,20));

	_toolBar->addAction(ui.actionSaveForm);
	_toolBar->addAction(ui.actionPreview);

	_toolBar->addSeparator();
	QWidget* spacer = new QWidget(this);
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	_toolBar->addWidget(spacer);

	_toolBar->addWidget(ui.editWidgetsBtn);
	_toolBar->addWidget(ui.editTabOrderBtn);
	_toolBar->addWidget(ui.editBuddiesBtn);

	connect(ui.editWidgetsBtn, SIGNAL(clicked()), SLOT(onSwitchWidgets()));
	ui.editWidgetsBtn->setChecked(true);
	connect(ui.editTabOrderBtn, SIGNAL(clicked()), SLOT(onSwitchTabOrder()));
	connect(ui.editBuddiesBtn, SIGNAL(clicked()), SLOT(onSwitchBuddies()));
	connect(ui.actionPreview, SIGNAL(triggered()),SLOT(previewForm()));
	connect(ui.actionSaveForm, SIGNAL(triggered()),SLOT(saveFile()));

	ui.toolBarLayout->addWidget(_toolBar);
}

Designer::~Designer()
{
	

}

void Designer::onUndoAvailable()
{
	emit undoAvailable(_designer->core()->formWindowManager()->actionUndo()->isEnabled());
}

void Designer::onRedoAvailable()
{
	emit redoAvailable(_designer->core()->formWindowManager()->actionRedo()->isEnabled());
}

void Designer::onCopyAvailable()
{
	emit copyAvailable(_designer->core()->formWindowManager()->actionCopy()->isEnabled());
}

void Designer::onPasteAvailable()
{
	emit pasteAvailable(_designer->core()->formWindowManager()->actionPaste()->isEnabled());
}

bool Designer::loadFile(const QString & fileName)
{
	if (!QFile::exists(fileName))
		return false;

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	setFileName(fileName);
	//_fileName = fileName;
	_form = qobject_cast<qdesigner_internal::FormWindowBase *>(_iface->formWindowManager()->createFormWindow(this));
	_form->setFileName(_fileName);
	//_form->addAction(_editTabAction);

	QFile f(fileName);
	f.open(QIODevice::ReadOnly | QIODevice::Text);
	_form->setContents(f.readAll());
	f.close();

	qdesigner_internal::FormWindowBase::setupDefaultAction(_form);
	_designer->setupFormWindow(_form);

	QMdiSubWindow *wnd = ui.formPlace->addSubWindow(_form, Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	const QSize containerSize = _form->mainContainer()->size();
	const QSize containerMinimumSize = _form->mainContainer()->minimumSize();
	const QSize containerMaximumSize = _form->mainContainer()->maximumSize();
	const QSize decorationSize = wnd->geometry().size() - wnd->contentsRect().size();

	wnd->resize(containerSize+decorationSize);
	wnd->setMinimumSize(containerMinimumSize+decorationSize);

	if( containerMaximumSize == QSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX) )
		wnd->setMaximumSize(containerMaximumSize);
	else
		wnd->setMaximumSize(containerMaximumSize+decorationSize);
	wnd->setWindowTitle( _form->mainContainer()->windowTitle() );
	wnd->show();

	_iface->formWindowManager()->setActiveFormWindow(_form);
	//_form->editWidgets();
	connect(_form, SIGNAL(changed()), SLOT(onFormChanged()));
	_form->setDirty(false);
	return true;
}
void Designer::setFileName( const QString & fileName )
{
	_fileName = fileName;
	ui.fileName->setText(_fileName);
}

void Designer::onSwitchWidgets()
{
	QDesignerFormWindowManagerInterface *formWindowManager = _iface->formWindowManager();
	for (int i=0; i<formWindowManager->formWindowCount(); ++i) {
		QDesignerFormWindowInterface *formWindow = formWindowManager->formWindow(i);
		if (formWindow == _form)
			formWindow->editWidgets();
	}
	ui.editWidgetsBtn->setChecked(true);
	ui.editTabOrderBtn->setChecked(false);
	ui.editBuddiesBtn->setChecked(false);
}

void Designer::onSwitchTabOrder()
{
	QDesignerFormWindowManagerInterface *formWindowManager = _designer->core()->formWindowManager();
	for (int i=0; i<formWindowManager->formWindowCount(); ++i) {
		QDesignerFormWindowInterface *formWindow = formWindowManager->formWindow(i);
		if (formWindow == _form)
			formWindow->setCurrentTool(3);
	}
	ui.editWidgetsBtn->setChecked(false);
	ui.editTabOrderBtn->setChecked(true);
	ui.editBuddiesBtn->setChecked(false);
}

void Designer::onSwitchBuddies()
{
	QDesignerFormWindowManagerInterface *formWindowManager = _designer->core()->formWindowManager();
	for (int i=0; i<formWindowManager->formWindowCount(); ++i) {
		QDesignerFormWindowInterface *formWindow = formWindowManager->formWindow(i);
		if (formWindow == _form)
			formWindow->setCurrentTool(2);
	}
	ui.editWidgetsBtn->setChecked(false);
	ui.editTabOrderBtn->setChecked(false);
	ui.editBuddiesBtn->setChecked(true);
}

void Designer::onFormChanged()
{
	emit fileChanged(_form->isDirty());
	if (_form->isDirty())
		ui.modifyLbl->setVisible(true);
	else
		ui.modifyLbl->setVisible(false);
}

void Designer::closeEvent( QCloseEvent * event)
{
	if (_form && _form->isDirty())
	{
 		QMessageBox::StandardButton res = QMessageBox::question(this, "Close editor",
 			QString("Would you like to save \"%1\" ?").arg(_fileName),
 			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
 
 		if ( res == QMessageBox::Yes)
 			saveFile();
 		if (res == QMessageBox::Cancel)
 			event->ignore();
		if (res == QMessageBox::No)
			 _form->setDirty(false);
	}
	saveSettings();
}

bool Designer::saveFile()
{
	QFile file(_fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
		file.write(_form->contents().toUtf8());
		file.close();
		_form->setDirty(false);
		onFormChanged();
		renderPyFile();
		return true;
	}
	return false;
}

void Designer::renderPyFile()
{
	QFileInfo info = QFileInfo(_fileName);
	QString pyFile = info.absoluteDir().path() + QDir::separator() + "ui_" + info.baseName() + ".py";
	qDebug() << pyFile;
	QStringList args;
 	args << "-o";
 	args << pyFile;
 	args << _fileName;
	QString pyside_uicpath = _settings->value("Designer/pyside_uic_PATH").toString() + QDir::separator() + "pyside-uic.exe";
	QProcess proc;
 	proc.start(pyside_uicpath, args);
 	proc.waitForFinished();
}

void Designer::saveSettings()
{
	_settings->setValue("Designer/geometry", this->saveGeometry());
	QString pyside_uicpath = "C://Python26//Scripts";
	_settings->setValue("Designer/pyside_uic_PATH", pyside_uicpath);
}

void Designer::loadSettings()
{
	_settings = new QSettings(SETTINGS_DESIGNER_FILE,	QSettings::IniFormat ,this);
	this->restoreGeometry(_settings->value("Designer/geometry").toByteArray());
}

void Designer::previewForm()
{
	QFileInfo info = QFileInfo(_fileName);
	QString pyFile = info.absoluteDir().path() + QDir::separator() + "ui_" + info.baseName() + ".py";
	qDebug() << pyFile;
	QStringList args;
	args << "-p";
	args << _fileName;
	QString pyside_uicpath = _settings->value("Designer/pyside_uic_PATH").toString() + QDir::separator() + "pyside-uic.exe";
	QProcess proc;
	proc.start(pyside_uicpath, args);
	proc.waitForFinished();
}
