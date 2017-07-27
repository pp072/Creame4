#include <QDebug>
#include "PythonLexer.h"

namespace Lexers {

PythonLexer::PythonLexer(QObject *parent):
    QsciLexerPython(parent)
{
    _keywords = QsciLexerPython::keywords(2)+QString(" self str True False int float bool");
}

PythonLexer::~PythonLexer()
{

}

QsciLexer * PythonLexer::lexer()
{
    return this;
}

QList<int> PythonLexer::styles()
{
    QList<int> styles;
    styles << Default
    << Comment
    << Number
    << DoubleQuotedString
    << SingleQuotedString
    << Keyword
    << TripleSingleQuotedString
    << TripleDoubleQuotedString
    << ClassName
    << FunctionMethodName
    << Operator
    << Identifier
    << CommentBlock
    << UnclosedString
    << HighlightedIdentifier
    << Decorator;
    return styles;
}

QString PythonLexer::description(int style) const
{
    switch(style){
    case Default:                  return trUtf8("Default");
    case Comment:                  return trUtf8("Comment");
    case Number:                   return trUtf8("Number");
    case DoubleQuotedString:       return trUtf8("DoubleQuotedString");
    case SingleQuotedString:       return trUtf8("SingleQuotedString");
    case Keyword:                  return trUtf8("Keyword");
    case TripleSingleQuotedString: return trUtf8("TripleSingleQuotedString");
    case TripleDoubleQuotedString: return trUtf8("TripleDoubleQuotedString");
    case ClassName:                return trUtf8("ClassName");
    case FunctionMethodName:       return trUtf8("FunctionMethodName");
    case Operator:                 return trUtf8("Operator");
    case Identifier:               return trUtf8("Identifier");
    case CommentBlock:             return trUtf8("CommentBlock");
    case UnclosedString:           return trUtf8("UnclosedString");
    case HighlightedIdentifier:    return trUtf8("HighlightedIdentifier");
    case Decorator:                return trUtf8("Decorator");
    }
    return trUtf8("Unknown");
}

QString PythonLexer::nameInTheme(int style) const
{
    switch(style){
    case Default:                  return "Default";
    case Comment:                  return "Comment";
    case Number:                   return "Number";
    case DoubleQuotedString:       return "DoubleQuotedString";
    case SingleQuotedString:       return "SingleQuotedString";
    case Keyword:                  return "Keyword";
    case TripleSingleQuotedString: return "TripleSingleQuotedString";
    case TripleDoubleQuotedString: return "TripleDoubleQuotedString";
    case ClassName:                return "ClassName";
    case FunctionMethodName:       return "FunctionMethodName";
    case Operator:                 return "Operator";
    case Identifier:               return "Identifier";
    case CommentBlock:             return "CommentBlock";
    case UnclosedString:           return "UnclosedString";
    case HighlightedIdentifier:    return "HighlightedIdentifier";
    case Decorator:                return "Decorator";
    }
    return "Unknown";
}

QString PythonLexer::name()
{
    return "Python";
}

void PythonLexer::setTheme(ITheme *){}

const char *PythonLexer::keywords(int set) const
{
    if (set == 2){
        return (const char*)_keywords.toAscii();
    }
    return QsciLexerPython::keywords(set);
}

QColor PythonLexer::color(int style)
{
    return defaultColor(style);
}

}
