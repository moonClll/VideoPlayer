

#ifndef SKTOP_LITE_REVIEWPLAYER_H
#define SKTOP_LITE_REVIEWPLAYER_H

#include <functional>
#include <iostream>
#include <string>

#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <gst/pbutils/pbutils.h>

using std::function;
using std::string;

class ReviewPlayer {
public:
    ReviewPlayer();
    ~ReviewPlayer();

    void pipeline_initial(string uri);
    void pipeline_destroy();

    void register_eos_callback(function<void(void)> fn);

    GstElement* get_playbin();

    int get_total_seconds();
    int get_current_seconds();

    void set_volume(float vol);
    void set_play(bool play);
    void set_speed(float speed);
    void set_current_seconds(int sec);

private:
    string     cmd;

    GstElement  *playbin;
    GstBus      *bus;

    function<void(void)> eos_handler;

    static void bus_eos_handler(GstElement* object,GstPad* pad,ReviewPlayer* player);
};


#endif //SKTOP2_REVIEWPLAYER_H
