
// You may need to build the project (run Qt uic code generator) to get "ui_MainPanel.h" resolved

#include "MainPanel.h"
#include "ui_MainPanel.h"


MainPanel::MainPanel(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainPanel) {
    ui->setupUi(this);
}

MainPanel::~MainPanel() {
    delete ui;
}

void MainPanel::on_actionImport_triggered(){
    QMessageBox mbox(QMessageBox::Warning,"WARNING",
                     "导入配置会清除现有对拉流窗口的一切设置，确定导入？",
                     QMessageBox::Ok|QMessageBox::Cancel);
    if(mbox.exec()==QMessageBox::Cancel)
        return;

    ui->tabWidget->setCurrentIndex(0);

    QString file_name;
    file_name=QFileDialog::getOpenFileName(this,"打开配置文件",
                                           "/home","YAML File(*.yaml)");
    if(file_name.isEmpty()){
        QMessageBox::critical(this,"ERROR","Invalid file path");
        return;
    }

    std::vector<QString> uris;
    try{
        YAML::Node nodes=YAML::LoadFile(file_name.toStdString());
        for(auto node : nodes){
            uris.push_back(QString::fromStdString(node.as<std::string>()));
        }
    }catch (YAML::Exception& e){
        QMessageBox::critical(this,"ERROR","无法导入配置");
        return;
    }
    ui->tabLive->import_uris(uris);
}

void MainPanel::on_actionExport_triggered(){
    QString file_name;
    file_name=QFileDialog::getSaveFileName(this,"配置文件保存",
                                           "/home","YAML File(*.yaml)");
    std::ofstream file;
    file.open(file_name.toStdString()+".yaml");
    if(!file.is_open())
        return;
    std::vector<QString> uris=ui->tabLive->export_uris();

    YAML::Node rootnode(YAML::NodeType::Sequence);
    for(auto uri : uris){
        rootnode.push_back(uri.toStdString());
    }
    file<<rootnode<<std::endl;
    file.close();

    QMessageBox::information(this,"Info","配置文件保存成功");
}
