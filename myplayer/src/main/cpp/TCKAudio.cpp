//
// Created by tck on 2019/7/19.
//

#include "TCKAudio.h"

TCKAudio::TCKAudio(TCKPlayStatus *playstatus) {
    this->playstatus = playstatus;
    queue = new TCKQueue(playstatus);
}

TCKAudio::~TCKAudio() {


}
