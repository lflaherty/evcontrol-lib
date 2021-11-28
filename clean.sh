#!/bin/bash

#
# Cleans build artifacts produced from S-Function Builders
#

echo Cleaning S-Function build artifacts
rm -f ./PMSMCurrentControllerWrapper*
rm -f ./SPWMWrapper*
rm -f ./PMSMCurrentRefWrapper*
rm -f ./SFB__*__SFB.mat
rm -f rtwmakecfg.m
echo Done

