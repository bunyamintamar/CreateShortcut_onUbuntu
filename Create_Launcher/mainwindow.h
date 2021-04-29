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
        bool        isTextBoxesEmpty();
        QFile       *createFileOnDesktop(QString fileName, QByteArray content);
        QString     commandLineFor_ExeFile(QString path);
        void        convertExecutableFile(QFile *f);
        QByteArray  shortcutProperties();
        void        closeWithApostrophe(QString *string);

    private slots:
        void on_buttonCreate_clicked();
        void on_buttonExeFile_clicked();
        void on_buttonIcon_clicked();
};
#endif // MAINWINDOW_H
