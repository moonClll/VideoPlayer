
// You may need to build the project (run Qt uic code generator) to get "ui_ReviewPanel.h" resolved

#include <QDir>
#include "ReviewPanel.h"
#include "ui_ReviewPanel.h"

ReviewPanel::ReviewPanel(QWidget *parent) :
        QWidget(parent), ui(new Ui::ReviewPanel),file_infos(4),file_list_widgets(4) {
    ui->setupUi(this);

    isFullscreen=false;

    play_speed=1.0;
    pending_play_speed_update=false;

    current_file_index=-1;
    current_ch_index=-1;

    file_list_widgets[0]=ui->lwCH0;
    file_list_widgets[1]=ui->lwCH1;
    file_list_widgets[2]=ui->lwCH2;
    file_list_widgets[3]=ui->lwCH3;

    connect(ui->lwCH0,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(channel_file_selected(QListWidgetItem *)));
    connect(ui->lwCH1,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(channel_file_selected(QListWidgetItem *)));
    connect(ui->lwCH2,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(channel_file_selected(QListWidgetItem *)));
    connect(ui->lwCH3,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(channel_file_selected(QListWidgetItem *)));

    update_files();
    reviewplayer.register_eos_callback(std::bind(&ReviewPanel::on_eos_handler,this));
}

ReviewPanel::~ReviewPanel() {
    delete ui;
}

void ReviewPanel::update_files(){
    for(int idx=0;idx<4;idx++){
        QDir dir(QDir::currentPath()+QString("/video%1").arg(idx));
        dir.setFilter(QDir::Files | QDir::Readable);

        QStringList filterlist;
        filterlist<<"*.mp4";
        dir.setNameFilters(filterlist);
        dir.setSorting(QDir::Time|QDir::Reversed);

        file_infos[idx].clear();
        file_infos[idx]=dir.entryInfoList();

        file_list_widgets[idx]->clear();
        for(auto info : file_infos[idx])
            file_list_widgets[idx]->addItem(info.lastModified().toString("MM.dd   hh:mm:ss"));
    }
}

void ReviewPanel::update_media_totaltime(){
    QTimer::singleShot(500,this,[&](){
        int total_sec=reviewplayer.get_total_seconds();
        ui->slPlay->setMaximum(total_sec);
        ui->lbTotal->setText(QString("%1:%2").arg(total_sec/60).arg(total_sec%60));
    });
}

void ReviewPanel::update_media_currenttime(){
    int current_sec=reviewplayer.get_current_seconds();
    ui->slPlay->setValue(current_sec);
    ui->lbCurrent->setText(QString("%1:%2").arg(current_sec/60).arg(current_sec%60));
}

void ReviewPanel::channel_file_selected(QListWidgetItem *item){
    if(current_ch_index>=0 && current_ch_index !=ui->twChannel->currentIndex())
        file_list_widgets[current_ch_index]->setCurrentRow(-1);
    current_ch_index=ui->twChannel->currentIndex();
    current_file_index=file_list_widgets[current_ch_index]->row(item);
    current_file_name=file_infos[current_ch_index][current_file_index].fileName();
    current_file_uri=file_infos[current_ch_index][current_file_index].filePath();

    play_speed=1.0f;
    ui->btmPlay->setChecked(true);
    reviewplayer.pipeline_initial(current_file_uri.toStdString());
    playbin=reviewplayer.get_playbin();
    if(isFullscreen){
        fs_vwidget_p->setUpdatesEnabled(false);
        fs_vwidget_p->overlay(playbin);
    }
    else{
        ui->vwidget->setUpdatesEnabled(false);
        ui->vwidget->overlay(playbin);
    }
    update_media_totaltime();
    media_update_timerid= startTimer(1000);
}

