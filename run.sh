make -C src/ -j5
cp src/webserv server/
cd server/
./webserv
