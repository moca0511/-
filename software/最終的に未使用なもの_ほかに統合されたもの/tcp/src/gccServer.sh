nowDir=$('pwd')
echo ${nowDir}
libDir=../tcpLib

cd ${nowDir}/${libDir}/myServer/
g++ -c tcpServer.cpp  -L${nowDir}/${libDir} -I${nowDir}/${libDir} 
ar -r libServer.a tcpServer.o 

cd ${nowDir}
g++ s1.cpp -lServer -o s.out  \
			-I${nowDir}/${libDir} \
			-L${nowDir}/${libDir} -L${nowDir}/${libDir}/myServer
