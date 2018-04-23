//
// Created by Semyon Tikhonenko on 4/22/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_MVXFILE_H
#define VOCALTRAINER_MVXFILE_H

#include <iostream>
#include "VxFile.h"

class MvxFileWriter {
public:
    static void writeToStream(const VxFile& vxFile, const char* instrumentalFilePath, std::ostream& os);
    static void writeToFile(const VxFile& vxFile, const char* instrumentalFilePath, const char* outFilePath);
};


#endif //VOCALTRAINER_MVXFILE_H