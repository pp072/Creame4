#ifndef SETTINGUI_H
#define SETTINGUI_H
#include <QList>
#include <QMainWindow>
#include <QSettings>

// #ifdef LIB_EXPORT // в директивах препроцессора проекта с dll нужно прописать LIB_EXPORT или LIB_IMPORT или ничего
// #   define DLL_EXPORT __declspec(dllexport)
// #endif
// #ifndef LIB_EXPORT
// #ifdef LIB_IMPORT_SETTINGUI 
// #   define DLL_EXPORT __declspec(dllimport)
// #else
// #   define DLL_EXPORT 
// #endif
// #endif

#ifdef LIB_EXPORT // в директивах препроцессора проекта с dll нужно прописать DLL_NONCLIENT_BUILD
#   define DLL_EXPORT_SETTINGUI __declspec(dllexport)
#else
#   define DLL_EXPORT_SETTINGUI __declspec(dllimport)
#endif

class IprefWidget;

namespace Ui {
    class SettingUI;
}

class DLL_EXPORT_SETTINGUI SettingUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingUI(QWidget *parent = 0, QString settings_file="");
    ~SettingUI();

public slots:
    void saveSettings();

private:


    void buildUIFromQSetting(QSettings *settings);

    QList<IprefWidget *> prefWidgetsList;
    Ui::SettingUI *ui;
	QString _settings_file;
};

#endif // SETTINGUI_H
