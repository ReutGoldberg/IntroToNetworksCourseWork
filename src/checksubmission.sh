#! /bin/bash

if [[ $1 == *.zip ]]; then
    unzip -o "$1" -d "${1%%.zip}"
    cd "${1%%.zip}" || { echo "Unzip failed" && exit; }
    dos2unix ./* &> /dev/null
    make || { cd .. && rm -rf "${1%%.zip}" && exit;}
    chmod a+x ./simulator &> /dev/null
    ./simulator 1000 2 60 30 1 0.8 0.5 0 > /dev/null || { cd .. && rm -rf "${1%%.zip}" && exit;}
    cd ..
    rm -rf "${1%%.zip}"
    echo "Your wet part is ready for submission :)"
else
    echo "Usage: ./checksubmission.sh <your zip submission>"
fi
