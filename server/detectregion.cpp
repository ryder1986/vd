#include "detectregion.h"
#include "c4processor.h"
#ifdef WITH_CUDA
#include "pvdprocessor.h"
#include "fvdprocessor.h"
#include "mvdprocessor.h"
#endif


DetectRegion::DetectRegion(DetectRegionInputData pkt):VdData(pkt),p(NULL)
{
    lock.lock();
    int valid=false;
    //  p=new PvdMvncProcessor();
    if(private_data.SelectedProcessor==LABLE_PROCESSOR_C4)
    {    p=new PvdC4Processor(pkt.data());valid=true;}
    if(private_data.SelectedProcessor==LABLE_PROCESSOR_DUMMY)
        //{   p=new DummyProcessor(private_data.ProcessorData);valid=true;}
    {   p=new DummyProcessor(private_data.ProcessorData);valid=true;}

#ifdef WITH_CUDA
    if(private_data.SelectedProcessor==LABLE_PROCESSOR_PVD)
    {   p=new PvdProcessor(private_data.ProcessorData);valid=true;}
    if(private_data.SelectedProcessor==LABLE_PROCESSOR_FVD)
    {   p=new FvdProcessor(private_data.ProcessorData);valid=true;}
    if(private_data.SelectedProcessor==LABLE_PROCESSOR_MVD)
    {   p=new MvdProcessor(private_data.ProcessorData);valid=true;}
#endif
    if(!valid){
        prt(info,"processor %s error ,exit",private_data.SelectedProcessor.data());
        //  exit(0);
    }
    detect_rect=reshape_2_rect(private_data.ExpectedAreaVers);

    lock.unlock();
}

DetectRegionOutputData DetectRegion::work(Mat frame)
{
    lock.lock();
    JsonPacket rst_r;
    valid_rect(detect_rect,frame.cols,frame.rows);
    if(detect_rect.width%2)detect_rect.width--;
    if(detect_rect.height%2)detect_rect.height--;
    Mat tmp=frame(detect_rect);
    if(p &&detect_rect.x>=0&&detect_rect.x<10000
            &&detect_rect.y>=0&&detect_rect.y<10000
            &&detect_rect.width>0&&detect_rect.width<10000
            &&detect_rect.height>0&&detect_rect.height<10000
            &&frame.cols>0&&frame.rows>0
            ){
#if 1
        p->process(tmp,rst_r);
#else
        p->process_whole_pic(frame,rst_r,detect_rect);
#endif
    }else{
        prt(info,"err arg");
    }
    //  p->process(frame,rst_r);
    VdRect r(detect_rect.x,detect_rect.y,detect_rect.width,detect_rect.height);
    JsonPacket dct_rct=r.data();
    DetectRegionOutputData rst(rst_r,dct_rct);
    lock.unlock();
    return rst;
}

void DetectRegion::modify(RequestPkt pkt)
{
    lock.lock();
    int op=pkt.Operation;
    switch(op){
    case OP::CHANGE_RECT:
    {
        AreaVersJsonDataRequest vs(pkt.Argument);
        detect_rect=reshape_2_rect(vs.ExpectedAreaVers);
        private_data.set_points(vs.ExpectedAreaVers);
        break;
    }
    case OP::CHANGE_PROCESSOR:
    {
        if(p){
            delete p;
            p=NULL;
        }
        ProcessorDataJsonDataRequest sp(pkt.Argument);
        string pro=sp.SelectedProcessor;
        //string pro=    pkt.Argument.get("SelectedProcessor").to_string();
        if(pro==LABLE_PROCESSOR_C4){
            p=new PvdC4Processor(sp.ProcessorData);
            private_data.set_processor(pro,sp.ProcessorData);
        }
        if(pro==LABLE_PROCESSOR_DUMMY){
            p=new DummyProcessor(sp.ProcessorData);
            private_data.set_processor(pro,sp.ProcessorData);
        }
#ifdef WITH_CUDA
        if(pro==LABLE_PROCESSOR_PVD)
        {
            p=new PvdProcessor(sp.ProcessorData);
            private_data.set_processor(pro,sp.ProcessorData);
        }
        if(pro==LABLE_PROCESSOR_FVD)
        {
            p=new FvdProcessor(sp.ProcessorData);
            private_data.set_processor(pro,sp.ProcessorData);
        }
        if(pro==LABLE_PROCESSOR_MVD)
        {
            p=new MvdProcessor(sp.ProcessorData);
            private_data.set_processor(pro,sp.ProcessorData);
        }

#endif
        break;
    }
    case OP::MODIFY_PROCESSOR:
    {
        p-> modify_processor(pkt.Argument);
        private_data.set_processor_data(pkt.Argument);//TODO:fetch data from processor
        break;
    }

defalut:break;
    }
    lock.unlock();
}
