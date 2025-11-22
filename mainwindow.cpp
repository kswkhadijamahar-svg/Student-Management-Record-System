#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButtonAdd->setEnabled(true);
    ui->pushButtonUpdate->setEnabled(false);
    ui->pushButtonDelete->setEnabled(false);

    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(ui->pushButtonNEXT, &QPushButton::clicked, this, &MainWindow::onNextClicked);
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &MainWindow::onUpdateClicked);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);

    currentIndex = -1;

    QFile file("students.txt");
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        in.readLine(); // skip header

        while(!in.atEnd()) {
            QStringList f = in.readLine().split(",");
            if(f.size() == 6) {
                students.append({f[0], f[1], f[2], f[3], f[4], f[5]});
            }
        }
        file.close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearFields() {
    ui->textEditName->clear();
    ui->textEditRoll->clear();
    ui->textEditFather->clear();
    ui->textEditCaste->clear();
    ui->textEditDistrict->clear();
    ui->textEditGPA->clear();
}

bool MainWindow::validateFields() {
    if (ui->textEditName->toPlainText().isEmpty()) return fieldError("Name");
    if (ui->textEditRoll->toPlainText().isEmpty()) return fieldError("Roll Number");
    if (ui->textEditFather->toPlainText().isEmpty()) return fieldError("Father Name");
    if (ui->textEditCaste->toPlainText().isEmpty()) return fieldError("Caste");
    if (ui->textEditDistrict->toPlainText().isEmpty()) return fieldError("District");
    if (ui->textEditGPA->toPlainText().isEmpty()) return fieldError("GPA");
    return true;
}

bool MainWindow::fieldError(QString field) {
    QMessageBox::warning(this, "Missing Field", field + " is required!");
    return false;
}

void MainWindow::saveAll() {
    QFile file("students.csv");
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&file);

        out << "Name,Roll,Father,Caste,District,GPA\n";
        for (auto &s : students)
            out << s.name << "," << s.roll << "," << s.father << ","
                << s.caste << "," << s.district << "," << s.gpa << "\n";

        file.close();
    }
}

void MainWindow::onAddClicked() {
    if (!validateFields()) return;

    students.append({
        ui->textEditName->toPlainText(),
        ui->textEditRoll->toPlainText(),
        ui->textEditFather->toPlainText(),
        ui->textEditCaste->toPlainText(),
        ui->textEditDistrict->toPlainText(),
        ui->textEditGPA->toPlainText()
    });

    saveAll();
    QMessageBox::information(this, "Added", "Record added successfully!");

    clearFields();
    currentIndex = -1;

    ui->pushButtonUpdate->setEnabled(false);
    ui->pushButtonDelete->setEnabled(false);
}

void MainWindow::onNextClicked() {
    if (students.isEmpty()) {
        QMessageBox::information(this, "Info", "No records available!");
        return;
    }

    currentIndex = (currentIndex + 1) % students.size();
    Student s = students[currentIndex];

    ui->textEditName->setText(s.name);
    ui->textEditRoll->setText(s.roll);
    ui->textEditFather->setText(s.father);
    ui->textEditCaste->setText(s.caste);
    ui->textEditDistrict->setText(s.district);
    ui->textEditGPA->setText(s.gpa);

    ui->pushButtonUpdate->setEnabled(true);
    ui->pushButtonDelete->setEnabled(true);
}

void MainWindow::onUpdateClicked() {
    if (currentIndex < 0 || currentIndex >= students.size()) return;
    if (!validateFields()) return;

    Student &s = students[currentIndex];

    QString oldRoll = s.roll;
    QString newRoll = ui->textEditRoll->toPlainText();

    if (oldRoll != newRoll) {
        QMessageBox::warning(this, "Primary Key Error", "Roll Number cannot be changed!");
        ui->textEditRoll->setText(oldRoll);
        return;
    }

    s.name = ui->textEditName->toPlainText();
    s.father = ui->textEditFather->toPlainText();
    s.caste = ui->textEditCaste->toPlainText();
    s.district = ui->textEditDistrict->toPlainText();
    s.gpa = ui->textEditGPA->toPlainText();

    saveAll();
    QMessageBox::information(this, "Updated", "Record updated successfully!");
}

void MainWindow::onDeleteClicked() {
    if (currentIndex < 0 || currentIndex >= students.size()) return;

    students.removeAt(currentIndex);
    saveAll();

    QMessageBox::information(this, "Deleted", "Record deleted successfully!");

    if (students.isEmpty()) {
        clearFields();
        currentIndex = -1;
        ui->pushButtonUpdate->setEnabled(false);
        ui->pushButtonDelete->setEnabled(false);
        return;
    }

    currentIndex %= students.size();
    Student s = students[currentIndex];

    ui->textEditName->setText(s.name);
    ui->textEditRoll->setText(s.roll);
    ui->textEditFather->setText(s.father);
    ui->textEditCaste->setText(s.caste);
    ui->textEditDistrict->setText(s.district);
    ui->textEditGPA->setText(s.gpa);

    ui->pushButtonUpdate->setEnabled(true);
    ui->pushButtonDelete->setEnabled(true);
}
