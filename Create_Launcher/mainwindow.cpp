#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDebug>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hideLog();
    ui->labelVersionInfo->setText("Version: 1.0.0");
}

MainWindow::~MainWindow()
{
    delete ui;
}

QFile* MainWindow::createFileOnDesktop(QString fileName)
{
    if(fileName.isEmpty())
        return NULL;

    QString path = "/home/" + qgetenv("USER")+"/Desktop/" + fileName + ".desktop";

    if(isFileExist(path))
    {
        showLog("File is already exist !");
        return NULL;
    }

    QFile *f;
    f = new QFile(path);
    f->open(QIODevice::WriteOnly);
    f->close();
    convertToExecutableFile(path);
    return f;
}

QByteArray MainWindow::shortcutProperties()
{
    QByteArray content;
    content.append("#!/usr/bin/env xdg-open\n\n");
    content.append("[Desktop Entry]\n");    
    content.append("Name=");
    content.append(m_ShortcutName.toLocal8Bit());
    content.append("\nStartupNotify=true\n");
    content.append("Terminal=false\n");
    content.append("Type=Application\n");
    content.append("Icon=");

    if(m_Icon.contains(" "))
    {
        content.append("'");
        content.append(m_Icon.toLocal8Bit());
        content.append("'");
    }
    else
        content.append(m_Icon.toLocal8Bit());

    content.append("\nExec=");

    if(m_ExecutableFile.contains(" "))
    {
        content.append("'");
        content.append(m_ExecutableFile.toLocal8Bit());
        content.append("'");
    }
    else
        content.append(m_ExecutableFile.toLocal8Bit());

    content.append(" %F");
    return content;
}

void MainWindow::writePropertiesToFile(QFile *f, QByteArray properties)
{
    f->open(QIODevice::WriteOnly);
    f->write(properties.data());
    f->close();
    showLog("Shortcut is created succesfully");
}

void MainWindow::showLog(QString log)
{
    ui->labelLog->setText(log);
}

void MainWindow::hideLog()
{
    ui->labelLog->setText("");
}

bool MainWindow::isFileExist(QString path)
{
    QFile file(path);
    if(file.exists())
        return true;
    else
        return false;
}

void MainWindow::convertToExecutableFile(QString path)
{
    QString cmd = "chmod +x " + path;
    QProcess::execute(cmd);
}

void MainWindow::on_buttonCreate_clicked()
{
    m_ShortcutName   = ui->textLauncherName->text();
    m_ExecutableFile = ui->textExecutableFile->text();
    m_Icon           = ui->textIcon->text();

    if(!isFileExist(m_ExecutableFile))
    {
        showLog("There is not any executable file !");
        return;
    }

    if(!isFileExist(m_Icon))
    {
        showLog("There is not any icon file !");
        return;
    }

    m_File = createFileOnDesktop(m_ShortcutName);
    if(m_File)
        writePropertiesToFile(m_File,shortcutProperties());
}
