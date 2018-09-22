#ifndef CAMERA_H
#define CAMERA_H
#include "tool.h"
#include "videosource.h"
#include "detectregion.h"
#include "camera_data.h"
class Camera:public VdData<CameraInputData>
{
public:
    enum OP{
        CHANGE_URL=1,
        INSERT_REGION,
        DELETE_REGION,
        MODIFY_REGION
    };
    function <void(Camera *,CameraOutputData)>callback_result;
public:
    Camera(CameraInputData cfg,function <void(Camera *,CameraOutputData)>fc):VdData(cfg),quit(false),callback_result(fc)
    {
        for(DetectRegionInputData p:private_data.DetectRegion){
            drs.push_back(new DetectRegion(p));
        }
        prt(info,"start %s",private_data.Url.data());
        src=new VideoSource(private_data.Url);
        start();
    }
    ~Camera()
    {
        prt(info,"exiting %s",private_data.Url.data());
        quit=true;
        if(work_trd->joinable())
            work_trd->join();
        prt(info,"exiting %s",private_data.Url.data());
        delete work_trd;
        for(DetectRegion *pro:drs)
            delete pro;
        drs.clear();
        prt(info,"exited %s",private_data.Url.data());
        delete src;
    }

    void run_process();

    void start()
    {
        //   _start(bind(&TestProcess::run_process,this));
        //  _start(bind(&TestProcess::run_process,this,placeholders::_1),99);
        //_start_async(bind(&Camera::run_process,this));
        work_trd=new thread(bind(&Camera::run_process,this));
        prt(info,"start done ~~~~~~~~~~~~~~~");
    }

    void change_source(string url)
    {
        if(src)
            delete src;
        src=new VideoSource(url);
    }
    void modify_detect_region(JsonPacket pkt)
    {
        int index=pkt.get("index").to_int();
    }
    int get_frame_ts()
    {
        return timestamp;
    }
    bool modify(RequestPkt req);

private:
    vector<DetectRegion*> drs;
    VideoSource *src;
    bool quit;
    thread *work_trd;
    int timestamp;
    mutex lock;
};



#endif // CAMERA_H
