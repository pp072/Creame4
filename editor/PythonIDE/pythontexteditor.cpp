#include "pythontexteditor.h"

#define stop __asm nop

PythonTextEditor::PythonTextEditor(QWidget *parent)
	: QsciScintilla(parent)
{
	init_editor();
	_hightliteWord = "";
	 allocatedIndicatorsOld = 7;
	_wordHightLite  = indicatorDefineOld(QsciScintilla::INDIC_SQUIGGLE);
	setIndicatorDrawUnderOld(true, _wordHightLite);
	setIndicatorForegroundColorOld(Qt::blue, _wordHightLite);

}
void PythonTextEditor::setIndicatorForegroundColorOld(const QColor &col, int indicatorNumber)
{
	if (indicatorNumber <= INDIC_MAX)
	{
		int alpha = col.alpha();

		// We ignore allocatedIndicators to allow any indicators defined
		// elsewhere (e.g. in lexers) to be set.
		if (indicatorNumber < 0)
		{
			for (int i = 0; i <= INDIC_MAX; ++i)
			{
				SendScintilla(SCI_INDICSETFORE, i, col);
				SendScintilla(SCI_INDICSETALPHA, i, alpha);
			}
		}
		else
		{
			SendScintilla(SCI_INDICSETFORE, indicatorNumber, col);
			SendScintilla(SCI_INDICSETALPHA, indicatorNumber, alpha);
		}
	}
}
void PythonTextEditor::setIndicatorDrawUnderOld(bool under, int indicatorNumber)
{
	if (indicatorNumber <= INDIC_MAX)
	{
		// We ignore allocatedIndicators to allow any indicators defined
		// elsewhere (e.g. in lexers) to be set.
		if (indicatorNumber < 0)
		{
			for (int i = 0; i <= INDIC_MAX; ++i)
				SendScintilla(SCI_INDICSETUNDER, i, under);
		}
		else
		{
			SendScintilla(SCI_INDICSETUNDER, indicatorNumber, under);
		}
	}
}
int PythonTextEditor::indicatorDefineOld(long style, int indicatorNumber)
{
	checkIndicatorOld(indicatorNumber);

	if (indicatorNumber >= 0)
		SendScintilla(SCI_INDICSETSTYLE, indicatorNumber, style);

	return indicatorNumber;
}
void PythonTextEditor::checkIndicatorOld(int &indicatorNumber)
{
	allocateId(indicatorNumber, allocatedIndicatorsOld, INDIC_CONTAINER, INDIC_MAX);
}
PythonTextEditor::~PythonTextEditor()
{
	//delete _lexer;
}

void PythonTextEditor::init_editor()
{
	_python_lexer = new Lexers::PythonLexer(this);
	setUtf8(true);
	setIndentationsUseTabs(false);
	setIndentationWidth(4);
	setEolMode(QsciScintilla::EolUnix);

	setIndentationGuides(true);
	setAutoIndent(true);


	QPixmap p(3, 17);
	p.fill(QColor(255,0,0));
	_errMarker = markerDefine(p);

	QPixmap p1(3, 17);
	p1.fill(QColor(255,255,0));
	_warnMarker = markerDefine(p1);

	QPixmap p2(3, 17);
	p2.fill(QColor(0,0,255));
	_convMarker = markerDefine(p2);

	registerImage(1, QPixmap(":/rope/images/attribute.png"));
	registerImage(2, QPixmap(":/rope/images/class.png"));
	registerImage(3, QPixmap(":/rope/images/method.png"));
	registerImage(4, QPixmap(":/rope/images/module.png"));
	registerImage(5, QPixmap(":/rope/images/c++.png"));


	setMarginWidth(3, 40);
	setMarginLineNumbers(3, true);
	setMarginMarkerMask(3, 0);

	setMarginWidth(0, 3);
	setMarginLineNumbers(0, false);
	setMarginMarkerMask(0, (1 << _errMarker));

	setMarginWidth(1, 3);
	setMarginLineNumbers(1, false);
	setMarginMarkerMask(1, (1 << _warnMarker));

	setMarginWidth(2, 3);
	setMarginLineNumbers(2, false);
	setMarginMarkerMask(2, (1 << _convMarker));


	//Автозавершение лексем

	QFont font;
	font.setFamily("Consolas");
	font.setFixedPitch(true);
	font.setBold(false);
	font.setPointSize(10);
	QFontMetrics fm(font);

	setFont(font);
	setFolding(QsciScintilla::BoxedTreeFoldStyle);


	setBraceMatching(QsciScintilla::StrictBraceMatch);
	setMatchedBraceBackgroundColor(Qt::yellow);
	setUnmatchedBraceForegroundColor(Qt::blue);

	setCaretLineVisible(true);
	setCaretLineBackgroundColor(QColor("#CDA869"));

	// ## Margins colors
	// # line numbers margin
	setMarginsBackgroundColor(QColor("#333333"));
	setMarginsForegroundColor(QColor("#CCCCCC"));

	//# folding margin colors (foreground,background)
	setFoldMarginColors(QColor("#99CC66"),QColor("#333300"));
// 	_lexer =new QsciLexerPython();
	setLexer(_python_lexer->lexer());
	_python_lexer->lexer()->setColor(QColor(50,80,50,35),10);
	setAutoCompletionSource(QsciScintilla::AcsAPIs);
	setAutoCompletionCaseSensitivity(false);
	setAutoCompletionReplaceWord(true);
	setAutoCompletionShowSingle(true);
	setAutoCompletionThreshold(2);
}

