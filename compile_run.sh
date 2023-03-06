set_vars=/opt/intel/openvino_2022.3.0/setupvars.sh
source $set_vars
cmake -S . -B cmakeBuild
make -C cmakeBuild
 ./cmakeBuild/main
