#include "ReviewPlayer.h"

ReviewPlayer::ReviewPlayer() {
    playbin= nullptr;
    bus= nullptr;
}

ReviewPlayer::~ReviewPlayer() {
    if(playbin!= nullptr)
        pipeline_destroy();
}

void ReviewPlayer::pipeline_initial(string uri){
    pipeline_destroy();
    cmd="playbin uri=file://"+ uri +" video-sink=xvimagesink audio-sink=pulsesink";
    playbin= gst_parse_launch(cmd.c_str(), nullptr);
    bus= gst_element_get_bus(playbin);
    gst_bus_add_signal_watch(bus);
    g_signal_connect(bus, "message::eos", G_CALLBACK(bus_eos_handler), this);
    gst_element_set_state(playbin, GST_STATE_PLAYING);
}

void ReviewPlayer::pipeline_destroy(){
    if(bus!= nullptr){
        gst_bus_remove_signal_watch(bus);
        gst_object_unref(bus);
        bus= nullptr;
    }

    if(playbin != nullptr) {
        gst_element_set_state(playbin, GST_STATE_NULL);
        gst_object_unref(playbin);
        playbin= nullptr;
    }
}

void ReviewPlayer::register_eos_callback(function<void(void)> fn){
    eos_handler=fn;
}

GstElement* ReviewPlayer::get_playbin(){
    return playbin;
}

int ReviewPlayer::get_total_seconds(){
    gint64 duration;
    gst_element_query_duration(playbin,GST_FORMAT_TIME,&duration);
    return duration/GST_SECOND;
}

int ReviewPlayer::get_current_seconds(){
    gint64 current;
    if(!gst_element_query_position(playbin, GST_FORMAT_TIME, &current))
        return -1;
    return current/GST_SECOND;
}

void ReviewPlayer::set_volume(float vol){
    if(playbin== nullptr)
        return;
    g_object_set(G_OBJECT(playbin),"volume",vol, nullptr);
}

void ReviewPlayer::set_play(bool play){
    if(playbin== nullptr)
        return;
    if(play)
        gst_element_set_state(playbin, GST_STATE_PLAYING);
    else
        gst_element_set_state(playbin, GST_STATE_PAUSED);
}

void ReviewPlayer::set_speed(float speed){
    if(playbin== nullptr)
        return;
    gint64 pos;
    GstEvent *seek_event;

    gst_element_query_position(playbin,GST_FORMAT_TIME,&pos);
    seek_event=gst_event_new_seek(speed,GST_FORMAT_TIME,(GstSeekFlags)(GST_SEEK_FLAG_FLUSH|GST_SEEK_FLAG_ACCURATE),
                                  GST_SEEK_TYPE_SET,pos,GST_SEEK_TYPE_END,0);
    gst_element_send_event(playbin,seek_event);
}

void ReviewPlayer::set_current_seconds(int sec){
    if(playbin== nullptr)
        return;
    gst_element_seek_simple(playbin, GST_FORMAT_TIME,
                            (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT),
                            sec*GST_SECOND);
}

void ReviewPlayer::bus_eos_handler(GstElement* object,GstPad* pad,ReviewPlayer* player){
    player->eos_handler();
}