void PythonTextEditor::mouseMoveEvent( QMouseEvent *e )
{
	if (e->modifiers() & Qt::ControlModifier){
		QString word = wordAtPoint(e->pos());
		int pos = SendScintilla(QsciScintilla::SCI_POSITIONFROMPOINTCLOSE, e->x(), e->y());
		int wordStart;
		int wordEnd;

		if (!word.isEmpty() && pos > 0){
			wordStart = SendScintilla(QsciScintilla::SCI_WORDSTARTPOSITION, pos, true);
			wordEnd = SendScintilla(QsciScintilla::SCI_WORDENDPOSITION, pos, true);
		}

		if (!word.isEmpty() && pos > 0 && (word != _hightliteWord || _hightliteWordStart != wordStart)){
			SendScintilla(QsciScintilla::SCI_SETINDICATORCURRENT, _wordHightLite);
			SendScintilla(QsciScintilla::SCI_INDICATORCLEARRANGE, 0, text().length());

			_hightliteWord = word;
			_hightliteWordStart = wordStart;

			int sline = 0, sindex = 0;
			lineIndexFromPosition(wordStart, &sline, &sindex);

			int eline = 0, eindex = 0;
			lineIndexFromPosition(wordEnd, &eline, &eindex);
			fillIndicatorRange(sline, sindex, eline, eindex, _wordHightLite);
		}
	} else {
		if (!_hightliteWord.isEmpty() || _hightliteWordStart){
			SendScintilla(QsciScintilla::SCI_SETINDICATORCURRENT, _wordHightLite);
			SendScintilla(QsciScintilla::SCI_INDICATORCLEARRANGE, 0,text().length());
			_hightliteWord = "";
			_hightliteWordStart = 0;
		} else {
			QsciScintilla::mouseMoveEvent(e);
		}
	}
}

 void PythonTextEditor::mousePressEvent( QMouseEvent *e )
 {
	 if (e->modifiers() & Qt::ControlModifier){
		 QString word = wordAtPoint(e->pos());
		 int pos = SendScintilla(QsciScintilla::SCI_POSITIONFROMPOINTCLOSE, e->x(), e->y());
		 int wordStart = SendScintilla(QsciScintilla::SCI_WORDSTARTPOSITION, pos, true);
		 if (!word.isEmpty() && pos > 0 && word == _hightliteWord && _hightliteWordStart == wordStart){
			 emit findImpl(text(), pos);
			 return;
		 }
	 }
	 QsciScintilla::mousePressEvent(e);
 }

 void PythonTextEditor::jumpOffset( int offset )
 {
	 int line = 0, index = 0;
	 lineIndexFromPosition(offset, &line, &index);
	 setCursorPosition(line, index);
	 setFocus();
 }

