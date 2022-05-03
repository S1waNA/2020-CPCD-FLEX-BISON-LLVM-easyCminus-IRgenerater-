# 2020-CPCD-FLEX-BISON-LLVM-easyCminus-IRgenerater-
system：centos-7
command:
flex cm.l
bison -d cm.y
g++ -std=c++11 cm.tab.c lex.yy.c treenodeAPI.c IRgenerator.cpp -o IRgenerator `llvm-config --cflags --ldflags` `llvm-config --libs` `llvm-config --system-libs`
