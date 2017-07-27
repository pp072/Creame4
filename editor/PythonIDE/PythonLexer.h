#ifndef PYTHONLEXER_H
#define PYTHONLEXER_H

#include <Qsci/qscilexerpython.h>
#include "ILexer.h"

namespace Lexers{

class PythonLexer: public QsciLexerPython, public ILexer
{
public:
    PythonLexer(QObject *parent);
    virtual ~PythonLexer();

    virtual QsciLexer * lexer();
    virtual void setTheme(ITheme *);

    virtual QList<int> styles();
    virtual QString description(int) const;
    virtual QString nameInTheme(int) const;
    virtual QString name();
    virtual QColor color(int);

    virtual const char *keywords(int set) const;
private:
    QString _keywords;
};

}
#endif // PYTHONLEXER_H
