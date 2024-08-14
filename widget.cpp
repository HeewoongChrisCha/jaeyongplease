#include "widget.h"
#include "./ui_widget.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    mainc();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Widget::~Widget()
{
    delete ui;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//메인 화면
void Widget::mainc(){
    exit->setCheckable(true);
    MakeAccount->setCheckable(true);
    resize(400,400);
    setWindowTitle("VEDA Bank");

    //아이콘
    logIn->setIcon(QIcon(":/resource/loginin.png"));
    MakeAccount->setIcon(QIcon(":/resource/create.png"));
    exit->setIcon(QIcon(":/resource/exit.png"));
    QObject::connect(logIn,&QPushButton::clicked,[=](){ login(); });
    QObject::connect(MakeAccount, &QPushButton::clicked,this,[=](bool flag){createAcc();});
    QObject::connect(exit, &QPushButton::clicked,this, [&](bool flag){QApplication::quit();});
    //사이즈 조정
    logIn->setMaximumSize(400,100);
    MakeAccount->setMaximumSize(400,100);
    exit->setMaximumSize(400,100);
    //QHBoxLayout
    vBoxLayout->setSpacing(0);
    vBoxLayout->addWidget(logIn);
    vBoxLayout->addWidget(MakeAccount);
    vBoxLayout->addWidget(exit);
    setLayout(vBoxLayout);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//로그인
void Widget::login() {
    QDialog* loginDial=new QDialog();
    QStringList labels;
    labels << "&아이디" << "&비밀번호";
    QLineEdit* ql[2];
    QFormLayout *qf = new QFormLayout;
    for (int i = 0; i < 2; i++) {
        ql[i] = new QLineEdit(this);
        qf->addRow(labels.at(i), ql[i]);
    }
    //로그인 창 버튼
    connect(loginButton, &QPushButton::clicked, this, &Widget::onLoginAttempt);
    qf->addRow(loginButton);
    ql[1]->setEchoMode(QLineEdit::Password);
    qg->setLayout(qf);
    //아이디 비밀번호
    this->idInput = ql[0];
    this->passwordInput = ql[1];
    loginLayout->setSpacing(0);
    loginLayout->addWidget(qg);
    loginLayout->addWidget(loginButton);
    loginDial->setWindowTitle("로그인");
    loginDial->setLayout(loginLayout);
    loginDial->exec();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//로그인 시도 파일에서 아이디 비밀번호 가져오기
void Widget::onLoginAttempt() {
    QString enteredID = idInput->text();
    QString enteredPassword = passwordInput->text();
    QFile file("C:/Users/DREAM07/Desktop/Qt/VEDAproject/list.txt");
    if (!file.exists()) {
        QMessageBox::warning(this, "Error", "파일이 없습니다!", QMessageBox::Ok);
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "파일을 열수 없습니다!", QMessageBox::Ok);
        return;
    }
    //확인해서 같으면 로그인 성공
    QTextStream in(&file);
    QString fileID, filePassword;
    bool loginSuccess = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("/");
        fileID = parts.at(0).trimmed();
        filePassword = parts.at(1).trimmed();
        if (enteredID == fileID && enteredPassword == filePassword) {
            loginSuccess = true;
            break;
        }
    }
    if (loginSuccess) {
        QMessageBox::information(this, "Login", "로그인에 성공했습니다!", QMessageBox::Ok);
        Welcome();
    } else {
        QMessageBox::warning(this, "Login", "아이디 or 비밀번호를 확인하세요.", QMessageBox::Ok);
    }
    file.close();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//계좌 저장하기
void Widget::saveAccount(const QString& id, const QString& password) {
    QFile file("C:/Users/DREAM07/Desktop/Qt/VEDAproject/list.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "파일을 열수 없습니다!", QMessageBox::Ok);
    }
    //random 변수로 계좌 생성
    int v1 = QRandomGenerator::global()->bounded(10, 99);
    int v2 = QRandomGenerator::global()->bounded(100000,999999);
    int v3 = QRandomGenerator::global()->bounded(100, 999);
    //파일에 저장
    QTextStream out(&file);
    out << id << "/" << password << "/"<< v1 << "-" << v2 << "-" << v3 << "/"<< "0" << "\n";
    file.close();
    QMessageBox::information(this, "Success", "계좌를 생성했습니다!", QMessageBox::Ok);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//계좌 생성 아이디 및 비밀번호
void Widget::createAcc() {
    QDialog *createAccount = new QDialog(this);
    createAccount->setWindowTitle("Create Account");

    QFormLayout *form = new QFormLayout(createAccount);
    QLineEdit *idInput = new QLineEdit(createAccount);
    QLineEdit *passwordInput = new QLineEdit(createAccount);
    passwordInput->setEchoMode(QLineEdit::Password);
    //계좌 생성 창
    QPushButton *save = new QPushButton("Save", createAccount);
    QPushButton *cancel = new QPushButton("Cancel", createAccount);
    form->addRow("ID:", idInput);
    form->addRow("Password:", passwordInput);
    form->addWidget(save);
    form->addWidget(cancel);
    connect(save, &QPushButton::clicked, [=]() {
        QString id = idInput->text();
        QString password = passwordInput->text();
        //아이디 중복 확인
        if (idcheck(id)) return;
        else{
            saveAccount(id, password);
            createAccount->accept();
        }
    });
    connect(cancel, &QPushButton::clicked, [=]() {createAccount->reject();});
    createAccount->exec();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//로그인 성공시
void Widget::Welcome()
{
    QDialog* welcomeDial = new QDialog();
    QString userId = idInput->text();
    welcomeDial->setWindowTitle(userId + "님 환영합니다!");
    //로그인 후 창 (입출력 및 조회)
    QObject::connect(inAout, &QPushButton::clicked,welcomeDial, [&](bool flag){InOut();});
    QObject::connect(check, &QPushButton::clicked,welcomeDial, [&](bool flag){cumoney();});
    welcomeLayout->setSpacing(0);
    welcomeLayout->addWidget(inAout);
    welcomeLayout->addWidget(check);
    inAout->setMaximumSize(400,100);
    check->setMaximumSize(400,100);
    inAout->setIcon(QIcon(":/resource/deposit.png"));
    check->setIcon(QIcon(":/resource/check.png"));
    welcomeDial->resize(250,200);
    welcomeDial-> setLayout( welcomeLayout);
    welcomeDial->exec();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//입출력
void Widget::InOut()
{
    QDialog*inoutDial=new QDialog();
    QObject::connect(in, &QPushButton::clicked,inoutDial, [&](bool flag){
        bool ok;
        moneyin();
    });
    QObject::connect(out, &QPushButton::clicked, inoutDial, [&](bool flag){
        bool ok;
        moneyout();
    });
    inoutDial->resize(250,200);
    inAoutLayout->setSpacing(0);
    inAoutLayout->addWidget(in);
    inAoutLayout->addWidget(out);
    in->setMaximumSize(400,100);
    out->setMaximumSize(400,100);
    in->setIcon(QIcon(":/resource/in.png"));
    out->setIcon(QIcon(":/resource/withdrawl.png"));
    inoutDial->setWindowTitle("업무");
    inoutDial->setLayout(inAoutLayout);
    inoutDial->exec();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//계좌 업데이트
void Widget::updateaccount(const QString &id, const QString &newmoney) {
    QFile file("C:/Users/DREAM07/Desktop/Qt/VEDAproject/list.txt");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "파일을 열수 없습니다!", QMessageBox::Ok);
        return;
    }
    QTextStream in(&file);
    QString content;
    QStringList parts;
    //돈 입출력 후 저장
    bool found = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        parts = line.split("/");
        if (parts.at(0) == id) {
            parts[3] = newmoney;
            line = parts.join("/");
            found = true;
        }
        content.append(line + "\n");
    }
    if (!found) {
        QMessageBox::warning(this, "Error", "계좌를 찾을수 없습니다.", QMessageBox::Ok);
        return;
    }
    file.resize(0);
    QTextStream out(&file);
    out << content;
    file.close();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//출금
void Widget::moneyout() {
    QDialog *money = new QDialog(this);
    money->setWindowTitle("출 금");
    QFormLayout *form = new QFormLayout(money);
    QLineEdit *outmoney = new QLineEdit(money);
    QLineEdit *passwordInput = new QLineEdit(money);
    passwordInput->setEchoMode(QLineEdit::Password);
    QPushButton *save = new QPushButton("출금", money);
    QPushButton *cancel = new QPushButton("취소", money);
    form->addRow("출금하실 금액을 적으시오:", outmoney);
    form->addRow("비밀번호:", passwordInput);
    form->addWidget(save);
    form->addWidget(cancel);
    //비밀번호 확인 후 출력
    QObject::connect(save, &QPushButton::clicked, [=]() {
        QString enteredpw = passwordInput->text();
        QString enteredamount = outmoney->text();
        havemoney();
        if (enteredpw != this->passwordInput->text()) {
            QMessageBox::warning(this, "Error", "비밀번호를 확인하세요.", QMessageBox::Ok);
            return;
        }
        int currentcumoney = this->savemoney.toInt();
        int outamount = enteredamount.toInt();
        //계좌에 돈 업을시
        if (outamount > currentcumoney) {
            QMessageBox::warning(this, "Error", "계좌에 돈이 충분하지 않습니다.", QMessageBox::Ok);
            return;
        }
        int newcumoney = currentcumoney - outamount;
        updateaccount(this->idInput->text(), QString::number(newcumoney));
        //출금 진행 상황 표시
        QProgressDialog pd("출금 중..","취소",0,100,this);
        pd.setWindowModality(Qt::WindowModal);
        pd.setAutoClose(false);
        QTimer* timer = new QTimer(this);
        int n=0;
        QObject::connect(timer,&QTimer::timeout,[&]{
            pd.setValue(++n);
            if(n>pd.maximum())
            {
                pd.close();
                timer->stop();
                delete timer;
                QMessageBox::information(this,"출금","완료");
            }
        });
        timer->start(10);
        pd.exec();
        QMessageBox::information(this, "Success", "금액 " + QString::number(outamount) + "원을 출금했습니다.\n\n남은 잔액은 " + QString::number(newcumoney) + "원 입니다.", QMessageBox::Ok);
        money->accept();
    });
    QObject::connect(cancel, &QPushButton::clicked, money, &QDialog::reject);
    money->exec();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//입금
void Widget::moneyin() {
    QDialog *money = new QDialog(this);
    money->setWindowTitle("입 금");
    QFormLayout *form = new QFormLayout(money);
    QLineEdit *inmoney = new QLineEdit(money);
    QLineEdit *passwordInput = new QLineEdit(money);
    passwordInput->setEchoMode(QLineEdit::Password);
    QPushButton *save = new QPushButton("입금", money);
    QPushButton *cancel = new QPushButton("취소", money);
    form->addRow("입금하실 금액을 적으시오:", inmoney);
    form->addRow("비밀번호:", passwordInput);
    form->addWidget(save);
    form->addWidget(cancel);
    QObject::connect(save, &QPushButton::clicked, [=]() {
        QString enteredpw = passwordInput->text();
        QString enteredamount = inmoney->text();
        havemoney();
        if (enteredpw != this->passwordInput->text()) {
            QMessageBox::warning(this, "Error", "비밀번호를 확인하세요.", QMessageBox::Ok);
            return;
        }
        int currentcumoney = this->savemoney.toInt();
        int inamount = enteredamount.toInt();
        int newcumoney = currentcumoney + inamount;
        updateaccount(this->idInput->text(), QString::number(newcumoney));
        QProgressDialog pd("입금 중..","취소",0,100,this);
        pd.setWindowModality(Qt::WindowModal);
        pd.setAutoClose(false);
        //입금 진행상황 표시
        QTimer* timer = new QTimer(this);
        int n=0;
        QObject::connect(timer,&QTimer::timeout,[&]{
            pd.setValue(++n);
            if(n>pd.maximum())
            {
                pd.close();
                timer->stop();
                delete timer;
                QMessageBox::information(this,"입금","완료");
            }
        });
        timer->start(10);
        pd.exec();
        QMessageBox::information(this, "Success", "금액 " + QString::number(inamount) + "원을 입금했습니다.\n\n현재 금액은 " + QString::number(newcumoney) + "원 입니다.", QMessageBox::Ok);
        money->accept();
    });
    QObject::connect(cancel, &QPushButton::clicked, money, &QDialog::reject);
    money->exec();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//현재 있는 돈 확인 (파일에서 가져오기)
void Widget::havemoney() {
    QFile filem("C:/Users/DREAM07/Desktop/Qt/VEDAproject/list.txt");
    if (!filem.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "파일을 열수 없습니다!", QMessageBox::Ok);
        return;
    }
    QTextStream in(&filem);
    QString line;
    QStringList parts;
    while (!in.atEnd()) {
        line = in.readLine();
        parts = line.split("/");
        if (parts.at(0) == this->idInput->text()) {
            this->savemoney = parts.at(3).trimmed();
            this->accountLine = line;
            break;
        }
    }
    filem.close();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//잔액조회
void Widget::cumoney() {
    havemoney();
    QMessageBox::information(this, "잔액 조회", "현재 계좌의 금액: " + savemoney, QMessageBox::Ok);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//아이디 중복 확인
bool Widget::idcheck(const QString& id) {
    QFile file("C:/Users/DREAM07/Desktop/Qt/VEDAproject/list.txt");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split('/');
        if (id == parts[0]) {
            QMessageBox::warning(this, "Error", "이미 사용 중인 ID입니다!", QMessageBox::Ok);
            return true;
            file.close();
        }
    }
    file.close();
    return false;
}
