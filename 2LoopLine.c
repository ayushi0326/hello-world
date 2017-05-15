#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define BSize32
#define BSize6464
#define RED 'R'
#define GREEN 'G'
#define YELLOW 'Y'
#define DURATION30
#define ON 1
#define OFF 0
#define FLASH2
char BlockInstrumentDKS='G',BlockInstrumentIKS='G';
char HomeDKS='R',AdvanceDKS='R',ShuntingSignalDKS='R',CrossingSignalDKS='o',StarterLL2DKS='R',StarterLL1DKS='R',StarterMainDKS='R',StarterMainIKS='R',StarterLL1IKS='R',StarterLL2IKS='R',ShuntingSignalIKS='R',CrossingSignalIKS='o',AdvanceIKS='R',HomeIKS='R';
char Relay8='f',Relay14='f',Relay8AT='f',Relay14AT='f',Relay2LAT='f',Relay2LBT='f',Relay1LAT='f',Relay1LBT='f',Relay0MAT='f',Relay0MBT='f',Relay1AT='f',Relay15T='f',Relay1T='f',Relay7='f',Relay7AT='f',Relay7BT='f',Relay13='f',Relay13AT='f',Relay13BT='f',Relay20T='f',Relay6T='f',Relay20AT='f';
char PointDKS7='N',PointIKS13='N',PointDKS8='N',PointIKS14='N',Point7L='N',Point8L='N',Point13L='N',Point14L='N';
int ch,ch1,ch2,ch3,ch4,ch5,StoppingDuration=0;
int HomeDKSArm1=0,HomeIKSArm1=0,HomeDKSArm2=0,HomeIKSArm2=0;
FILE *fTraffic;
FILE *fOutput;
char cmdbuf[1024] = "";
void SetSignal(int SignalNo, char Color, int SignalMode){
    if(SignalNo <= 9){
        sprintf(cmdbuf,"#TSIG:0%d:%c:%d",SignalNo,Color,SignalMode);
    } else {
        sprintf(cmdbuf,"#TSIG:%d:%c:%d",SignalNo,Color,SignalMode);
    }
    fputs(cmdbuf,fTraffic);
    fflush(fTraffic);
    sleep(1);
}
void PrintingSignalStatus(){
    printf("DKS<------------------------------------------------------------------------------------------------------->IKS\n");
    printf("                                           StrLL1DKS     StrLL1IKS\n");
    printf("                                              %c              %c\n",StarterLL1DKS,StarterLL1IKS);
    printf("HomeDKS   HomeArm1   HomeArm2    AdvDKS    StrMainDKS    StrMainIKS   AdvIKS   HomeArm2    HomeArm1   HomeIKS\n");
    printf("  %c          %d          %d           %c         %c              %c          %c          %d          %d          %c\n",HomeDKS,HomeDKSArm1,HomeDKSArm2,AdvanceDKS,StarterMainDKS,StarterMainIKS,AdvanceIKS,HomeIKSArm2,HomeIKSArm1,HomeIKS);
    printf("                                           StrLL2DKS     StrLL2IKS\n");
    printf("                                              %c              %c\n",StarterLL2DKS,StarterLL2IKS);
    printf("Point7    Point7L    Point8    Point8L    Point13    Point13L    Point14    Point14L\n");
    printf("%c          %c          %c          %c          %c          %c          %c          %c\n",PointDKS7,Point7L,PointDKS8,Point8L,PointIKS13,Point13L,PointIKS14,Point14L);
    printf("ShuntingSignalDKS:%c\nCrossingSignalDKS:%c\nShuntingSignalIKS:%c\nCrossingSignalIKS:%c\n",ShuntingSignalDKS,CrossingSignalDKS,ShuntingSignalIKS,CrossingSignalIKS);
}

void PrintingRelayStatus(){
    printf("LoopLine1<--------------------------------------------------------------------------------------------------->\n");
    printf("7          7AT          1LAT          1LBT          13AT          13\n");
    printf("%c          %c              %c           %c              %c            %c\n",Relay7,Relay7AT,Relay1LAT,Relay1LBT,Relay13AT,Relay13);
    printf("HomeDKS   HomeArm1   HomeArm2    AdvDKS    StrMainDKS    StrMainIKS   AdvIKS   HomeArm2    HomeArm1   HomeIKS\n");
    printf("%c            %c             %c           %c            %c             %c              %c            %c         %c          %c\n",Relay1AT,Relay15T,Relay1T,Relay7BT,Relay0MAT,Relay0MBT,Relay13BT,Relay20T,Relay6T,Relay20AT);
    printf("LoopLine2<--------------------------------------------------------------------------------------------------->\n");
    printf("8          8AT          2LAT          2LBT          14AT          14\n");
    printf("%c          %c              %c           %c              %c            %c\n",Relay8,Relay8AT,Relay2LAT,Relay2LBT,Relay14AT,Relay14);
    printf("Block Instrument for DKS:%c\n",BlockInstrumentDKS);
    printf("Block Instrument for IKS:%c\n",BlockInstrumentIKS);
    }

void AllRedOn(){
    int i=1;
    while(i<=10){
        SetSignal(i,RED,ON);
        i++;
    }
}

