nowDir=$('pwd')
echo ${nowDir}
libDir=../tcpLib

cd ${nowDir}/${libDir}/myClient/
g++ -c tcpClient.cpp  -L${nowDir}/${libDir} -I${nowDir}/${libDir} 
ar -r libClient.a tcpClient.o 

cd ${nowDir}

g++ c3.cpp -lClient -o c.out  \
			-I${nowDir}/${libDir} \
			-L${nowDir}/${libDir} -L${nowDir}/${libDir}/myClient
