#ifndef SKTOP_LITE_REVIEWPANEL_H
#define SKTOP_LITE_REVIEWPANEL_H

#include <glib.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <QWidget>
#include <QListWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QFileInfoList>
#include <vector>
#include "vwidget/VWidget.h"
#include "lib/ReviewPlayer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ReviewPanel; }
QT_END_NAMESPACE

class ReviewPanel : public QWidget {
Q_OBJECT

public:
    explicit ReviewPanel(QWidget *parent = nullptr);

    ~ReviewPanel() override;

public slots:
    void channel_file_selected(QListWidgetItem *item);
    void update_channel_files();

    void on_slPlay_sliderPressed();
    void on_slPlay_sliderReleased();
    void on_slPlay_sliderMoved(int value);

    void on_btmLast_clicked();
    void on_btmNext_clicked();
    void on_btmFast_clicked();
    void on_btmSlow_clicked();
    void on_btmPlay_clicked(bool checked);

    void on_btmMute_clicked(bool checked);
    void on_slVolume_valueChanged(int value);

    void on_vwidget_doubleClicked();
    void fullscreen_vwidget_doubleClicked();

private:
    Ui::ReviewPanel *ui;
    ReviewPlayer                reviewplayer;
    std::shared_ptr<VWidget>    fs_vwidget_p;

    int                         current_ch_index;
    int                         current_file_index;
    QString                     current_file_name;
    QString                     current_file_uri;
    std::vector<QFileInfoList>  file_infos;
    std::vector<QListWidget*>   file_list_widgets;

    GstElement  *playbin;

    int         volume;
    gdouble     play_speed;
    bool        isFullscreen;

    bool        pending_play_speed_update;

    int         media_update_timerid;

    void update_files();

    void update_media_totaltime();
    void update_media_currenttime();

    void on_eos_handler();

    void timerEvent(QTimerEvent* event) override;

protected:
    void showEvent(QShowEvent *event) override;
};


#endif //SKTOP2_REVIEWPANEL_H
