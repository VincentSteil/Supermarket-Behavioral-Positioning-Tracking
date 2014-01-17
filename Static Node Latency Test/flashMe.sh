#!/bin/bash

sed  's#\[\[--filename--\]\]#\./_build/software-uart-test_xxaa.bin#' ../nrf51_sdk/Segger/burn-template.seg > _build/burn.seg
LD_LIBRARY_PATH=../../JLink_Linux_V474/ "/afs/inf.ed.ac.uk/user/s13/s1367762/slip/slip2/SLIP/JLink_Linux_V474/JLinkExe" ./_build/burn.seg
