Readme file for HW1

# Build -O0 instructions:

% cd HW1 

% cd build_o0

% cmake ../ -DCMAKE_CXX_FLAGS_RELEASE="-O0"

% make

% ./loop.sh       # this will loop over the problem size as argument 

# Build -O3 instructions:

% cd HW1 

% cd build_o3

% cmake ../

% make

% ./loop.sh       # this will loop over the problem size as argument 


# EOF