void GiveLineClearance(){//route set,give permission to receive train
    if(Relay0MAT=='o'&&Relay0MBT=='o'&&Relay1LAT=='o'&&Relay1LBT=='o'&&Relay2LAT=='o'&&Relay2LBT=='o'){
        printf("We cannot take the train right now,both loop lines and main line are occupied\n");
    }
    else{
        printf("Enter type of train:\nenter 1 for through train and 2 for stopping train\n");
        scanf("%d",&ch2);
        if(ch2==1&&Relay0MAT=='c'&&Relay0MBT=='c'){
            printf("Can't pass a through train,main line is committed\n");
        }
        else{
            if(Relay0MAT=='f'&&Relay0MBT=='f'){
            printf("Train is coming from which direction?\nenter 1 for DKS AND 2 FOR IKS\n");
            scanf("%d",&ch1);
            if(ch1==2&&ch2==1){//through train coming from IKS
                if(Relay1AT=='f'&&Relay15T=='f'&&Relay1T=='f'&&Relay7=='f'&&Relay7BT=='f'&&Relay0MAT=='f'&&Relay0MBT=='f'&&Relay13BT=='f'&&Relay13=='f'&&Relay20T=='f'&&Relay6T=='f'&&Relay20AT=='f'){
                    //mandatory conditions
                    SetSignal(1,RED,OFF);SetSignal(1,GREEN,ON);HomeIKS='G';
                    SetSignal(9,RED,OFF);SetSignal(9,GREEN,ON);AdvanceDKS='G';
                    SetSignal(3,RED,OFF);SetSignal(3,GREEN,ON);StarterMainDKS='G';
                    HomeDKS='R';
                    SetSignal(9,YELLOW,OFF);SetSignal(13,GREEN,OFF);SetSignal(13,RED,OFF);SetSignal(10,YELLOW,OFF);PointDKS7=PointIKS13=PointDKS8=PointIKS14='N';
                    CrossingSignalDKS=CrossingSignalIKS='c';
                    //safety conditions
                    StarterLL1DKS=StarterLL1IKS=StarterLL2DKS=StarterLL2IKS='R';
                    StarterMainIKS='R';
                    Relay1AT=Relay15T=Relay1T=Relay7BT=Relay0MAT=Relay0MBT=Relay13BT=Relay20T=Relay6T=Relay20AT='c';
                    SetSignal(11,GREEN,OFF);SetSignal(12,GREEN,OFF);SetSignal(11,YELLOW,ON);SetSignal(12,YELLOW,ON);BlockInstrumentDKS=BlockInstrumentIKS='Y';
                    printf("We give consent to pass a through train coming from IKS,we have set the signal and relay status to:\n");
                    PrintingSignalStatus();
                    PrintingRelayStatus();
                    sleep(1);
                    SetSignal(11,YELLOW,OFF);SetSignal(12,YELLOW,OFF);
        }
    }
            else{
                if(ch1==1&&ch2==1){//through train from DKS
                    if(Relay1AT=='f'&&Relay15T=='f'&&Relay1T=='f'&&Relay7=='f'&&Relay7BT=='f'&&Relay0MAT=='f'&&Relay0MBT=='f'&&Relay13BT=='f'&&Relay13=='f'&&Relay20T=='f'&&Relay6T=='f'&&Relay20AT=='f'){
                        //mandatory conditions
                        SetSignal(11,GREEN,OFF);SetSignal(12,GREEN,OFF);SetSignal(11,YELLOW,ON);SetSignal(12,YELLOW,ON);
                        SetSignal(2,RED,OFF);SetSignal(2,GREEN,ON);HomeDKS='G';
                        SetSignal(10,RED,OFF);SetSignal(10,GREEN,ON);AdvanceIKS='G';
                        SetSignal(4,RED,OFF);SetSignal(4,GREEN,ON);StarterMainIKS='G';
                        HomeIKS='R';
                        SetSignal(9,YELLOW,OFF);SetSignal(13,GREEN,OFF);SetSignal(13,RED,OFF);SetSignal(10,YELLOW,OFF);PointDKS7=PointIKS13=PointDKS8=PointIKS14='N';
                        CrossingSignalDKS=CrossingSignalIKS='c';
                        //safety conditions
                        StarterLL1DKS=StarterLL1IKS=StarterLL2DKS=StarterLL2IKS='R';
                        StarterMainDKS='R';
                        Relay1AT=Relay15T=Relay1T=Relay7BT=Relay0MAT=Relay0MBT=Relay13BT=Relay20T=Relay6T=Relay20AT='c';
                        BlockInstrumentDKS=BlockInstrumentIKS='Y';
                        printf("We give consent to pass a through train coming from DKS,we have set the signal and relay status to:\n");
                        PrintingSignalStatus();
                        PrintingRelayStatus();
                        sleep(1);
                    }
                }
            else{//stopping train
                if(ch1==2&&ch2==2){//stopping train from IKS
                        if(Relay2LAT=='f'&&Relay2LBT=='f'){//we take the train on loop line 2
                                //mandatory conditions
                                SetSignal(1,RED,OFF);SetSignal(1,YELLOW,ON);SetSignal(15,YELLOW,ON);HomeIKS='Y';HomeIKSArm2=1;
                                SetSignal(13,RED,ON);SetSignal(8,GREEN,ON);SetSignal(10,YELLOW,OFF);PointIKS14=Point14L='R';PointIKS13='N';
                                StarterLL2DKS='R';
                                SetSignal(14,YELLOW,OFF);HomeDKS='R';HomeDKSArm1=0;
                                CrossingSignalIKS='c';
                                Relay20AT=Relay20T=Relay6T=Relay14=Relay2LAT=Relay2LBT='c';
                                //safety conditions
                                StarterMainIKS='R';StarterLL1IKS='R';
                                SetSignal(13,YELLOW,OFF);SetSignal(9,YELLOW,OFF);HomeDKSArm2=0;PointDKS8='N';
                                SetSignal(12,GREEN,OFF);SetSignal(12,YELLOW,ON);BlockInstrumentIKS='Y';
                                printf("We give consent to take a train on loop line 2 from IKS,signal status is:\n");
                                PrintingSignalStatus();
                                PrintingRelayStatus();
                                sleep(1);
                                SetSignal(12,YELLOW,OFF);
                            }
                        else{
                            if(Relay1LAT=='f'&&Relay1LBT=='f'){//Loop 2 is busy but loop line 1 is free,we take the train on loop line 1
                                //mandatory conditions
                                SetSignal(1,RED,OFF);SetSignal(1,YELLOW,ON);SetSignal(16,YELLOW,ON);HomeIKS='Y';HomeIKSArm1=1;
                                SetSignal(10,RED,OFF);SetSignal(10,YELLOW,ON);SetSignal(6,GREEN,ON);PointIKS13=Point13L='R';
                                StarterLL1DKS='R';
                                CrossingSignalIKS='c';
                                Relay20T=Relay6T=Relay20AT=Relay13=Relay1LAT=Relay1LBT='c';
                                //safety conditions
                                SetSignal(13,GREEN,OFF);PointDKS7='N';
                                StarterLL2IKS='R';StarterMainIKS='R';
                                printf("Loop line 2 is busy,and loop line 1 is free. We give consent to take a train on loop line 1 from IKS,signal status is:\n");
                                SetSignal(12,YELLOW,ON);BlockInstrumentIKS='Y';
                                PrintingSignalStatus();
                                PrintingRelayStatus();
                                sleep(1);
                                SetSignal(1,YELLOW,OFF);SetSignal(16,YELLOW,OFF);
                                SetSignal(10,YELLOW,OFF);SetSignal(6,GREEN,OFF);
                                //safety conditions
                                SetSignal(13,GREEN,OFF);
                            }
                            else{
                                if(Relay0MAT=='f'&&Relay0MBT=='f'&&ch1==2){//both loop lines are busy but main line is free,train is coming from IKS
                                        //mandatory conditions
                                        SetSignal(1,RED,OFF);SetSignal(1,YELLOW,ON);SetSignal(16,YELLOW,OFF);SetSignal(15,YELLOW,OFF);HomeIKS='Y';HomeIKSArm1=HomeIKSArm2=0;
                                        StarterMainDKS='R';
                                        SetSignal(13,RED,OFF);SetSignal(10,YELLOW,OFF);PointIKS13=PointIKS14='N';
                                        Relay20T=Relay6T=Relay20AT=Relay0MAT=Relay0MBT='c';
                                        //safety conditions
                                        StarterLL2IKS='R';StarterLL1IKS='R';
                                        printf("Both loop lines are occupied,we give consent to take train on main line from IKS,Signal status is:\n");
                                        SetSignal(12,GREEN,OFF);SetSignal(12,YELLOW,ON);BlockInstrumentIKS='Y';
                                        PrintingSignalStatus();
                                        PrintingRelayStatus();
                                    }
                            }
                        }
                }
                else{
                    if(ch1==1&&ch2==2){//stopping train from DKS
                            if(Relay1LAT=='f'&&Relay1LBT=='f'){//loop line 1 is free
                                //mandatory conditions
                                StarterLL1IKS='R';
                                SetSignal(9,YELLOW,OFF);SetSignal(13,GREEN,ON);SetSignal(7,GREEN,ON);PointDKS8='N';PointDKS7=Point7L='R';
                                SetSignal(2,RED,OFF);SetSignal(2,YELLOW,ON);SetSignal(14,YELLOW,ON);HomeDKS='Y';HomeDKSArm1=1;
                                CrossingSignalDKS='c';
                                Relay1AT=Relay15T=Relay1T=Relay7=Relay1LAT=Relay1LBT='c';
                                //safety conditions
                                StarterLL2DKS=StarterMainDKS='R';
                                SetSignal(10,YELLOW,OFF);SetSignal(16,YELLOW,OFF);PointIKS13='N';HomeIKSArm1=0;
                                printf("We give consent to take a train on loop line 1 from DKS,signal status is:\n",ch1);
                                SetSignal(11,GREEN,OFF);SetSignal(11,YELLOW,ON);BlockInstrumentDKS='Y';
                                PrintingSignalStatus();
                                PrintingRelayStatus();
                                }
                            else{
                                    if(Relay2LAT=='f'&&Relay2LBT=='f'){//loop line 1 is busy,but loop line2 is free,we take the train on loop line 2
                                        //mandatory conditions
                                        StarterLL2IKS='R';
                                        SetSignal(9,YELLOW,ON);SetSignal(5,GREEN,ON);Point8L=PointDKS8='R';
                                        SetSignal(2,RED,OFF);SetSignal(2,YELLOW,ON);SetSignal(13,YELLOW,ON);HomeDKS='Y';HomeDKSArm2=1;
                                        CrossingSignalDKS='c';
                                        Relay1AT=Relay1T=Relay15T=Relay7=Relay2LAT=Relay2LBT='c';
                                        //safety conditions
                                        StarterLL1DKS='R';StarterMainDKS='R';
                                        SetSignal(13,RED,OFF);PointIKS14='N';
                                        SetSignal(15,YELLOW,OFF);HomeIKSArm2=0;
                                        printf("We give consent to take a train on loop line 2 from DKS,signal status is:\n");
                                        SetSignal(11,RED,OFF);SetSignal(11,YELLOW,ON);BlockInstrumentDKS='Y';
                                        PrintingSignalStatus();
                                        PrintingRelayStatus();
                                    }
                                    else{
                                        if(Relay0MAT=='f'&&Relay0MBT=='f'&&ch1==1&&ch2==2){//both lines are busy,but main line is free so we take the train on main line
                                            //mandatory conditions
                                            SetSignal(2,RED,OFF);SetSignal(2,YELLOW,ON);SetSignal(14,YELLOW,OFF);SetSignal(13,YELLOW,OFF);HomeDKS='Y';HomeDKSArm1=HomeDKSArm2=0;
                                            StarterMainIKS='R';
                                            SetSignal(9,YELLOW,OFF);SetSignal(13,GREEN,OFF);PointDKS7=PointDKS8='N';
                                            Relay1AT=Relay15T=Relay1T=Relay0MAT=Relay0MBT='c';
                                            //safety conditions
                                            StarterMainDKS='R';StarterLL1DKS='R';
                                            printf("We give consent to take train on main line from DKS,Signal status is:\n");
                                            SetSignal(11,YELLOW,ON);BlockInstrumentDKS='Y';
                                            PrintingSignalStatus();
                                            PrintingRelayStatus();
                                        }
                                    }
                            }
                    }
                }
            }
           }
           }
        }
    }
}


