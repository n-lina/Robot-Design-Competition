#ifndef Setup_h
#define Setup_h

class Setup{
    Setup();
    static void setup();
    int state; 
    const bool TEAM; 
    int stoneNumber; 
    int splitNumber;
    bool direction_facing; 
    bool direction;
    int KP_WHEEL;
    int KD_WHEEL;
    int THRESHOLD;
    int SPLIT_THRESHOLD;
    int TAB_THRESHOLD;
    int GAUNTLET_TAPE_THRESHOLD;
    int EDGE_THRESHOLD;
    int COLLISION_THRESHOLD;
    int PILLAR_DISTANCE; 
};

#endif
