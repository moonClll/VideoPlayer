// You may need to build the project (run Qt uic code generator) to get "ui_LivePanel.h" resolved

#include <QListWidgetItem>
#include "livewindow/LiveWindow.h"
#include "LivePanel.h"
#include "ui_LivePanel.h"

LivePanel::LivePanel(QWidget *parent) :
        QWidget(parent), ui(new Ui::LivePanel) {
    ui->setupUi(this);

    layout=new QGridLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    ui->vmdiarea->setLayout(layout);

    next_pos=QPoint(0,0);
    current_pos=QPoint(-1,-1);

    pending_vmdiarea_update=false;
}

LivePanel::~LivePanel() {
    delete ui;
}

void LivePanel::import_uris(std::vector<QString> uris){
    livewindow_list.clear();
    ui->lwChannel->clear();

    for(auto uri : uris){
        ui->lwChannel->addItem(new QListWidgetItem(uri));

        std::shared_ptr<LiveWindow> livewindow=std::make_shared<LiveWindow>(nullptr,uri);
        livewindow_list.push_back(livewindow);
        ui->vmdiarea->addSubWindow(livewindow->get_preview_vwidget(),Qt::FramelessWindowHint);
    }
    update_vmdiarea();
}

std::vector<QString> LivePanel::export_uris(){
    std::vector<QString> uris;
    for(auto livewindow : livewindow_list)
        uris.push_back(livewindow->get_uri());
    return uris;
}

void LivePanel::on_btmAdd_clicked(){
    if(livewindow_list.size()==16){
        QMessageBox::critical(this,"WARNING","Could not add more channels\n");
        return;
    }


    QString uri=QString("rtsp://%1").arg(ui->edtURI->text());
    ui->lwChannel->addItem(new QListWidgetItem(uri));

    std::shared_ptr<LiveWindow> livewindow=std::make_shared<LiveWindow>(nullptr,uri);
    livewindow_list.push_back(livewindow);
    ui->vmdiarea->addSubWindow(livewindow->get_preview_vwidget(),Qt::FramelessWindowHint);

    if(!pending_vmdiarea_update){
        pending_vmdiarea_update=true;
        QTimer::singleShot(50,this,[&](){
            update_vmdiarea();
            pending_vmdiarea_update=false;
        });
    }
}

void LivePanel::on_btmDel_clicked(){
    int idx=ui->lwChannel->currentRow();
    if(idx<0)
        return;
    delete ui->lwChannel->currentItem();
    ui->vmdiarea->removeSubWindow(livewindow_list[idx]->get_preview_vwidget());
    livewindow_list.erase(livewindow_list.begin()+idx);
    if(!pending_vmdiarea_update){
        pending_vmdiarea_update=true;
        QTimer::singleShot(50,this,[&](){
            update_vmdiarea();
            pending_vmdiarea_update=false;
        });
    }
}

void LivePanel::update_vmdiarea(){
    set_preview_widget_show(false);

    QLayoutItem *item;
    while(1){
        item=layout->itemAt(0);
        if(!item)
            break;
        layout->removeItem(item);
    }

    int window_num=livewindow_list.size();
    if(window_num==1){
        layout->addWidget(livewindow_list[0]->get_preview_vwidget(),0,0,1,1);
    }
    else if(window_num>1 && window_num<=4){
        for(int y=0;y<2;y++){
            for(int x=0;x<2;x++){
                if(x+2*y<window_num)
                    layout->addWidget(livewindow_list[x+2*y]->get_preview_vwidget(),y,x,1,1);
                else
                    goto Return_Point;
            }
        }
    }
    else if(window_num>4 && window_num<=9){
        for(int y=0;y<3;y++){
            for(int x=0;x<3;x++){
                if(x+3*y<window_num)
                    layout->addWidget(livewindow_list[x+3*y]->get_preview_vwidget(),y,x,1,1);
                else
                    goto Return_Point;
            }
        }
    }
    else if(window_num>9){
        for(int y=0;y<4;y++){
            for(int x=0;x<4;x++){
                if(x+4*y<window_num)
                    layout->addWidget(livewindow_list[x+4*y]->get_preview_vwidget(),y,x,1,1);
                else
                    goto Return_Point;
            }
        }
    }

Return_Point:
    set_preview_widget_show(true);
}

void LivePanel::set_preview_widget_show(bool show){
    for(auto & livewidget : livewindow_list){
        livewidget->show_preview_vwidget(show);
    }
}