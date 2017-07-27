#ifndef DESIGNER_H
#define DESIGNER_H

#ifdef DLL_DESIGNER_NONCLIENT_BUILD // в директивах препроцессора проекта с dll нужно прописать DLL_NONCLIENT_BUILD
#   define DLL_DESIGNER_EXPORT __declspec(dllexport)
#else
#   define DLL_DESIGNER_EXPORT __declspec(dllimport)
#endif

#include <QtGui/QWidget>
#include "GeneratedFiles/ui_designer.h"
#include <QSettings>
#include <QToolBar>

class QDesignerFormWindowInterface;
class QDesignerFormEditorInterface;
class QAbstractTableModel;
class StatusBar;
class QDesignerWidgetBoxInterface;
class QDesignerObjectInspectorInterface;
class QDesignerPropertyEditorInterface;
class QDesignerActionEditorInterface;

namespace qdesigner_internal {
	class QDesignerIntegration;
	class FormWindowBase;
}
static QDesignerFormEditorInterface *_iface = NULL;
#define SETTINGS_DESIGNER_FILE "designer.ini"

class DLL_DESIGNER_EXPORT Designer : public QWidget
{
	Q_OBJECT

public:
	Designer(QWidget *parent = 0, Qt::WFlags flags = 0, QString fileName = "");
	~Designer();

	bool loadFile(const QString & fileName);
	void setFileName( const QString & fileName );
private:
	Ui::DesignerClass ui;
	qdesigner_internal::QDesignerIntegration *_designer;
	qdesigner_internal::FormWindowBase * _form;
	QDesignerWidgetBoxInterface *_wb;
	QDesignerObjectInspectorInterface *_oi;
	QDesignerPropertyEditorInterface * _pe;
	QDesignerActionEditorInterface *_ae;

	QString _fileName;
	void renderPyFile();
	
	QToolBar	*_toolBar;
	void addToolBar();

	QSettings	*_settings;
	void loadSettings();
	void saveSettings();

private slots:
	void onSwitchWidgets();
	void onSwitchTabOrder();
	void onSwitchBuddies();
	void onFormChanged();

	virtual void closeEvent(QCloseEvent *);

	void onUndoAvailable();
	void onRedoAvailable();
	void onCopyAvailable();
	void onPasteAvailable();

	bool saveFile();
	void previewForm();
signals:
	void fileChanged(bool);
	void copyAvailable(bool);
	void undoAvailable(bool);
	void redoAvailable(bool);
	void pasteAvailable(bool);
};

#endif // DESIGNER_H