void ReceiveTrain(){//receiving train
    if(ch2==1&&ch1==1){//through train coming from DKS
        printf("\n\nThe train is arriving from DKS,now the signal and relay status is:\n");
        SetSignal(11,YELLOW,OFF);SetSignal(11,RED,ON);SetSignal(12,YELLOW,OFF);SetSignal(12,RED,ON);BlockInstrumentDKS=BlockInstrumentIKS='R';
SetSignal(15,GREEN,ON);SetSignal(15,RED,ON);
        Relay1AT=Relay15T=Relay1T=Relay7=Relay8=Relay7BT=Relay0MAT=Relay0MBT=Relay13BT=Relay13=Relay14=Relay20T=Relay6T=Relay20AT='o';
        PrintingSignalStatus();
        PrintingRelayStatus();
        printf("\n\nTrain has passed by,now the signal and relay status is:\n");
SetSignal(15,GREEN,OFF);SetSignal(15,RED,OFF);
SetSignal(9,GREEN,OFF);SetSignal(9,RED,ON);SetSignal(3,GREEN,OFF);SetSignal(3,RED,ON);SetSignal(1,GREEN,OFF);SetSignal(1,RED,ON);
        SetSignal(11,RED,OFF);SetSignal(11,GREEN,ON);SetSignal(12,RED,OFF);SetSignal(12,GREEN,ON);BlockInstrumentDKS=BlockInstrumentIKS='G';
        CrossingSignalDKS=CrossingSignalIKS='o';
        Relay1AT=Relay15T=Relay1T=Relay7=Relay8=Relay7BT=Relay0MAT=Relay0MBT=Relay13BT=Relay13=Relay14=Relay20T=Relay6T=Relay20AT='f';
        HomeIKS='R';HomeDKS='R';AdvanceDKS='R';AdvanceIKS='R';
        StarterMainDKS='R';StarterMainIKS='R';
        PrintingSignalStatus();
        PrintingRelayStatus();
        sleep(1);
        AllRedOn();
    }
    else{
if(ch2==1&&ch1==2){//through train from IKS
	printf("\n\nThe train is arriving from IKS,now the signal and relay status is:\n");
        SetSignal(12,YELLOW,OFF);SetSignal(12,RED,ON);SetSignal(11,YELLOW,OFF);SetSignal(11,RED,ON);BlockInstrumentDKS=BlockInstrumentIKS='R';
        SetSignal(15,RED,ON);SetSignal(15,GREEN,ON);
        Relay1AT=Relay15T=Relay1T=Relay7=Relay8=Relay7BT=Relay0MAT=Relay0MBT=Relay13BT=Relay13=Relay14=Relay20T=Relay6T=Relay20AT='o';
        PrintingSignalStatus();
        PrintingRelayStatus();
        printf("\n\nTrain has passed by,now the signal and relay status is:\n");
        SetSignal(15,RED,OFF);SetSignal(15,GREEN,OFF);
        SetSignal(10,GREEN,OFF);SetSignal(10,RED,ON);SetSignal(4,GREEN,OFF);SetSignal(4,RED,ON);SetSignal(2,GREEN,OFF);SetSignal(2,RED,ON);
        SetSignal(12,RED,OFF);SetSignal(12,GREEN,ON);SetSignal(11,RED,OFF);SetSignal(11,GREEN,ON);BlockInstrumentDKS=BlockInstrumentIKS='G';
        CrossingSignalDKS=CrossingSignalIKS='o';
        Relay1AT=Relay15T=Relay1T=Relay7=Relay8=Relay7BT=Relay0MAT=Relay0MBT=Relay13BT=Relay13=Relay14=Relay20T=Relay6T=Relay20AT='f';
        HomeIKS='R';HomeDKS='R';AdvanceDKS='R';AdvanceIKS='R';
        StarterMainDKS='R';StarterMainIKS='R';
        PrintingSignalStatus();
        PrintingRelayStatus();
        sleep(1);
        AllRedOn();
    }
else{
        if(ch1==2 &&ch2==2 &&Relay2LAT=='c'&&Relay2LBT=='c'){//it is a stopping train and loop line 2 is free and train is coming from IKS
            Relay20AT=Relay6T=Relay20T=Relay14=Relay2LAT=Relay2LBT='o';HomeIKSArm2=1;
            printf("\n\nTrain is arriving on loop line 2 from IKS,now status of signals and relays is:\n");
            SetSignal(12,YELLOW,OFF);SetSignal(12,RED,ON);BlockInstrumentIKS='R';
            PrintingSignalStatus();
            PrintingRelayStatus();
            sleep(1);
            SetSignal(1,YELLOW,OFF);SetSignal(1,RED,ON);
            SetSignal(13,YELLOW,OFF);SetSignal(9,YELLOW,OFF);
            printf("\n\nTrain is standing on loop line 2,status of signal and relay is:\n");
	SetSignal(14,GREEN,ON);SetSignal(14,RED,ON);
            SetSignal(12,RED,OFF);SetSignal(12,GREEN,ON);BlockInstrumentIKS='G';
            Relay20AT=Relay6T=Relay20T=Relay14='f';Relay2LAT=Relay2LBT='o';
            HomeIKS='R';
            SetSignal(15,YELLOW,OFF);HomeIKSArm2=0;
            SetSignal(8,GREEN,OFF);SetSignal(13,RED,OFF);PointIKS14=Point14L='N';
            CrossingSignalIKS='o';
            PrintingSignalStatus();
            PrintingRelayStatus();
            sleep(1);
            AllRedOn();
        }
        else{
            if(ch1==2 &&ch2==2 &&Relay1LAT=='c'&&Relay1LBT=='c'&&Relay2LAT=='o'&&Relay2LBT=='o'){//it is a stopping train is coming from IKS and loop line 2 is busy ,but loop line 1 is free
                Relay20AT=Relay6T=Relay20T=Relay13=Relay1LAT=Relay1LBT='o';HomeIKSArm1=1;
                printf("\n\nTrain is arriving on loop line 1 from IKS,now status of signals and relays is:\n");
                SetSignal(12,YELLOW,OFF);SetSignal(12,RED,ON);BlockInstrumentIKS='R';
                PrintingSignalStatus();
                PrintingRelayStatus();
                sleep(1);
                printf("\n\nTrain is standing on loop line 1,status of signal and relay is:\n");
		SetSignal(16,GREEN,ON);SetSignal(16,RED,ON);
                SetSignal(12,RED,OFF);SetSignal(12,GREEN,ON);BlockInstrumentIKS='G';
                SetSignal(1,RED,ON);HomeIKS='R';
                SetSignal(16,YELLOW,OFF);SetSignal(15,YELLOW,OFF);HomeDKSArm1=HomeIKSArm1=0;
                SetSignal(13,GREEN,OFF);SetSignal(10,YELLOW,OFF);SetSignal(6,GREEN,OFF);PointDKS7='N';PointIKS13=Point13L='N';
                SetSignal(7,RED,ON);StarterLL1DKS='R';
                CrossingSignalIKS='o';
                Relay20T=Relay6T=Relay20AT=Relay13='f';
                PrintingSignalStatus();
                PrintingRelayStatus();
                sleep(1);
                AllRedOn();
            }
            else{
                if(Relay1LAT=='o'&&Relay1LBT=='o'&&Relay2LAT=='o'&&Relay2LBT=='o'&&Relay0MAT=='c'&&Relay0MBT=='c'&&ch2==2&&ch1==2&&HomeDKS!='G'){//both loop line are busy we take the train on main line,it is a stopping train coming from IKS
                    Relay20AT=Relay6T=Relay20T=Relay13BT=Relay0MAT=Relay0MBT='o';
                    SetSignal(16,YELLOW,OFF);SetSignal(15,YELLOW,OFF);HomeIKSArm1=HomeIKSArm2=0;
                    printf("\n\nTrain is arriving on main line from IKS,now status of signals and relays is:\n");
                    SetSignal(12,YELLOW,OFF);SetSignal(12,RED,ON);BlockInstrumentIKS='R';
                    PrintingSignalStatus();
                    PrintingRelayStatus();
                    sleep(1);
                    SetSignal(1,YELLOW,OFF);SetSignal(1,RED,ON);
                    Relay20AT=Relay6T=Relay20T=Relay13BT='f';Relay0MAT=Relay0MBT='o';
                    HomeIKS='R';CrossingSignalIKS='o';
                    printf("\n\nTrain is standing on the main line,signal and relay status is:\n");
		SetSignal(15,GREEN,ON);SetSignal(15,RED,ON);
                    SetSignal(12,RED,OFF);SetSignal(12,GREEN,ON);BlockInstrumentIKS='G';
                    PrintingSignalStatus();
                    PrintingRelayStatus();
                    sleep(1);
                    AllRedOn();
                }
            }
        }
}

                if(ch1==1&&ch2==2&&Relay1LAT=='c'&&Relay1LBT=='c'){//stopping train coming from DKS,loop line 1 is free
                    Relay1AT=Relay15T=Relay1T=Relay7=Relay1LAT=Relay1LBT='o';HomeDKSArm1=1;
                    SetSignal(11,YELLOW,OFF);SetSignal(11,RED,ON);BlockInstrumentDKS='R';
                    printf("\n\nTrain is arriving on loop line 1 from DKS,now status of signals and relays is:\n");
                    PrintingSignalStatus();
                    PrintingRelayStatus();
                    printf("\n\nTrain is standing on loop line 1,status of signal and relay is:\n");
		SetSignal(16,GREEN,ON);SetSignal(16,RED,ON);
                    SetSignal(2,YELLOW,OFF);SetSignal(2,RED,ON);
                    SetSignal(14,YELLOW,OFF);HomeDKS='R';HomeDKSArm1=0;
                    SetSignal(16,YELLOW,OFF);HomeIKSArm1=0;
                    SetSignal(13,GREEN,OFF);SetSignal(7,GREEN,OFF);SetSignal(10,YELLOW,OFF);PointDKS7=Point7L='N';PointIKS13='N';
                    Relay1AT=Relay15T=Relay1T=Relay7='f';Relay1LAT=Relay1LBT='o';
                    StarterLL1IKS='R';
                    CrossingSignalDKS='o';
                    SetSignal(11,RED,OFF);SetSignal(11,GREEN,ON);BlockInstrumentDKS='G';
                    PrintingSignalStatus();
                    PrintingRelayStatus();
                    sleep(1);
                    AllRedOn();
                } else {
                    if(Relay2LAT=='c'&&Relay2LBT=='c'&&Relay1LAT=='o'&&Relay1LBT=='o'&&ch2==2&&ch1==1){//loop line1 is busy ,but loop line 2 is free so we take the train on loop line 2,it is a stopping train coming from DKS
                        Relay1AT=Relay15T=Relay1T=Relay8=Relay2LAT=Relay2LBT='o';HomeDKSArm2=1;HomeDKSArm2=1;
                        printf("\n\nTrain is arriving on loop line 2 from DKS,now status of signals and relays is:\n");
                        SetSignal(11,YELLOW,OFF);SetSignal(11,RED,ON);BlockInstrumentDKS='R';
                        PrintingSignalStatus();
                        PrintingRelayStatus();
                        SetSignal(2,YELLOW,OFF);SetSignal(2,RED,ON);SetSignal(13,YELLOW,OFF);
                        Relay1AT=Relay15T=Relay1T=Relay8='f';Relay2LAT=Relay2LBT='o';
                        SetSignal(13,YELLOW,OFF);HomeDKS='R';CrossingSignalDKS='o';HomeDKSArm2=0;
                        SetSignal(9,YELLOW,OFF);SetSignal(5,GREEN,OFF);SetSignal(13,RED,OFF);PointDKS8=Point8L=PointIKS14='N';
                        printf("\n\nTrain is standing on loop line 2,signal and relay status is:\n");
		SetSignal(14,GREEN,ON);SetSignal(14,RED,ON);
                        SetSignal(11,RED,OFF);SetSignal(11,GREEN,ON);BlockInstrumentDKS='G';
                        PrintingSignalStatus();
                        PrintingRelayStatus();
                        sleep(1);
                        AllRedOn();
                    } else {
                        if(Relay2LAT=='o'&&Relay2LBT=='o'&&Relay1LAT=='o'&&Relay1LBT=='o'&&Relay0MAT=='c'&&Relay0MBT=='c'&&ch2==2&&ch1==1&&HomeIKS!='G'){//both loop lines are busy,but main line is free,so we take train coming from DKS on main line
                            Relay1AT=Relay15T=Relay1T=Relay7BT=Relay0MAT=Relay0MBT='o';HomeIKSArm1=HomeIKSArm2=0;
                            printf("\n\nTrain is arriving on main line from DKS,now status of signals and relays is:\n");
                            SetSignal(11,YELLOW,OFF);SetSignal(11,RED,ON);BlockInstrumentDKS='R';
                            PrintingSignalStatus();
                            PrintingRelayStatus();
                            sleep(1);
                            SetSignal(2,YELLOW,OFF);SetSignal(2,RED,ON);HomeDKS='R';
                            Relay1AT=Relay15T=Relay1T='f';Relay7BT=Relay0MAT=Relay0MBT='o';
                            CrossingSignalDKS='o';
                            printf("\n\nTrain is standing on the main line,signal and relay status is:\n");
			SetSignal(15,GREEN,ON);SetSignal(15,RED,ON);
                            SetSignal(11,RED,OFF);SetSignal(11,GREEN,ON);BlockInstrumentDKS='G';
                            PrintingSignalStatus();
                            PrintingRelayStatus();
                            sleep(1);
                            AllRedOn();
                        }
                    }
                }
    }
}

