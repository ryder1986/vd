#ifndef DETECTREGION_DATA_H
#define DETECTREGION_DATA_H
#include "tool.h"
#include "jsonpacket.h"
#include "videoprocessor.h"

class DetectRegionInputData:public JsonData,public DataEvent
{

public:
    enum ChangeType{
        RegionData=1,
        RegionShape
    };
    string SelectedProcessor;
    vector <VdPoint>ExpectedAreaVers;
    JsonPacket ProcessorData;//TODO: dynamic binding
    DataEvent *p_e;
    DetectRegionInputData(JsonPacket pkt):JsonData(pkt)
    {
        p_e=NULL;
        decode();
        if(p_e==NULL);
        if(SelectedProcessor==LABLE_PROCESSOR_C4)
        {
            C4ProcessorInputData *p=  new C4ProcessorInputData();
            p_e=(DataEvent*)p;
        }
    }
    DetectRegionInputData(JsonPacket pdata,string type,vector<VdPoint> vers)
        :ProcessorData(pdata),SelectedProcessor(type),ExpectedAreaVers(vers)
    {
        p_e=NULL;
        encode();
    }

    void set_point(VdPoint p,int index)
    {
        ExpectedAreaVers[index-1]=p;
        encode();
    }

    void set_points(vector <VdPoint> p)
    {
        ExpectedAreaVers=p;
        encode();
    }

    void set_processor_data(JsonPacket p)
    {
        ProcessorData=p;
        encode();
    }

    void set_processor(string p,JsonPacket pd)
    {
        SelectedProcessor=p;
        ProcessorData=pd;
        encode();
    }
    void operator =(DetectRegionInputData dt)
    {
        config=dt.config;
        decode();
    }
    void decode()
    {
        DECODE_JSONDATA_ARRAY_MEM(ExpectedAreaVers);
        DECODE_PKT_MEM(ProcessorData);
        DECODE_STRING_MEM(SelectedProcessor);
    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(ExpectedAreaVers);
        ENCODE_STRING_MEM(SelectedProcessor);
        ENCODE_PKT_MEM(ProcessorData);
    }
    inline int p_on_v(const vector <VdPoint> points,VdPoint p,int distance=20)
    {
        for(int i=0;i<points.size();i++){
            if(abs(points[i].x-p.x)<distance&&(abs(points[i].y-p.y))<distance){
                return (i+1);
            }
        }
        return 0;
    }
    inline bool p_on_l(VdPoint b,VdPoint e, VdPoint dst)
    {
        bool v1= (((dst.x<b.x+10)||(dst.x<e.x+10))&&((dst.x>b.x-10)||(dst.x>e.x-10)));
        bool v2=(  ((dst.y<b.y+10)||(dst.y<e.y+10))&&((dst.y>b.y-10)||(dst.y>e.y-10)));
        bool v3= (abs(((dst.x-e.x)*(dst.y-b.y))-((dst.y-e.y)*(dst.x-b.x)))<1000);
        if(v1&&v2&&v3)
            return true;
        else
            return false;
    }

    inline bool p_on_ver(VdPoint pnt)
    {
        focus_index=2;
        if((focus_index=p_on_v(ExpectedAreaVers,pnt))>0)
            return true;
        else
            return false;
    }

    inline bool p_on_line(VdPoint pnt)
    {
        int i;
        bool ret;
        for( i=1;i<ExpectedAreaVers.size();i++){
            ret=p_on_l(ExpectedAreaVers[i-1],ExpectedAreaVers[i],pnt);
            if(ret)
                return ret;
        }
        ret=p_on_l(ExpectedAreaVers[0],ExpectedAreaVers[i-1],pnt);
        return ret;
    }

