# 2020-CPCD-FLEX-BISON-LLVM-easyCminus-IRgenerater-
南京航空航天大学-编译原理课程（李静老师）-课程设计C-语言编译器（使用FLEX+BISON+LLVM，不包含PASS）
系统环境：centos-7
编译指令：
flex cm.l
bison -d cm.y
g++ -std=c++11 cm.tab.c lex.yy.c treenodeAPI.c IRgenerator.cpp -o IRgenerator `llvm-config --cflags --ldflags` `llvm-config --libs` `llvm-config --system-libs`