void ReviewPanel::update_channel_files(){
    for(int ch=0;ch<4;ch++) {
        QDir dir(QDir::currentPath() + QString("/video%1").arg(ch));
        dir.setFilter(QDir::Files | QDir::Readable);

        QStringList filterlist;
        filterlist << "*.mp4";
        dir.setNameFilters(filterlist);
        dir.setSorting(QDir::Time | QDir::Reversed);

        file_infos[ch].clear();
        file_infos[ch] = dir.entryInfoList();

        file_list_widgets[ch]->clear();
        for (auto info: file_infos[ch])
            file_list_widgets[ch]->addItem(info.lastModified().toString("MM.dd   hh:mm:ss"));

        bool current_file_deleted=true;
        if (ch == current_ch_index) {
            for (int idx = 0; idx < file_infos[ch].count(); idx++) {
                if (file_infos[ch][idx].fileName() == current_file_name) {
                    current_file_index = idx;
                    file_list_widgets[current_ch_index]->setCurrentRow(current_file_index);
                    current_file_deleted=false;
                    break;
                }
            }

            if(!current_file_deleted)
                continue;

            killTimer(media_update_timerid);

            current_file_index = 0;
            current_file_name = file_infos[current_ch_index][current_file_index].fileName();
            current_file_uri = file_infos[current_ch_index][current_file_index].filePath();
            file_list_widgets[current_ch_index]->setCurrentRow(current_file_index);

            reviewplayer.pipeline_initial(current_file_uri.toStdString());
            playbin=reviewplayer.get_playbin();
            if(isFullscreen){
                fs_vwidget_p->setUpdatesEnabled(false);
                fs_vwidget_p->overlay(playbin);
            }
            else{
                ui->vwidget->setUpdatesEnabled(false);
                ui->vwidget->overlay(playbin);
            }
            update_media_totaltime();
            media_update_timerid = startTimer(1000);
        }
    }
}

void ReviewPanel::on_slPlay_sliderPressed(){
    reviewplayer.set_play(false);
    killTimer(media_update_timerid);
}

void ReviewPanel::on_slPlay_sliderReleased(){
    reviewplayer.set_play(true);
    reviewplayer.set_speed(play_speed);
    media_update_timerid= startTimer(1000);
}

void ReviewPanel::on_slPlay_sliderMoved(int value){
    ui->lbCurrent->setText(QString("%1:%2").arg(value/60).arg(value%60));
    int current_time=ui->slPlay->value();
    reviewplayer.set_current_seconds(current_time);
}

void ReviewPanel::on_btmLast_clicked(){
    if(current_file_index==0)
        return;
    killTimer(media_update_timerid);

    current_file_index--;
    current_file_name=file_infos[current_ch_index][current_file_index].fileName();
    current_file_uri=file_infos[current_ch_index][current_file_index].filePath();
    file_list_widgets[current_ch_index]->setCurrentRow(current_file_index);

    play_speed=1.0f;
    ui->btmPlay->setChecked(true);
    reviewplayer.pipeline_initial(current_file_uri.toStdString());
    playbin=reviewplayer.get_playbin();
    if(isFullscreen){
        fs_vwidget_p->setUpdatesEnabled(false);
        fs_vwidget_p->overlay(playbin);
    }
    else{
        ui->vwidget->setUpdatesEnabled(false);
        ui->vwidget->overlay(playbin);
    }
    update_media_totaltime();
    media_update_timerid=startTimer(1000);
}

void ReviewPanel::on_btmNext_clicked(){
    if((current_file_index+1)==file_infos[current_ch_index].count())
        return;
    killTimer(media_update_timerid);

    current_file_index++;
    current_file_name=file_infos[current_ch_index][current_file_index].fileName();
    current_file_uri=file_infos[current_ch_index][current_file_index].filePath();
    file_list_widgets[current_ch_index]->setCurrentRow(current_file_index);

    play_speed=1.0f;
    ui->btmPlay->setChecked(true);
    reviewplayer.pipeline_initial(current_file_uri.toStdString());
    playbin=reviewplayer.get_playbin();
    if(isFullscreen){
        fs_vwidget_p->setUpdatesEnabled(false);
        fs_vwidget_p->overlay(playbin);
    }
    else{
        ui->vwidget->setUpdatesEnabled(false);
        ui->vwidget->overlay(playbin);
    }
    update_media_totaltime();
    media_update_timerid=startTimer(1000);
}

void ReviewPanel::on_btmFast_clicked(){
    play_speed=play_speed*2.0;
    if(ui->btmPlay->isChecked() && !pending_play_speed_update){
        pending_play_speed_update=true;
        QTimer::singleShot(50,this,[&](){
            reviewplayer.set_speed(play_speed);
            pending_play_speed_update=false;
        });
    }

}

