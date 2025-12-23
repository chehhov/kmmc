
CPP_FILES="input.cpp sort.cpp output.cpp main.cpp"
APP=example
FOLDER=bin

if [ ! -d "bin" ]; then
    mkdir -p $FOLDER
fi

if [ -f /$FOLDER/$APP ]; then rm $FOLDER/$APP
fi

g++ $CPP_FILES -o $FOLDER/$APP

cd $FOLDER && ./$APP
