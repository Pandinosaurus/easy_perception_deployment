#!/usr/bin/env bash

export PATH=~/anaconda3/bin:$PATH
PATH_TO_THIS_SCRIPT=$( realpath "$0"  )
START_DIR=$( dirname $PATH_TO_THIS_SCRIPT )

cd $START_DIR

# Check if Anaconda has been installed in general.
# If true, get the first digit of the string which should reflect the major version of conda.
if detected_conda=$(conda --version); then
    # echo $detected_conda - FOUND
    declare -i conda_ver
    conda_ver=$(echo $detected_conda | grep -o -E '[0-9]+' | head -1 | sed -e 's/^0\+//')
else
    echo "Please install Anaconda by refering to the installation docs."
    echo "Exiting terminal in 10 seconds."
    echo "[ https://docs.anaconda.com/anaconda/install/linux/ ]"
    sleep 10
    exit 1
fi

# Check if Anaconda is Anaconda2 or below.
if (( conda_ver < 2 )); then
    echo "Anaconda3 - NOTFOUND. Please install Anaconda3."
    echo "Exiting terminal in 10 seconds."
    sleep 10
    exit 1
fi

# Check if pretrained models have been downloaded.
P1FILE=./data/model/squeezenet1.1-7.onnx
if [ ! -f "$P1FILE" ]; then
    echo "Downloading $P1FILE."
    wget \
    https://github.com/onnx/models/raw/main/vision/classification/squeezenet/model/squeezenet1.1-7.onnx \
    --directory-prefix=./data/model/
fi
unset P1FILE

P2FILE=./data/model/FasterRCNN-10.onnx
if [ ! -f "$P2FILE" ]; then
    echo "Downloading $P2FILE."
    wget \
    https://github.com/onnx/models/raw/main/vision/object_detection_segmentation/faster-rcnn/model/FasterRCNN-10.onnx \
    --directory-prefix=./data/model/
fi
unset P2FILE

P3FILE=./data/model/MaskRCNN-10.onnx
if [ ! -f "$P3FILE" ]; then
    echo "Downloading $P3FILE."
    wget \
    https://github.com/onnx/models/raw/main/vision/object_detection_segmentation/mask-rcnn/model/MaskRCNN-10.onnx \
    --directory-prefix=./data/model/
fi
unset P3FILE

# Checking if the epd_gui_env conda environment has been installed.
env_exists=$(conda env list | grep epd_gui_env)

if [ -z "$env_exists" ]
then
      echo "Installing epd_gui_env conda environment."
      conda create -n epd_gui_env python=3.6 -y
      eval "$(conda shell.bash hook)"
      conda activate epd_gui_env
      pip install pyside2
      pip install dateutils
      pip install pycocotools
      pip install labelme
      pip install pytest-qt
      conda install pytest -y
      conda install pytorch torchvision cpuonly -c pytorch -y
      pip install lark-parser
      pip install empy
      conda deactivate
      echo "[epd_gui_env] env created."
fi

eval "$(conda shell.bash hook)"
conda activate epd_gui_env
cd $PWD/gui
python main.py

unset START_DIR PATH_TO_THIS_SCRIPT env_exists
