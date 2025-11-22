#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Student {
    QString name;
    QString roll;
    QString father;
    QString caste;
    QString district;
    QString gpa;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QList<Student> students;
    int currentIndex = -1;

    bool validateFields();
    bool fieldError(const QString &field);
    void loadStudent(int index);
    void clearFields();

private slots:
    void onAddClicked();
    void onNextClicked();
    void onUpdateClicked();
    void onDeleteClicked();
    void on_pushButtonADD_selectionChanged();
};

#endif // MAINWINDOW_H
