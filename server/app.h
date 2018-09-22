﻿#ifndef APP_H
#define APP_H

#include "server.h"
#include "tool.h"
#include "camera.h"
#include "configmanager.h"
#include "app_data.h"

class DestClient{
public:

    DestClient(string i):ip(i)
    {

    }

    string get_ip()
    {
        return ip;
    }

private:
    string ip;
};

class App:public VdData<AppInputData>
{
public:
    //define operation for request pkt
//    enum Operation{
//        GET_CONFIG=1,
//        SET_CONFIG,
//        INSERT_CAMERA,
//        MODIFY_CAMERA,
//        DELETE_CAMERA,
//        OPEN_CAMERA_DATA,
//        CLOSE_CAMERA_DATA,
//        HEART_PKT,
//        REBOOT
//    };
    App(ConfigManager *p);
    ~App();

    void start()
    {
    }
private:
    bool process_event(RequestPkt e,ReplyPkt &r);
    void process_client_cmd(Session *clt,char *data,int len);
    void client_data_request(Session *clt, char *data, int len);
    void process_camera_data(Camera *clt,CameraOutputData);
    Camera* new_cam(CameraInputData data)
    {
        return new Camera(data,bind(&App::process_camera_data,
                                    this,placeholders::_1,
                                    placeholders::_2));
    }
    void restart_all()
    {
        stop_cams();
        start_cams();
    }
    void start_cams()
    {
        for(CameraInputData p:private_data.CameraData){
            cms.push_back(new_cam( p));
        }
    }
    void stop_cams()
    {
        for(Camera *c:cms){
            delete c;
        }
        cms.clear();
    }
    bool add_camera(int index,CameraInputData data)//after who ?  0~size
    {
        bool ret=false;
        if(0<=index&&index<=cms.size()){
            Camera *c=new_cam( (data));
            vector<Camera*>::iterator it=cms.begin();
            cms.insert(it+index,c);
            private_data.insert_camera(data,index);
            ret=true;
        }
        return ret;
    }
    bool del_camera(int index)//delete who ? 1~size
    {
        if(1<=index&&index<=cms.size()){
            delete cms[index-1];
            vector<Camera*>::iterator it=cms.begin();
            cms.erase(it+index-1);
            private_data.delete_camera(index);
            return true;
        }
        return false;
    }
    bool mod_camera(int index,RequestPkt pkt)//delete who ? 1~size
    {
        bool ret=false;
        if(1<=index&&index<=cms.size()){
            vector<Camera*>::iterator it=cms.begin();
            Camera *c=cms[index-1];
            if(c->modify(pkt)){
                private_data.modify_camera(c->get_data(),index);
                ret=true;
            }
        }
        return ret;
    }
private:
    vector <Session*> *stream_cmd;//clients who connected to our server
    string str_stream;//
    vector <Camera*> cms;
    LocationService lservice;
    ConfigManager *p_cm;
    vector <DestClient > dest_clients;
    string client_tmp_ip;
    int udp_fd;
};

#endif // APP_H
