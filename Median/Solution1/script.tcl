############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
open_project Median
set_top top
add_files Median/Header/config.h
add_files Median/Source/top.cpp
add_files Median/Header/top.h
add_files -tb Median/EasyBMP/EasyBMP.cpp
add_files -tb Median/EasyBMP/EasyBMP.h
add_files -tb Median/EasyBMP/EasyBMP_BMP.h
add_files -tb Median/EasyBMP/EasyBMP_DataStructures.h
add_files -tb Median/EasyBMP/EasyBMP_VariousBMPutilities.h
add_files -tb Median/Header/config.h
add_files -tb Median/Images/input_image.bmp
add_files -tb Median/Images/output_image_reference.bmp
add_files -tb Median/Testbench/testbench.cpp
add_files -tb Median/Header/top.h
open_solution "Solution1"
set_part {xc7z010clg400-1} -tool vivado
create_clock -period 10 -name default
source "./Median/Solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
