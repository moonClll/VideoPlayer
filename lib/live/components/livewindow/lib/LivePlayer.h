#ifndef SKTOP_LITE_LIVEPLAYER_H
#define SKTOP_LITE_LIVEPLAYER_H

#include <functional>
#include <iostream>
#include <string>

#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <gst/pbutils/pbutils.h>

using std::string;

class LivePlayer {
public:
    LivePlayer();
    ~LivePlayer();

    void initial(const string uri);
    void register_uri_found_callback(std::function<void()> fn);
    void register_uri_not_found_callback(std::function<void()> fn);
    void register_pipeline_err_callback(std::function<void()> fn);

    void pipeline_initial(float vol);
    void pipeline_destroy();
    void pipeline_recover();

    GstElement* get_videosink();
    void set_volume(float vol);
    void set_play(bool play);

private:
    std::function<void()> uri_found_handler;
    std::function<void()> uri_not_found_handler;
    std::function<void()> pipeline_err_handler;

    string              uri, cmd;

    GstElement          *pipeline;
    GstElement          *videosink;
    GstElement          *audiosink;
    GstElement          *videovalve, *audiovalve;
    GstBus              *bus;
    GstDiscoverer       *discoverer;

    static void uri_discovered_handler(GstDiscoverer *discoverer, GstDiscovererInfo *info,
                                       GError *err, LivePlayer* player);
    static void bus_err_handler(GstElement* object, GstPad* pad, LivePlayer* player);
};


#endif //SKTOP2_LIVEPLAYER_H