void TakeLineClearance(){//asking permission to send the train
    printf("Train has to go in which direction?\nenter 1 for DKS and 2 for IKS\n");
    scanf("%d",&ch3);
        if(ch3==1){
            if(Relay1T!='c'&&Relay15T!='c'&&Relay1AT!='c'&&Relay7BT!='c'&&ShuntingSignalDKS=='R'){
                printf("Do we get a receiving consent from station at DKS?\nenter 1 for yes and 0 for no\n");
                scanf("%d",&ch5);
            }
        }
            else{
                if(ch3==2){
                    if(Relay20T!='c'&&Relay6T!='c'&&Relay20AT!='c'&&Relay13BT!='c'&&ShuntingSignalIKS=='R'){
                        printf("Do we get a receiving consent from station at IKS?\nenter 1 for yes and 0 for no\n");
                        scanf("%d",&ch5);
                    }
                }
            }
}

void ReceivingPermission(){//receiving permission ,set signal+set point for dispatch
    if(ch5==1){
        printf("Enter 1 if train is standing on loop line 1\nenter 2 if train is standing on main line\nenter 3 if train is standing on loop line 2\n");
        scanf("%d",&ch4);
        if(ch4==1&&ch3==1){//train is on the loop line 1 and it has to go to DKS
                SetSignal(11,GREEN,OFF);SetSignal(11,YELLOW,ON);BlockInstrumentDKS='Y';
                //mandatory conditions
                SetSignal(13,GREEN,ON);SetSignal(7,GREEN,ON);SetSignal(9,YELLOW,OFF);PointDKS7=Point7L='R';PointDKS8='N';
                SetSignal(9,RED,OFF);SetSignal(9,GREEN,ON);AdvanceDKS='G';
                SetSignal(7,RED,OFF);SetSignal(7,YELLOW,ON);StarterLL1DKS='Y';
                CrossingSignalDKS='c';
                //safety conditions
                SetSignal(14,YELLOW,OFF);SetSignal(13,YELLOW,OFF);HomeDKS='R';HomeDKSArm1=HomeDKSArm2=0;
                StarterMainDKS=StarterLL2DKS='R';
                SetSignal(6,GREEN,OFF);SetSignal(10,YELLOW,OFF);Point13L=PointIKS13='N';
                Relay1AT=Relay15T=Relay1T=Relay7='c';Relay1LAT=Relay1LBT='o';
        } else {
            if(ch4==2&&ch3==1){//train is on main line and it has to go to DKS
                SetSignal(11,GREEN,OFF);SetSignal(11,YELLOW,ON);BlockInstrumentDKS='Y';
                //mandatory conditions
                SetSignal(9,YELLOW,OFF);SetSignal(13,GREEN,OFF);PointDKS7=PointDKS8='N';
                SetSignal(9,GREEN,ON);AdvanceDKS='G';
                StarterMainDKS='Y';
                CrossingSignalDKS='c';
                //safety conditions
                SetSignal(14,YELLOW,OFF);SetSignal(13,YELLOW,OFF);HomeDKS='R';HomeDKSArm1=HomeDKSArm2=0;
                StarterLL1DKS=StarterLL2DKS='R';
                Relay0MAT=Relay0MBT='o';Relay7BT=Relay1T=Relay15T=Relay1AT='c';
            } else {
                  if(ch4==1&&ch3==2){//train is on loop line 1 and it has to go to IKS
                           SetSignal(12,GREEN,OFF);SetSignal(12,YELLOW,ON);BlockInstrumentIKS='Y';
                           //mandatory conditions
                           SetSignal(10,YELLOW,ON);SetSignal(6,GREEN,ON);PointIKS13=Point13L='R';
                           SetSignal(10,RED,OFF);SetSignal(10,GREEN,ON);AdvanceIKS='G';
                           SetSignal(6,RED,OFF);SetSignal(6,YELLOW,ON);StarterLL1IKS='Y';
                           CrossingSignalIKS='c';
                           //safety conditions
                           StarterMainIKS=StarterLL2IKS='R';
                           SetSignal(16,YELLOW,OFF);SetSignal(15,YELLOW,OFF);HomeIKS='R';HomeIKSArm1=HomeIKSArm2=0;
                           SetSignal(13,GREEN,OFF);SetSignal(7,GREEN,OFF);PointDKS7=Point7L='N';
                           Relay20AT=Relay6T=Relay20T=Relay13='c';Relay1LAT=Relay1LBT='o';
                  }
            else{
                if(ch4==2&&ch3==2){//train is on main line and it has to go to IKS
                            SetSignal(12,GREEN,OFF);SetSignal(12,YELLOW,ON);BlockInstrumentIKS='Y';
                            //mandatory conditions
                            SetSignal(13,RED,OFF);SetSignal(10,YELLOW,OFF);PointIKS13=PointIKS14='N';
                            SetSignal(10,GREEN,ON);AdvanceIKS='G';
                            SetSignal(4,YELLOW,ON);StarterMainIKS='Y';
                            CrossingSignalIKS='c';
                            //safety conditions
                            SetSignal(16,YELLOW,OFF);SetSignal(15,YELLOW,OFF);HomeIKS='R';HomeIKSArm1=HomeIKSArm2=0;
			SetSignal(4,RED,OFF);SetSignal(10,RED,OFF);
                            StarterLL1IKS=StarterLL2IKS='R';
                            Relay0MAT=Relay0MBT='o';Relay13BT=Relay20T=Relay6T=Relay20AT='c';
                }
                else{
                    if(ch3==2&&ch4==3){//train is standing on loop line 2 and it has to go to IKS
                                SetSignal(12,GREEN,OFF);SetSignal(12,YELLOW,ON);BlockInstrumentIKS='Y';
                                //mandatory conditions
                                SetSignal(13,RED,ON);SetSignal(8,GREEN,ON);SetSignal(10,YELLOW,OFF);PointIKS14=Point14L='R';PointIKS13='N';
                                SetSignal(10,GREEN,ON);AdvanceIKS='G';
                                SetSignal(8,YELLOW,ON);StarterLL2IKS='Y';
                                CrossingSignalIKS='c';
                                //safety conditions
                                StarterLL1IKS=StarterMainIKS='R';
                                SetSignal(16,YELLOW,OFF);SetSignal(15,YELLOW,OFF);HomeIKS='R';HomeIKSArm1=HomeIKSArm2=0;
			SetSignal(8,RED,OFF);SetSignal(10,RED,OFF);
                                Relay2LAT=Relay2LBT='o';Relay20T=Relay6T=Relay20AT='c';
                    }
                    else{
                        if(ch3==1&&ch4==3){//train is on Loop line 2 and it has to go to DKS
                                SetSignal(11,GREEN,OFF);SetSignal(11,YELLOW,ON);BlockInstrumentDKS='Y';
                                //mandatory conditions
                                SetSignal(9,YELLOW,ON);SetSignal(5,GREEN,ON);PointDKS8=Point8L='R';PointDKS7='N';
                                SetSignal(9,RED,OFF);SetSignal(9,GREEN,ON);AdvanceDKS='G';
                                SetSignal(5,RED,OFF);SetSignal(5,YELLOW,ON);StarterLL2DKS='Y';
                                CrossingSignalDKS='c';
                                //safety conditions
                                SetSignal(14,YELLOW,OFF);SetSignal(13,YELLOW,OFF);HomeDKS='R';HomeDKSArm1=HomeDKSArm2=0;
                                StarterLL1DKS=StarterMainDKS='R';
                                SetSignal(13,RED,OFF);SetSignal(8,GREEN,OFF);Point14L=PointIKS14='N';
                                Relay2LAT=Relay2LBT='o';Relay1T=Relay15T=Relay1AT='c';
                        }
                    }
                }
            }
        }
    }
    }
    else{
        printf("We did not get a receiving consent\n");
    }
}

