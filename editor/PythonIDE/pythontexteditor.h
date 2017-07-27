#ifndef PYTHONTEXTEDITOR_H
#define PYTHONTEXTEDITOR_H

#include <Qsci/qsciscintilla.h>
#include <Qsci/qsciscintillabase.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qsciapis.h>
#include <QMouseEvent>
#include "PythonLexer.h"
class PythonTextEditor : public QsciScintilla
{
	Q_OBJECT

public:
	PythonTextEditor(QWidget *parent = 0);
	~PythonTextEditor();
	void init_editor();
	void jumpOffset(int offset);
protected:
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mousePressEvent(QMouseEvent *e);
	int indicatorDefineOld(long style, int indicatorNumber = -1);
	void checkIndicatorOld(int &indicatorNumber);
	void setIndicatorDrawUnderOld(bool under, int indicatorNumber );
	void setIndicatorForegroundColorOld(const QColor &col, int indicatorNumber);
private:
	Lexers::PythonLexer *_python_lexer;
	QString _hightliteWord;
	int _hightliteWordStart;
	int _wordHightLite;
	 unsigned allocatedIndicatorsOld;

	
	int _errMarker, _warnMarker, _convMarker;
signals:
	void findImpl(QString str, int pos);
	
};

#endif // PYTHONTEXTEDITOR_H