void ReviewPanel::on_btmSlow_clicked(){
    play_speed=play_speed/2.0;
    if(ui->btmPlay->isChecked() && !pending_play_speed_update){
        pending_play_speed_update=true;
        QTimer::singleShot(50,this,[&](){
            reviewplayer.set_speed(play_speed);
            pending_play_speed_update=false;
        });
    }
}

void ReviewPanel::on_btmPlay_clicked(bool checked){
    if(playbin== nullptr){
        reviewplayer.pipeline_initial(current_file_uri.toStdString());
        playbin=reviewplayer.get_playbin();
        if(isFullscreen){
            fs_vwidget_p->setUpdatesEnabled(false);
            fs_vwidget_p->overlay(playbin);
        }
        else{
            ui->vwidget->setUpdatesEnabled(false);
            ui->vwidget->overlay(playbin);
        }
        update_media_totaltime();
        media_update_timerid=startTimer(1000);
        reviewplayer.set_speed(play_speed);
    }
    else{
        reviewplayer.set_play(checked);
    }
}

void ReviewPanel::on_btmMute_clicked(bool checked){
    ui->slVolume->setEnabled(!checked);
    if(checked){
        volume=ui->slVolume->value();
        ui->slVolume->setValue(0);
    }
    else
        ui->slVolume->setValue(volume);
}

void ReviewPanel::on_slVolume_valueChanged(int value){
    ui->lbVolume->setText(QString::number(value));
    reviewplayer.set_volume(value/100.0f);
}

void ReviewPanel::on_vwidget_doubleClicked(){
    isFullscreen=true;

    fs_vwidget_p=std::make_shared<VWidget>();
    if(playbin) {
        QTimer::singleShot(10,this,[&](){
            fs_vwidget_p->overlay(playbin);
        });
        connect(fs_vwidget_p.get(), SIGNAL(doubleClicked()), this, SLOT(fullscreen_vwidget_doubleClicked()));
        fs_vwidget_p->setUpdatesEnabled(false);
        ui->vwidget->setUpdatesEnabled(true);
    }
    fs_vwidget_p->showFullScreen();
}

void ReviewPanel::fullscreen_vwidget_doubleClicked(){
    isFullscreen=false;
    if(playbin) {
        ui->vwidget->overlay(playbin);
        ui->vwidget->setUpdatesEnabled(false);
    }
    fs_vwidget_p->close();
}

void ReviewPanel::on_eos_handler(){
    killTimer(media_update_timerid);
    if(!ui->btmAuto->isChecked() || (current_file_index+1)==file_infos[current_ch_index].count()){
        reviewplayer.pipeline_destroy();
        playbin= nullptr;
        if(isFullscreen){
            fs_vwidget_p->setUpdatesEnabled(true);
        }
        else{
            ui->vwidget->setUpdatesEnabled(true);
        }
        ui->slPlay->setValue(0);
        ui->lbCurrent->setText(QString("%1:%2").arg(0).arg(0));
        ui->btmPlay->setChecked(false);
        return;
    }

    (current_file_index)++;
    current_file_name=file_infos[current_ch_index][current_file_index].fileName();
    current_file_uri=file_infos[current_ch_index][current_file_index].filePath();
    file_list_widgets[current_ch_index]->setCurrentRow(current_file_index);

    play_speed=1.0f;
    reviewplayer.pipeline_initial(current_file_uri.toStdString());
    playbin=reviewplayer.get_playbin();
    if(isFullscreen){
        fs_vwidget_p->setUpdatesEnabled(false);
        fs_vwidget_p->overlay(playbin);
    }
    else{
        ui->vwidget->setUpdatesEnabled(false);
        ui->vwidget->overlay(playbin);
    }
    update_media_totaltime();
    media_update_timerid=startTimer(1000);
}

void ReviewPanel::timerEvent(QTimerEvent* event){
    int timerid=event->timerId();
    if(timerid==media_update_timerid)
        update_media_currenttime();
}


void ReviewPanel::showEvent(QShowEvent *event){
    update_files();
    QWidget::showEvent(event);
}

