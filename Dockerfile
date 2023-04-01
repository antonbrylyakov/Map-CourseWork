FROM ubuntu:16.04
RUN apt update
RUN apt -y install g++ cmake
WORKDIR /helloworld
COPY main.cpp .
COPY CMakeLists.txt .
WORKDIR out
RUN cmake ..
RUN make
ENTRYPOINT ["/helloworld/out/homework"]
