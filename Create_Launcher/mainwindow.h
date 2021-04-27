#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;

        QString     m_ShortcutName;
        QString     m_ExecutableFile;
        QString     m_Icon;
        QFile       *m_File;
        QFile       *createFileOnDesktop(QString fileName);
        QByteArray  shortcutProperties();
        void        writePropertiesToFile(QFile *f, QByteArray properties);
        void        showLog(QString log);
        void        hideLog();
        bool        isFileExist(QString path);
        void        convertToExecutableFile(QString path);

    private slots:
        void on_buttonCreate_clicked();
};
#endif // MAINWINDOW_H