void DispatchTrain(){
    if(ch4==1&&ch3==1){
        Relay1AT=Relay15T=Relay1T=Relay7='o';
        printf("\n\nTrain is going from loop line 1 to DKS,signal status is:\n");
SetSignal(16,GREEN,OFF);SetSignal(16,RED,OFF);
        SetSignal(11,YELLOW,OFF);SetSignal(11,RED,ON);BlockInstrumentDKS='R';
        PrintingSignalStatus();
        PrintingRelayStatus();
        sleep(1);
        SetSignal(7,YELLOW,OFF);SetSignal(7,RED,ON);
        SetSignal(13,GREEN,OFF);SetSignal(7,GREEN,OFF);SetSignal(9,YELLOW,OFF);
        SetSignal(9,GREEN,OFF);SetSignal(9,RED,ON);
        SetSignal(14,YELLOW,OFF);SetSignal(13,YELLOW,OFF);
        SetSignal(6,GREEN,OFF);SetSignal(10,YELLOW,OFF);
        Relay1AT=Relay15T=Relay1T=Relay7=Relay1LAT=Relay1LBT='f';
        CrossingSignalDKS='o';
        StarterLL1DKS='R';
        SetSignal(7,GREEN,OFF);SetSignal(13,GREEN,OFF);PointDKS7=Point7L='N';
        StarterMainDKS='R';
        HomeDKS='R';
        AdvanceDKS='R';
        printf("\n\nNow the train has gone to DKS,signal status is:\n");
SetSignal(6,YELLOW,OFF);SetSignal(10,GREEN,OFF);
        SetSignal(11,RED,OFF);SetSignal(11,GREEN,ON);BlockInstrumentDKS='G';
        PrintingSignalStatus();
        PrintingRelayStatus();
        sleep(1);
        AllRedOn();
    }
    else{
        if(ch4==2&&ch3==1){
            Relay1AT=Relay15T=Relay1T=Relay7BT='o';
            printf("\n\nTrain is going from main line to DKS,signal status is:\n");
	SetSignal(15,GREEN,OFF);SetSignal(15,RED,OFF);
            SetSignal(11,YELLOW,OFF);SetSignal(11,RED,ON);BlockInstrumentDKS='R';
            PrintingSignalStatus();
            PrintingRelayStatus();
            sleep(1);
            SetSignal(9,GREEN,OFF);SetSignal(9,RED,ON);
            SetSignal(9,YELLOW,OFF);SetSignal(13,GREEN,OFF);
            SetSignal(14,YELLOW,OFF);SetSignal(13,YELLOW,OFF);
            Relay0MAT=Relay0MBT=Relay7BT=Relay1T=Relay15T=Relay1AT='f';
            CrossingSignalDKS='c';
            AdvanceDKS='R';
            StarterMainDKS='R';
            HomeIKS='R';
            printf("\n\nNow the train has gone to DKS,signal status is:\n");
            SetSignal(11,RED,OFF);SetSignal(11,GREEN,ON);BlockInstrumentDKS='G';
            PrintingSignalStatus();
            PrintingRelayStatus();
            sleep(1);
            AllRedOn();
        }
        else{
            if(ch4==1&&ch3==2){
                Relay20AT=Relay20T=Relay6T=Relay13='o';
                printf("\n\nTrain is going from loop line 1 to IKS,signal status is:\n");
	SetSignal(15,GREEN,OFF);SetSignal(15,RED,OFF);
                SetSignal(12,YELLOW,OFF);SetSignal(12,RED,ON);BlockInstrumentIKS='R';
                PrintingSignalStatus();
                PrintingRelayStatus();
                sleep(1);
                SetSignal(6,YELLOW,OFF);SetSignal(6,RED,ON);
                SetSignal(10,GREEN,OFF);SetSignal(10,RED,ON);
                SetSignal(16,YELLOW,OFF);SetSignal(15,YELLOW,OFF);
                SetSignal(13,GREEN,OFF);SetSignal(7,GREEN,OFF);
                SetSignal(10,YELLOW,OFF);SetSignal(6,GREEN,OFF);SetSignal(7,GREEN,OFF);PointIKS13=Point13L=Point7L='N';
                Relay7=Relay20AT=Relay6T=Relay20T=Relay13=Relay1LAT=Relay1LBT='f';
                CrossingSignalIKS='o';
                StarterLL1IKS='R';
                StarterMainIKS='R';
                HomeIKS='R';
                AdvanceIKS='R';
                printf("\n\nNow the train has gone to IKS,signal status is:\n");
                SetSignal(12,RED,OFF);SetSignal(12,GREEN,ON);BlockInstrumentIKS='G';
                PrintingSignalStatus();
                PrintingRelayStatus();
                sleep(1);
                AllRedOn();
            }
            else{
                if(ch4==2&&ch3==2){
                    Relay13BT=Relay20AT=Relay6T=Relay20T='o';
                    printf("\n\nTrain is going from main line to IKS,signal status is:\n");
		SetSignal(15,GREEN,OFF);SetSignal(15,RED,OFF);
                    SetSignal(12,YELLOW,OFF);SetSignal(12,RED,ON); BlockInstrumentIKS='R';
                    PrintingSignalStatus();
                    PrintingRelayStatus();
                    sleep(1);
                    SetSignal(4,YELLOW,OFF);SetSignal(4,RED,ON);StarterMainIKS='R';
                    SetSignal(10,GREEN,OFF);SetSignal(10,RED,ON);AdvanceIKS='R';
                    SetSignal(13,RED,OFF);SetSignal(10,YELLOW,OFF);
                    SetSignal(16,YELLOW,OFF);SetSignal(15,YELLOW,OFF);
                    Relay0MAT=Relay0MBT=Relay7BT=Relay20T=Relay6T=Relay20AT=Relay13BT='f';
                    CrossingSignalIKS='c';
                    printf("\n\nNow the train has gone to IKS,signal status is:\n");
                    SetSignal(12,RED,OFF);SetSignal(12,GREEN,ON);BlockInstrumentIKS='G';
                    PrintingSignalStatus();
                    PrintingRelayStatus();
                }
                else{
                    if(ch3==2&&ch4==3){
                        Relay14=Relay20AT=Relay20T=Relay6T='o';
                        printf("\n\nTrain is going from loop line 2 to IKS,signal status is:\n");
		SetSignal(14,GREEN,OFF);SetSignal(14,RED,OFF);
                        SetSignal(12,YELLOW,OFF);SetSignal(12,RED,ON);BlockInstrumentIKS='R';
                        PrintingSignalStatus();
                        PrintingRelayStatus();
                        sleep(1);
                        SetSignal(8,YELLOW,OFF);SetSignal(8,RED,ON);StarterLL2IKS='R';
                        SetSignal(10,GREEN,OFF);SetSignal(10,RED,ON);AdvanceIKS='R';
                        SetSignal(13,RED,OFF);SetSignal(8,GREEN,OFF);SetSignal(10,YELLOW,OFF);PointIKS14=Point14L=PointIKS13='N';
                        SetSignal(16,YELLOW,OFF);SetSignal(15,YELLOW,OFF);
                        Relay2LAT=Relay2LBT=Relay14AT=Relay20T=Relay6T=Relay20AT=Relay14='f';
                        CrossingSignalIKS='c';
                        printf("\n\nNow the train has gone to IKS,signal status is:\n");
                        SetSignal(12,RED,OFF);SetSignal(12,GREEN,ON);BlockInstrumentIKS='G';
                        PrintingSignalStatus();
                        PrintingRelayStatus();
                    }
                    else{
                        if(ch3==1&&ch4==3){
                            Relay8=Relay1T=Relay15T=Relay1AT='o';
                            printf("\n\nTrain is going from loop line 2 to DKS,signal status is:\n");
			SetSignal(14,GREEN,OFF);SetSignal(14,RED,OFF);
                            SetSignal(11,YELLOW,OFF);SetSignal(11,RED,ON);BlockInstrumentDKS='R';
                            PrintingSignalStatus();
                            PrintingRelayStatus();
                            sleep(1);
                            SetSignal(5,YELLOW,OFF);SetSignal(5,RED,ON);StarterLL2DKS='R';
                            SetSignal(9,GREEN,OFF);SetSignal(9,RED,ON);AdvanceDKS='R';
                            SetSignal(9,YELLOW,OFF);SetSignal(5,GREEN,OFF);PointDKS8=Point8L='N';
                            SetSignal(14,YELLOW,OFF);SetSignal(13,YELLOW,OFF);
                            SetSignal(13,RED,OFF);SetSignal(8,GREEN,OFF);
                            Relay2LAT=Relay2LBT=Relay8AT=Relay20T=Relay6T=Relay20AT='f';
                            CrossingSignalIKS='o';
                            printf("\n\nNow the train has gone to DKS,signal status is:\n");
                            SetSignal(11,RED,OFF);SetSignal(11,GREEN,ON);BlockInstrumentDKS='G';
                            PrintingSignalStatus();
                            PrintingRelayStatus();
                            sleep(1);
                            AllRedOn();
                        }
                    }
                }
            }
        }
    }
}


