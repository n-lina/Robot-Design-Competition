#ifndef Setup_h
#define Setup_h

class Setup{
    Setup();
    static void setup();
    int state; 
    const bool TEAM; //true = thanos, false = methanos 
    int stoneNumber; 
    int splitNumber;
    bool direction_facing; //true = forwards, false = backwards
    bool direction; // true = right, left = false 
    const static PinName GAUNTLET_TAPE_SENSOR;
    static int KP_WHEEL;
    static int KD_WHEEL;
    static int THRESHOLD;
    static int SPLIT_THRESHOLD;
    static int TAB_THRESHOLD;
    static int GAUNTLET_TAPE_THRESHOLD;
    static int EDGE_THRESHOLD;
    static int COLLISION_THRESHOLD;
    static int PILLAR_DISTANCE; //can u make it const & not initialize in constructor? so that the calibrated
    //value is the final one 
};

#endif
