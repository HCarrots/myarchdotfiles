#!/usr/bin/env fish

# Source global definitions
if test -f /etc/bashrc
    source /etc/bashrc
end

set workdir (pwd)
set SCRIPT_DIR (dirname (realpath $argv[1]))
set PROJECT_DIR $SCRIPT_DIR

# Check if the BluetAna subdirectory exists within PROJECT_DIR
if test -d "$PROJECT_DIR/BluetAna"
    echo "===> PROJECT_DIR is set to: $PROJECT_DIR"
else
    echo "***: The BluetAna subdirectory does not exist within PROJECT_DIR."
end

###################################
# bluet
###################################
set -x BLUETBase $PROJECT_DIR

# set -x PATH /cvmfs/software.csns.ihep.ac.cn/softwares/gnuplot-5.0.7/bin:$PATH
# set -x LD_LIBRARY_PATH /cvmfs/software.csns.ihep.ac.cn/softwares/fftw/lib:$LD_LIBRARY_PATH
# set -x LD_LIBRARY_PATH /cvmfs/software.csns.ihep.ac.cn/softwares/libgd-2.2.5/lib:$LD_LIBRARY_PATH
# set -x LD_LIBRARY_PATH /cvmfs/software.csns.ihep.ac.cn/softwares/wxWidges/lib:$LD_LIBRARY_PATH

###################################
# cmake
###################################
# set -x PATH "/cvmfs/software.csns.ihep.ac.cn/softwares/cmake/cmake-3.19.2/bin:$PATH"

###################################
# gcc
###################################
# set -x PATH "/cvmfs/software.csns.ihep.ac.cn/softwares/gcc-9.3.0/bin:$PATH"
# set -x LD_LIBRARY_PATH "/cvmfs/software.csns.ihep.ac.cn/softwares/gcc-9.3.0/lib64:$LD_LIBRARY_PATH"
# set -x CC "/cvmfs/software.csns.ihep.ac.cn/softwares/gcc-9.3.0/bin/gcc"
# set -x CXX "/cvmfs/software.csns.ihep.ac.cn/softwares/gcc-9.3.0/bin/g++"

###################################
# root
###################################
# source /cvmfs/software.csns.ihep.ac.cn/softwares/root-6.28.06-gcc9.3-cxx17/bin/thisroot.sh
set -x ROOT_LIBRARIES (root-config --libs)

###################################
# geant4
###################################
# source /cvmfs/software.csns.ihep.ac.cn/softwares/geant4.10.07/bin/geant4.sh
# source /cvmfs/software.csns.ihep.ac.cn/softwares/geant4.10.07/share/Geant4-10.7.0/geant4make/geant4make.sh

###################################
# qt
###################################
# set -x PATH "/cvmfs/software.csns.ihep.ac.cn/softwares/qtbase/bin:$PATH"
# set -x LD_LIBRARY_PATH "/cvmfs/software.csns.ihep.ac.cn/softwares/qtbase/lib:$LD_LIBRARY_PATH"

###################################

# alias
###################################
alias duc='du -h --max-depth=1'
alias la='ls -all'

###################################
# ulimit set
###################################
ulimit -d unlimited
ulimit -s unlimited
ulimit -m unlimited
ulimit -v unlimited

###################################
# elmer
###################################
# set -x ELMER_HOME /cvmfs/software.csns.ihep.ac.cn/softwares/elmerfem-release-9.0-gcc9.3-openmpi4.1
# set -x PATH $PATH:$ELMER_HOME/bin
# set -x LD_LIBRARY_PATH $LD_LIBRARY_PATH:$ELMER_HOME/lib

###################################
# gmsh
###################################
# set -x PATH $PATH:/wns_ana/bagshare/gmsh/install/bin

###################################
# openmpi
###################################
# set -x PATH /cvmfs/software.csns.ihep.ac.cn/softwares/openmpi-4.1.0-gcc9.3/bin:$PATH
# set -x LD_LIBRARY_PATH /cvmfs/software.csns.ihep.ac.cn/softwares/openmpi-4.1.0-gcc9.3/lib/:$LD_LIBRARY_PATH

###################################
# opencascade
###################################
# source /cvmfs/software.csns.ihep.ac.cn/softwares/opencascade-7.6.0-gcc9.3/bin/env.sh

# python $PROJECT_DIR/Scripts/dependence/Json_Start.py

# cd $workdir