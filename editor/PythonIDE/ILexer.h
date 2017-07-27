#ifndef IDEFAULTLEXER_H
#define IDEFAULTLEXER_H

#include <Qsci/qscilexer.h>

class ITheme;

namespace Lexers{

class ILexer
{
public:
    virtual ~ILexer() {}

    virtual QsciLexer * lexer() = 0;
    virtual void setTheme(ITheme *) = 0;
    virtual QColor color(int) = 0;

    virtual QList<int> styles() = 0;
    virtual QString description(int) const = 0;
    virtual QString nameInTheme(int) const = 0;
    virtual QString name() = 0;
};

}
#endif
