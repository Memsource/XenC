/**
 *  @file mode.cpp
 *  @brief Abstract class defining the filtering modes architecture
 *  @author Anthony Rousseau
 *  @version 2.0.0
 *  @date 18 March 2016
 */

/*  This file is part of the cross-entropy tool for data selection (XenC)
 *  aimed at speech recognition and statistical machine translation.
 *
 *  Copyright 2013-2016, Anthony Rousseau, LIUM, University of Le Mans, France
 *
 *  Development of the XenC tool has been partially funded by the
 *  European Commission under the MateCat project.
 *
 *  The XenC tool is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License version 3 as
 *  published by the Free Software Foundation
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "../include/mode.h"
#include <iostream>

Mode::~Mode() {
    
}

long long Mode::findSampleSize(boost::shared_ptr<Corpus> idCorp, boost::shared_ptr<Corpus> oodCorp) {
  long long iW = idCorp->getWC();
	long long oW = oodCorp->getWC();
    
	return std::min(iW, oW);
}

Corpus Mode::extractSample(boost::shared_ptr<Corpus> ptrCorp, long long sSize, bool mean) {
    std::string rnd = "";
    
    if (mean) {
        std::srand ((unsigned int)std::time(NULL));
        rnd = XenCommon::toString0(std::rand() % 10000);
    }
    
    std::string sampleName = ptrCorp->getXenFile()->getPrefix() + rnd + "-sample" + XenCommon::toString(sSize);
    std::string outFile = sampleName + "." + ptrCorp->getLang();
    
    if (!boost::filesystem::exists(outFile.c_str())) {
        std::srand((unsigned int)std::time(NULL));
        
        std::ofstream out(outFile.c_str(), std::ios::out | std::ios::trunc);
        
        int count = 0;
        
        while (count < sSize) {
            std::string line = ptrCorp->getLine(std::rand() % ptrCorp->getSize());
            out << line << std::endl;
            int n = XenCommon::wordCount(line);
            count += (n + 1);
        }
        
        out.close();
    }
    else {
        std::cout << "Sample file " << outFile << " already exists, reusing." << std::endl;
    }
    
	Corpus r;
    r.initialize(outFile, ptrCorp->getLang());
    
	return r;
}
