//
// Created by tck88 on 2019/7/20.
//

#ifndef FFMPEGTRAIN_TCKPLAYSTATUS_H
#define FFMPEGTRAIN_TCKPLAYSTATUS_H


class TCKPlayStatus {
public:
    bool exit = false;
    bool load = true;
    bool seek = false;

public:
    TCKPlayStatus();
    ~TCKPlayStatus();
};


#endif //FFMPEGTRAIN_TCKPLAYSTATUS_H
