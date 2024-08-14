#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QDialog>
#include <QLabel>
#include <QInputDialog>
#include <QFileDialog>
#include <QDebug>
#include <QTextBrowser>
#include <QMenubar>
#include <QObject>
#include <QStringList>
#include <QString>
#include <QFileInfo>
#include <QByteArray>
#include <QMessageBox>
#include <QMdiSubWindow>
#include <QAction>
#include <QIcon>
#include <QRandomGenerator>
#include <QProgressDialog>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE
class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QHash<QAction*,QWidget*> windowHash;
    QLineEdit* idInput;
    QLineEdit* passwordInput;
    QString accountLine;
    QString savemoney;
    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    QVBoxLayout* loginLayout= new QVBoxLayout();
    QVBoxLayout* welcomeLayout = new QVBoxLayout();
    QVBoxLayout* inAoutLayout= new QVBoxLayout();
    QVBoxLayout* aclog = new QVBoxLayout();
    //처음 창
    QPushButton*logIn= new QPushButton("로그인",0);
    QPushButton*MakeAccount= new QPushButton("회원가입",0);
    QPushButton*exit= new QPushButton("종료",0);
    //입출금 창
    QPushButton*inAout= new QPushButton("입출금",0);
    QPushButton*in= new QPushButton("입금",0);
    QPushButton*out= new QPushButton("출금",0);
    QPushButton*check= new QPushButton("조회",0);
    QGroupBox *qg = new QGroupBox("&로그인",0);
    QPushButton *loginButton = new QPushButton("로그인", 0);
    QString id;
    QString newmoney;
private:
    Ui::Widget *ui;
    QMdiArea *mdiArea;
public slots:
    void login();
    void onLoginAttempt();
    void mainc();
    void createAcc();
    void Welcome();
    void saveAccount(const QString& id, const QString& password);
    void InOut();
    void cumoney();
    void moneyin();
    void moneyout();
    void havemoney();
    void updateaccount(const QString &id, const QString &newmoney);
    bool idcheck(const QString& id);
};
#endif // WIDGET_H
