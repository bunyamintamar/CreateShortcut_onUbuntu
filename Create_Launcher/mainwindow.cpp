#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->labelLog->setText("");
    ui->textExecutableFile->setText("");
    ui->textIcon->setText("");
    ui->labelVersionInfo->setText("Version: 2.0.0");
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isTextBoxesEmpty()
{
    if(m_ShortcutName.isEmpty())
    {
        ui->labelLog->setText("Enter shortcut name !!!");
        return true;
    }

    if(m_ExecutableFile.isEmpty())
    {
        ui->labelLog->setText("Select an executable file !!!");
        return true;
    }

    if(m_Icon.isEmpty())
    {
        ui->labelLog->setText("Select an icon file !!!");
        return true;
    }

    return false;
}

QFile* MainWindow::createFileOnDesktop(QString fileName, QByteArray content)
{
    QString path = "/home/" + qgetenv("USER")+"/Desktop/" + fileName;

    QFile *f;
    f = new QFile(path);

    if(f->exists())
        return NULL;

    f->open(QIODevice::WriteOnly);
    f->write(content);
    f->close();
    return f;
}

QString MainWindow::commandLineFor_ExeFile(QString path)
{
    QFileInfo info(path);
    QString cmdString;

    if(!(info.suffix() == "sh") && !(info.suffix() == ""))
        cmdString.append("xdg-open ");

    if(path.contains(" "))
        closeWithApostrophe(&path);

    cmdString.append(path.toLocal8Bit());

    return cmdString;
}

void MainWindow::convertExecutableFile(QFile *f)
{
    QFileInfo info(f->fileName());
    QString cmd = "chmod +x " + info.filePath();
    QProcess::execute(cmd);
}

QByteArray MainWindow::shortcutProperties()
{
    QByteArray content;

    content.append("#!/usr/bin/env xdg-open\n\n");
    content.append("[Desktop Entry]\n");

    content.append("Name=" + m_ShortcutName.toLocal8Bit() + "\n");

    content.append("StartupNotify=true\n");
    content.append("Terminal=false\n");
    content.append("Type=Application\n");

    content.append("Icon=" + m_Icon.toLocal8Bit());

    content.append("\nExec=");
    content.append(commandLineFor_ExeFile(m_ExecutableFile) + " %F");

    return content;
}

void MainWindow::closeWithApostrophe(QString *string)
{
    QString s;
    s.append("'" + *string + "'");
    *string = s;
}

void MainWindow::on_buttonExeFile_clicked()
{
    m_ExecutableFile = QFileDialog::getOpenFileName(this, "Select an executable file", "/home/");
    ui->textExecutableFile->setText(m_ExecutableFile);
}

void MainWindow::on_buttonIcon_clicked()
{
    m_Icon = QFileDialog::getOpenFileName(this, "Select an icon", "/home/");
    if(m_Icon.isEmpty())
    {
        ui->buttonIcon->setText("Select icon file");
        return;
    }
    ui->buttonIcon->setText("");
    ui->textIcon->setText(m_Icon);
    QIcon icon = QIcon(m_Icon);
    ui->buttonIcon->setIcon(icon);
}

void MainWindow::on_buttonCreate_clicked()
{
    m_ShortcutName = ui->textLauncherName->text();

    if(isTextBoxesEmpty())
        return;

    QString fileName = m_ShortcutName + ".desktop";

    QFile *f;
    f = createFileOnDesktop(fileName,shortcutProperties());

    convertExecutableFile(f);

    ui->labelLog->setText("Shortcut is created succesfully");
}

