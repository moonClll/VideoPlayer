// You may need to build the project (run Qt uic code generator) to get "ui_LiveWindow.h" resolved

#include "LiveWindow.h"
#include "ui_LiveWindow.h"

LiveWindow::LiveWindow(QWidget *parent, const QString uri) :
        QMainWindow(parent), ui(new Ui::LiveWindow){
    ui->setupUi(this);
    this->uri=uri;
    pre_widget_p=std::make_shared<VWidget>();

    QSizePolicy policy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    pre_widget_p->setSizePolicy(policy);

    connect(pre_widget_p.get(), SIGNAL(doubleClicked()),
            this,SLOT(preview_vwidget_doubleClicked()));

    current_state=EMBEDDED;

    loss_timer.setSingleShot(true);
    connect(&loss_timer,SIGNAL(timeout()),this,SLOT(screen_loss_handler()));

    volume=0;
    videosink= nullptr;

    liveplayer.register_uri_found_callback(std::bind(&LiveWindow::on_uri_found,this));
    liveplayer.register_uri_not_found_callback(std::bind(&LiveWindow::on_uri_not_found,this));
    liveplayer.register_pipeline_err_callback(std::bind(&LiveWindow::on_pipeline_error,this));

    liveplayer.initial(uri.toStdString());
}

LiveWindow::~LiveWindow() {
    delete ui;
}

VWidget* LiveWindow::get_preview_vwidget(){
    return pre_widget_p.get();
}

QString LiveWindow::get_uri(){
    return uri;
}

void LiveWindow::show_preview_vwidget(bool show){
    if(show)
        pre_widget_p->show();
    else
        pre_widget_p->hide();
}

void LiveWindow::screen_loss(){
    switch (current_state) {
        case EMBEDDED:
            pre_widget_p->show_loss();
            pre_widget_p->setUpdatesEnabled(true);
            break;

        case DISCRETE:
            ui->vwidget->show_loss();
            ui->vwidget->setUpdatesEnabled(true);
            break;

        case FULLSCREEN:
            fs_widget_p->show_loss();
            fs_widget_p->setUpdatesEnabled(true);
            break;

        default:
            qDebug()<<"LiveWindow in unknow state\n";
            return;
    }
}

void LiveWindow::screen_normal(){
    switch (current_state) {
        case EMBEDDED:
            pre_widget_p->show_normal();
            break;

        case DISCRETE:
            ui->vwidget->show_normal();
            break;

        case FULLSCREEN:
            fs_widget_p->show_normal();
            break;

        default:
            qDebug()<<"LiveWindow in unknow state\n";
            return;
    }
}

void LiveWindow::screen_loss_handler(){
    screen_loss();
}

void LiveWindow::on_btmPlay_clicked(bool checked){
    liveplayer.set_play(checked);
}

void LiveWindow::on_btmMute_clicked(bool checked){
    ui->slVolume->setEnabled(!checked);
    if(checked){
        volume=ui->slVolume->value();
        ui->slVolume->setValue(0);
    }
    else
        ui->slVolume->setValue(volume);
}

void LiveWindow::on_slVolume_valueChanged(int value){
    ui->lbVolume->setText(QString::number(value));
    liveplayer.set_volume(value/100.0f);
}

void LiveWindow::on_vwidget_doubleClicked(){
    current_state=FULLSCREEN;
    fs_widget_p=std::make_shared<VWidget>();
    if(videosink){
        QTimer::singleShot(10,this,[&](){
            fs_widget_p->overlay(videosink);
        });
        fs_widget_p->setUpdatesEnabled(false);
        ui->vwidget->setUpdatesEnabled(true);
    }

    connect(fs_widget_p.get(), SIGNAL(doubleClicked()),
            this, SLOT(fullscreen_vwidget_doubleClicked()));

    hide();
    fs_widget_p->showFullScreen();
}

void LiveWindow::preview_vwidget_doubleClicked(){
    current_state=DISCRETE;
    if(videosink) {
        ui->vwidget->overlay(videosink);
        ui->vwidget->setUpdatesEnabled(false);
        pre_widget_p->setUpdatesEnabled(true);
    }
    else{

    }
    show();
}

void LiveWindow::fullscreen_vwidget_doubleClicked(){
    current_state=DISCRETE;
    if(videosink){
        ui->vwidget->overlay(videosink);
        ui->vwidget->setUpdatesEnabled(false);
    }
    fs_widget_p->close();
    show();
}

void LiveWindow::on_action2160P_triggered(){
    resize(RES_2160P+RES_BIAS);
}

void LiveWindow::on_action1440P_triggered(){
    resize(RES_1440P+RES_BIAS);
}

void LiveWindow::on_action1080P_triggered(){
    resize(RES_1080P+RES_BIAS);
}

void LiveWindow::on_action720P_triggered(){
    resize(RES_720P+RES_BIAS);
}

void LiveWindow::on_action480P_triggered(){
    resize(RES_480P+RES_BIAS);
}

void LiveWindow::on_action360P_triggered(){
    resize(RES_360P+RES_BIAS);
}

void LiveWindow::on_action240P_triggered(){
    resize(RES_240P+RES_BIAS);
}

void LiveWindow::on_uri_found(){
    loss_timer.stop();
    screen_normal();
    liveplayer.pipeline_initial(ui->slVolume->value()/100.0f);
    videosink=liveplayer.get_videosink();

    switch(current_state){
        case EMBEDDED:
            ui->vwidget->overlay(videosink);
            if(!ui->btmMute->isChecked()){
                ui->btmMute->setChecked(true);
                on_btmMute_clicked(true);
            }
            QTimer::singleShot(10,this,[&](){
                pre_widget_p->overlay(videosink);
            });
            pre_widget_p->setUpdatesEnabled(false);
            break;

        case DISCRETE:
            pre_widget_p->setUpdatesEnabled(true);
            ui->vwidget->setUpdatesEnabled(false);
            ui->vwidget->overlay(videosink);
            break;

        case FULLSCREEN:
            ui->vwidget->setUpdatesEnabled(true);
            fs_widget_p->setUpdatesEnabled(false);
            fs_widget_p->overlay(videosink);
            break;

        default: qDebug()<<"LiveWindow in unknow state\n";return;
    }
}

void LiveWindow::on_uri_not_found(){
    QTimer::singleShot(500,this,[&](){
        liveplayer.pipeline_recover();
    });
}

void LiveWindow::on_pipeline_error(){
    loss_timer.start(5000);
    liveplayer.pipeline_destroy();
    QTimer::singleShot(500,this,[&](){
        liveplayer.pipeline_recover();
    });
}

void LiveWindow::closeEvent(QCloseEvent *event){
    event->ignore();
    current_state=EMBEDDED;
    if(videosink) {
        pre_widget_p->overlay(videosink);
        ui->vwidget->setUpdatesEnabled(true);
        pre_widget_p->setUpdatesEnabled(false);
    }
    hide();
}
