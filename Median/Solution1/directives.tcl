############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_unroll "median/median_label0"
set_directive_unroll "FillBuffer/FIFO"
set_directive_array_partition -type complete -dim 1 "top" buffer
set_directive_array_partition -type complete -dim 1 "top" pixel
