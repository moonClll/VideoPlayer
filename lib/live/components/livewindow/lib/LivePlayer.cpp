#include "LivePlayer.h"

using std::to_string;
LivePlayer::LivePlayer() {
    pipeline= nullptr;
}

LivePlayer::~LivePlayer(){
    if(pipeline!= nullptr)
        pipeline_destroy();
    gst_discoverer_stop(discoverer);
}

void LivePlayer::initial(const string uri){
    discoverer= gst_discoverer_new(GST_SECOND,nullptr);
    g_signal_connect(discoverer,"discovered",G_CALLBACK(uri_discovered_handler),this);
    gst_discoverer_start(discoverer);

    this->uri=uri;
    gst_discoverer_discover_uri_async(discoverer,uri.c_str());
}

void LivePlayer::register_uri_found_callback(std::function<void()> fn){
    uri_found_handler=fn;
}

void LivePlayer::register_uri_not_found_callback(std::function<void()> fn){
    uri_not_found_handler=fn;
}

void LivePlayer::register_pipeline_err_callback(std::function<void()> fn) {
    pipeline_err_handler=fn;
}

void LivePlayer::pipeline_initial(float vol){
    cmd="rtspsrc location="+ uri + " tcp-timeout=1000000 name=src ! valve drop=false name=vvalve ! "
       "rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! xvimagesink sync=true name=xvsink "
       "src. ! valve drop=false name=avalve ! rtpmp4gdepay ! aacparse ! avdec_aac ! audioconvert ! "
       "pulsesink name=asink sync=true volume="+ to_string(vol);


//    cmd="rtspsrc location="+ uri + " tcp-timeout=1000000 name=src ! queue ! decodebin ! "
//       "valve drop=false name=vvalve ! videoconvert ! xvimagesink sync=true name=xvsink "
//       "src. ! queue ! decodebin ! valve drop=false name=avalve ! audioconvert ! pulsesink sync=true name=asink "
//       "volume="+to_string(vol);

//    cmd="rtspsrc location="+ uri + " tcp-timeout=1000000 ! valve drop=false name=vvalve ! "
//       "rtph264depay ! h264parse ! mppvideodec name=decoder ! xvimagesink sync=true name=xvsink ";

    pipeline= gst_parse_launch(cmd.c_str(),NULL);
    videosink=gst_bin_get_by_name(GST_BIN(pipeline),"xvsink");
    videovalve=gst_bin_get_by_name(GST_BIN(pipeline),"vvalve");
    audiovalve=gst_bin_get_by_name(GST_BIN(pipeline),"avalve");
    audiosink=gst_bin_get_by_name(GST_BIN(pipeline),"asink");

    bus= gst_element_get_bus(pipeline);
    gst_bus_add_signal_watch(bus);
    g_signal_connect(bus, "message::error", G_CALLBACK(bus_err_handler), this);

    gst_element_set_state (pipeline, GST_STATE_PLAYING);
}

void LivePlayer::pipeline_destroy() {
    gst_bus_remove_signal_watch(bus);
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    gst_object_unref(bus);
    pipeline= nullptr;
}

void LivePlayer::pipeline_recover(){
    gst_discoverer_discover_uri_async(discoverer,uri.c_str());
}

GstElement* LivePlayer::get_videosink(){
    return videosink;
}

void LivePlayer::set_volume(float vol){
    g_object_set(G_OBJECT(audiosink),"volume",vol,NULL);
}

void LivePlayer::set_play(bool play){
    if(play){
        g_object_set(G_OBJECT(videovalve),"drop",false,NULL);
        g_object_set(G_OBJECT(audiovalve),"drop",false,NULL);
    }
    else {
        g_object_set(G_OBJECT(videovalve), "drop", true, NULL);
        g_object_set(G_OBJECT(audiovalve), "drop", true, NULL);
    }
}

void LivePlayer::bus_err_handler(GstElement* object,GstPad* pad,LivePlayer* player){
    player->pipeline_err_handler();
}

void LivePlayer::uri_discovered_handler(GstDiscoverer *discoverer,GstDiscovererInfo *info,
                                        GError *err,LivePlayer* player){
    GstDiscovererResult         res;
    res= gst_discoverer_info_get_result(info);
    switch(res){
        case GST_DISCOVERER_OK:
            player->uri_found_handler();
            break;

        case GST_DISCOVERER_ERROR:
        case GST_DISCOVERER_TIMEOUT:
        case GST_DISCOVERER_URI_INVALID:
            player->uri_not_found_handler();
            break;
        default:
            std::cout<<"Unknow DIS STATE"<<std::endl;
    }

}
