#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    showMaximized();
    int MW_width= QApplication::desktop()->availableGeometry().right()+1;
    int MW_height= QApplication::desktop()->availableGeometry().bottom()+1;
    ui->gridLayoutWidget->setGeometry(5,5,MW_width-10,MW_height-70);
    ui->GV_main->setScene(new QGraphicsScene);

    connect(ui->PB_Open,SIGNAL(clicked()),this,SLOT(Open()));
    connect(ui->LW_images,SIGNAL(currentRowChanged(int)),this,SLOT(Load(int)));

}

Widget::~Widget()
{
    delete ui->GV_main->scene();
    delete ui;
}

void Widget::Open()
{
    ui->LW_images->clear();
    folder = QDir(QFileDialog::getExistingDirectory(this, tr("Папка с изображениями"),"C://MelaHunter//src//Images//Test"));
    folder.setNameFilters({"*.jpg","*.bmp"});
    ui->LW_images->addItems(folder.entryList());
}

void Widget::Load(int num)
{
    ui->GV_main->scene()->clear();
    ui->GV_main->scene()->addPixmap(folder.absoluteFilePath(folder[num]));
}