    virtual bool start_event(VdPoint pnt)
    {
        if(p_on_ver(pnt)){
            triggered=true;
            ori_pnt=pnt;
            flag=VerTriggered;
            return true;
        }
        if(p_on_line(pnt)){
            triggered=true;
            ori_pnt=pnt;
            flag=VersTriggered;
            return true;
        }
        if(SelectedProcessor==LABLE_PROCESSOR_C4) {
            C4ProcessorInputData *pp=(C4ProcessorInputData*)p_e;
              pp->set_pkt( ProcessorData);
            if(pp->start_event(pnt)){
                return true;
//                ProcessorData=c4data.data();
//                encode();
            }
        }
        return false;
    }
    virtual bool process_event(VdPoint pnt)
    {

        if(triggered){
            switch (flag) {
            case Event::VerTriggered:
                ExpectedAreaVers[focus_index-1]=pnt;
                break;
            case Event::VersTriggered:
            {
                int offx=pnt.x-ori_pnt.x;
                int offy=pnt.y-ori_pnt.y;
                ori_pnt=pnt;
                int i=0;
                int sz=ExpectedAreaVers.size();
                for(i=0;i<sz;i++){
                    ExpectedAreaVers[i]=VdPoint(ExpectedAreaVers[i].x+offx,ExpectedAreaVers[i].y+offy);
                }
                break;
            }
            default:
                break;
            }
            encode();
            change_type=RegionShape;
            return true;

        }
        if(SelectedProcessor==LABLE_PROCESSOR_C4) {
            C4ProcessorInputData *pp=(C4ProcessorInputData*)p_e;
              pp->set_pkt( ProcessorData);
            if(pp->process_event(pnt)){
                ProcessorData=pp->get_pkt();
                encode();
                return true;
//                ProcessorData=c4data.data();

            }
        }
        return false;

    }
    void end_event()
    {
        if(SelectedProcessor==LABLE_PROCESSOR_C4) {
            C4ProcessorInputData *pp=(C4ProcessorInputData*)p_e;
              pp->set_pkt( ProcessorData);
          pp->end_event();


        }
        triggered=false;
    }
    void draw(function <void(VdPoint start,VdPoint end,int colour,int size)>drawline_callback)
    {
        if(SelectedProcessor==LABLE_PROCESSOR_C4) {

            C4ProcessorInputData *pp=(C4ProcessorInputData*)p_e;
              pp->set_pkt( ProcessorData);
pp->draw(drawline_callback);
        }
        int i;
        for( i=1;i<ExpectedAreaVers.size();i++){
            if(!triggered)
                drawline_callback(ExpectedAreaVers[i-1],ExpectedAreaVers[i],DataEvent::Colour::Blue,3);
            else
                drawline_callback(ExpectedAreaVers[i-1],ExpectedAreaVers[i],DataEvent::Colour::Red,3);
        }
        if(!triggered)
            drawline_callback(ExpectedAreaVers[0],ExpectedAreaVers[i-1],DataEvent::Colour::Blue,3);
        else
            drawline_callback(ExpectedAreaVers[0],ExpectedAreaVers[i-1],DataEvent::Colour::Red,3);

    }

};
class ProcessorDataJsonDataRequest:public JsonData{
public:
    string SelectedProcessor;
    JsonPacket ProcessorData;
    ProcessorDataJsonDataRequest(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    ProcessorDataJsonDataRequest(string pro,JsonPacket d)
    {
        ProcessorData=d;
        SelectedProcessor=pro;
        encode();
    }
    void decode()
    {
        DECODE_STRING_MEM(SelectedProcessor);
        DECODE_PKT_MEM(ProcessorData);
    }
    void encode()
    {
        ENCODE_STRING_MEM(SelectedProcessor);
        ENCODE_PKT_MEM(ProcessorData);
    }
};
class AreaVersJsonDataRequest:public JsonData{
public:
    vector <VdPoint>ExpectedAreaVers;
    AreaVersJsonDataRequest(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    AreaVersJsonDataRequest(vector <VdPoint> vs)
    {
        ExpectedAreaVers.assign(vs.begin(),vs.end());
        encode();
    }
    void decode()
    {
        DECODE_JSONDATA_ARRAY_MEM(ExpectedAreaVers);
    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(ExpectedAreaVers);
    }
};
class DetectRegionOutputData:public JsonData
{
public:
    VdRect DetectionRect;
    JsonPacket Result;
    DetectRegionOutputData(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }

    DetectRegionOutputData(JsonPacket rst ,VdRect rct)
    {
        DetectionRect=rct;
        Result=rst;
        encode();
    }

    void decode()
    {
        DECODE_PKT_MEM(DetectionRect);
        DECODE_PKT_MEM(Result);
    }

    void encode()
    {
        ENCODE_JSONDATA_MEM(DetectionRect);
        ENCODE_PKT_MEM(Result);
    }

};


#endif // DETECTREGION_DATA_H
