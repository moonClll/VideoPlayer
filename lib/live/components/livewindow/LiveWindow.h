#ifndef SKTOP_LITE_LIVEWINDOW_H
#define SKTOP_LITE_LIVEWINDOW_H

#include <thread>
#include <QMainWindow>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTimer>

#include "lib/LivePlayer.h"
#include "vwidget/VWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LiveWindow; }
QT_END_NAMESPACE

class LiveWindow : public QMainWindow {
Q_OBJECT
    enum State{
        EMBEDDED = 0,
        DISCRETE = 1,
        FULLSCREEN = 2
    };

    static constexpr QSize RES_2160P=QSize(3840,2560);
    static constexpr QSize RES_1440P=QSize(2560,1440);
    static constexpr QSize RES_1080P=QSize(1920,1080);
    static constexpr QSize RES_720P=QSize(1280,720);
    static constexpr QSize RES_480P=QSize(720,480);
    static constexpr QSize RES_360P=QSize(480,360);
    static constexpr QSize RES_240P=QSize(360,240);

    static constexpr QSize RES_BIAS=QSize(4,80);

public:
    explicit LiveWindow(QWidget *parent, const QString uri);
    VWidget* get_preview_vwidget();
    QString get_uri();
    void show_preview_vwidget(bool show);

    ~LiveWindow() override;

public slots:
    void screen_loss_handler();

    void on_btmMute_clicked(bool checked);
    void on_slVolume_valueChanged(int value);
    void on_btmPlay_clicked(bool checked);

    void on_vwidget_doubleClicked();
    void preview_vwidget_doubleClicked();
    void fullscreen_vwidget_doubleClicked();

    void on_action2160P_triggered();
    void on_action1440P_triggered();
    void on_action1080P_triggered();
    void on_action720P_triggered();
    void on_action480P_triggered();
    void on_action360P_triggered();
    void on_action240P_triggered();

private:
    Ui::LiveWindow *ui;
    QString                   uri;

    LivePlayer                liveplayer;
    std::shared_ptr<VWidget>  pre_widget_p;
    std::shared_ptr<VWidget>  fs_widget_p;
    std::shared_ptr<std::thread> listen_thread_p;

    State               current_state;

    GstElement          *videosink;
    int                 volume;

    QTimer              loss_timer;

    void screen_loss();
    void screen_normal();

    void on_uri_found();
    void on_uri_not_found();
    void on_pipeline_error();

protected:
    void closeEvent(QCloseEvent *event) override;
};


#endif //SKTOP2_LIVEWINDOW_H