int main(){
char TrafficPort[1024];
system("sudo chmod 666 /dev/ttyUSB0");
sprintf(TrafficPort,"/dev/ttyUSB0");
if((fTraffic=fopen(TrafficPort,"w+"))==NULL){
    printf("could not open traffic port\n");
    return -1;
}else{
printf("open traffic port\n");
}
    SetSignal(11,GREEN,ON);SetSignal(12,GREEN,ON);
    AllRedOn();
    printf("I am S2 station\nI have 2 loop lines and 1 main line\n");
    printf("Initial condition of relay and signal is:\n");
    printf("For Relays:\no means relay is occupied\nc means relay is committed\nf means relay is free\n");
    printf("Initial condition of relay and signal is:\n");
    printf("For Relays:\no means relay is occupied\nc means relay is committed\nf means relay is free\n");
    PrintingSignalStatus();
    PrintingRelayStatus();
    while(1){
        printf("\n\nEnter your choice:\n1:To give line clearance\n2:Receiving train\n3:To take clearance\n4:Receiving permission from other station permission(set signal+point of dispatch)\n5:Send a train\n");
        scanf("%d",&ch);
        if(ch==1){
            GiveLineClearance();
                }
        else{
            if(ch==2){
                ReceiveTrain();
            }
            else{
                    if(ch==3){
                        TakeLineClearance();
                    }
                    else{
                        if(ch==4){
                            ReceivingPermission();
                        }
                        else{
                            if(ch==5){
                                DispatchTrain();
                            }
                        }
                    }
                }
                }
            }
    return 0;
}
